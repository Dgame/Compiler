#ifndef VAR_MANAGER_HPP
#define VAR_MANAGER_HPP

#include <map>
#include <memory>
#include <list>
#include <cassert>
#include "types.hpp"
#include "unique.hpp"

struct Variable;

typedef std::map<const std::string, std::unique_ptr<Variable>> VarMap;

struct VarManager {
	int16 curScope = -1;
	uint16 stackSize = 0;
	std::list<std::unique_ptr<VarMap>> variables;

	VarManager();

	bool createVar(const std::string& name, int16 size = 4);
	Variable* getVar(const std::string& name) const;

	void pushScope() {
		curScope++;
		this->variables.emplace_front(patch::make_unique<VarMap>());
	}

	void popScope() {
		curScope--;
		this->variables.pop_front();
	}
};

#endif