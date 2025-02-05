#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include "scanner.h"
#include "parser.h"

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

	const auto input = [&source_code]() {
		std::stringstream iss;
		iss << source_code.rdbuf();
		return iss.str();
	}();

	Scanner scanner(input);
	Parser parser;
	parser.parse(scanner);

	std::cout << "\nParsing sucessful!\n";
}
