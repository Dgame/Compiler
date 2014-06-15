#ifndef ENV_HPP
#define ENV_HPP

class Loc;
class Assembler;
class VarManager;
class DataSection;

struct Env {
	Loc* loc;
	Assembler* as;
	VarManager* var;
	DataSection* data;

	explicit Env(Loc* _loc, Assembler* _as, VarManager* _var, DataSection* ds) : loc(_loc), as(_as), var(_var), data(ds) {

	}
};

#endif