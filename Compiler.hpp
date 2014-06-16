#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <iostream>
#include <vector>
#include <map>
#include "Env.hpp"

#define OPTIMIZE_VAR_ASSIGN 1
#define OPTIMIZE_PRINT_ASSIGN 1

class Assembler;

struct Loc {
	const char* pos;
	const char* const end;
	const char* _bufPos = nullptr;

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

	void pushPos() {
		_bufPos = this->pos;
	}

	void popPos() {
		if (_bufPos != nullptr) {
			this->pos = _bufPos;
			_bufPos = nullptr;
		}
	}
};

enum class Tok {
	Print,
	Var
};

const std::map<Tok, const char*> TokStr = {
	{Tok::Print, "print"},
	{Tok::Var, "var"}
};

enum class Label {
	PrintI,
	PrintlnI,
	PrintS,
	PrintlnS
};

const std::map<Label, const char*> LabelStr = {
	{Label::PrintI, "_print_int"},
	{Label::PrintlnI, "_println_int"},
	{Label::PrintS, "_print_string"},
	{Label::PrintlnS, "_println_string"}
};

static bool isEOL(char c) {
	return c == '\n' || c == '\r';
}

char unescape(char c);

bool readString(Loc& loc, std::string* str);

void skipComment(Loc& loc);
void skipSpaces(Loc& loc);

bool read(Loc& loc, const char* what);
bool read(Loc& loc, const char what);
bool read(Loc& loc, Tok tok);
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