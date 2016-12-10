/**
 * Code section for a Baby C language compiler
 * Author: Kyle Szombathy
 */
#include "BabyC_code.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct SymbolTableEntry *symbolTableHead = NULL;
int regNumber = 0;
int labelNum = 0;
FILE * fileToPrintTo = NULL;

// =================  Token Nodes =================
ASTNode *CreateNumNode(int num) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_NUM;
    node->num = num;
    return node;
}

ASTNode *CreateIdentNode(char *name) {
    HandleIdentLookup(name);
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_IDENT;
    node->name = name;
    return node;
}

void HandleIdentLookup(char *name) {
    if(DoesEntryExist(name)) {
        // Do something???
    } else {
        printf("ERROR: identifier %s has not been declared.\n", name);
        exit(1);
    }
}

// =================  Maths =================
ASTNode *CreateAddNode(ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_ARITH_OP;
    node->op = ADD_OP;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *CreateSubNode(ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_ARITH_OP;
    node->op = SUB_OP;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *CreateMultNode(ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_ARITH_OP;
    node->op = MULT_OP;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *CreateDivNode(ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_ARITH_OP;
    node->op = DIV_OP;
    node->left = left;
    node->right = right;
    return node;
}

// ================= Statement Types =================
// Left node of statementList is statement, right is another statementList
ASTNode *CreateStatementListNode(ASTNode *statement, ASTNode *statementList) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_ST_LIST;
    node->left = statement;
    node->right = statementList;
    return node;
}

ASTNode *CreateAssignNode(char *name, ASTNode *right) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_ASSIGN;
    node->name = name;
    node->left = CreateIdentNode(name);
    node->right = right;
    return node;
}

ASTNode *CreateIfNode(ASTNode *condition, ASTNode *body, ASTNode *elseNode) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_IF;
    node->left = condition;
    node->right = body;
    node->elseNode = elseNode;
    return node;
}

ASTNode *CreateWhileNode(ASTNode *condition, ASTNode *loop) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_WHILE;
    node->left = condition;
    node->right = loop;
    return node;
}

