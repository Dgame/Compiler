#include "Expression.hpp"

Term::~Term() {
	for (Literal* lp : this->literals) {
		delete lp;
	}
}

Literal* Term::top() const {
	if (this->literals.size() == 0)
		return nullptr;

	return this->literals.front();
}

Literal* Term::at(uint16 index) const {
	if (this->literals.size() <= index)
		return nullptr;

	auto it = this->literals.begin();

	return *std::next(it, index);
}

Literal* Term::pop() {
	if (this->literals.size() == 0)
		return nullptr;

	Literal* lp = this->top();
	this->literals.pop_front();

	return lp;
}

ImmedAssign::ImmedAssign(Literal* lp) : literal(lp) {

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

VarAssign::VarAssign(const Variable* vp) {
	this->var = patch::make_unique<Variable>(*vp);
}

Compare::Compare(Expression* eplhs, Cmp cmp, Expression* eprhs) : lhs(eplhs), rhs(eprhs) {
	this->cmp = cmp;
}

Join::Join(Compare* cep) : mexp(cep) {

}

If::If(Join* jep, const std::string& ifL) : jexp(jep), ifLabel(ifL) {

}