%{
#include <stdio.h>
#include "ast.h"

void yyerror(const char *str);
%}

%token VAR WHILE DO PRINT
%token ASSIGN NOT
%token MINUS PLUS MUL DIV LESS MORE EQUALS
%token LCBR RCBR LBR RBR
%token LETTER NUMBER
%token SEMICOLON COMMA

%left LESS MORE EQUALS
%left PLUS MINUS
%left MUL DIV

%type

%union {

}

%%

program:
|       vars LCBR calculations RCBR {printf("CORRECT\n");}
;

vars:   VAR vars_list SEMICOLON
;

vars_list:  ident
|           ident COMMA vars_list
;

calculations:   operator SEMICOLON
|               operator SEMICOLON calculations
;

operator:   assign
|           complex_op
|           PRINT LBR ident RBR
;

assign:     ident ASSIGN expr
;

expr:       unary_op sub_expr
|           sub_expr
;

sub_expr:   LBR expr RBR
|           operand
|           sub_expr binary_op sub_expr
;

unary_op:   MINUS
|           NOT
;

binary_op:  MINUS
|           PLUS
|           MUL
|           DIV
|           LESS
|           MORE
|           EQUALS
;

operand:    ident
|           const
;

complex_op: cycle_op
|           comb_op
;

cycle_op:   WHILE expr DO operator
;

comb_op:    LCBR calculations RCBR
;

ident:  LETTER
|       LETTER ident
;

const:  NUMBER
|       NUMBER const
;

%%

void yyerror(const char *str) {
	fprintf(stderr, "Error: %s", str);
}

int main() {
	return yyparse();
}