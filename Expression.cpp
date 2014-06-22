#include "Expression.hpp"

Term::~Term() {
	for (Literal* lp : this->values) {
		delete lp;
	}
}

ImmedAssign::ImmedAssign(Literal* value) {
	this->value = value;
}

ImmedAssign::~ImmedAssign() {
	delete this->value;
}

Operator::Operator(Op op) {
	this->lty = LiTy::Operator;

	this->op = op;
}

Value::Value(int value) {
	this->lty = LiTy::Value;

	this->value = value;
}

Variable::Variable(const std::string& name, int16 offset, int16 size) {
	this->lty = LiTy::Variable;

	this->name = name;
	this->offset = offset;
	this->size = size;
}

Variable::Variable(const Variable& var) {
	this->lty = LiTy::Variable;

	this->name = var.name;
	this->offset = var.offset;
	this->size = var.size;

	Expression* exp = var.exp.get();
	if (Term* t = exp->isTerm())
		this->exp = patch::make_unique<Term>(*t);
	else if (ImmedAssign* ia = exp->isImmedAssign())
		this->exp = patch::make_unique<ImmedAssign>(*ia);
	else
		assert(0);
}

Print::Print(Expression* exp, const std::string& label) {
	this->exp.reset(exp);
	this->label = label;
}

VarAssign::VarAssign(Variable* var) {
	this->var = patch::make_unique<Variable>(*var);
}