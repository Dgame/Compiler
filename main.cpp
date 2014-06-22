#include <vector>
#include <fstream>
#include <sstream>
#include "Compiler.hpp"
#include "Backend.hpp"

#define BASIC_PRINT 0
#define BASIC_VAR 0
#define EXT_VAR 1
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

	}

	if (loc.errors == 0) {
		as::start(content);
		if (vm.stackSize != 0) {
			as::sub(content, vm.stackSize, ESP);
			content << std::endl;
		}

		for (std::unique_ptr<Command>& cmd : env.commands) {
			buildAssembler(content, cmd.get());
		}

		if (vm.stackSize != 0)
			as::add(content, vm.stackSize, ESP);
		as::end(content);
	}

	return 0;
}