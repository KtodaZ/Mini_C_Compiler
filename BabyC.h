#ifndef BABYC_H
#define BABYC_H

typedef enum {
    ARITH_OP, LOGIC_OP, COMPARE, ASSIGN, IDENT, NUM, IF, WHILE
} ASTNodeType;

// Define all operation types (add, mult, etc) here.
typedef enum {
    ADD_OP, SUB_OP, MULT_OP, DIV_OP
} ASTOp;

typedef struct ASTNode ASTNode;
struct ASTNode {
    ASTNodeType type;
    ASTOp op; // The actual operation (add, mult, etc)
    int num;  //Need to store the actual value for number nodes
    char *name; //Need to store the actual variable name for ident nodes
    ASTNode *left; // Left child
    ASTNode *right; // Right child

// Depending on your implementation, you may need to add other fields to this struct 

};

// Function to create an AST Node
ASTNode *CreateNewASTNode(ASTNodeType type, ASTOp op, int num, char *name, ASTNode *left, ASTNode *right);

// Functions to create the different kinds of ASTNodes
ASTNode *CreateNumNode(int num);

ASTNode *CreateIdentNode(char *name);

ASTNode *CreateAssignNode(char *name, ASTNode *right);

//ASTNode *CreateIfNode(ASTNode *condition, ASTNode *result, ASTNode *Else);

//ASTNode *CreateWhileNode(ASTNode *codition, ASTNode *loop);

ASTNode *CreateAddNode(ASTNode *left, ASTNode *right);

ASTNode *CreateSubNode(ASTNode *left, ASTNode *right);

ASTNode *CreateMultNode(ASTNode *left, ASTNode *right);

ASTNode *CreateDivNode(ASTNode *left, ASTNode *right);

// Need a function to add a declaration to your symbol table
void AddDeclaration(char *name);

// This is the function that generates ILOC code after the construction of the AST
void GenerateILOC(ASTNode *node);

// malloc that throws an error if null
void *emalloc(unsigned size);





#endif
