#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <map>

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

namespace as {
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
	void push(std::ostream& out, Accu accu);
	void push(std::ostream& out, Pointer ptr, int offset);
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

	void start(std::ostream& out);
	void end(std::ostream& out);
}

#endif