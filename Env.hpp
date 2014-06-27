#ifndef ENV_HPP
#define ENV_HPP

#include <vector>
#include <memory>

struct Loc;
struct Expression;
struct Scopes;

struct Env {
	Loc* loc;
	std::unique_ptr<Expression> exp;

	Scopes* scope;
};

#endif