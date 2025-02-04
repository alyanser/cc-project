#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include "scanner.h" // Assuming you have a scanner header providing token definitions

class Parser {
public:
	explicit Parser(Scanner & scanner);
	void parse(); // Entry point for parsing

private:
	Token currentToken;
	Scanner & scanner;

	void advance();		     // Fetch the next token
	void expect(TokenType type); // Ensure the token matches expectation

	void parseFunction();
	void parseArgList();
	void parseArg();
	void parseDeclaration();
	void parseStmt();
	void parseForStmt();
	void parseLoopStmt();
	void parseIfStmt();
	void parseCompoundStmt();
	void parseExpr();
	void parseRvalue();
	void parseMag();
	void parseTerm();
	void parseFactor();
	void parseOptExpr();
};

#endif // PARSER_H
