/**
 * Bison Parser for a Baby C language
 * CSC 151 Assignment 5
 * Author: Kyle Szombathy
 */

%{
/**C DECLARATIONS SECTIONS*/
    #include <stdio.h>
    #include "BabyC.h"

// The parser needs to call the scanner to get the next token 
    extern int yylex();

// The error function 
    extern int yyerror(const char *);

// The ASTNode root
    extern ASTNode* gASTRoot;
%}
/**BISON DECLARATIONS SECTION*/

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
{   ASTNode* node; // Most $$ values will be ASTNodes
    int num; // Integer numbers
    char* string; // Strings for identifiers
}

//Specify the type for each token. Only needed for IDENT and NUM, because they are the only ones that have actual regexp rules
%token <string> IDENT
%token <num> NUM

//Specify the type for each non-terminal in the grammar
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



%%
/**GRAMMAR RULES SECTION
 * In this section we will write the rules for each grammar and add them to the AST*/

// Language can start with declarations, followed by statements.
Goal
        : MAIN '(' ')' '{' DeclarationList StatementList '}'	{gASTRoot=$6;} // Store the AST root node in gASTRoot
;

DeclarationList
        :
        | Declaration DeclarationList // Note that a DeclarationList may be empty
;

Declaration
        : INT IDENT ';' {AddDeclaration($2);}
;

StatementList
        :
        | StatementList Statement
            {
                $$ = $1; //? Should this create a node???
            }
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
        : Term            {$$ = $1;}
        | Expr '+' Term   {$$ = CreateAddNode($1, $3);}
        | Expr '-' Term   {$$ = CreateSubNode($1, $3);}
;

Term
        : Factor          {$$ = $1;}
        | Term '*' Factor {$$ = CreateMultNode($1, $3);}
        | Term '/' Factor {$$ = CreateDivNode($1, $3);}
;

Factor
        : IDENT 		  {$$ = CreateIdentNode($1);}
        | NUM 		      {$$ = CreateNumNode($1);}
        | '('Expr')'      {$$ = $2;}
;

If
        : IF '(' Condition ')' '{' StatementList //{$$ = CreateIfNode($3, $6, 0);}
        | IF '(' Condition ')' '{' StatementList '}' ELSE '{' StatementList '}' //{$$ = CreateIfNode($3, $6, $10);}
;

While
        : WHILE '(' Condition ')'  '{' StatementList '}' //{$$ = CreateWhileNode($3, $6);}
;

Condition
        : Compare AND Compare
        | Compare OR Compare
        | Compare
;

Compare
        : Expr LE Expr
        | Expr LT Expr
        | Expr GE Expr
        | Expr GT Expr
        | Expr EQ Expr
        | Expr NE Expr
;


%%

/**ADDITIONAL C CODE (OPTIONAL)*/
//========Functions========
ASTNode *CreateNewASTNode(ASTNodeType type, ASTOp op, int num, char *name, ASTNode *left, ASTNode *right) {
    ASTNode *newNode = emalloc (sizeof (ASTNode));
    newNode->type = type;
    newNode->op = op;
    newNode->num = num;
    newNode->name = name;
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

ASTNode *CreateNumNode(int num) {
    return CreateNewASTNode(NUM, 0, num, 0, 0, 0);
}

ASTNode *CreateIdentNode(char *name) {
    return CreateNewASTNode(IDENT, 0, 0, name, 0, 0);
}

ASTNode *CreateAssignNode(char *name, ASTNode *right) {
    return CreateNewASTNode(ASSIGN, 0, 0, 0, CreateIdentNode(name), right);
}

void AddDeclaration(char *name) {

}

void *emalloc(unsigned size)
{
    void *p = malloc(size);
    if (p == 0) {
        fprintf(stderr, "out of memory!\n");
        exit(1);
    }
    return p;
}