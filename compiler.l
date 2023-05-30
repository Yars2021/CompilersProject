%option noyywrap nodefault yylineno

%{
#include <stdio.h>
#include "compiler.tab.h"
%}

%%

"var"  {return VAR;}
"while" {return WHILE;}
"do"   {return DO;}
"print" {return PRINT;}

"="    {return ASSIGN;}

"not"  {return NOT;}

"-"    {return MINUS;}
"+"    {return PLUS;}
"*"    {return MUL;}
"/"    {return DIV;}
"<"    {return LESS;}
">"    {return MORE;}
"=="   {return EQUALS;}

"{"    {return LCBR;}
"}"    {return RCBR;}

"("    {return LBR;}
")"    {return RBR;}

";"    {return SEMICOLON;}
","    {return COMMA;}

[a-z]  {return LETTER;}
[0-9]  {return NUMBER;}

[\n]                            /* ignore newlines */;
[ \t]+                          /* ignore whitespace */;
"//".*                          /* ignore comments */;

.       {yyerror("Undefined character\n");}

%%