#include "VarManager.hpp"
#include "Expression.hpp"
#include "unique.hpp"

bool VarManager::createVar(const std::string& name, int16 size) {
	if (this->variables.find(name) != this->variables.end())
		return false;

	this->variables[name] = patch::make_unique<Variable>(name, this->stackSize, size);
	this->stackSize += size;

	return true;
}

Variable* VarManager::getVar(const std::string& name) const {
	auto it = this->variables.find(name);
	if (it == this->variables.end())
		return nullptr;

	return it->second.get();
}