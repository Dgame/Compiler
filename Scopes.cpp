#include "Scopes.hpp"
#include "Expression.hpp"

Scopes::Scopes() {
	this->curLabel = GlobalLabel;

	this->variables.push_front(VariableMap());
	this->commands[this->curLabel] = CommandVector();
}

std::string Scopes::_createLabel() {
	this->ss << 'L' << this->nr++;

	const std::string label = this->ss.str();
	this->ss.clear();

	return label;
}

void Scopes::pushScope() {
	this->labels.push(this->curLabel);
	this->curLabel = _createLabel();

	this->variables.push_front(VariableMap());
	this->commands[this->curLabel] = CommandVector();
}

void Scopes::popScope() {
	this->lastLabel = this->curLabel;

	this->variables.pop_front();
	this->curLabel = this->labels.top();
	this->labels.pop();
}

void Scopes::addCmd(Command* cmd) {
	this->commands[this->curLabel].emplace_back(std::unique_ptr<Command>(cmd));
}

bool Scopes::createVar(const std::string& name, int16 size) {
	if (this->getVar(name) != nullptr)
		return false;

	this->variables.front()[name] = patch::make_unique<Variable>(name, this->stackSize, size);

	return true;
}

Variable* Scopes::getVar(const std::string& name) {
	for (VariableMap& vmap : this->variables) {
		auto it = vmap.find(name);
		if (it != vmap.end())
			return it->second.get();
	}

	return nullptr;
}