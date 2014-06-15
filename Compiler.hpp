#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <iostream>
#include <vector>
#include "Env.hpp"

class Assembler;

struct Loc {
	const char* pos;
	const char* const end;

	unsigned int lineNr = 1;

	explicit Loc(const char* _pos, const char* const _end);

	bool eof() const {
		return this->pos == this->end;
	}

	void checkNewLine() {
		if (*this->pos == '\n')
			this->lineNr++;
	}

	void error(const std::string& error) {
		std::cerr << error << " -> On line " << this->lineNr << '.' << std::endl;
	}
};

char unescape(char c);

bool readString(Loc& loc, std::string* str);

void skipComment(Loc& loc);
void skipSpaces(Loc& loc);
bool read(Loc& loc, const char* what);
bool read(Loc& loc, const char what);
bool readNumber(Loc& loc, int* n);
bool readIdentifier(Loc& loc, std::string* identifier);

bool parsePrint(Env& env);
bool parseVar(Env& env);
bool parseVarAssign(Env& env, bool duty);
bool parseLiteral(Env &env);
bool parseFactor(Env& env);
bool parseTerm(Env& env);
bool parseExpression(Env& env);
bool parseCommand(Env& env);

#endif