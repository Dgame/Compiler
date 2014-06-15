#ifndef ENV_HPP
#define ENV_HPP

class Loc;
class Assembler;
class VarManager;

struct Env {
	Loc* loc;
	Assembler* as;
	VarManager* var;

	explicit Env(Loc* _loc, Assembler* _as, VarManager* _var) : loc(_loc), as(_as), var(_var) {

	}
};

#endif