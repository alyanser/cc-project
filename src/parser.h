#pragma once

#include <map>
#include <vector>
#include <string>
#include <stack>
#include <variant>

#include "scanner.h"

enum Non_terminal {
    ARG,
    ARG_LIST,
    COMPARE,
    COMPOUND_STMT,
    DECLARATION,
    EXPR,
    FACTOR,
    FOR_STMT,
    FUNCTION,
    IDENT_LIST,
    IF_STMT,
    LOOP_STMT,
    MAG,
    MAGAR_PART,
    OPT_EXPR,
    RVALUE,
    STMT,
    STMT_LIST,
    TERM,
    TYPE
};

enum Terminal {
    T_FALSE,
    T_FLOAT,
    T_INT,
    T_COMMA,
    T_RANGLE,
    T_NOTEQUAL,
    T_LT,
    T_LE,
    T_NE,
    T_EQ,
    T_GT,
    T_GE,
    T_IDENTIFIER,
    T_NUMBER,
    T_LBRACE,
    T_RBRACE,
    T_SEMICOLON,
    T_FOR,
    T_MAGAR,
    T_AGAR,
    T_LOOP,
    T_EPSILON,
    T_END
};

inline Terminal to_terminal(const TokenType t) {

	switch(t) {
		case TokenType::IDENTIFIER: return T_IDENTIFIER;
		case TokenType::NUMBER: return T_NUMBER;
		case TokenType::COMMA: return T_COMMA;
		case TokenType::LT: return T_LT;
		case TokenType::GT: return T_GT;
		case TokenType::LBRACE: return T_LBRACE;
		case TokenType::RBRACE: return T_RBRACE;
		case TokenType::SEMICOLON: return T_SEMICOLON;
		case TokenType::FOR: return T_FOR;
		case TokenType::IF: return T_MAGAR;
		case TokenType::ELSE: return T_AGAR;
		case TokenType::END_OF_FILE: return T_END;

		default : {
			std::cerr << "Error: Invalid token type\n";
			std::exit(1);
		}
	}
}

inline std::string to_string(const Terminal t) {

	switch(t) {
		case T_FALSE: return "false";
		case T_FLOAT: return "float";
		case T_INT: return "int";
		case T_COMMA: return ",";
		case T_RANGLE: return ">";
		case T_NOTEQUAL: return "!=";
		case T_LT: return "<";
		case T_LE: return "<=";
		case T_NE: return "<>";
		case T_EQ: return "==";
		case T_GT: return ">";
		case T_GE: return ">=";
		case T_IDENTIFIER: return "identifier";
		case T_NUMBER: return "number";
		case T_LBRACE: return "{";
		case T_RBRACE: return "}";
		case T_SEMICOLON: return ";";
		case T_FOR: return "for";
		case T_MAGAR: return "magar";
		case T_AGAR: return "agar";
		case T_LOOP: return "loop";
		case T_EPSILON: return "ϵ";
		case T_END: return "$";
	}

	std::cerr << "Error: Invalid terminal\n";
	std::exit(1);
}

/*
   Function -> Type identifier < ArgList > CompoundStmt
ArgList -> Arg | ArgList ,Arg
Arg -> Type identifier
Declaration -> Type IdentList ;
Type -> int | float
IdentList -> identifier ,IdentList | identifier
Stmt -> ForStmt | WhileStmt | Expr ; | IfStmt
| CompoundStmt | Declaration | ;
ForStmt -> for < Expr ; OptExpr ; OptExpr > Stmt
OptExpr -> Expr |  
LoopStmt -> loop < Expr > Stmt
IfStmt -> agar < Expr > Stmt MagarPart
MagarPart -> magar Stmt |  
CompoundStmt -> { StmtList }
StmtList -> StmtListStmt |  
Expr -> Rvalue
Rvalue -> Rvalue Compare Mag | Mag
Compare -> == | < | > | <= | >= | != | <>
Mag -> Mag + Term | Mag – Term | Term
Term -> Term * Factor | Term / Factor | Factor
Factor -> < Expr > | identifier | number
*/

/*
Symbol	First			Follow
Arg	float int					,Arg >
ArgList	float int				,Arg >
Compare	!= < <= <> == > >=				< identifier number
CompoundStmt	{				$ magar
Declaration	float int				magar
Expr	< identifier number				; >
Factor	< identifier number			!= * + / ; < <= <> == > >= –
ForStmt	for				magar
Function	float int				$
IdentList	identifier					;
IfStmt	agar				magar
LoopStmt	loop	
Mag	< identifier number				!= + ; < <= <> == > >= –
MagarPart	magar ϵ					magar
OptExpr	< identifier number ϵ				; >
Rvalue	< identifier number				!= ; < <= <> == > >=
Stmt	; < WhileStmt agar float for identifier int number {			magar
StmtList	StmtListStmt ϵ			}
Term	< identifier number				!= * + / ; < <= <> == > >= –
Type	float int					identifier
*/

class Parser {
public:

	Parser() {
		setup();
	}

	void parse(Scanner & scanner) {
		std::stack<std::variant<Terminal, Non_terminal>> stack;

		stack.push(T_END);
		stack.push(FUNCTION);

		while(std::holds_alternative<Terminal>(stack.top()) || std::get<0>(stack.top()) != T_END) {
			const auto top = stack.top();
			stack.pop();

			if(std::holds_alternative<Terminal>(top)) {
				const auto terminal = std::get<Terminal>(top);
				const auto token = scanner.nextToken();

				if(token.type == TokenType::IDENTIFIER || to_string(terminal) == token.value) {
					std::cout << "Matched " << to_string(terminal) << "\n";
					continue;
				}

				std::cout << "Error: Expected " << to_string(terminal) << " but got " << token.value << "\n";
			} else {
				const auto non_terminal = std::get<Non_terminal>(top);
				const auto token = scanner.nextToken();

				auto production = parse_table_[non_terminal][to_terminal(token.type)];

				if(production.empty()) {
					std::cout << "Error: Unexpected token " << token.value << "\n";
					return;
				}

				std::vector<std::string> symbols;
				std::string symbol;

				for(const auto c : production) {
					if(c == ' ') {
						symbols.push_back(symbol);
						symbol.clear();
					} else {
						symbol += c;
					}
				}
			}
		}
	}

private:

