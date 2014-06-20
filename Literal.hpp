#ifndef LITERAL_HPP
#define LITERAL_HPP

#include <ostream>

enum class Op : char {
	Plus = '+',
	Minus = '-',
	Mul = '*',
	Div = '/',
	Mod = '%'
};

struct Addr {
	int offset;

	Addr() = default;
	explicit Addr(int offset);
};

struct Literal {
	enum class Type {
		Numeric,
		Operator,
		Address,
		Negate
	};
	
	Type type;
	
	union {
		int value;
		Addr addr;
		bool negate;
		Op op;
	};
	
	explicit Literal(int value);
	explicit Literal(Addr addr);
	explicit Literal(Op op);
	explicit Literal(bool negate);

	void output(std::ostream& out);
};

#endif