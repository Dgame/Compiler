#include "Compiler.hpp"
#include "Assembler.hpp"
#include "VarManager.hpp"
#include "DataSection.hpp"

Loc::Loc(const char* _pos, const char* const _end) : pos(_pos), end(_end) {

}

void buildAssembler(Env& env, Literal::Type* lty) {
	while (env.mathExp.size() != 0) {
		const Literal le = env.mathExp.front();
		env.mathExp.pop_front();

		if (lty != nullptr)
			*lty = le.type;

		const bool isLastRun = env.mathExp.size() == 0;

		if (le.type == Literal::Type::Numeric) {
			if (!isLastRun && env.mathExp.front().type == Literal::Type::Operator)
				as::move(env.out, le.value, EAX);
			else
				as::push(env.out, le.value);
		} else if (le.type == Literal::Type::Address) {
			if (!isLastRun && env.mathExp.front().type == Literal::Type::Operator)
				as::move(env.out, EAX, ESP, le.value);
			else
				as::push(env.out, ESP, le.value);
		} else if (le.type == Literal::Type::Operator) {
			switch (le.op) {
				case Op::Plus:
					as::add(env.out, ESP, 0, EAX);
					as::add(env.out, ESP, 4);
				break;

				case Op::Minus:
					as::sub(env.out, EAX, ESP, 0);
					as::pop(env.out, EAX);
				break;

				case Op::Mul:
					as::mul(env.out, ESP, 0, EAX);
					as::add(env.out, ESP, 4);
				break;

				case Op::Div:
					as::move(env.out, EAX, EBX);
					as::pop(env.out, EAX);
					as::div(env.out, EBX);
				break;

				case Op::Mod:
					as::move(env.out, EAX, EBX);
					as::pop(env.out, EAX);
					as::div(env.out, EBX); 
					as::move(env.out, EDX, EAX);
				break;
			}

			if (!isLastRun && 
				(env.mathExp.front().type == Literal::Type::Numeric || 
				 env.mathExp.front().type == Literal::Type::Address))
			{
				as::push(env.out, EAX);
			}
		} else if (le.type == Literal::Type::Negate) {
			if (!isLastRun && env.mathExp.front().type == Literal::Type::Numeric) {
				if (le.negate)
					env.mathExp.front().value *= -1;
			} else
				env.loc->error("Cannot negate non numeric literal.");
		}
	}
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

bool readEOL(Loc& loc) {
	skipSpacesAndComment(loc);

	return loc.eof() || *(loc.pos - 1) == '\n';
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

bool parsePrint(Env& env) {
	if (read(*env.loc, Tok::Print)) {
		do {
			std::string str;
			
			if (readString(*env.loc, &str)) {
				const std::string label = env.data->addStringData(str);
				const bool comma = peek(*env.loc, ',');
				
				as::push(env.out, label);
				as::call(env.out, comma ? LabelStr.at(Label::PrintS) : LabelStr.at(Label::PrintlnS));
				as::add(env.out, ESP, 4);
			} else if (parseExpression(env)) {
				const bool comma = peek(*env.loc, ',');

				Literal::Type lty;
				buildAssembler(env, &lty);

				if (lty == Literal::Type::Operator)
					as::push(env.out, EAX);

				as::call(env.out, comma ? LabelStr.at(Label::PrintI) : LabelStr.at(Label::PrintlnI));
				as::add(env.out, ESP, 4);
			} else {
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
		// as::move(env.out, num, EAX);
		env.mathExp.push_back(Literal(num));

		return true;
	}

	std::string identifier;
	if (readIdentifier(*env.loc, &identifier)) {
		if (env.var->exist(identifier)) {
			const int addr = env.var->addrOf(identifier);
			assert(addr != -1);

			// as::move(env.out, ESP, addr, EAX);
			env.mathExp.push_back(Literal(Addr(addr)));

			return true;
		}

		env.loc->error("Undefinied variable '" + identifier + '"');
	}

	return false;
}

bool parseFactor(Env& env) {
	// bool negate = false;
	if (read(*env.loc, '-')) {
		// negate = true;
		env.mathExp.push_back(Literal(true));
	}

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

	// if (negate)
		// as::neg(env.out, EAX);

	return true;
}

bool parseTerm(Env& env) {
	if (parseFactor(env)) {
		for (;;) {
			if (read(*env.loc, '*')) {
				// as::push(env.out, EAX);

				if (!parseFactor(env)) {
					env.loc->error("Expected factor after *");

					return false;
				}

				// as::mul(env.out, ESP, 0, EAX);
				// as::add(env.out, ESP, 4);

				env.mathExp.push_back(Literal(Op::Mul));
			} else if (read(*env.loc, '/')) {
				// as::push(env.out, EAX);

				if (!parseFactor(env)) {
					env.loc->error("Expected factor after /");

					return false;
				}

				// as::move(env.out, EAX, EBX);
				// as::pop(env.out, EAX);
				// as::div(env.out, EBX);

				env.mathExp.push_back(Literal(Op::Div));
			} else if (read(*env.loc, '%')) {
				// as::push(env.out, EAX);

				if (!parseFactor(env)) {
					env.loc->error("Expected factor after %");

					return false;
				}

				// as::move(env.out, EAX, EBX);
				// as::pop(env.out, EAX);
				// as::div(env.out, EBX); 
				// as::move(env.out, EDX, EAX);

				env.mathExp.push_back(Literal(Op::Mod));
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
				// as::push(env.out, EAX);

				if (!parseTerm(env)) {
					env.loc->error("Expected Term after +");

					return false;
				}

				// as::add(env.out, ESP, 0, EAX);
				// as::add(env.out, ESP, 4);

				env.mathExp.push_back(Literal(Op::Plus));
			} else if (read(*env.loc, '-')) {
				// as::push(env.out, EAX);

				if (!parseTerm(env)) {
					env.loc->error("Expected Term after -");

					return false;
				}

				// as::sub(env.out, EAX, ESP, 0);
				// as::pop(env.out, EAX);
				env.mathExp.push_back(Literal(Op::Minus));
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