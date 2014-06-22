#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <iostream>
#include "Assembler.hpp"

struct Expression;
struct Command;
struct Print;
struct Exit;
struct VarAssign;

void buildAssembler(std::ostream& out, Expression* exp);
void buildAssembler(std::ostream& out, Print* print);
void buildAssembler(std::ostream& out, VarAssign* var);
void buildAssembler(std::ostream& out, Exit* exit);
void buildAssembler(std::ostream& out, Command* cmd);

#endif