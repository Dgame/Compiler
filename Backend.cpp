#include "Backend.hpp"
#include "Expression.hpp"

void buildAssembler(std::ostream& out, Expression* exp) {
	if (ImmedAssign* ia = exp->isImmedAssign()) {
		if (Value* v = ia->literal->isValue()) {
			as::move(out, v->value, EAX);
		} else if (Variable* v = ia->literal->isVariable()) {
			as::move(out, ESP, v->offset, EAX);
		}
	} else if (Term* t = exp->isTerm()) {
		int pushed = 0;
		Variable* cur_var = nullptr;

		std::stack<Variable*> variables;

		while (Literal* lit = t->pop()) {
			if (Variable* var = lit->isVariable()) {
				Literal* next = t->at(1);

				if (t->top() != nullptr && t->top()->isOperator()) {
					cur_var = var;
				} else if (next != nullptr && next->isOperator()) {
					std::cout << "Move -> " << var->offset << std::endl;
					as::move(out, ESP, var->offset, EAX);
				} else {
					std::cout << "Push to stack -> " << var->offset << std::endl;
					variables.push(var);
				}
			} else if (Value* val = lit->isValue()) {
				if (t->top() == nullptr 
					|| t->top()->isOperator() 
					|| t->top()->isVariable())
				{
					as::move(out, val->value, EAX);
				} else {
					pushed++;
					as::push(out, val->value);
				}
			} else if (Operator* op = lit->isOperator()) {
				const int offset = cur_var != nullptr ? cur_var->offset : 0;

				switch (op->op) {
					case Op::Plus:
						as::add(out, ESP, offset, EAX);

						if (pushed != 0) {
							as::add(out, ESP, 4);
							--pushed;
						}
					break;

					case Op::Minus:
						// swap operation, because variables aren't on the stack
						if (cur_var == nullptr)
							as::sub(out, EAX, ESP, offset);
						else
							as::sub(out, ESP, offset, EAX);

						if (pushed != 0) {
							as::pop(out, EAX);
							--pushed;
						}
					break;

					case Op::Mul:
						as::mul(out, ESP, offset, EAX);

						if (pushed != 0) {
							as::add(out, ESP, 4);
							--pushed;
						}
					break;

					case Op::Div:
						as::move(out, 0, EDX);

						// swap operation, because variables aren't on the stack
						if (cur_var == nullptr) {
							as::move(out, EAX, EBX);
							as::pop(out, EAX);
						} else {
							as::move(out, ESP, offset, EBX);
						}

						as::div(out, EBX);

						// Da wir ja etwas popen
						--pushed;
					break;

					case Op::Mod:
						as::move(out, 0, EDX);

						// swap operation, because variables aren't on the stack
						if (cur_var == nullptr) {
							as::move(out, EAX, EBX);
							as::pop(out, EAX);
						} else {
							as::move(out, ESP, offset, EBX);
						}

						as::div(out, EBX);
						as::move(out, EDX, EAX);

						// Da wir ja etwas popen
						--pushed;
					break;

					case Op::Negate:
						as::neg(out, EAX);
					break;
				}

				if (op->op != Op::Negate) {
					if (!variables.empty()) {
						cur_var = variables.top();
						variables.pop();
					} else
						cur_var = nullptr;

					if (t->top() != nullptr && t->top()->isValue()) {
						as::push(out, EAX);
						pushed++;
					}
				}
			}
		}
	}
}

void buildAssembler(std::ostream& out, Print* print) {
	if (ImmedAssign* ia = print->exp->isImmedAssign()) {
		if (Value* v = ia->literal->isValue()) {
			as::push(out, v->value);
		} else if (Variable* v = ia->literal->isVariable()) {
			as::push(out, ESP, v->offset);
		}
	} else {
		buildAssembler(out, print->exp.get());
		as::push(out, EAX);
	}

	as::call(out, print->label);
	as::add(out, 4, ESP);
}

void buildAssembler(std::ostream& out, VarAssign* va) {
	Variable* var = va->var.get();

	if (ImmedAssign* ia = var->exp->isImmedAssign()) {
		if (Value* v = ia->literal->isValue()) {
			as::move(out, v->value, ESP, var->offset);
		} else if (Variable* v = ia->literal->isVariable()) {
			as::move(out, ESP, v->offset, EAX);
			as::move(out, EAX, ESP, var->offset);
		}
	} else {
		buildAssembler(out, var->exp.get());
		as::move(out, EAX, ESP, var->offset);
	}
}

void buildAssembler(std::ostream& out, Exit* exit) {
	// as::ret(out);
}

void buildAssembler(std::ostream& out, If* _if) {
	Join* join = _if->jexp.get();

	int flag = static_cast<int>(join->mexp->cmp) * FLAG_FACTOR;

	Expression* exp = join->mexp->lhs.get();
	if (exp != nullptr) {
		buildAssembler(out, exp);
	}

	exp = join->mexp->rhs.get();
	if (exp != nullptr) {
		as::push(out, EAX);
		buildAssembler(out, exp);
	}

	as::cmp(out, EAX, ESP, 0);
	as::move(out, 0, EAX);
	as::set(out, static_cast<Flag>(flag), AL);
	as::add(out, 4, ESP);

	for (auto& pair : join->exp) {
		switch (pair.first) {
			case Link::And:
				as::op_and(out, EAX, ESP, 0);
			break;

			case Link::Or:
				as::op_or(out, EAX, ESP, 0);
			break;

			case Link::Xor:
				as::op_xor(out, EAX, ESP, 0);
			break;
		}

		Compare* cmp = pair.second.get();

		flag = static_cast<int>(cmp->cmp) * FLAG_FACTOR;

		Expression* exp = cmp->lhs.get();
		if (exp != nullptr) {
			buildAssembler(out, exp);
		}

		exp = cmp->rhs.get();
		if (exp != nullptr) {
			as::push(out, EAX);
			buildAssembler(out, exp);
		}

		as::cmp(out, EAX, ESP, 0);
		as::move(out, 0, EAX);
		as::set(out, static_cast<Flag>(flag), AL);
		as::add(out, 4, ESP);
	}

	as::test(out, EAX, EAX);
	as::jump(out, Flag::Zero, _if->ifLabel);

	if (_if->elseLabel.size() != 0)
		as::jump(out, _if->elseLabel);
}

void buildAssembler(std::ostream& out, Command* cmd) {
	if (Print* p = cmd->isPrint()) {
		buildAssembler(out, p);
	} else if (VarAssign* va = cmd->isVarAssign()) {
		buildAssembler(out, va);
	} else if (Exit* e = cmd->isExit()) {
		buildAssembler(out, e);
	} else if (If* _if = cmd->isIf()) {
		buildAssembler(out, _if);
	}

	out << std::endl;
}
