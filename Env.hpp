#ifndef ENV_HPP
#define ENV_HPP

#include <vector>
#include <memory>

struct Loc;
struct Expression;
struct VarManager;
struct Command;

struct Env {
	Loc* loc;
	std::unique_ptr<Expression> exp;

	VarManager* varManager;
	std::vector<std::unique_ptr<Command>> commands;
};

#endif