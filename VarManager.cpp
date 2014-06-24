#include "VarManager.hpp"
#include "Expression.hpp"

VarManager::VarManager() {
	this->pushScope();
}

bool VarManager::createVar(const std::string& name, int16 size) {
	if (this->variables.front()->find(name) != this->variables.front()->end())
		return false;

	this->variables.front()->insert(
		std::make_pair(name, patch::make_unique<Variable>(name, this->stackSize, size))
	);
	
	this->stackSize += size;

	return true;
}

Variable* VarManager::getVar(const std::string& name) const {
	auto it = this->variables.front()->find(name);
	if (it == this->variables.front()->end())
		return nullptr;

	return it->second.get();
}