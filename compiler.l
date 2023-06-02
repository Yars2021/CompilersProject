%option noyywrap nodefault yylineno

%{
#include <stdio.h>
#include "ast.h"
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

[a-z]+  {sscanf(yytext,"%s", yylval.var_name); return IDENT;}
[0-9]+  {yylval.number = atoi(yytext); return NUMBER;}

[\n]                            /* ignore newlines */;
[ \t]+                          /* ignore whitespace */;
"//".*                          /* ignore comments */;

.       {yyerror("Undefined character\n");}

%%