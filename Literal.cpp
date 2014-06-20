#include "Literal.hpp"

Addr::Addr(int offset) {
	this->offset = offset;
}

Literal::Literal(int value) : type(Type::Numeric) {
	this->value = value;
}

Literal::Literal(Addr addr) : type(Type::Address) {
	this->addr = addr;
}

Literal::Literal(Op op) : type(Type::Operator) {
	this->op = op;
}

Literal::Literal(bool negate) : type(Type::Negate) {
	this->negate = negate;
}

void Literal::output(std::ostream& out) {
	if (this->type == Type::Numeric)
		out << this->value << std::endl;
	else if (this->type == Type::Operator)
		out << static_cast<char>(this->op) << std::endl;
	else if (this->type == Type::Address)
		out << this->addr.offset << std::endl;
	else if (this->type == Type::Negate)
		out << (this->negate ? '-' : '+') << std::endl;
}