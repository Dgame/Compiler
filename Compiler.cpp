#include "Compiler.hpp"

bool skipComment(Loc& loc) {
	if (!loc.eof() && *loc.pos == '#') {
		do {
			loc.pos++;
		} while (!loc.eof() && *loc.pos != '\n');

		return true;
	}

	return false;
}

void skipSpaces(Loc& loc) {
	while (!loc.eof() && std::isspace(*loc.pos)) {
		if (*loc.pos == '\n')
			loc.lineNr++;
		++loc.pos;
	}

	while (skipComment(loc)) {

	}

	if (!loc.eof() && std::isspace(*loc.pos))
		return skipSpaces(loc);
}

bool read(Loc& loc, const std::string& what) {
	skipSpaces(loc);

	for (char c : what) {
		if (!loc.eof() && *loc.pos == c)
			++loc.pos;
		else
			return false;
	}

	if (!loc.eof() && !std::isspace(*loc.pos)) {
		return false;
	}

	return true;
}

bool read(Loc& loc, char what) {
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

bool peek(Loc& loc, char what) {
	skipSpaces(loc);

	return *loc.pos == what;
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
			env.exp = patch::make_unique<Term>();

			if (parseExpression(env)) {
				const bool comma = peek(*env.loc, ',');
				const Label label = comma ? Label::PrintI : Label::PrintlnI;

				env.commands.emplace_back(patch::make_unique<Print>(env.exp.release(), LabelStr.at(label)));
			} else
				env.loc->error("Missing or invalid print argument.");
		} while (read(*env.loc, ','));

		return true;
	}

	return false;
}

bool parseVar(Env& env) {
	std::string identifier;

	if (read(*env.loc, Tok::Var)) {
		if (readIdentifier(*env.loc, &identifier)) {
			if (!env.varManager->createVar(identifier)) {
				env.loc->error("Redefinition of variable '" + identifier + '\'');

				return false;
			}

			if (peek(*env.loc, '=')) {
				if (!parseVarAssign(env, identifier)) {
					env.loc->error("Missing assignment for Variable '" + identifier + "'.");

					return false;
				}
			}

			return true;
		}
	} else {
		if (readIdentifier(*env.loc, &identifier)) {
			if (parseVarAssign(env, identifier))
				return true;
		}
	}

	return false;
}

bool parseVarAssign(Env& env, const std::string& name) {
	if (read(*env.loc, '=')) {
		env.exp = patch::make_unique<Term>();

		if (!parseExpression(env)) {
			env.loc->error("Invalid assignment.");

			return false;
		}

		if (Variable* var = env.varManager->getVar(name)) {
			var->exp.reset(env.exp.release());

			env.commands.emplace_back(patch::make_unique<VarAssign>(var));

			return true;
		}

		env.loc->error("Variable '" + name + "' does not exist.");

		return false;
	}

	return false;
}

bool parseExit(Env& env) {
	if (read(*env.loc, Tok::Exit)) {
		env.commands.emplace_back(patch::make_unique<Exit>());

		return true;
	}

	return false;
}

bool parseLiteral(Env &env) {
	int num;
	if (readNumber(*env.loc, &num)) {
		env.exp->assign(new Value(num));

		return true;
	}

	std::string identifier;
	if (readIdentifier(*env.loc, &identifier)) {
		if (Variable* var = env.varManager->getVar(identifier)) {
			// env.exp->assign(new Variable(*var));
			env.exp->assign(var);

			return true;
		}
	}

	return false;
}

bool parseFactor(Env& env) {
	bool negate = false;
	if (read(*env.loc, '-')) {
		negate = true;
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

	if (negate) {
		env.exp->assign(new Operator(Op::Negate));
	}

	return true;
}

bool parseTerm(Env& env) {
	if (parseFactor(env)) {
		for (;;) {
			if (read(*env.loc, '*')) {
				if (!parseFactor(env)) {
					env.loc->error("Expected factor after *");

					return false;
				}

				env.exp->assign(new Operator(Op::Mul));
			} else if (read(*env.loc, '/')) {
				if (!parseFactor(env)) {
					env.loc->error("Expected factor after /");

					return false;
				}

				env.exp->assign(new Operator(Op::Div));
			} else if (read(*env.loc, '%')) {
				if (!parseFactor(env)) {
					env.loc->error("Expected factor after %");

					return false;
				}

				env.exp->assign(new Operator(Op::Mod));
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
				if (!parseTerm(env)) {
					env.loc->error("Expected Term after +");

					return false;
				}

				env.exp->assign(new Operator(Op::Plus));
			} else if (read(*env.loc, '-')) {
				if (!parseTerm(env)) {
					env.loc->error("Expected Term after -");

					return false;
				}

				env.exp->assign(new Operator(Op::Minus));
			} else
				break;
		}

		Term* t = env.exp->isTerm();
		if (t != nullptr && t->values.size() == 1) {
			env.exp.reset(new ImmedAssign(t->pop()));
		}

		return true;
	}

	return false;
}

bool parseCommand(Env& env) {
	if (parsePrint(env)) {
		// std::cout << "Parsed print" << std::endl;
		/*
		for (auto& uq : env.commands) {
			if (Print* p = uq->isPrint()) {
				if (Term* t = p->exp->isTerm()) {
					while (Literal* lit = t->pop()) {
						lit->output(std::cout);
					}
				}
			}
		}
		*/
		return true;
	} else if (parseVar(env)) {
		// std::cout << "Parsed Var" << std::endl;
		/*
		for (auto& kv : env.varManager->variables) {
			std::cout << kv.first << std::endl;
		}
		*/
		return true;
	} else if (parseExit(env)) {
		return false;
	}

	return false;
}