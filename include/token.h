#pragma once

#include <iostream>
#include <string>

enum class TokenType {
	INVALID,
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
	END_OF_FILE
};

inline std::ostream & operator << (std::ostream & os, const TokenType type) {
	switch(type) {
		case TokenType::INVALID:
			return os << "INVALID";
		case TokenType::TYPE:
			return os << "TYPE";
		case TokenType::IDENTIFIER:
			return os << "IDENTIFIER";
		case TokenType::NUMBER:
			return os << "NUMBER";
		case TokenType::FOR:
			return os << "FOR";
		case TokenType::IF:
			return os << "IF";
		case TokenType::ELSE:
			return os << "ELSE";
		case TokenType::LBRACE:
			return os << "LBRACE";
		case TokenType::RBRACE:
			return os << "RBRACE";
		case TokenType::LPAREN:
			return os << "LPAREN";
		case TokenType::RPAREN:
			return os << "RPAREN";
		case TokenType::SEMICOLON:
			return os << "SEMICOLON";
		case TokenType::LT:
			return os << "LT";
		case TokenType::GT:
			return os << "GT";
		case TokenType::EQUAL:
			return os << "EQUAL";
		case TokenType::PLUS:
			return os << "PLUS";
		case TokenType::MINUS:
			return os << "MINUS";
		case TokenType::MULTIPLY:
			return os << "MULTIPLY";
		case TokenType::DIVIDE:
			return os << "DIVIDE";
		case TokenType::COMPARE:
			return os << "COMPARE";
		case TokenType::COMMA:
			return os << "COMMA";
		case TokenType::END_OF_FILE:
			return os << "END_OF_FILE";

		default : {
			return os << "Unknown token type";
		}
	}
}

struct Token {
	TokenType type{};
	std::string value;

	Token() = default;
	Token(const TokenType type, std::string value = "") : type(type), value(std::move(value)) {}

	std::ostream & operator << (std::ostream & os) const {
		return os << type << ' ' << value;
	}
};
