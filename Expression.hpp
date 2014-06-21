#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <list>
#include <vector>
#include <map>
#include <iostream>

#include "unique.hpp"
#include "types.hpp"

enum class Op : char {
	Plus = '+',
	Minus = '-',
	Mul = '*',
	Div = '/',
	Mod = '%',
	Negate = '-'
};

struct Operator;
struct Value;
struct Variable;

struct Literal {
	virtual Operator* isOperator() {
		return nullptr;
	}

	virtual Value* isValue() {
		return nullptr;
	}

	virtual Variable* isVariable() {
		return nullptr;
	}

	virtual void output(std::ostream& out) {
		out << "<Literal>" << std::endl;
	}
};

struct Term;
struct ImmedAssign;

struct Expression {
	virtual Term* isTerm() {
		return nullptr;
	}

	virtual ImmedAssign* isImmedAssign() {
		return nullptr;
	}

	virtual void assign(Literal* lp) {

	}
};

struct Term : public Expression {
	std::list<Literal*> values;

	virtual ~Term();

	virtual Term* isTerm() override {
		return this;
	}

	Literal* top() const {
		if (this->values.size() == 0)
			return nullptr;

		return this->values.front();
	}

	Literal* pop() {
		if (this->values.size() == 0)
			return nullptr;

		Literal* lp = this->top();
		this->values.pop_front();

		return lp;
	}

	void push(Literal* lp) {
		this->values.push_back(lp);
	}

	virtual void assign(Literal* lp) override {
		this->push(lp);
	}
};

struct ImmedAssign : public Expression {
	Literal* value;

	explicit ImmedAssign(Literal* value);
	virtual ~ImmedAssign();

	virtual ImmedAssign* isImmedAssign() override {
		return this;
	}

	virtual void assign(Literal* lp) override {
		this->value = lp;
	}
};

struct Operator : public Literal {
	Op op;

	explicit Operator(Op op);

	virtual Operator* isOperator() override {
		return this;
	}

	virtual void output(std::ostream& out) override {
		out << static_cast<char>(this->op) << std::endl;
	}
};

struct Value : public Literal {
	int32 value;

	explicit Value(int32 value);

	virtual Value* isValue() override {
		return this;
	}

	virtual void output(std::ostream& out) override {
		out << this->value << std::endl;
	}
};

struct Variable : public Literal {
	std::string name;
	std::shared_ptr<Expression> exp;
	int16 offset = -1;
	uint16 size = 0;

	explicit Variable(const std::string& name, int16 offset, int16 size = 4);

	void assign(Expression& exp) {
		this->exp = std::make_shared<Expression>(exp);
	}

	virtual Variable* isVariable() override {
		return this;
	}

	virtual void output(std::ostream& out) override {
		out << this->name << " (Offset: " << this->offset << "; Size: " << this->size << ')' << std::endl;
	}
};

struct Print;
struct Exit;

struct Command {
	virtual void output(std::ostream& out) = 0;

	virtual Print* isPrint() {
		return nullptr;
	}

	virtual Exit* isExit() {
		return nullptr;
	}

	virtual void eval() {
		
	}
};

struct Print : public Command {
	std::unique_ptr<Expression> exp;
	std::string label;

	Print() = default;
	explicit Print(Expression* exp, const std::string& label);

	virtual Print* isPrint() override {
		return this;
	}

	void assign(Expression& exp) {
		this->exp = patch::make_unique<Expression>(exp);
	}

	virtual void output(std::ostream& out) override;
};

struct Exit : public Command {
	virtual Exit* isExit() override {
		return this;
	}
};

#endif