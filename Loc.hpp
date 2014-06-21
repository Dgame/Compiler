#ifndef LOC_HPP
#define LOC_HPP

#include <iostream>
#include <string>
#include "types.hpp"

struct Loc {
	const char* pos;
	const char* const end;

	uint32 errors = 0;
	uint32 lineNr = 1;

	explicit Loc(const char* _pos, const char* const _end);

	bool eof() const {
		return this->pos == this->end;
	}

	void error(const std::string& error) {
		this->errors++;
		
		std::cerr << error << " -> On line " << this->lineNr << '.' << std::endl;
	}
};

#endif