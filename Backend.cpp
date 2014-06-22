#include "Backend.hpp"
#include "Expression.hpp"

void buildAssembler(std::ostream& out, Expression* exp) {
	if (ImmedAssign* ia = exp->isImmedAssign()) {

	} else if (Term* t = exp->isTerm()) {

	}
}

void buildAssembler(std::ostream& out, Print* print) {
	buildAssembler(out, print->exp.get());

	as::push(out, EAX);
	as::call(out, print->label);
	as::add(out, 4, ESP);
}

void buildAssembler(std::ostream& out, Variable* var) {
	buildAssembler(out, var->exp.get());

	as::move(out, EAX, ESP, var->offset);
}

void buildAssembler(std::ostream& out, Exit* exit) {
	as::ret(out);
}
