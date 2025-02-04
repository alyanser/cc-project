#include "scanner.h"
#include <iostream>
#include <unordered_map>

std::unordered_map<std::string, TokenType> Scanner::keywords = {
    {"int", TokenType::TYPE}, {"float", TokenType::TYPE}, {"for", TokenType::FOR}, {"agar", TokenType::IF}, {"magar", TokenType::ELSE}};

Scanner::Scanner(const std::string & input) : input(input), currentChar(input.empty() ? '\0' : input[0]) {
}

void Scanner::advance() {
	position++;
	currentChar = (position < input.size()) ? input[position] : '\0';
}

void Scanner::skipWhitespace() {
	while(std::isspace(currentChar)) {
		advance();
	}
}

Token Scanner::nextToken() {
	skipWhitespace();

	if(std::isalpha(currentChar)) {
		return identifierOrKeyword();
	}

	if(std::isdigit(currentChar)) {
		return number();
	}

	switch(currentChar) {
	case '<':
		return compareToken();
	case '>':
		advance();
		return Token(TokenType::GT);
	case '+':
		return singleCharToken(TokenType::PLUS);
	case '-':
		return singleCharToken(TokenType::MINUS);
	case '*':
		return singleCharToken(TokenType::MULTIPLY);
	case '/':
		return singleCharToken(TokenType::DIVIDE);
	case '{':
		return singleCharToken(TokenType::LBRACE);
	case '}':
		return singleCharToken(TokenType::RBRACE);
	case ';':
		return singleCharToken(TokenType::SEMICOLON);
	case ',':
		return singleCharToken(TokenType::COMMA);
	case '=':
		advance();
		return Token(TokenType::EQUAL);
	case '\0':
		return Token(TokenType::END_OF_FILE);
	default:
		std::cerr << "Unexpected character: " << currentChar << "\n";
		advance();
		return Token(TokenType::INVALID);
	}
}

Token Scanner::identifierOrKeyword() {
	std::string result;
	while(std::isalnum(currentChar) || currentChar == '_') {
		result += currentChar;
		advance();
	}

	if(keywords.find(result) != keywords.end()) {
		return Token(keywords[result], result);
	}
	return Token(TokenType::IDENTIFIER, result);
}

Token Scanner::number() {
	std::string result;
	while(std::isdigit(currentChar)) {
		result += currentChar;
		advance();
	}
	return Token(TokenType::NUMBER, result);
}

Token Scanner::singleCharToken(TokenType type) {
	char value = currentChar;
	advance();
	return Token(type, std::string(1, value));
}

Token Scanner::compareToken() {
	char first = currentChar;
	advance();

	if(currentChar == '=') {
		advance();
		return Token(TokenType::COMPARE, std::string(1, first) + "=");
	} else {
		return Token(first == '<' ? TokenType::LT : TokenType::INVALID, std::string(1, first));
	}
}
