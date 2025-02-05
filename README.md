# Input
	int foo < int arg > {
		int val;
	}


# Output
	[INFO] Using production rule "Type identifier < ArgList > CompoundStmt"
	[INFO] Using production rule "int"
	[INFO] Matched int
	[INFO] Matched identifier 'foo'
	[INFO] Matched <
	[INFO] Using production rule "Arg"
	[INFO] Using production rule "Type identifier"
	[INFO] Using production rule "int"
	[INFO] Matched int
	[INFO] Matched identifier 'arg'
	[INFO] Matched >
	[INFO] Using production rule "{ StmtList }"
	[INFO] Matched {
	[INFO] Using production rule "StmtListStmt"
	[INFO] Using production rule "Declaration"
	[INFO] Using production rule "Type IdentList ;"
	[INFO] Using production rule "int"
	[INFO] Matched int
	[INFO] Using production rule "identifier"
	[INFO] Matched identifier 'val'
	[INFO] Matched ;
	[INFO] Matched }

	Parsing sucessful!
