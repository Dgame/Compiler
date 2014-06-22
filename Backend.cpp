#include "Backend.hpp"
#include "Expression.hpp"

void buildAssembler(std::ostream& out, Expression* exp) {
	if (ImmedAssign* ia = exp->isImmedAssign()) {
		if (ia->value->isOperator()) {
			std::cerr << "Cannot work with only an operator." << std::endl;

			return;
		} else if (Value* v = ia->value->isValue()) {
			as::move(out, v->value, EAX);
		} else if (Variable* v = ia->value->isVariable()) {
			as::move(out, ESP, v->offset, EAX);
		}
	} else if (Term* t = exp->isTerm()) {
		// TODO: 
	}
}

void buildAssembler(std::ostream& out, Print* print) {
	buildAssembler(out, print->exp.get());
	as::push(out, EAX);

	as::call(out, print->label);
	as::add(out, 4, ESP);
}

void buildAssembler(std::ostream& out, Variable* var) {
	if (ImmedAssign* ia = var->exp->isImmedAssign()) {
		if (ia->value->isOperator()) {
			std::cerr << "Cannot assign only an operator." << std::endl;

			return;
		} else if (Value* v = ia->value->isValue()) {
			as::move(out, v->value, ESP, var->offset);
		} else if (Variable* v = ia->value->isVariable()) {
			as::move(out, ESP, v->offset, EAX);
			as::move(out, EAX, ESP, var->offset);
		}
	} else {
		buildAssembler(out, var->exp.get());
		as::move(out, EAX, ESP, var->offset);
	}
}

void buildAssembler(std::ostream& out, Exit* exit) {
	as::ret(out);
}
