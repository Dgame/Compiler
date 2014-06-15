#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

namespace {
	enum Accu {
		EAX = 1,
		EBX = 2,
		ECX = 3,
		EDX = 4,
	};

	enum Index {
		ESI = 5,
		EDI = 6
	};

	enum Pointer {
		EBP = 7,
		ESP = 8,
		EIP = 9
	};

	const std::map<int, const std::string> Register = {
		{EAX, "%eax"},
		{EBX, "%ebx"},
		{ECX, "%ecx"},
		{EDX, "%edx"},

		{ESI, "%esi"},
		{EDI, "%edi"},

		{EBP, "%ebp"},
		{ESP, "%esp"},
		{EIP, "%eip"},
	};
	
	std::string numToStr(int number) {
		std::ostringstream ss;
		ss << number;
		
		return ss.str();
	}

	std::string addr(Pointer ptr, int offset) {
		if (offset == 0)
			return '(' + Register.at(ptr) + ')';
		return numToStr(offset) + '(' + Register.at(ptr) + ')';
	}

	void move(std::ostream& out, Pointer p1, Pointer p2) {
		out << "movl\t" << Register.at(p1) << ", " << Register.at(p2) << std::endl;
	}

	void move(std::ostream& out, Accu acc1, Accu acc2) {
		out << "movl\t" << Register.at(acc1) << ", " << Register.at(acc2) << std::endl;
	}

	void move(std::ostream& out, int num, Accu accu) {
		out << "movl\t$" << numToStr(num) << ", " << Register.at(accu) << std::endl;
	}

	void move(std::ostream& out, Accu accu, Pointer ptr, int offset = 0) {
		out << "movl\t" << Register.at(accu) << ", " << addr(ptr, offset) << std::endl;
	}

	void move(std::ostream& out, int num, Pointer ptr, int offset = 0) {
		out << "movl\t$" << numToStr(num) << ", " << addr(ptr, offset) << std::endl;
	}

	void move(std::ostream& out, Pointer ptr, Accu accu, int offset = 0) {
		out << "movl\t" << addr(ptr, offset) << ", " << Register.at(accu) << std::endl;
	}

	void add(std::ostream& out, int num, Pointer ptr) {
		out << "addl\t$" << numToStr(num) << ", " << Register.at(ptr) << std::endl;
	}

	void add(std::ostream& out, Pointer ptr, Accu accu, int offset = 0) {
		out << "addl\t" << addr(ptr, offset) << ", " << Register.at(accu) << std::endl;

		add(out, 4, ptr);
	}

	void sub(std::ostream& out, int num, Pointer ptr) {
		out << "subl\t$" << numToStr(num) << ", " << Register.at(ptr) << std::endl;
	}

	void sub(std::ostream& out, Pointer ptr, Accu accu, int offset = 0) {
		out << "subl\t" << addr(ptr, offset) << ", " << Register.at(accu) << std::endl;

		add(out, 4, ptr);
	}

	void push(std::ostream& out, Accu accu) {
		out << "push\t" << Register.at(accu) << std::endl;
	}

	void push(std::ostream& out, Index index) {
		out << "push\t" << Register.at(index) << std::endl;
	}

	void push(std::ostream& out, Pointer ptr) {
		out << "push\t" << Register.at(ptr) << std::endl;
	}

	void push(std::ostream& out, int val) {
		out << "push\t$" << numToStr(val) << std::endl;
	}

	void push(std::ostream& out, const std::string& label) {
		out << "push\t$" << label << std::endl;
	}

	void pop(std::ostream& out, Accu accu) {
		out << "pop \t" << Register.at(accu) << std::endl;
	}

	void pop(std::ostream& out, Index index) {
		out << "pop \t" << Register.at(index) << std::endl;
	}

	void pop(std::ostream& out, Pointer ptr) {
		out << "pop \t" << Register.at(ptr) << std::endl;
	}

	void neg(std::ostream& out, Accu accu) {
		out << "negl\t" << Register.at(accu) << std::endl;
	}

	void neg(std::ostream& out, Pointer ptr, int offset = 0) {
		out << "negl\t" << addr(ptr, offset) << std::endl;
	}

	void mul(std::ostream& out, Pointer ptr, Accu accu, int offset = 0) {
		out << "imull\t" << addr(ptr, offset) << ", " << Register.at(accu) << std::endl;

		add(out, 4, ESP);
	}

	void div(std::ostream& out, Accu accu) {
		if (accu != EBX)
			move(out, accu, EBX);
		pop(out, accu);
		move(out, 0, EDX);

		out << "idiv\t" << Register.at(EBX) << std::endl;
	}

	void mod(std::ostream& out, Accu accu) {
		div(out, accu);
		move(out, EDX, accu);
	}

	void call(std::ostream& out, const std::string& label) {
		out << "call\t" << label << std::endl;

		add(out, 4, ESP);
	}

	void start(std::ostream& out);
	void end(std::ostream& out);
}

class Assembler final {
private:
	std::ostream* _out;
	std::ostringstream _buf;

	unsigned int _stackSize = 0;
	bool _leaved = false;

public:
	explicit Assembler(std::ostream& out);
	virtual ~Assembler();

	void ret();

	unsigned int getStackSize() const {
		return _stackSize;
	}

	void push() {
		::push(_buf, EAX);
	}

	void pop() {
		::pop(_buf, EAX);
	}

	void move(int num) {
		::move(_buf, num, EAX);
	}

	void assignVar(unsigned int offset) {
		_stackSize += 4;

		::move(_buf, EAX, ESP, offset);
	}

	void useVar(unsigned int offset) {
		::move(_buf, ESP, EAX, offset);
	}

	void add() {
		::add(_buf, ESP, EAX, _stackSize);
	}

	void sub() {
		::sub(_buf, ESP, EAX, _stackSize);
	}

	void mul() {
		::mul(_buf, ESP, EAX, _stackSize);
	}

	void div() {
		::div(_buf, EAX);
	}

	void mod() {
		::mod(_buf, EAX);
	}

	void neg() {
		::neg(_buf, EAX);
	}

	void neg(int offset) {
		::neg(_buf, ESP, offset);
	}

	void call(const std::string& func) {
		::push(_buf, EAX);
		::call(_buf, func);
	}

	void call(const std::string& func, const std::string& label) {
		::push(_buf, label);
		::call(_buf, func);
	}
};

#endif