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
		loc.error("Unknown command.");

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

			if (parseNumericExpression(env)) {
				const bool comma = peek(*env.loc, ',');
				const RTLabel label = comma ? RTLabel::PrintI : RTLabel::PrintlnI;

				auto print = patch::make_unique<Print>(env.exp.release(), RTLabelStr.at(label));
				env.scope->addCmd(print.release());
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
			if (!env.scope->createVar(identifier)) {
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

		if (!parseNumericExpression(env)) {
			env.loc->error("Invalid assignment.");

			return false;
		}

		if (Variable* var = env.scope->getVar(name)) {
			var->exp.reset(env.exp.release());

			env.scope->addCmd(new VarAssign(var));

			return true;
		}

		env.loc->error("Variable '" + name + "' does not exist.");

		return false;
	}

	return false;
}

bool parseExit(Env& env) {
	if (read(*env.loc, Tok::Exit)) {
		env.scope->addCmd(new Exit());

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
		if (Variable* var = env.scope->getVar(identifier)) {
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
		if (!parseNumericExpression(env)) {
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

bool parseNumericExpression(Env& env) {
	if (parseTerm(env)) {
		while (true) {
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
		if (t != nullptr && t->count() == 1) {
			env.exp.reset(new ImmedAssign(t->pop()));
		}

		return true;
	}

	return false;
}

bool parseCompareExpression(Env& env) {
	env.exp = patch::make_unique<Term>();

	if (parseNumericExpression(env)) {
		Cmp cmp;

		if (read(*env.loc, Tok::Equal))
			cmp = Cmp::Equal;
		else if (read(*env.loc, Tok::NotEqual))
			cmp = Cmp::NotEqual;
		else if (read(*env.loc, Tok::LessOrEqual))
			cmp = Cmp::LessOrEqual;
		else if (read(*env.loc, Tok::GreaterOrEqual))
			cmp = Cmp::GreaterOrEqual;
		else if (read(*env.loc, Tok::Less))
			cmp = Cmp::Less;
		else if (read(*env.loc, Tok::Greater))
			cmp = Cmp::Greater;
		else {
			env.loc->error("Unknown compare operator");

			return false;
		}

		Expression* lhs = env.exp.release();
		env.exp = patch::make_unique<Term>();

		if (parseNumericExpression(env)) {
			Expression* rhs = env.exp.release();
			env.exp = patch::make_unique<Compare>(lhs, cmp, rhs);

			return true;
		}

		env.loc->error("Expected right-hand-side expression.");
	}

	return false;
}

bool parseBooleanExpression(Env& env) {
	if (parseCompareExpression(env)) {
		Compare* cexp = env.exp.release()->isCompare();
		assert(cexp != nullptr);

		std::unique_ptr<Join> join = patch::make_unique<Join>(cexp);

		while (true) {
			Link lnk;

			if (read(*env.loc, Tok::And))
				lnk = Link::And;
			else if (read(*env.loc, Tok::Or))
				lnk = Link::Or;
			else if (read(*env.loc, Tok::Xor))
				lnk = Link::Xor;
			else
				break;
			
			if (!parseCompareExpression(env)) {
				env.loc->error("Expected expression after linkage.");

				return false;
			}

			Compare* cexp = env.exp.release()->isCompare();
			assert(cexp != nullptr);

			join->exp[lnk] = std::unique_ptr<Compare>(cexp);
		}

		env.exp = std::move(join);

		return true;
	}

	return false;
}

bool parseBlock(Env& env) {
	if (read(*env.loc, '{')) {
		env.scope->pushScope();

		while (parseCommand(env)) {

		}

		if (read(*env.loc, '}')) {
			env.scope->popScope();

			return true;
		} else {
			env.loc->error("Missing '}'.");

			return false;
		}
	}

	return false;
}

bool parseIf(Env& env) {
	if (read(*env.loc, Tok::If)) {
		if (parseBooleanExpression(env)) {
			if (parseBlock(env)) {
				Join* join = env.exp.release()->isJoin();
				assert(join != nullptr);

				auto _if = patch::make_unique<If>(join, env.scope->lastLabel);

				if (read(*env.loc, Tok::Else)) {
                    if (!parseBlock(env)) {
                    	env.loc->error("Missing else-block.");

                    	return false;
                    }

                    _if->elseLabel = env.scope->lastLabel;
                }

                env.scope->addCmd(_if.release());

				return true;
			} else {
				env.loc->error("Missing if-block.");
			}
		} else {
			env.loc->error("Invalid expression for if.");
		}
	}

	return false;
}

bool parseCommand(Env& env) {
	if (parsePrint(env)) {
		return true;
	} else if (parseIf(env)) {
		return true;
	} else if (parseVar(env)) {
	 	return true;
	} else if (parseExit(env)) {
		return false;
	}

	return false;
}