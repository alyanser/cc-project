#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
	TYPE,
	IDENTIFIER,
	NUMBER,
	FOR,
	IF,
	ELSE,
	LBRACE,
	RBRACE,
	LPAREN,
	RPAREN,
	SEMICOLON,
	LT,
	GT,
	EQUAL,
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE,
	COMPARE,
	COMMA,
	END_OF_FILE,
	INVALID
};

struct Token {
	TokenType type;
	std::string value;

	Token() = default;

	Token(TokenType type, std::string value = "") : type(type), value(std::move(value)) {
	}
};

#endif // TOKEN_H
