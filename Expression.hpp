#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <list>
#include <vector>
#include <map>
#include <iostream>
#include <cassert>

#include "unique.hpp"
#include "types.hpp"

enum class Op : char {
	Plus = '+',
	Minus = '-',
	Mul = '*',
	Div = '/',
	Mod = '%',
	Negate = '_' // To solve the duplicate switch value error
};

struct Operator;
struct Value;
struct Variable;

enum class LiTy {
	None,
	Operator,
	Value,
	Variable
};

struct Literal {
	LiTy lty;

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
struct Compare;
struct Join;

struct Expression {
	virtual Term* isTerm() {
		return nullptr;
	}

	virtual ImmedAssign* isImmedAssign() {
		return nullptr;
	}

	virtual Compare* isCompare() {
		return nullptr;
	}

	virtual Join* isJoin() {
		return nullptr;
	}

	virtual void assign(Literal* lp) {
		assert(0);
	}
};

struct Term : public Expression {
	std::list<Literal*> literals;

	virtual ~Term();

	virtual Term* isTerm() override {
		return this;
	}

	uint16 count() const {
		return this->literals.size();
	}

	Literal* top() const;
	Literal* at(uint16 index) const;
	Literal* pop();

	void push(Literal* lp) {
		this->literals.emplace_back(lp);
	}

	virtual void assign(Literal* lp) override {
		this->push(lp);
	}
};

struct ImmedAssign : public Expression {
	std::shared_ptr<Literal> literal;

	explicit ImmedAssign(Literal* lp);

	virtual ImmedAssign* isImmedAssign() override {
		return this;
	}

	virtual void assign(Literal* lp) override {
		this->literal.reset(lp);
	}
};

enum class Link {
	And,
	Or,
	Xor
};

enum class Cmp {
	Equal = 1,
	NotEqual = 2,
	Greater = 3,
	GreaterOrEqual = 4,
	Less = 5,
	LessOrEqual = 6
};

struct Compare : public Expression {
	Cmp cmp;

	std::unique_ptr<Expression> lhs;
	std::unique_ptr<Expression> rhs;

	explicit Compare(Expression* eplhs, Cmp cmp, Expression* eprhs);

	virtual Compare* isCompare() override {
		return this;
	}
};

struct Join : public Expression {
	std::unique_ptr<Compare> mexp;
	std::map<Link, std::unique_ptr<Compare>> exp;

	explicit Join(Compare* cep);

	virtual Join* isJoin() override {
		return this;
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
	int value;

	explicit Value(int value);

	virtual Value* isValue() override {
		return this;
	}

	virtual void output(std::ostream& out) override {
		out << this->value << std::endl;
	}
};

struct Variable : public Literal {
	std::string name;
	std::unique_ptr<Expression> exp;
	int16 offset = -1;
	uint16 size = 0;

	explicit Variable(const std::string& name, int16 offset, int16 size = 4);
	Variable(const Variable& var);

	void assign(Expression& exp) {
		this->exp = patch::make_unique<Expression>(exp);
	}

	virtual Variable* isVariable() override {
		return this;
	}

	virtual void output(std::ostream& out) override {
		out << this->name << " (Offset: " << this->offset << "; Size: " << this->size << ')' << std::endl;
	}
};

struct Print;
struct VarAssign;
struct Exit;
struct If;

struct Command {
	virtual Print* isPrint() {
		return nullptr;
	}

	virtual VarAssign* isVarAssign() {
		return nullptr;
	}

	virtual Exit* isExit() {
		return nullptr;
	}

	virtual If* isIf() {
		return nullptr;
	}
};

struct Print : public Command {
	std::unique_ptr<Expression> exp;
	std::string label;

	explicit Print(Expression* ep, const std::string& label);

	virtual Print* isPrint() override {
		return this;
	}

	void assign(Expression& exp) {
		this->exp = patch::make_unique<Expression>(exp);
	}
};

struct VarAssign : public Command {
	std::unique_ptr<Variable> var;

	explicit VarAssign(const Variable* vp);

	virtual VarAssign* isVarAssign() override {
		return this;
	}
};

struct Exit : public Command {
	virtual Exit* isExit() override {
		return this;
	}
};

struct If : public Command {
	std::unique_ptr<Join> jexp;

	std::string ifLabel;
	std::string elseLabel;

	explicit If(Join* jep, const std::string& ifL);

	virtual If* isIf() override {
		return this;
	}
};

#endif