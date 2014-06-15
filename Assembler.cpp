#include "Assembler.hpp"

namespace {
	void start(std::ostream& out) {
		out << ".text\n"
			<< ".globl _prog\n\n"
			<< "_prog:" << std::endl;

		push(out, EBP);
		move(out, ESP, EBP);

		out << std::endl;
	}

	void end(std::ostream& out) {
		out << std::endl;

		pop(out, EBP);

		out << "ret" << std::endl;
	}
}

Assembler::Assembler(std::ostream& out) {
	_out = &out;

	::start(out);
}

Assembler::~Assembler() {
	if (!_leaved)
		this->ret();
}

void Assembler::ret() {
	if (_stackSize > 0)
		::sub(*_out, _stackSize, ESP);

	*_out << _buf.str();

	if (_stackSize > 0)
		::add(*_out, _stackSize, ESP);

	::end(*_out);

	_leaved = true;
}