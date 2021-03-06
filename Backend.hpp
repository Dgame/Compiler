#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <iostream>
#include <stack>
#include "Assembler.hpp"

struct Expression;
struct Command;
struct Print;
struct Exit;
struct VarAssign;
struct If;

void buildAssembler(std::ostream& out, Expression* exp);
void buildAssembler(std::ostream& out, Print* print);
void buildAssembler(std::ostream& out, VarAssign* var);
void buildAssembler(std::ostream& out, Exit* exit);
void buildAssembler(std::ostream& out, Exit* exit);
void buildAssembler(std::ostream& out, If* _if);
void buildAssembler(std::ostream& out, Command* cmd);

#endif