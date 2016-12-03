#include "BabyC.h"
#include <stdlib.h>
#include <stdio.h>


// Write the implementations of the functions that do the real work here

ASTNode *CreateNumNode(int num) {
    ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
    node->type = NUM;
    node->num = num;
    return node;
}


ASTNode *CreateIdentNode(char *name) {


}


void AddDeclaration(char *name) {


}


void GenerateILOC(ASTNode *node) {

}

