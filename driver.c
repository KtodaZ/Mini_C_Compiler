#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "BabyC_code.h"
#include "BabyC_code.c"

extern FILE *yyin;

extern int yyparse();
void removeSubstring(char *s,const char *toremove);

// A global variable that holds a pointer to the AST root
ASTNode *gASTRoot;

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("ERROR: Invalid number of command-line arguments. Usage: bcc File_Name.bc\n");
        exit(1);
    }
    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        printf("ERROR: Failed to open the input file\n");
        exit(1);
    }

    removeSubstring(argv[1], ".bc"); // Remove file extension from input filename
    char * newfiletype = ".iloc";
    char * fileURL ;
    if((fileURL = malloc(strlen(argv[1])+strlen(newfiletype)+1)) != NULL){
        fileURL[0] = '\0';   // ensures the memory is an empty string
        strcat(fileURL,argv[1]);
        strcat(fileURL,newfiletype);
    } else {
        printf("ERROR: malloc failed!\n");
        exit(1);
    }

    // Create output file
    FILE * fp;
    fp = fopen (fileURL, "w+");

    // Call the parser.
    // Add embedded actions to the parser (in BabyC.y) to construct the AST and store its root in gASTRoot.
    yyparse();
    fclose(yyin);

    // Now that the AST has been constructed, pass its root to the function that traverses it and generates the ILOC code.
    GenerateILOC(gASTRoot, fp);
    PrintSymbolTable();
    printf("Program reached end.\n");
}

void removeSubstring(char *s,const char *toremove) {
    s=(strstr(s,toremove) );
    memmove(s,s+strlen(toremove),1+strlen(s+strlen(toremove)));
}