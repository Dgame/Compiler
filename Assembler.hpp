#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <map>

enum Cond {
	OVERFLOW,
	ZERO,
	EQ,
	N_EQ,
	ABV,
	ABV_EQ,
	BLW,
	BLW_EQ
};

enum Accu {
	RAX = 1, // 64 Bit
	EAX = 2, // 32 Bit
	 AX = 3, // 16 Bit
	 AH = 4,
	 AL = 5,
	RBX = 6, // 64 Bit
	EBX = 7, // 32 Bit
	 BX = 8, // 16 Bit
	 BH = 9,
	 BL = 10,
	RCX = 11, // 64 Bit
	ECX = 12, // 32 Bit
	 CX = 13, // 16 Bit
	 CH = 14,
	 CL = 15,
	RDX = 16, // 64 Bit
	EDX = 17, // 32 Bit
	 DX = 18, // 16 Bit
	 DH = 19,
	 DL = 20,
};

enum Index {
	RSI = 21, // 64 Bit
	ESI = 22, // 32 Bit
	 SI = 23,
	RDI = 24, // 64 Bit
	EDI = 25, // 32 Bit
	 DI = 26
};

enum Pointer {
	RBP = 27, // 64 Bit
	EBP = 28, // 32 Bit
	 BP = 29,
	RSP = 30, // 64 Bit
	ESP = 31, // 32 Bit
	 SP = 32,
	RIP = 33, // 64 Bit
	EIP = 34, // 32 Bit
	 IP = 35
};

namespace as {
	const std::map<int, const std::string> Register = {
		{RAX, "%rax"},
		{EAX, "%eax"},
		{ AX, "%ax"},
		{ AH, "%ah"},
		{ AL, "%al"},

		{RBX, "%rbx"},
		{EBX, "%ebx"},
		{ BX, "%bx"},
		{ BH, "%bh"},
		{ BL, "%bl"},

		{RCX, "%rcx"},
		{ECX, "%ecx"},
		{ CX, "%cx"},
		{ CH, "%ch"},
		{ CL, "%cl"},

		{RDX, "%rdx"},
		{EDX, "%edx"},
		{ DX, "%dx"},
		{ DH, "%dh"},
		{ DL, "%dl"},

		{RSI, "%rsi"},
		{ESI, "%esi"},
		{ SI, "%si"},

		{RDI, "%rdi"},
		{EDI, "%edi"},
		{ DI, "%di"},

		{RBP, "%rbp"},
		{EBP, "%ebp"},
		{ BP, "%bp"},

		{RSP, "%rsp"},
		{ESP, "%esp"},
		{ SP, "%sp"},

		{RIP, "%rip"},
		{EIP, "%eip"},
		{ IP, "%ip"},
	};

	namespace patch {
		template <typename T>
		std::string to_string(const T& n) {
			std::ostringstream stm;
			stm << n;
			return stm.str();
		}
	}

	static inline std::string addrOf(Pointer ptr, int offset) {
		return patch::to_string(offset) + '(' + Register.at(ptr) + ')';
	}

	void push(std::ostream& out, int num);
	void push(std::ostream& out, Pointer ptr);
	void push(std::ostream& out, Pointer ptr, int offset);
	void push(std::ostream& out, Accu accu);
	void push(std::ostream& out, const std::string& label);

	void pop(std::ostream& out, Accu accu);
	void pop(std::ostream& out, Pointer ptr);

	void move(std::ostream& out, int num, Accu accu);
	void move(std::ostream& out, int num, Pointer ptr, int offset);
	void move(std::ostream& out, Accu a1, Accu a2);
	void move(std::ostream& out, Pointer ptr1, Pointer ptr2);
	void move(std::ostream& out, Accu accu, Pointer ptr, int offset);
	void move(std::ostream& out, Pointer ptr, int offset, Accu accu);

	void add(std::ostream& out, int num, Pointer ptr);
	void add(std::ostream& out, int num, Pointer ptr, int offset);
	void add(std::ostream& out, Pointer ptr, int offset, Accu accu);
	void add(std::ostream& out, Accu accu, Pointer ptr, int offset);
	void add(std::ostream& out, Accu a1, Accu a2);
	void add(std::ostream& out, Pointer ptr, int offset);

	void sub(std::ostream& out, int num, Pointer ptr);
	void sub(std::ostream& out, int num, Pointer ptr, int offset);
	void sub(std::ostream& out, Pointer ptr, int offset, Accu accu);
	void sub(std::ostream& out, Accu accu, Pointer ptr, int offset);
	void sub(std::ostream& out, Accu a1, Accu a2);
	void sub(std::ostream& out, Pointer ptr, int offset);

	void mul(std::ostream& out, int num, Accu accu);
	void mul(std::ostream& out, int num, Pointer ptr, int offset);
	void mul(std::ostream& out, Accu a1, Accu a2);
	void mul(std::ostream& out, Accu accu, Pointer ptr, int offset);
	void mul(std::ostream& out, Pointer ptr, int offset, Accu accu);
	void mul(std::ostream& out, Pointer ptr1, int offset1, Pointer ptr2, int offset2);

	void div(std::ostream& out, Accu accu);

	void neg(std::ostream& out, Accu accu);
	void neg(std::ostream& out, Pointer ptr, int offset);

	void call(std::ostream& out, const std::string& label);
	void ret(std::ostream& out);

	void jump(std::ostream& out);
	void jump(std::ostream& out, Cond cond, const std::string& label);

	void set(std::ostream& out, Cond cond, Accu accu);
	void cmp(std::ostream& out, Accu accu, Pointer ptr, int offset);

	void op_and(std::ostream& out, Accu accu);
	void op_or(std::ostream& out, Accu accu);
	void op_xor(std::ostream& out, Accu accu);
	void op_not(std::ostream& out, Accu accu);

	void test(std::ostream& out, Accu a1, Accu a2);

	void start(std::ostream& out);
	void end(std::ostream& out);
}

#endif