#ifndef BABYC_H
#define BABYC_H

typedef enum {
    ARITH_OP, LOGIC_OP, COMPARE, ASSIGN, IDENT, NUM, IF, WHILE
} ASTNodeType;

// Define all operation types (add, mult, etc) here.
typedef enum {
    ADD_OP, MULT_OP, ....
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

// Add functions to create the different kinds of ASTNodes 
// These functions are called by the code embedded in the grammar.
// Here are a couple of samples:  
ASTNode *CreateNumNode(int num);

ASTNode *CreateIdentNode(char *name);

// Need a function to add a declaration to your symbol table
void AddDeclaration(char *name);

// This is the function that generates ILOC code after the construction of the AST
void GenerateILOC(ASTNode *node);

#endif
