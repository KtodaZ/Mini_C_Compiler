/**
 * Bison Parser for a Baby C language
 * Author: Kyle Szombathy
 */

%{
    #include <stdio.h>
    #include "BabyC_code.h"

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
%token LT "<"
%token GE ">="
%token GT ">"
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
%type <node> Goal
%type <node> DeclarationList
%type <node> Declaration
%type <node> StatementList
%type <node> Statement
%type <node> Assignment
%type <node> Expr
%type <node> Term
%type <node> Factor
%type <node> If
%type <node> While
%type <node> Condition
%type <node> Compare
// Add the rest of the types for the grammar's symbols


%%
// Write the grammar for BabyC, and write an embedded action for each production. Here are some samples for you:

Goal: "main" '(' ')' '{' DeclarationList StatementList '}'	{gASTRoot=$6;} // Store the AST root node in gASTRoot
;

DeclarationList
        :
| Declaration DeclarationList // Note that a DeclarationList may be empty
;

Declaration: "int" IDENT ';' {AddDeclaration($2);}
;

StatementList
        : {$$ = NULL; }
| Statement StatementList	{$$ = CreateStatementListNode($1, $2);}
;

Statement
        : Assignment      {$$ = $1;}
| If              {$$ = $1;}
| While           {$$ = $1;}
;

Assignment
        : IDENT '=' Expr ';' {$$ = CreateAssignNode($1, $3);}
;

Expr
        : Term            {$$ = $1; }
| Expr '+' Term   {$$ = CreateAddNode($1, $3); }
| Expr '-' Term   {$$ = CreateSubNode($1, $3); }
;

Term
        : Factor          {$$ = $1;}
| Term '*' Factor {$$ = CreateMultNode($1, $3); }
| Term '/' Factor {$$ = CreateDivNode($1, $3); }
;

Factor
        : IDENT 		  {$$ = CreateIdentNode($1);}
| NUM 		      {$$ = CreateNumNode($1); }
| '('Expr')'      {$$ = $2; }
;

If
        : IF '(' Condition ')' '{' StatementList '}' {$$ = CreateIfNode($3, $6, NULL);}
| IF '(' Condition ')' '{' StatementList '}' ELSE '{' StatementList '}' {$$ = CreateIfNode($3, $6, $10);}
;

While
        : WHILE '(' Condition ')'  '{' StatementList '}' {$$ = CreateWhileNode($3, $6);}
;

Condition
        : Compare AND Compare   {$$ = CreateAndNode($1, $3);}
| Compare OR Compare    {$$ = CreateOrNode($1, $3);}
| Compare               {$$ = $1;}
;

Compare
        : Expr LE Expr {$$ = CreateLENode($1,$3);}
| Expr LT Expr {$$ = CreateLTNode($1,$3);}
| Expr GE Expr {$$ = CreateGENode($1,$3);}
| Expr GT Expr {$$ = CreateGTNode($1,$3);}
| Expr EQ Expr {$$ = CreateEQNode($1,$3);}
| Expr NE Expr {$$ = CreateNENode($1,$3);}
;

%%
