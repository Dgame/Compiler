#ifndef VAR_MANAGER_HPP
#define VAR_MANAGER_HPP

#include <map>
#include <memory>
#include <vector>
#include <cassert>
#include "types.hpp"

struct Variable;

struct VarManager {
	uint16 stackSize = 0;
	std::map<const std::string, std::unique_ptr<Variable>> variables;

	bool createVar(const std::string& name, int16 size = 4);
	Variable* getVar(const std::string& name) const;
};

#endif