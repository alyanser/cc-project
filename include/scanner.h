#pragma once

#include <string>
#include <cctype>
#include <unordered_map>

#include "token.h"

class Scanner {

public:
	Scanner(const std::string & input) : input_(input), current_char_(input[0]) {}
	Token nextToken();

private:
	std::string input_;
	size_t position_ = 0;
	char current_char_ = 0;

	void advance();
	void skip_whitespace();
	Token identifier_or_keyword();
	Token number();
	Token single_char_token(TokenType type);
	Token compare_token();

	inline static std::unordered_map<std::string, TokenType> keywords {
		{"int", TokenType::TYPE}, {"float", TokenType::TYPE}, {"for", TokenType::FOR}, {"agar", TokenType::IF}, {"magar", TokenType::ELSE}
	};
};
