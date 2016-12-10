/**
 * Header file for a Baby C language compiler
 * Author: Kyle Szombathy
 */
#ifndef BABYC_H
#define BABYC_H
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    ASTNODE_ARITH_OP, ASTNODE_LOGIC_OP, ASTNODE_COMPARE, ASTNODE_ASSIGN,
    ASTNODE_IDENT, ASTNODE_NUM, ASTNODE_IF, ASTNODE_WHILE, ASTNODE_ST_LIST, ASTNODE_ELSE
} ASTNodeType;

// Define all operation types (add, mult, etc) here.
typedef enum {
    ADD_OP, SUB_OP, MULT_OP, DIV_OP, AND_OP, OR_OP, LE_OP, LT_OP, GE_OP, GT_OP, EQ_OP, NE_OP,
    loadI_op, loadAI_op, storeAI_op, label_op, jump_op, cbr_op // This row isn't necessarily AST ops, but it will do
} ASTOp;

typedef struct ASTNode ASTNode;
struct ASTNode {
    ASTNodeType type;
    ASTOp op; // The actual operation (add, mult, etc)
    int num;  //Need to store the actual value for number nodes
    char *name; //Need to store the actual variable name for ident nodes
    ASTNode *left; // Left child
    ASTNode *right; // Right child
    ASTNode *elseNode; // Used purely for if statements
    char * regNum;
    char * labelNum;

};

// Linked list type of Symbol table
struct SymbolTableEntry {
    char *ident;
    char* regNum;
    struct SymbolTableEntry *next;
};


// =================  Token Nodes =================
ASTNode *CreateNumNode(int num);

ASTNode *CreateIdentNode(char *name);

void HandleIdentLookup(char *name);

// =================  Maths =================
ASTNode *CreateAddNode(ASTNode *left, ASTNode *right);

ASTNode *CreateSubNode(ASTNode *left, ASTNode *right);

ASTNode *CreateMultNode(ASTNode *left, ASTNode *right);

ASTNode *CreateDivNode(ASTNode *left, ASTNode *right);

// ================= Statement Types =================
// Left node of statementList is statement, right is another statementList
ASTNode *CreateStatementListNode(ASTNode *statement, ASTNode *statementList);

ASTNode *CreateAssignNode(char *name, ASTNode *right);

ASTNode *CreateIfNode(ASTNode *condition, ASTNode *body, ASTNode *elseNode);

ASTNode *CreateWhileNode(ASTNode *condition, ASTNode *loop);

// ============= Condition =================
ASTNode *CreateAndNode(ASTNode *left, ASTNode *right);

ASTNode *CreateOrNode(ASTNode *left, ASTNode *right);

// ================= Compare =================

ASTNode *CreateLENode(ASTNode *left, ASTNode *right);

ASTNode *CreateLTNode(ASTNode *left, ASTNode *right);

ASTNode *CreateGENode(ASTNode *left, ASTNode *right);

ASTNode *CreateGTNode(ASTNode *left, ASTNode *right);

ASTNode *CreateEQNode(ASTNode *left, ASTNode *right);

ASTNode *CreateNENode(ASTNode *left, ASTNode *right);

// ================= Symbol Table =================

void AddDeclaration(char *name);

struct SymbolTableEntry *GetSymbolTableEntry(char *name);

bool DoesEntryExist(char *name);

void PrintSymbolTable();

// ================= Other =================

// This is the function that generates ILOC code after the construction of the AST
void GenerateILOC(ASTNode *node, FILE * fp);

void Emit(ASTOp op, char *src1, char *src2, char * target);

char * GetNextReg();

char * GetNextLabel();

char* itoa(int value, char* result, int base);

char * appendStr(char *str1, char *str2);

void freeAll();

#endif
