#include "Compiler.hpp"
#include "Assembler.hpp"
#include "VarManager.hpp"
#include "DataSection.hpp"

Loc::Loc(const char* _pos, const char* const _end) : pos(_pos), end(_end) {

}

void skipComment(Loc& loc) {
	if (!loc.eof() && *loc.pos == '#') {
		do {
			++loc.pos;
			// std::cout << "ignore comment at line " << loc.lineNr << std::endl;
		} while (!loc.eof() && *loc.pos != '\n');
	}
}

void skipSpaces(Loc& loc) {
	while (!loc.eof() && std::isspace(*loc.pos)) {
		loc.checkNewLine();
		// std::cout << loc.lineNr << std::endl;
		++loc.pos;
	}

	skipComment(loc);
}

char unescape(char c) {
	switch (c) { 
		case 'n': return '\n'; 
		case 't': return '\t'; 
		case '\\': return '\\'; 
		case '\"': return '\"'; 
	}

	std::cerr << "Unknown escape sequence \\" << c << std::endl;

	return c;
}

bool readString(Loc& loc, std::string* str) { 
	skipSpaces(loc);

	if (!loc.eof() && *loc.pos == '"') { 
		++loc.pos;

		str->clear();

		do {
			if(*loc.pos == '\\') {
				++loc.pos;

				if (loc.eof()) { 
					loc.error("EOF in string literal");

					return false;
				}

				*str += unescape(*loc.pos);
			} else 
				*str += *loc.pos;

			++loc.pos;
		} while(!loc.eof() && *loc.pos != '\"');

		++loc.pos;

		return true;
	}

	return false;
}

bool read(Loc& loc, const char* what) {
	skipSpaces(loc);

	while (!loc.eof() && *loc.pos == *what) {
		++what;
		++loc.pos;

		if (*what == '\0') {
			--what;

			if (std::isalpha(*what)) {
				if (std::isalnum(*loc.pos))
					return false;
			}

			return true;
		}
	}

	return false;
}

bool read(Loc& loc, const char what) {
	skipSpaces(loc);

	if (!loc.eof() && *loc.pos == what) {
		++loc.pos;

		return true;
	}

	return false;
}

bool read(Loc& loc, Tok tok) {
	return read(loc, TokStr.at(tok));
}

bool readNumber(Loc& loc, int* n) {
	skipSpaces(loc);

	if (!loc.eof() && std::isdigit(*loc.pos)) {
		*n = 0;
		do {
			*n *= 10;
			*n += *loc.pos - '0';

			++loc.pos;
		} while (!loc.eof() && std::isdigit(*loc.pos));

		return true;
	}

	return false;
}

bool readIdentifier(Loc& loc, std::string* identifier) {
	skipSpaces(loc);

	if (!loc.eof() && (std::isalpha(*loc.pos) || *loc.pos == '_')) {
		identifier->clear();

		do {
			*identifier += *loc.pos;

			++loc.pos;
		} while (!loc.eof() && (std::isalnum(*loc.pos) || *loc.pos == '_'));

		return true;
	}

	return false;
}

bool parsePrint(Env& env) {
	if (read(*env.loc, Tok::Print)) {
		do {
			std::string str;
			if (readString(*env.loc, &str)) {
				const std::string label = env.data->addStringData(str);

				if (*env.loc->pos == ',')
					env.as->call(LabelStr.at(Label::PrintS), label);
				else
					env.as->call(LabelStr.at(Label::PrintlnS), label);
			} else if (parseExpression(env)) {
				if (*env.loc->pos == ',')
					env.as->call(LabelStr.at(Label::PrintI));
				else
					env.as->call(LabelStr.at(Label::PrintlnI));
			}
			else
				env.loc->error("Missing or invalid print argument.");
		} while (read(*env.loc, ','));

		return true;
	}

	return false;
}

bool parseVarAssign(Env& env, const std::string& identifier, bool duty) {
	if (read(*env.loc, '=')) {
		if (parseExpression(env)) {
			const int addr = env.var->addrOf(identifier);
			assert(addr != -1);

			env.as->assignVar(addr);

			return true;
		} else
			env.loc->error("Missing right operant of '='.");

		return false;
	} else if (duty) {
		env.loc->error("Missing '=' in assignment.");

		return false;
	}

	return true;
}

