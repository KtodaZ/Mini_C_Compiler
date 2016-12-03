%{
	#include <stdio.h>
	#include "your_code.h"

// The parser needs to call the scanner to get the next token 
	extern int yylex();

// The error function 
	extern int yyerror(const char *);

// The ASTNode root
        extern ASTNode* gASTRoot;
%}

//Put any initialization code here 
%initial-action 
{

}

%token LE "<="
%token GE ">="
%token EQ "=="
%token NE "!="
%token OR "||"
%token AND "&&"
%token MAIN "main"
%token INT "int"
%token IF "if"
%token ELSE "else"
%token WHILE "while"

//Define the types for the grammar attributes ($$, $1, $2, ...) 
%union 
{
	ASTNode* node; // Most $$ values will be ASTNodes 
	int num; // Integer numbers
	char* string; // Strings for identifiers 
}

//Specify the type for each token. Only needed for IDENT and NUM, because they are the only ones that have actual regexp rules
%token <string> IDENT
%token <num> NUM

//Specify the type for each non-terminal in the grammar. Here are some samples:
%type <node> Statement
%type <node> Assignment
%type <node> Expr
%type <node> Term
%type <node> Factor
// Add the rest of the types for the grammar's symbols


%%

// Write the grammar for BabyC, and write an embedded action for each production. Here are some samples for you:

Goal: "main" '(' ')' '{' DeclarationList StatementList '}'	{gASTRoot=$6;} // Store the AST root node in gASTRoot
;

DeclarationList:  | Declaration DeclarationList // Note that a DeclarationList may be empty
;

Declaration: "int" IDENT ';' {AddDeclaration($2);}
;

Factor: IDENT 		{$$ = CreateIdentNode($1);}
	| NUM 		{$$ = CreateNumNode($1);}
	| '('Expr')'	{$$ = $2;}
;

%%
