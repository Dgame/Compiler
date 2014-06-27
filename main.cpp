#include <vector>
#include <fstream>
#include <sstream>
#include "Compiler.hpp"
#include "Backend.hpp"

#define BASIC_PRINT 0
#define BASIC_VAR 0
#define EXT_VAR 0
#define STRING_PRINT 0
#define BASIC_IF 1

int main(int argc, char const *argv[]) {
#if BASIC_VAR
	std::ifstream in("test/basic_var.txt");
#elif EXT_VAR
	std::ifstream in("test/ext_var.txt");
#elif STRING_PRINT
	std::ifstream in("test/string_print.txt");
#elif BASIC_IF
	std::ifstream in("test/basic_if.txt");
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
	Scopes scopes;

	Env env;
	env.loc = &loc;
	env.scope = &scopes;

	while (parseCommand(env)) {

	}

	if (loc.errors == 0) {
		as::start(content);
		if (scopes.stackSize != 0) {
			as::sub(content, scopes.stackSize, ESP);
			content << std::endl;
		}

		for (std::unique_ptr<Command>& cmd : scopes.commands[GlobalLabel]) {
			buildAssembler(content, cmd.get());
		}

		if (scopes.stackSize != 0)
			as::add(content, scopes.stackSize, ESP);
		
		as::end(content);
		content << std::endl;

		auto it = scopes.commands.find(GlobalLabel);
		scopes.commands.erase(it);

		for (auto& pair : scopes.commands) {
			content << pair.first << ':' << std::endl;
			content << std::endl;

			for (std::unique_ptr<Command>& cmd : pair.second) {
				buildAssembler(content, cmd.get());
			}

			content << std::endl;

			as::ret(content);
		}
	}

	return 0;
}