bool parseVar(Env& env) {
	 if (read(*env.loc, Tok::Var)) {
		std::string identifier;

		if (readIdentifier(*env.loc, &identifier)) {
			if (env.var->create(identifier)) {
				if (parseVarAssign(env, identifier, false))
					return true;
			} else
				env.loc->error("Redefinition of variable '" + identifier + '"');
		}
	} else {
		std::string identifier;

		if (readIdentifier(*env.loc, &identifier)) {
			if (parseVarAssign(env, identifier, true))
				return true;
		}
	}

	return false;
}

bool parseLiteral(Env &env) {
	int num;
	if (readNumber(*env.loc, &num)) {
		env.as->move(num);

		return true;
	}

	std::string identifier;
	if (readIdentifier(*env.loc, &identifier)) {
		if (env.var->exist(identifier)) {
			const int addr = env.var->addrOf(identifier);
			assert(addr != -1);

			env.as->useVar(addr);

			return true;
		}

		env.loc->error("Undefinied variable '" + identifier + '"');
	}

	return false;
}

bool parseFactor(Env& env) {
	bool negate = false;
	if (read(*env.loc, '-'))
		negate = true;

	if (parseLiteral(env)) {
		// nothing to do
	} else if (read(*env.loc, '(')) {
		if (!parseExpression(env)) {
			env.loc->error("Expected expression after '('.");

			return false;
		} else if (!read(*env.loc, ')')) {
			env.loc->error("Missing ')'.");

			return false;
		}
	} else
		return false;

	if (negate)
		env.as->neg();

	return true;
}

bool parseTerm(Env& env) {
	if (parseFactor(env)) {
		for (;;) {
			if (read(*env.loc, '*')) {
				env.as->push();;

				if (!parseFactor(env)) {
					env.loc->error("Expected factor after *");

					return false;
				}

				env.as->mul();
			} else if (read(*env.loc, '/')) {
				env.as->push();

				if (!parseFactor(env)) {
					env.loc->error("Expected factor after /");

					return false;
				}

				env.as->div();
			} else if (read(*env.loc, '%')) {
				env.as->push();

				if (!parseFactor(env)) {
					env.loc->error("Expected factor after %");

					return false;
				}

				env.as->mod();
			} else
				break;
		}

		return true;
	}

	return false;
}

bool parseExpression(Env& env) {
	if (parseTerm(env)) {
		for (;;) {
			if (read(*env.loc, '+')) {
				env.as->push();

				if (!parseTerm(env)) {
					env.loc->error("Expected Term after +");

					return false;
				}

				env.as->add();
			} else if (read(*env.loc, '-')) {
				env.as->push();

				if (!parseTerm(env)) {
					env.loc->error("Expected Term after -");

					return false;
				}
				
				env.as->sub();
				env.as->pop(); // ?
			} else
				break;
		}

		return true;
	}

	return false;
}

bool parseCommand(Env& env) {
	if (parsePrint(env))
		return true;
	else if (parseVar(env))
		return true;

	return false;
}

#define ALL 0
#define BASIC_PRINT 0
#define BASIC_VAR 0
#define STRING_PRINT 1

int main(int argc, char const *argv[]) {
#if ALL
	std::ifstream in("in.txt");
#elif BASIC_VAR
	std::ifstream in("basic_var.txt");
#elif STRING_PRINT
	std::ifstream in("string_print.txt");
#else
	std::ifstream in("basic_print.txt");
#endif

	std::vector<char> code;
	std::copy(
		std::istreambuf_iterator<char>(in.rdbuf()),
		std::istreambuf_iterator<char>(),
		std::back_inserter(code));

	std::ofstream out("out.s");

	Assembler as(out);
	Loc loc(&code[0], &code.back() + 1);
	VarManager vm;
	DataSection data;

	Env env(&loc, &as, &vm, &data);

	while (parseCommand(env)) {

	}

	as.ret();
	data.writeDataSection(out);

	return 0;
}