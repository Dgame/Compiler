#include "Compiler.hpp"
#include "Assembler.hpp"
#include "VarManager.hpp"
#include "DataSection.hpp"

Loc::Loc(const char* _pos, const char* const _end) : pos(_pos), end(_end) {

}

void skipComment(Loc& loc) {
	if (!loc.eof() && *loc.pos == '#') {
		while (!loc.eof() && *loc.pos != '\n') {
			++loc.pos;
		}

		if (!loc.eof()) {
			++loc.pos; // skip the '\n'
			loc.lineNr++;
		}
	}
}

void skipSpaces(Loc& loc) {
	while (!loc.eof() && std::isspace(*loc.pos)) {
		if (*loc.pos == '\n')
			loc.lineNr++;
		++loc.pos;
	}
}

void skipSpacesAndComment(Loc& loc) {
	skipSpaces(loc);
	skipComment(loc);
}

bool readEOL(Loc& loc) {
	skipSpacesAndComment(loc);

	return loc.eof() || *(loc.pos - 1) == '\n';
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
	skipSpacesAndComment(loc);

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

bool read(Loc& loc, const std::string& what) {
	for (char c : what) {
		if (!loc.eof() && *loc.pos == c)
			++loc.pos;
		else
			return false;
	}

	if (!loc.eof() && !std::isspace(*loc.pos)) {
		loc.error("Unknown command '" + what + '[' + *loc.pos + "]'. Use spaces.");

		return false;
	}

	return true;
}

bool read(Loc& loc, char what) {
	skipSpacesAndComment(loc);

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
	skipSpacesAndComment(loc);

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
	skipSpacesAndComment(loc);

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

bool peek(Loc& loc, char what) {
	loc.bufPos();
	const bool result = read(loc, what);
	loc.unbufPos();

	return result;
}

//TODO: optimize
/*

 -- 1 -- 

 print 4

Derzeit:
	movl $4, %eax
	push %eax
	call ...
Besser:
	push $4
	call ...

================

 -- 2 --

 print a

 Derzeit:
	movl 0(%esp), %eax
	push %eax
	call ...
Besser:
	push 0(%esp)
	call ...
*/
bool parsePrintOptimized(Env& env) {
// <Optimize>
	env.loc->bufPos();

	int num;
	if (readNumber(*env.loc, &num) && readEOL(*env.loc)) {
		const bool comma = peek(*env.loc, ',');

		as::push(env.out, num);
		as::call(env.out, comma ? LabelStr.at(Label::PrintI) : LabelStr.at(Label::PrintlnI));
		as::add(env.out, ESP, 4);

		env.loc->unbuf();

		return true;
	}

	env.loc->unbufPos();
	env.loc->bufPos();

	std::string identifier;
	if (readIdentifier(*env.loc, &identifier) && readEOL(*env.loc)) {
		// std::cout << "optimized read: " << identifier << std::endl;
		const bool comma = peek(*env.loc, ',');
		const int addr = env.var->addrOf(identifier);
		assert(addr != -1);

		as::push(env.out, ESP, addr);
		as::call(env.out, comma ? LabelStr.at(Label::PrintI) : LabelStr.at(Label::PrintlnI));
		as::add(env.out, ESP, 4);

		env.loc->unbuf();

		return true;
	}

	env.loc->unbufPos();

	return false;
// </Optimize>
}

bool parsePrint(Env& env) {
	if (read(*env.loc, Tok::Print)) {
		do {
			if (!parsePrintOptimized(env)) {
				std::string str;
				if (readString(*env.loc, &str)) {
					const std::string label = env.data->addStringData(str);
					const bool comma = peek(*env.loc, ',');
					
					as::push(env.out, label);
					as::call(env.out, comma ? LabelStr.at(Label::PrintS) : LabelStr.at(Label::PrintlnS));
					as::add(env.out, ESP, 4);
				} else if (parseExpression(env)) {
					const bool comma = peek(*env.loc, ',');

					as::push(env.out, EAX);
					as::call(env.out, comma ? LabelStr.at(Label::PrintI) : LabelStr.at(Label::PrintlnI));
					as::add(env.out, ESP, 4);
				} else
					env.loc->error("Missing or invalid print argument.");
			}
		} while (read(*env.loc, ','));

		return true;
	}

	return false;
}

// DONE: optimize
/*
	var a = a
Derzeit:
	movl	$4, %eax
	movl	%eax, 0(%esp)
Besser:
	movl	$4, 0(%esp)
*/
bool parseVarAssign(Env& env, const std::string& identifier, bool duty) {
	if (read(*env.loc, '=')) {
		const int addr = env.var->addrOf(identifier);
		assert(addr != -1);

// <Optimize>
		env.loc->bufPos();

		int num;
		if (readNumber(*env.loc, &num) && readEOL(*env.loc)) {
			as::move(env.out, num, ESP, addr);

			return true;
		}

		env.loc->unbufPos();
// </Optimize>

		if (parseExpression(env)) {
			as::move(env.out, EAX, ESP, addr);

			return true;
		}

		env.loc->error("Missing right operant of '='.");

		return false;
	} else if (duty) {
		env.loc->error("Missing '=' in assignment.");

		return false;
	}

	return true;
}

bool parseVar(Env& env) {
	std::string identifier;

	 if (read(*env.loc, Tok::Var)) {
		if (readIdentifier(*env.loc, &identifier)) {
			if (env.var->create(identifier)) {
				if (parseVarAssign(env, identifier, false))
					return true;
			} else
				env.loc->error("Redefinition of variable '" + identifier + '"');
		}
	} else {
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
		as::move(env.out, num, EAX);

		return true;
	}

	std::string identifier;
	if (readIdentifier(*env.loc, &identifier)) {
		if (env.var->exist(identifier)) {
			const int addr = env.var->addrOf(identifier);
			assert(addr != -1);

			as::move(env.out, ESP, addr, EAX);

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
		as::neg(env.out, EAX);

	return true;
}

bool parseTerm(Env& env) {
	if (parseFactor(env)) {
		for (;;) {
			if (read(*env.loc, '*')) {
				as::push(env.out, EAX);

				if (!parseFactor(env)) {
					env.loc->error("Expected factor after *");

					return false;
				}

				as::mul(env.out, ESP, 0, EAX);
				as::add(env.out, ESP, 4);
			} else if (read(*env.loc, '/')) {
				as::push(env.out, EAX);

				if (!parseFactor(env)) {
					env.loc->error("Expected factor after /");

					return false;
				}

                as::move(env.out, EAX, EBX);
                as::pop(env.out, EAX);
				as::div(env.out, EBX);
			} else if (read(*env.loc, '%')) {
				as::push(env.out, EAX);

				if (!parseFactor(env)) {
					env.loc->error("Expected factor after %");

					return false;
				}

				as::move(env.out, EAX, EBX);
                as::pop(env.out, EAX);
				as::div(env.out, EBX); 
				as::move(env.out, EDX, EAX);
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
				as::push(env.out, EAX);

				if (!parseTerm(env)) {
					env.loc->error("Expected Term after +");

					return false;
				}

                as::add(env.out, ESP, 0, EAX);
                as::add(env.out, ESP, 4);
			} else if (read(*env.loc, '-')) {
				as::push(env.out, EAX);

				if (!parseTerm(env)) {
					env.loc->error("Expected Term after -");

					return false;
				}

                as::sub(env.out, EAX, ESP, 0);
                as::pop(env.out, EAX);
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