// ============= Condition =================
ASTNode *CreateAndNode(ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_LOGIC_OP;
    node->op = AND_OP;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *CreateOrNode(ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_LOGIC_OP;
    node->op = OR_OP;
    node->left = left;
    node->right = right;
    return node;
}

// ================= Compare =================

ASTNode *CreateLENode(ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_COMPARE;
    node->op = LE_OP;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *CreateLTNode(ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_COMPARE;
    node->op = LT_OP;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *CreateGENode(ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_COMPARE;
    node->op = GE_OP;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *CreateGTNode(ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_COMPARE;
    node->op = GT_OP;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *CreateEQNode(ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_COMPARE;
    node->op = EQ_OP;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *CreateNENode(ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = ASTNODE_COMPARE;
    node->op = NE_OP;
    node->left = left;
    node->right = right;
    return node;
}

// ================= Symbol Table stuffs =================
void AddDeclaration(char *name) {
    struct SymbolTableEntry *entry = (struct SymbolTableEntry *) malloc(sizeof(struct SymbolTableEntry));
    if (NULL == entry) printf("\n Symbol Table Entry creation failed \n");
    entry->ident = name;
    entry->regNum = NULL;
    entry->next = NULL;

    struct SymbolTableEntry *tail = symbolTableHead;
    if (tail) {
        // Move tail to last node
        do {
            //printf("strcmp %s, %s = %d\n", name, tail->ident, strcmp(name, tail->ident));
            if(0 == strcmp(name, tail->ident)) {
                printf("ERROR: Identifier %s has already been declared.\n", name);
                exit(1);
            }
            if(tail->next != NULL) tail = tail->next;
        } while(tail->next);
        tail->next = entry;
    } else {
        symbolTableHead = entry;
    }
}

struct SymbolTableEntry *GetSymbolTableEntry(char *name) {
    struct SymbolTableEntry *curEntry = symbolTableHead;
    while (curEntry != NULL) {
        if (curEntry->ident != NULL && 0 == strcmp(name, curEntry->ident)) {
            return curEntry;
        }
        curEntry = curEntry->next;
    }

    return NULL;
}

bool DoesEntryExist(char *name) {
    return (NULL != GetSymbolTableEntry(name));
}

void PrintSymbolTable() {
    printf("Printing Symbol Table:\n");
    struct SymbolTableEntry *curEntry = symbolTableHead;
    int n = 0;
    while (curEntry != NULL) {
        printf("%d: ident:%s reg:%s\n", n, curEntry->ident, curEntry->regNum);
        curEntry = curEntry->next;
        n++;
    }
    printf("\n");
}

void GenerateILOC(ASTNode *node, FILE * fp) {
    if(NULL != fp && fileToPrintTo == NULL) fileToPrintTo = fp;

    // If StList, go down left side first, until you have traversed the left side of the tree fully
    if (node->left != NULL && (node->type != ASTNODE_IF) && (node->type != ASTNODE_WHILE)) GenerateILOC(node->left, NULL);
    if ((node->right != NULL) && (node->type != ASTNODE_ST_LIST)
        && (node->type != ASTNODE_IF) && (node->type != ASTNODE_WHILE)) GenerateILOC(node->right, NULL);

    switch (node->type) {
        case ASTNODE_ST_LIST:
            printf("Found StList node.\n\n");
            if (node->right != NULL) GenerateILOC(node->right, NULL);
            break;
        case ASTNODE_ARITH_OP:
            printf("Found Arithmetic node - %d\n", node->op);
            node->regNum = GetNextReg();
            Emit(node->op, node->left->regNum, node->right->regNum, node->regNum);
            break;
        case ASTNODE_LOGIC_OP:
            printf("Found Logic node - %d\n", node->op);
            node->regNum = GetNextReg();
            Emit(node->op, node->left->regNum, node->right->regNum, node->regNum);
            break;
        case ASTNODE_COMPARE:
            printf("Found Compare node - %d\n", node->op);
            node->regNum = GetNextReg();
            Emit(node->op, node->left->regNum, node->right->regNum, node->regNum);
            break;
        case ASTNODE_ASSIGN:
            printf("Found Assign node.\n");
            Emit(storeAI_op, node->right->regNum, NULL, appendStr("rarp, @", node->left->name));
            break;
        case ASTNODE_IDENT:
            printf("Found Ident node - %s\n", node->name);
            // Load a proper reg for the ident
            node->regNum = GetSymbolTableEntry(node->name)->regNum;
            if(node->regNum == NULL) node->regNum = GetNextReg();
            // Gen code
            Emit(loadAI_op, "rarp", appendStr("@", node->name), node->regNum);
            // Store the reg in the symbol table
            GetSymbolTableEntry(node->name)->regNum = node->regNum;
            break;
        case ASTNODE_NUM:
            printf("Found Num node - %d\n", node->num);
            node->regNum = GetNextReg();
            // Convert num to string
            char numStr[5];
            itoa(node->num, numStr, 10);
            Emit(loadI_op, numStr, NULL, node->regNum);
            break;
        case ASTNODE_IF:
            printf("Found If node.\n");

            // If statement block
            if(node->labelNum == NULL) node->labelNum = GetNextLabel();
            Emit(label_op, node->labelNum, NULL, NULL);
            GenerateILOC(node->left, NULL);

            // Create #basic blocks
            char * afterLabel;
            node->right->labelNum = GetNextLabel();
            if(NULL != node->elseNode) {
                node->elseNode->labelNum = GetNextLabel();
                Emit(cbr_op, node->left->regNum, NULL, appendStr(node->right->labelNum, appendStr(", ", node->elseNode->labelNum)) );
                afterLabel = GetNextLabel();
            } else {
                //If else doesn't exist use afterlabel instead
                afterLabel = GetNextLabel();
                Emit(cbr_op, node->left->regNum, NULL, appendStr(node->right->labelNum, appendStr(", ", afterLabel)) );
            }

            // Create body block
            Emit(label_op, node->right->labelNum, NULL, NULL);
            if(node->right != NULL) GenerateILOC(node->right, NULL);
            Emit(jump_op, afterLabel, NULL, NULL);

            // Create else block (if exists)
            if(NULL != node->elseNode) {
                Emit(label_op, node->elseNode->labelNum, NULL, NULL);
                GenerateILOC(node->elseNode, NULL);
                // Don't need a jump to afterLabel because it comes next
            }

            // After block
            Emit(label_op, afterLabel, NULL, NULL);
            break;
        case ASTNODE_ELSE:
            break;
        case ASTNODE_WHILE:
            printf("Found While node.\n");

            // While statement block
            if(node->labelNum == NULL) node->labelNum = GetNextLabel();
            Emit(label_op, node->labelNum , NULL, NULL);
            GenerateILOC(node->left, NULL);

            // Create #basic blocks
            char * afterLabelWhile;
            node->right->labelNum = GetNextLabel();
            afterLabelWhile = GetNextLabel();
            Emit(cbr_op, node->left->regNum, NULL, appendStr(node->right->labelNum, appendStr(", ", afterLabelWhile)) );

            // Body block
            Emit(label_op, node->right->labelNum, NULL, NULL);
            if(node->right != NULL) GenerateILOC(node->right, NULL);
            Emit(jump_op, node->labelNum, NULL, NULL);

            // After block
            Emit(label_op, afterLabelWhile, NULL, NULL);
            break;
        default:
            printf("ERROR: Invalid node\n");
            break;
    }

}



void Emit(ASTOp op, char *src1, char *src2, char *target) {
    char * opcode;

    switch (op) {
        case loadI_op:
            opcode = "loadI";
            break;
        case loadAI_op:
            opcode = "loadAI";
            break;
        case storeAI_op:
            opcode = "storeAI";
            break;
        case jump_op:
            opcode = "jump";
            break;
        case cbr_op:
            opcode = "cbr";
            break;
        case ADD_OP:
            opcode = "add";
            break;
        case SUB_OP:
            opcode = "sub";
            break;
        case MULT_OP:
            opcode = "mult";
            break;
        case DIV_OP:
            opcode = "div";
            break;
        case AND_OP:
            opcode = "and";
            break;
        case OR_OP:
            opcode = "or";
            break;
        case LE_OP:
            opcode = "cmp_LE";
            break;
        case LT_OP:
            opcode = "cmp_LT";
            break;
        case GE_OP:
            opcode = "cmp_GE";
            break;
        case GT_OP:
            opcode = "cmp_GT";
            break;
        case EQ_OP:
            opcode = "cmp_EQ";
            break;
        case NE_OP:
            opcode = "cmp_NE";
            break;
        case label_op:
            // Skip
            break;
        default:
            printf("ERROR: Invalid operation.\n");
            exit(1);
            break;
    }
    if(op == label_op) fprintf(fileToPrintTo, "\n%s: ", src1); // Don't print a new line
    else if(src2 == NULL && target == NULL) fprintf(fileToPrintTo, "%s => %s\n", opcode, src1);
    else if(src2 != NULL) fprintf(fileToPrintTo, "%s %s, %s => %s\n", opcode, src1, src2, target);
    else fprintf(fileToPrintTo, "%s %s => %s\n", opcode, src1, target);
}

char * GetNextReg() {
    char * prefix = "r";
    char regNumberStr[5];
    itoa(regNumber, regNumberStr, 10);
    regNumber++;
    return appendStr(prefix, regNumberStr);
}

char * GetNextLabel() {
    char * prefix = "L";
    char labelStr[5];
    itoa(labelNum, labelStr, 10);
    labelNum++;
    return appendStr(prefix, labelStr);
}

char * appendStr(char *str1, char *str2) {
    char * returnString;
    if((returnString = malloc(strlen(str1)+strlen(str2)+1)) != NULL){
        returnString[0] = '\0';   // ensures the memory is an empty string
        strcat(returnString,str1);
        strcat(returnString,str2);
    } else {
        printf("ERROR: malloc failed!\n");
        exit(1);
    }
    return returnString;
}


/**
	 * C++ version 0.4 char* style "itoa":
	 * Written by Lukás Chmela
	 * Released under GPLv3.

	 */
char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}


void freeAll() {
    //TODO for good practice
}