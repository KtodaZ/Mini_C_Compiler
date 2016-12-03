#include <stdio.h>
#include <stdlib.h>
#include "BabyC.h"

extern FILE* yyin;
extern int yyparse();

// A global variable that holds a pointer to the AST root
ASTNode* gASTRoot;

int main(int argc, char**argv)
{
        if (argc != 2){
                 printf("ERROR: Invalid number of command-line arguments. Usage: bcc File_Name.bc\n");
                 exit(1);
        }
	yyin = fopen(argv[1], "r" );
        if (yyin == NULL){
                 printf("ERROR: Failed to open the input file\n");
                 exit(1);
        }

        // Call the parser. 
        // Add embedded actions to the parser (in BabyC.y) to construct the AST and store its root in gASTRoot. 
        yyparse();
        fclose(yyin);

        // Now that the AST has been constructed, pass its root to the function that traverses it and generates the ILOC code.
        GenerateILOC(gASTRoot);	
}
