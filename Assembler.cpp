#include "Assembler.hpp"

namespace as {
	void push(std::ostream& out, int num) {
		out << "pushl\t$" << patch::to_string(num) << std::endl;
	}

	void push(std::ostream& out, Pointer ptr) {
		out << "pushl\t" << Register.at(ptr) << std::endl;
	}

	void push(std::ostream& out, Accu accu) {
		out << "pushl\t" << Register.at(accu) << std::endl;
	}

	void push(std::ostream& out, Pointer ptr, int offset) {
		out << "pushl\t" << addrOf(ptr, offset) << std::endl;
	}

	void push(std::ostream& out, const std::string& label) {
		out << "pushl\t" << label << std::endl;
	}

	void pop(std::ostream& out, Accu accu) {
		out << "popl\t" << Register.at(accu) << std::endl;
	}

	void pop(std::ostream& out, Pointer ptr) {
		out << "popl\t" << Register.at(ptr) << std::endl;
	}

	void move(std::ostream& out, int num, Accu accu) {
		out << "movl\t$" << patch::to_string(num) << ", " << Register.at(accu) << std::endl;
	}

	void move(std::ostream& out, int num, Pointer ptr, int offset) {
		out << "movl\t$" << patch::to_string(num) << ", " << addrOf(ptr, offset) << std::endl;
	}

	void move(std::ostream& out, Accu a1, Accu a2) {
		out << "movl\t" << Register.at(a1) << ", " << Register.at(a2) << std::endl;
	}

	void move(std::ostream& out, Pointer ptr1, Pointer ptr2) {
		out << "movl\t" << Register.at(ptr1) << ", " << Register.at(ptr2) << std::endl;
	}

	void move(std::ostream& out, Accu accu, Pointer ptr, int offset) {
		out << "movl\t" << Register.at(accu) << ", " << addrOf(ptr, offset) << std::endl;
	}

	void move(std::ostream& out, Pointer ptr, int offset, Accu accu) {
		out << "movl\t" << addrOf(ptr, offset) << ", " << Register.at(accu) << std::endl;
	}

	void add(std::ostream& out, int num, Pointer ptr) {
		out << "addl\t$" << patch::to_string(num) << ", " << Register.at(ptr) << std::endl;
	}

	void add(std::ostream& out, int num, Pointer ptr, int offset) {
		out << "addl\t$" << patch::to_string(num) << ", " << addrOf(ptr, offset) << std::endl;
	}

	void add(std::ostream& out, Pointer ptr, int offset, Accu accu) {
		out << "addl\t" << addrOf(ptr, offset) << ", " << Register.at(accu) << std::endl;
	}

	void add(std::ostream& out, Accu accu, Pointer ptr, int offset) {
		out << "addl\t" << Register.at(accu) << ", " << addrOf(ptr, offset) << std::endl;
	}

	void add(std::ostream& out, Accu a1, Accu a2) {
		out << "addl\t" << Register.at(a1) << ", " << Register.at(a2) << std::endl;
	}

	void add(std::ostream& out, Pointer ptr, int offset) {
		out << "addl\t$" << patch::to_string(offset) << ", " << Register.at(ptr) << std::endl;
	}

	void sub(std::ostream& out, int num, Pointer ptr) {
		out << "subl\t$" << patch::to_string(num) << ", " << Register.at(ptr) << std::endl;
	}

	void sub(std::ostream& out, int num, Pointer ptr, int offset) {
		out << "subl\t$" << patch::to_string(num) << ", " << addrOf(ptr, offset) << std::endl;
	}

	void sub(std::ostream& out, Pointer ptr, int offset, Accu accu) {
		out << "subl\t" << addrOf(ptr, offset) << ", " << Register.at(accu) << std::endl;
	}

	void sub(std::ostream& out, Accu accu, Pointer ptr, int offset) {
		out << "subl\t" << Register.at(accu) << ", " << addrOf(ptr, offset) << std::endl;
	}

	void sub(std::ostream& out, Accu a1, Accu a2) {
		out << "subl\t" << Register.at(a1) << ", " << Register.at(a2) << std::endl;
	}

	void sub(std::ostream& out, Pointer ptr, int offset) {
		out << "subl\t$" << patch::to_string(offset) << ", " << Register.at(ptr) << std::endl;
	}

	void mul(std::ostream& out, int num, Accu accu) {
		out << "imull\t" << patch::to_string(num) << ", " << Register.at(accu) << std::endl;
	}

	void mul(std::ostream& out, int num, Pointer ptr, int offset) {
		out << "imull\t" << patch::to_string(num) << ", " << addrOf(ptr, offset) << std::endl;
	}

	void mul(std::ostream& out, Accu a1, Accu a2) {
		out << "imull\t" << Register.at(a1) << ", " << Register.at(a2) << std::endl;
	}

	void mul(std::ostream& out, Accu accu, Pointer ptr, int offset) {
		out << "imull\t" << Register.at(accu) << ", " << addrOf(ptr, offset) << std::endl;
	}

