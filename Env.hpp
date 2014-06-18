#ifndef ENV_HPP
#define ENV_HPP

#include <iostream>
#include "Literal.hpp"

class Loc;
class VarManager;
class DataSection;

struct Env {
	Loc* loc;
	VarManager* var;
	DataSection* data;
	std::ostream& out;
	Range<Literal> mathExp;

	explicit Env(Loc* _loc, VarManager* _var, DataSection* ds, std::ostream& output)
		: loc(_loc), var(_var), data(ds), out(output)
	{

	}
};

#endif