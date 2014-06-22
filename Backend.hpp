#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "Assembler.hpp"

struct Expression;
struct Print;
struct Exit;
struct Variable;

void buildAssembler(std::ostream& out, Expression* exp);
void buildAssembler(std::ostream& out, Print* print);
void buildAssembler(std::ostream& out, Variable* var);
void buildAssembler(std::ostream& out, Exit* exit);

#endif