#include "parser.h"
#include <fstream>
#include <string>

int main(int argc, char ** argv) {

	if(argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <filename>\n";
		return 1;
	}

	std::ifstream source_code(argv[1]);

	if(!source_code) {
		std::cerr << "Error: Could not open file " << argv[1] << "\n";
		return 1;
	}

	std::istreambuf_iterator<char> start(source_code), end;
	std::string input(start, end);

	Scanner scanner(input);
	Parser parser(scanner);
	parser.parse();
}
