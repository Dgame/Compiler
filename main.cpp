#include <vector>
#include <fstream>
#include <sstream>
#include "Compiler.hpp"

#define BASIC_PRINT 1
#define BASIC_VAR 0
#define EXT_VAR 0
#define STRING_PRINT 0

int main(int argc, char const *argv[]) {
#if BASIC_VAR
	std::ifstream in("test/basic_var.txt");
#elif EXT_VAR
	std::ifstream in("test/ext_var.txt");
#elif STRING_PRINT
	std::ifstream in("test/string_print.txt");
#else
	std::ifstream in("test/basic_print.txt");
#endif

	std::vector<char> code;
	std::copy(
		std::istreambuf_iterator<char>(in.rdbuf()),
		std::istreambuf_iterator<char>(),
		std::back_inserter(code));

	std::ofstream content("out2.s");
	std::ostringstream out;

	Loc loc(&code[0], &code.back() + 1);
	VarManager vm;

	Env env;
	env.loc = &loc;
	env.varManager = &vm;

	while (parseCommand(env)) {
		// std::cout << "Command found" << std::endl;
		std::cout << "----" << std::endl;
	}

	if (loc.errors == 0) {
		// as::start(content);
		// if (vm.getStackSize() != 0)
		// 	as::sub(content, vm.getStackSize(), ESP);

		// content << "\n" << out.str() << std::endl;

		// if (vm.getStackSize() != 0)
		// 	as::add(content, vm.getStackSize(), ESP);
		// as::end(content);
	}

	return 0;
}