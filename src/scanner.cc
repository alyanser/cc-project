#include "scanner.h"
#include <iostream>
#include <unordered_map>

void Scanner::advance() {
	position_++;
	current_char_ = (position_ < input_.size()) ? input_[position_] : '\0';
}

void Scanner::skip_whitespace() {

	while(std::isspace(current_char_)) {
		advance();
	}
}

Token Scanner::nextToken() {
	skip_whitespace();

	if(std::isalpha(current_char_)) {
		return identifier_or_keyword();
	}

	if(std::isdigit(current_char_)) {
		return number();
	}

	switch(current_char_) {

	case '<':
		return compare_token();
	case '>':
		return compare_token();
	case '+':
		return single_char_token(TokenType::PLUS);
	case '-':
		return single_char_token(TokenType::MINUS);
	case '*':
		return single_char_token(TokenType::MULTIPLY);
	case '/':
		return single_char_token(TokenType::DIVIDE);
	case '{':
		return single_char_token(TokenType::LBRACE);
	case '}':
		return single_char_token(TokenType::RBRACE);
	case ';':
		return single_char_token(TokenType::SEMICOLON);
	case ',':
		return single_char_token(TokenType::COMMA);
	case '(':
		return single_char_token(TokenType::LPAREN);
	case ')':
		return single_char_token(TokenType::RPAREN);
	case '=':
		return compare_token();
	case '\0':
		return Token(TokenType::END_OF_FILE);
	default:
		std::cerr << "Unexpected character: " << current_char_ << "\n";
		advance();
		return Token(TokenType::INVALID);
	}
}

Token Scanner::identifier_or_keyword() {
	std::string result;

	while(std::isalnum(current_char_) || current_char_ == '_') {
		result += current_char_;
		advance();
	}

	if(keywords.find(result) != keywords.end()) {
		return Token(keywords[result], result);
	}

	return Token(TokenType::IDENTIFIER, result);
}

Token Scanner::number() {
	std::string result;

	while(std::isdigit(current_char_)) {
		result += current_char_;
		advance();
	}

	return Token(TokenType::NUMBER, result);
}

Token Scanner::single_char_token(TokenType type) {
	char value = current_char_;
	advance();
	return Token(type, std::string(1, value));
}

Token Scanner::compare_token() {
	char first = current_char_;
	advance();

	if(current_char_ == '=') {
		advance();
		return Token(TokenType::COMPARE, std::string(1, first) + "=");
	} else {
		return Token(first == '<' ? TokenType::LT : TokenType::INVALID, std::string(1, first));
	}
}
