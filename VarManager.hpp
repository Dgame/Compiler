#ifndef VAR_MANAGER
#define VAR_MANAGER

#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <algorithm>

class VarManager {
public:
	bool create(const std::string& name);
	bool exist(const std::string& name) const;
	int addrOf(const std::string& name) const;

	void pushScope() {
		_scope_start.push_back(_names.size());
	}

	unsigned int popScope();

	bool hasOpenScopes() const {
		return !_scope_start.empty();
	}

private:
	std::vector<std::string> _names;
	std::vector<unsigned int> _scope_start;
};

#endif