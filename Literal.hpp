#ifndef LITERAL_HPP
#define LITERAL_HPP

#include <iostream>
#include <cstdlib>

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

template <typename T>
class Range {
private:
	T* _data = nullptr;
	unsigned int _length = 0;
	unsigned int _cap = 0;

public:
	Range() = default;

	unsigned int size() const {
		return _length;
	}

	void resetSize() {
		_length = 0;
	}

	void reserve(unsigned int num) {
		_cap = num + _length;
		_data = reinterpret_cast<T*>(realloc(_data, _cap * sizeof(T)));
	}

	T& operator [](unsigned int index);

	const T& at(unsigned int index) const {
		return _data[index];
	}

	void push_back(const T& value);
};

template <typename T>
T& Range<T>::operator [](unsigned int index) {
	return _data[index];
}

template <typename T>
void Range<T>::push_back(const T& value) {
	if (_length == _cap)
		this->reserve(_cap + 3);
	_data[_length++] = value;
}

#endif