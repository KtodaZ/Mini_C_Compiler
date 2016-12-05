/**
 * Flex scanner for a Baby C language
 * CSC 151 Assignment 5
 * Author: Kyle Szombathy
 */

%{
#include <stdio.h>
#include <string.h>
#include "BabyC.tab.h"
#include "BabyC.h"

void yyerror (const char *s) 
{
   printf("ERROR on line %d: %s.\n", yylineno, s);
   exit(1);
}


%}

/*Option to get the line number*/
%option yylineno

/*We are not using yywrap. So, use this option to disable it and supress a compile error*/
%option noyywrap

%%
"," return ',';
";" return ';';
"+" return '+';
"-" return '-';
"*" return '*';
"/" return '/';
"<" return '<';
">" return '>';
"=" return '=';
"(" return '(';
")" return ')';
"{" return '{';
"}" return '}';

"||" return OR;
"&&" return AND; 
"==" return EQ;
"!=" return NE;
"<=" return LE;
">=" return GE;

"int" return INT;
"main" return MAIN;
"if"  return IF;
"else" return ELSE;
"while" return WHILE;

[a-z|A-Z]([a-z]|[A-Z]|[0-9])*  yylval.string = strdup(yytext);  return IDENT; //Identifier
0|[1-9][0-9]*	               yylval.num = atoi(yytext);       return NUM;   //Number

[ \t\n]+		//Whitespace is ignored
.           printf( "ERROR on Line %d: Unrecognized token \n", yylineno ); exit(1); //No match. Fatal error.


