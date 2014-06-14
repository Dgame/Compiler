#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <vector>

class Assembler;

struct Loc {
	const char* pos;
	const char* const end;

	unsigned int lineNr = 1;

	explicit Loc(const char* _pos, const char* const _end);

	bool eof() const {
		return pos == end;
	}

	void checkNewLine() {
		if (*pos == '\n' || *pos == '\r')
			lineNr++;
	}
};

void skipSpaces(Loc& loc);
bool read(Loc& loc, const char* what);
bool read(Loc& loc, const char what);
bool readNumber(Loc& loc, int* n);

bool parseFactor(Loc& loc, Assembler& as);
bool parseTerm(Loc& loc, Assembler& as);
bool parseExpression(Loc& loc, Assembler& as);
bool parseCommand(Loc& loc, Assembler& as);

#endif