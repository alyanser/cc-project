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
    T_END,
    T_ADD,
    T_SUBTRACT,
    T_MULTIPLY,
    T_DIVIDE,
};

inline Terminal to_terminal(const Token & t) {

	switch(t.type) {
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

		case TokenType::TYPE: {

			if(t.value == "int") {
				return T_INT;
			} else if(t.value == "float") {
				return T_FLOAT;
			}
		} [[fallthrough]];

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

inline std::string to_string(const Non_terminal t) {

	switch(t) {
		case ARG: return "Arg";
		case ARG_LIST: return "ArgList";
		case COMPARE: return "Compare";
		case COMPOUND_STMT: return "CompoundStmt";
		case DECLARATION: return "Declaration";
		case EXPR: return "Expr";
		case FACTOR: return "Factor";
		case FOR_STMT: return "ForStmt";
		case FUNCTION: return "Function";
		case IDENT_LIST: return "IdentList";
		case IF_STMT: return "IfStmt";
		case LOOP_STMT: return "LoopStmt";
		case MAG: return "Mag";
		case MAGAR_PART: return "MagarPart";
		case OPT_EXPR: return "OptExpr";
		case RVALUE: return "Rvalue";
		case STMT: return "Stmt";
		case STMT_LIST: return "StmtList";
		case TERM: return "Term";
		case TYPE: return "Type";
	}

	std::cerr << "Error: Invalid non-terminal\n";
	std::exit(1);
}

/*
Function -> Type identifier < ArgList > CompoundStmt
ArgList -> Arg | ArgList ,Arg
Arg -> Type identifier
Declaration -> Type IdentList ;
Type -> int | float
IdentList -> identifier ,IdentList | identifier
Stmt -> ForStmt | WhileStmt | Expr ; | IfStmt | CompoundStmt | Declaration | ;
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
		compute_first();
		compute_follow();
		setup();
	}

	void compute_first() {
		// ARG
		first_[ARG][T_FLOAT] = true;
		first_[ARG][T_INT] = true;

		// ARG_LIST
		first_[ARG_LIST][T_FLOAT] = true;
		first_[ARG_LIST][T_INT] = true;

		// COMPARE
		first_[COMPARE][T_NOTEQUAL] = true;
		first_[COMPARE][T_LT] = true;
		first_[COMPARE][T_LE] = true;
		first_[COMPARE][T_NE] = true;
		first_[COMPARE][T_EQ] = true;
		first_[COMPARE][T_GT] = true;
		first_[COMPARE][T_GE] = true;

		// COMPOUND_STMT
		first_[COMPOUND_STMT][T_LBRACE] = true;

		// DECLARATION
		first_[DECLARATION][T_FLOAT] = true;
		first_[DECLARATION][T_INT] = true;

		// EXPR
		first_[EXPR][T_LT] = true;
		first_[EXPR][T_IDENTIFIER] = true;
		first_[EXPR][T_NUMBER] = true;

		// FACTOR
		first_[FACTOR][T_LT] = true;
		first_[FACTOR][T_IDENTIFIER] = true;
		first_[FACTOR][T_NUMBER] = true;

		// FOR_STMT
		first_[FOR_STMT][T_FOR] = true;

		// FUNCTION
		first_[FUNCTION][T_FLOAT] = true;
		first_[FUNCTION][T_INT] = true;

		// IDENT_LIST
		first_[IDENT_LIST][T_IDENTIFIER] = true;

		// IF_STMT
		first_[IF_STMT][T_AGAR] = true;

		// LOOP_STMT
		first_[LOOP_STMT][T_LOOP] = true;

		// MAG
		first_[MAG][T_LT] = true;
		first_[MAG][T_IDENTIFIER] = true;
		first_[MAG][T_NUMBER] = true;

		// MAGAR_PART
		first_[MAGAR_PART][T_MAGAR] = true;
		first_[MAGAR_PART][T_EPSILON] = true;

		// OPT_EXPR
		first_[OPT_EXPR][T_LT] = true;
		first_[OPT_EXPR][T_IDENTIFIER] = true;
		first_[OPT_EXPR][T_NUMBER] = true;
		first_[OPT_EXPR][T_EPSILON] = true;

		// RVALUE
		first_[RVALUE][T_LT] = true;
		first_[RVALUE][T_IDENTIFIER] = true;
		first_[RVALUE][T_NUMBER] = true;

		// STMT
		first_[STMT][T_SEMICOLON] = true;
		first_[STMT][T_LT] = true;
		first_[STMT][T_IDENTIFIER] = true;
		first_[STMT][T_NUMBER] = true;
		first_[STMT][T_AGAR] = true;
		first_[STMT][T_FLOAT] = true;
		first_[STMT][T_FOR] = true;
		first_[STMT][T_INT] = true;
		first_[STMT][T_LBRACE] = true;

		// STMT_LIST
		first_[STMT_LIST][T_EPSILON] = true;

		// TERM
		first_[TERM][T_LT] = true;
		first_[TERM][T_IDENTIFIER] = true;
		first_[TERM][T_NUMBER] = true;

		// TYPE
		first_[TYPE][T_FLOAT] = true;
		first_[TYPE][T_INT] = true;
	}

	void compute_follow() {
		follow_[ARG][T_COMMA] = true;
		follow_[ARG][T_RANGLE] = true;

		follow_[ARG_LIST][T_COMMA] = true;
		follow_[COMPARE][T_LT] = true;

		follow_[COMPARE][T_IDENTIFIER] = true;
		follow_[COMPARE][T_NUMBER] = true;

		follow_[COMPOUND_STMT][T_END] = true;
		follow_[COMPOUND_STMT][T_MAGAR] = true;

		follow_[DECLARATION][T_MAGAR] = true;

		follow_[EXPR][T_SEMICOLON] = true;
		follow_[EXPR][T_RANGLE] = true;

		follow_[FACTOR][T_NOTEQUAL] = true;
		follow_[FACTOR][T_SEMICOLON] = true;
		follow_[FACTOR][T_LT] = true;
		follow_[FACTOR][T_LE] = true;
		follow_[FACTOR][T_NE] = true;
		follow_[FACTOR][T_EQ] = true;
		follow_[FACTOR][T_GT] = true;
		follow_[FACTOR][T_GE] = true;
		follow_[FACTOR][T_ADD] = true;
		follow_[FACTOR][T_SUBTRACT] = true;
		follow_[FACTOR][T_DIVIDE] = true;

		follow_[FOR_STMT][T_MAGAR] = true;

		follow_[FUNCTION][T_END] = true;

		follow_[IDENT_LIST][T_SEMICOLON] = true;

		follow_[IF_STMT][T_MAGAR] = true;

		follow_[MAG][T_NOTEQUAL] = true;
		follow_[MAG][T_ADD] = true;
		follow_[MAG][T_SEMICOLON] = true;
		follow_[MAG][T_LT] = true;
		follow_[MAG][T_LE] = true;
		follow_[MAG][T_NE] = true;
		follow_[MAG][T_EQ] = true;
		follow_[MAG][T_GT] = true;
		follow_[MAG][T_GE] = true;
		follow_[MAG][T_SUBTRACT] = true;

		follow_[MAGAR_PART][T_MAGAR] = true;
		follow_[OPT_EXPR][T_SEMICOLON] = true;

		follow_[RVALUE][T_NOTEQUAL] = true;
		follow_[RVALUE][T_SEMICOLON] = true;
		follow_[RVALUE][T_LT] = true;
		follow_[RVALUE][T_LE] = true;
		follow_[RVALUE][T_NE] = true;
		follow_[RVALUE][T_EQ] = true;
		follow_[RVALUE][T_GT] = true;
		follow_[RVALUE][T_GE] = true;

		follow_[STMT][T_MAGAR] = true;

		follow_[STMT_LIST][T_RBRACE] = true;

		follow_[TERM][T_NOTEQUAL] = true;
		follow_[TERM][T_MULTIPLY] = true;
		follow_[TERM][T_ADD] = true;
		follow_[TERM][T_DIVIDE] = true;
		follow_[TERM][T_SEMICOLON] = true;
		follow_[TERM][T_LT] = true;
		follow_[TERM][T_LE] = true;
		follow_[TERM][T_NE] = true;
		follow_[TERM][T_EQ] = true;
		follow_[TERM][T_GT] = true;
		follow_[TERM][T_GE] = true;
		follow_[TERM][T_SUBTRACT] = true;

		follow_[TYPE][T_IDENTIFIER] = true;
	}

	void parse(Scanner & scanner) {
		std::stack<std::variant<Terminal, Non_terminal>> stack;

		stack.push(T_END);
		stack.push(FUNCTION);

		auto token = scanner.nextToken();

		while(std::holds_alternative<Non_terminal>(stack.top()) || std::get<Terminal>(stack.top()) != T_END) {
			const auto top = stack.top();
			stack.pop();

			if(std::holds_alternative<Terminal>(top)) {
				const auto terminal = std::get<Terminal>(top);

				if(token.type == TokenType::IDENTIFIER || to_string(terminal) == token.value) {
					std::cout << "[INFO] Matched " << to_string(terminal) << "\n";
					token = scanner.nextToken();
					continue;
				}

				std::cerr << "[Error] Expected " << to_string(terminal) << " but got " << token.value << "\n";
				return;
			}

			const auto non_terminal = std::get<Non_terminal>(top);

			auto production = parse_table_[non_terminal][to_terminal(token)];

			if(production.empty()) {
				std::cout << "[Warning] No production found for " << to_string(non_terminal) << " and " << token.value << '\n';

				if(follow_[non_terminal][to_terminal(token)]) {
					std::cout << "[Panic mode]: using sync\n";
					continue;
				}

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

			if(!symbol.empty()) {
				symbols.push_back(symbol);
			}

			for(auto it = symbols.rbegin(); it != symbols.rend(); ++it) {

				if(*it == "identifier") {
					stack.push(T_IDENTIFIER);
				} else if(*it == "Type") {
					stack.push(TYPE);
				} else if(*it == "ArgList") {
					stack.push(ARG_LIST);
				} else if(*it == "CompoundStmt") {
					stack.push(COMPOUND_STMT);
				} else if(*it == "int") {
					stack.push(T_INT);
				} else if(*it == "<") {
					stack.push(T_LT);
				} else if(*it == ">") {
					stack.push(T_GT);
				} else if(*it == "Arg") {
					stack.push(ARG);
				} else if(*it == "}") {
					stack.push(T_RBRACE);
				} else if(*it == "{") {
					stack.push(T_LBRACE);
				} else if(*it == "StmtList") {
					stack.push(STMT_LIST);
				} else if(*it == "StmtListStmt") {
					stack.push(STMT);
				} else if(*it == "Declaration") {
					stack.push(DECLARATION);
				} else if(*it == ";") {
					stack.push(T_SEMICOLON);
				} else if(*it == "=") {
					stack.push(T_EQ);
				} else if(*it == "IdentList") {
					stack.push(IDENT_LIST);
				} else if(*it == ",") {
					stack.push(T_COMMA);
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
	std::map<Non_terminal, std::map<Terminal, bool>> first_;
	std::map<Non_terminal, std::map<Terminal, bool>> follow_;
};
