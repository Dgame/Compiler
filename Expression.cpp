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

Value::Value(int32 value) {
	this->lty = LiTy::Value;

	this->value = value;
}

Variable::Variable(const std::string& name, int16 offset, int16 size) {
	this->lty = LiTy::Variable;

	this->name = name;
	this->offset = offset;
	this->size = size;
}

Print::Print(Expression* exp, const std::string& label) {
	this->exp.reset(exp);
	this->label = label;
}