	void setup() {
		parse_table_[FUNCTION][T_INT] = "Type identifier < ArgList > CompoundStmt";
		parse_table_[FUNCTION][T_FLOAT] = "Type identifier < ArgList > CompoundStmt";

		parse_table_[ARG_LIST][T_INT] = "Arg";
		parse_table_[ARG_LIST][T_FLOAT] = "Arg";

		parse_table_[ARG][T_INT] = "Type identifier";
		parse_table_[ARG][T_FLOAT] = "Type identifier";


		parse_table_[DECLARATION][T_INT] = "Type IdentList ;";
		parse_table_[DECLARATION][T_FLOAT] = "Type IdentList ;";

		parse_table_[TYPE][T_INT] = "int";
		parse_table_[TYPE][T_FLOAT] = "float";

		parse_table_[IDENT_LIST][T_IDENTIFIER] = "identifier , IdentList";
		parse_table_[IDENT_LIST][T_EPSILON] = "identifier";

		parse_table_[STMT][T_FOR] = "ForStmt";
		parse_table_[STMT][T_LOOP] = "LoopStmt";
		parse_table_[STMT][T_IDENTIFIER] = "Expr ;";
		parse_table_[STMT][T_FLOAT] = "Declaration";
		parse_table_[STMT][T_INT] = "Declaration";
		parse_table_[STMT][T_AGAR] = "IfStmt";

		parse_table_[FOR_STMT][T_FOR] = "for < Expr ; OptExpr ; OptExpr > Stmt";

		parse_table_[OPT_EXPR][T_IDENTIFIER] = "Expr";
		parse_table_[OPT_EXPR][T_NUMBER] = "Expr";
		parse_table_[OPT_EXPR][T_EPSILON] = "";

		parse_table_[LOOP_STMT][T_LOOP] = "loop < Expr > Stmt";

		parse_table_[IF_STMT][T_AGAR] = "agar < Expr > Stmt MagarPart";

		parse_table_[MAGAR_PART][T_MAGAR] = "magar Stmt";
		parse_table_[MAGAR_PART][T_EPSILON] = "";

		parse_table_[COMPOUND_STMT][T_LBRACE] = "{ StmtList }";

		parse_table_[STMT_LIST][T_IDENTIFIER] = "StmtListStmt";
		parse_table_[STMT_LIST][T_FLOAT] = "StmtListStmt";
		parse_table_[STMT_LIST][T_INT] = "StmtListStmt";
		parse_table_[STMT_LIST][T_AGAR] = "StmtListStmt";
		parse_table_[STMT_LIST][T_FOR] = "StmtListStmt";
		parse_table_[STMT_LIST][T_LOOP] = "StmtListStmt";
		parse_table_[STMT_LIST][T_LBRACE] = "StmtListStmt";
		parse_table_[STMT_LIST][T_RBRACE] = "";

		parse_table_[EXPR][T_IDENTIFIER] = "Rvalue";
		parse_table_[EXPR][T_NUMBER] = "Rvalue";
		parse_table_[EXPR][T_LBRACE] = "Rvalue";

		parse_table_[RVALUE][T_IDENTIFIER] = "Rvalue";
		parse_table_[RVALUE][T_NUMBER] = "Rvalue";
		parse_table_[RVALUE][T_LBRACE] = "Rvalue";

		parse_table_[TERM][T_IDENTIFIER] = "Factor";
		parse_table_[TERM][T_NUMBER] = "Factor";
		parse_table_[TERM][T_LBRACE] = "Factor";

		parse_table_[FACTOR][T_IDENTIFIER] = "identifier";
		parse_table_[FACTOR][T_NUMBER] = "number";
		parse_table_[FACTOR][T_LBRACE] = "< Expr";

		parse_table_[COMPARE][T_EQ] = "==";
		parse_table_[COMPARE][T_LT] = "<";
		parse_table_[COMPARE][T_GT] = ">";
		parse_table_[COMPARE][T_LE] = "<=";
		parse_table_[COMPARE][T_GE] = ">=";
		parse_table_[COMPARE][T_NE] = "!=";
		parse_table_[COMPARE][T_NOTEQUAL] = "<>";

		parse_table_[MAG][T_IDENTIFIER] = "Term";
		parse_table_[MAG][T_NUMBER] = "Term";
		parse_table_[MAG][T_LBRACE] = "Term";

		parse_table_[TERM][T_IDENTIFIER] = "Term * Factor";
		parse_table_[TERM][T_NUMBER] = "Term * Factor";
		parse_table_[TERM][T_LBRACE] = "Term * Factor";

		parse_table_[TERM][T_IDENTIFIER] = "Term / Factor";
		parse_table_[TERM][T_NUMBER] = "Term / Factor";
		parse_table_[TERM][T_LBRACE] = "Term / Factor";

		parse_table_[TERM][T_IDENTIFIER] = "Factor";
		parse_table_[TERM][T_NUMBER] = "Factor";
		parse_table_[TERM][T_LBRACE] = "Factor";
	}

	std::map<Non_terminal, std::map<Terminal, std::string>> parse_table_;
};
