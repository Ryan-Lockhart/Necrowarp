#include <necrowarp/game.tpp>

int main(int argc, char* argv[]) {
	using namespace necrowarp;

	const int exit_code{ game_s::run() };

	if (exit_code == EXIT_FAILURE) {
		if constexpr (IsReleaseBuild) {
			message_log.flush_to_file();
			error_log.flush_to_file();
		} else {
			message_log.flush_to_console(std::cout);
			error_log.flush_to_console(std::cerr);
		}
	}

	return exit_code;
}