	void mul(std::ostream& out, Pointer ptr, int offset, Accu accu) {
		out << "imull\t" << addrOf(ptr, offset) << ", " << Register.at(accu) << std::endl;
	}

	void mul(std::ostream& out, Pointer ptr1, int offset1, Pointer ptr2, int offset2) {
		out << "imull\t" << addrOf(ptr1, offset1) << ", " << addrOf(ptr2, offset2) << std::endl;
	}

	void div(std::ostream& out, Accu accu) {
		out << "idiv\t" << Register.at(accu) << std::endl;
	}

	void neg(std::ostream& out, Accu accu) {
		out << "negl\t" << Register.at(accu) << std::endl;
	}

	void neg(std::ostream& out, Pointer ptr, int offset) {
		out << "negl\t" << addrOf(ptr, offset) << std::endl;
	}

	void call(std::ostream& out, const std::string& label) {
		out << "call\t" << label << std::endl;
	}

	void ret(std::ostream& out) {
		out << "ret" << std::endl;
	}

	void jump(std::ostream& out, const std::string& label) {
		out << "jmp\t" << label << std::endl;
	}

	void jump(std::ostream& out, Flag flag, const std::string& label) {
		std::string op = "j";

		switch (flag) {
			case Flag::Zero:
				op += "z";
			break;

			case Flag::Equal:
				op += "e";
			break;

			case Flag::NotEqual:
				op += "ne";
			break;

			case Flag::Above:
				op += "a";
			break;

			case Flag::AboveEq:
				op += "ae";
			break;

			case Flag::Below:
				op += "b";
			break;

			case Flag::BelowEq:
				op += "be";
			break;

			case Flag::Overflow:
				op += "o";
			break;

			case Flag::Carry:
				op += "c";
			break;

			case Flag::NotCarry:
				op += "nc";
			break;
		}

		out << op << "\t\t" << label << std::endl;
	}

	void set(std::ostream& out, Flag flag, Accu accu) {
		std::string op = "set";

		switch (flag) {
			case Flag::Zero:
				op += "z";
			break;

			case Flag::Equal:
				op += "e";
			break;

			case Flag::NotEqual:
				op += "ne";
			break;

			case Flag::Above:
				op += "a";
			break;

			case Flag::AboveEq:
				op += "ae";
			break;

			case Flag::Below:
				op += "b";
			break;

			case Flag::BelowEq:
				op += "be";
			break;

			case Flag::Overflow:
				op += "o";
			break;

			case Flag::Carry:
				op += "c";
			break;

			case Flag::NotCarry:
				op += "nc";
			break;
		}

		out << op << " \t" << Register.at(accu) << std::endl;
	}

	void cmp(std::ostream& out, Accu a1, Accu a2) {
		out << "cmpl\t" << Register.at(a1) << ", " << Register.at(a2) << std::endl;
	}

	void cmp(std::ostream& out, Accu accu, Pointer ptr, int offset) {
		out << "cmpl\t" << Register.at(accu) << ", " << addrOf(ptr, offset) << std::endl;
	}

	void op_and(std::ostream& out, Accu a1, Accu a2) {
		out << "and\t" << Register.at(a1) << ", " << Register.at(a2) << std::endl;
	}

	void op_and(std::ostream& out, Accu accu, Pointer ptr, int offset) {
		out << "and\t" << Register.at(accu) << ", " << addrOf(ptr, offset) << std::endl;
	}

	void op_or(std::ostream& out, Accu a1, Accu a2) {
		out << "or\t" << Register.at(a1) << ", " << Register.at(a2) << std::endl;
	}

	void op_or(std::ostream& out, Accu accu, Pointer ptr, int offset) {
		out << "and\t" << Register.at(accu) << ", " << addrOf(ptr, offset) << std::endl;
	}
	
	void op_xor(std::ostream& out, Accu a1, Accu a2) {
		out << "xor\t" << Register.at(a1) << ", " << Register.at(a2) << std::endl;
	}

	void op_xor(std::ostream& out, Accu accu, Pointer ptr, int offset) {
		out << "and\t" << Register.at(accu) << ", " << addrOf(ptr, offset) << std::endl;
	}
	
	void op_not(std::ostream& out, Accu accu) {
		out << "notl\t" << Register.at(accu) << std::endl;
	}

	void test(std::ostream& out, Accu a1, Accu a2) {
		out << "testl\t" << Register.at(a1) << ", " << Register.at(a2) << std::endl;
	}

	void start(std::ostream& out) {
		out << ".text\n"
			<< ".globl _prog\n"
			<< "_prog:\n" << std::endl;

		push(out, EBP);
		move(out, ESP, EBP);

		out << std::endl;
	}

	void end(std::ostream& out) {
		out << std::endl;

		pop(out, EBP);
		ret(out);

		out << "\n\n.data" << std::endl;
	}
}