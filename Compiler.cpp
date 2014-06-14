#include "Compiler.hpp"
#include "Assembler.hpp"

Loc::Loc(const char* _pos, const char* const _end) : pos(_pos), end(_end) {

}

void skipSpaces(Loc& loc) {
	while (!loc.eof() && std::isspace(*loc.pos)) {
		loc.checkNewLine();

		++loc.pos;
	}
}

bool read(Loc& loc, const char* what) {
	skipSpaces(loc);

	while (!loc.eof() && *loc.pos == *what) {
		++what;
		++loc.pos;

		if (*what == '\0') {
			--what;

			if (std::isalpha(*what)) {
				if (std::isalnum(*loc.pos))
					return false;
			}

			return true;
		}
	}

	return false;
}

bool read(Loc& loc, const char what) {
	skipSpaces(loc);

	if (!loc.eof() && *loc.pos == what) {
		++loc.pos;

		return true;
	}

	return false;
}

bool readNumber(Loc& loc, int* n) {
	skipSpaces(loc);

	if (!loc.eof() && std::isdigit(*loc.pos) && n != nullptr) {
		*n = 0;
		do {
			*n *= 10;
			*n += *loc.pos - '0';

			++loc.pos;
		} while (loc.eof()&& std::isdigit(*loc.pos));

		return true;
	}

	return false;
}

bool parseFactor(Loc& loc, Assembler& as) {
	bool negate = false;
	if (read(loc, '-'))
		negate = true;

	int n = 0;
	if (readNumber(loc, &n))
		as.move(n);
	else
		return false;

	if (negate)
		as.neg();

	return true;
}

bool parseTerm(Loc& loc, Assembler& as) {
	if (parseFactor(loc, as)) {
		for (;;) {
			if (read(loc, '*')) {
				as.push();;

				if (!parseFactor(loc, as)) {
					std::cerr << "Expected factor after *" << std::endl;

					return false;
				}

				as.mul();
			} else if (read(loc, '/')) {
				as.push();

				if (!parseFactor(loc, as)) {
					std::cerr << "Expected factor after /" << std::endl;

					return false;
				}

				as.div();
			} else if (read(loc, '%')) {
				as.push();

				if (!parseFactor(loc, as)) {
					std::cerr << "Expected factor after %" << std::endl;

					return false;
				}

				as.mod();
			} else
				break;
		}

		return true;
	}

	return false;
}

bool parseExpression(Loc& loc, Assembler& as) {
	if (parseTerm(loc, as)) {
		for (;;) {
			if (read(loc, '+')) {
				as.push();

				if (!parseTerm(loc, as)) {
					std::cerr << "Expected Term after +" << std::endl;

					return false;
				}

				as.add();
			} else if (read(loc, '-')) {
				as.push();

				if (!parseTerm(loc, as)) {
					std::cerr << "Expected Term after -" << std::endl;

					return false;
				}
				
				as.sub();
				as.pop(); // ?
			} else
				break;
		}

		return true;
	}

	return false;
}

bool parseCommand(Loc& loc, Assembler& as) {
	if (read(loc, "print")) {
		if (parseExpression(loc, as)) {
			as.print();

			return true;
		} else
			std::cerr << "Missing or invalid print argument" << std::endl;
	}

	return false;
}

int main(int argc, char const *argv[]) {
	std::ifstream in("in.txt");

	std::vector<char> code;
	std::copy(
		std::istreambuf_iterator<char>(in.rdbuf()),
		std::istreambuf_iterator<char>(),
		std::back_inserter(code));

	std::ofstream out("out.s");
	Assembler as(out);

	Loc loc(&code[0], &code.back() + 1);

	while (parseCommand(loc, as)) {

	}

	return 0;
}