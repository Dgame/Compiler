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
	this->op = op;
}

Value::Value(int32 value) {
	this->value = value;
}

Variable::Variable(const std::string& name, int16 offset, int16 size) {
	this->name = name;
	this->offset = offset;
	this->size = size;
}

Print::Print(Expression* exp, const std::string& label) {
	this->exp.reset(exp);
	this->label = label;
}

void Print::output(std::ostream& out) {

}