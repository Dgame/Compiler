#ifndef SCOPES_HPP
#define SCOPES_HPP

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <list>
#include <stack>
#include <sstream>
#include "unique.hpp"
#include "types.hpp"

const std::string GlobalLabel = "main";

struct Variable;
struct Command;

typedef std::map<const std::string, std::unique_ptr<Variable>> VariableMap;
typedef std::vector<std::unique_ptr<Command>> CommandVector;

struct Scopes {
	uint32 stackSize = 0;

	std::ostringstream ss;
	uint32 nr = 0;

	std::string lastLabel;
	std::string curLabel;

	std::stack<std::string> labels;
	std::list<VariableMap> variables;
	std::map<const std::string, CommandVector> commands;

	Scopes();

	std::string _createLabel();

	void pushScope();
	void popScope();

	void addCmd(Command* cmd);

	bool createVar(const std::string& name, int16 size = 4);
	Variable* getVar(const std::string& name);
};

#endif