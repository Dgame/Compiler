#include "VarManager.hpp"

bool VarManager::create(const std::string& name) {
	// assert(!_scope_start.empty());

	_stackSize += 4;
	
	auto it = _names.begin() + (!_scope_start.empty() ? _scope_start.back() : 0);

	if (std::find(it, _names.end(), name) == _names.end()) {
		_names.push_back(name);

		return true;
	}

	return false;
}

bool VarManager::exist(const std::string& name) const {
	// assert(!_scope_start.empty());

	return std::find(_names.begin(), _names.end(), name) != _names.end();
}

int VarManager::addrOf(const std::string& name) const {
	// assert(!_scope_start.empty());

	for (unsigned int i_plus_one = _names.size(); i_plus_one > 0; --i_plus_one) {
		if (_names[i_plus_one - 1] == name) {
			return 4 * (i_plus_one - 1);
		}
	}

	// assert(false);
	return -1;
}

unsigned int VarManager::popScope() {
	assert(!_scope_start.empty());

	const unsigned int to_pop = 4 * (_names.size() - _scope_start.back());
	if (to_pop != 0) {
		_names.erase(_names.begin() + _scope_start.back(), _names.end());
	}

	_scope_start.pop_back();

	return to_pop;
}