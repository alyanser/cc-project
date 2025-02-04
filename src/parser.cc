#include "parser.h"

Parser::Parser(Scanner & scanner) : scanner(scanner) {
	advance(); // Initialize with the first token
}

void Parser::advance() {
	currentToken = scanner.nextToken();
}

void Parser::expect(TokenType type) {

	if(currentToken.type == type) {
		advance();
	} else {
		std::cerr << "Unexpected token: " << currentToken.value << "\n";
		exit(EXIT_FAILURE);
	}
}

void Parser::parse() {
	parseFunction(); // Start parsing from the top grammar rule
}

void Parser::parseFunction() {
	expect(TokenType::TYPE);	 // Type
	expect(TokenType::IDENTIFIER); // Identifier
	expect(TokenType::LT);		 // '<'
	parseArgList();
	expect(TokenType::GT); // '>'
	parseCompoundStmt();
}

void Parser::parseArgList() {
	parseArg();
	while(currentToken.type == TokenType::COMMA) {
		advance();
		parseArg();
	}
}

void Parser::parseArg() {
	expect(TokenType::TYPE);
	expect(TokenType::IDENTIFIER);
}

void Parser::parseDeclaration() {
	expect(TokenType::TYPE);
	expect(TokenType::IDENTIFIER);
	while(currentToken.type == TokenType::COMMA) {
		advance();
		expect(TokenType::IDENTIFIER);
	}
	expect(TokenType::SEMICOLON);
}

void Parser::parseStmt() {
	switch(currentToken.type) {
	case TokenType::FOR:
		parseForStmt();
		break;
	case TokenType::IF:
		parseIfStmt();
		break;
	case TokenType::LBRACE:
		parseCompoundStmt();
		break;
	case TokenType::TYPE:
		parseDeclaration();
		break;
	case TokenType::SEMICOLON:
		advance();
		break;
	default:
		parseExpr();
		expect(TokenType::SEMICOLON);
		break;
	}
}

void Parser::parseForStmt() {
	expect(TokenType::FOR);
	expect(TokenType::LT);
	parseExpr();
	expect(TokenType::SEMICOLON);
	parseOptExpr();
	expect(TokenType::SEMICOLON);
	parseOptExpr();
	expect(TokenType::GT);
	parseStmt();
}

void Parser::parseIfStmt() {
	expect(TokenType::IF);
	expect(TokenType::LT);
	parseExpr();
	expect(TokenType::GT);
	parseStmt();
	if(currentToken.type == TokenType::ELSE) {
		advance();
		parseStmt();
	}
}

void Parser::parseCompoundStmt() {
	expect(TokenType::LBRACE);
	while(currentToken.type != TokenType::RBRACE) {
		parseStmt();
	}
	expect(TokenType::RBRACE);
}

void Parser::parseExpr() {
	parseRvalue();
}

void Parser::parseRvalue() {
	parseMag();
	if(currentToken.type == TokenType::COMPARE) {
		advance();
		parseMag();
	}
}

void Parser::parseMag() {
	parseTerm();
	while(currentToken.type == TokenType::PLUS || currentToken.type == TokenType::MINUS) {
		advance();
		parseTerm();
	}
}

void Parser::parseTerm() {
	parseFactor();

	while(currentToken.type == TokenType::MULTIPLY || currentToken.type == TokenType::DIVIDE) {
		advance();
		parseFactor();
	}
}

void Parser::parseFactor() {
	if(currentToken.type == TokenType::LPAREN) {
		advance();
		parseExpr();
		expect(TokenType::RPAREN);
	} else if(currentToken.type == TokenType::IDENTIFIER || currentToken.type == TokenType::NUMBER) {
		advance();
	} else {
		std::cerr << "Unexpected factor: " << currentToken.value << "\n";
		exit(EXIT_FAILURE);
	}
}

void Parser::parseOptExpr() {
	if(currentToken.type != TokenType::SEMICOLON) {
		parseExpr();
	}
}
