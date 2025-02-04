#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
#include <string>
#include <cctype>
#include <unordered_map>

class Scanner {
public:
	explicit Scanner(const std::string & input);

	Token nextToken();

private:
	std::string input;
	size_t position = 0;
	char currentChar = 0;

	void advance();
	void skipWhitespace();
	Token identifierOrKeyword();
	Token number();
	Token singleCharToken(TokenType type);
	Token compareToken();

	static std::unordered_map<std::string, TokenType> keywords;
};

#endif // SCANNER_H
