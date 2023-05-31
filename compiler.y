%{
#include <stdio.h>
#include "ast.h"

void yyerror(const char *str);
%}

%token VAR WHILE DO PRINT
%token ASSIGN NOT
%token MINUS PLUS MUL DIV LESS MORE EQUALS
%token LCBR RCBR LBR RBR
%token IDENT NUMBER
%token SEMICOLON COMMA

%left LESS MORE EQUALS
%left PLUS MINUS
%left MUL DIV

%type <node> calculations operator assign complex_op comb_op cycle_op expr sub_expr operand
%type <operation> binary_op unary_op
%type <name> IDENT
%type <number> NUMBER

%union {
    struct ast_node *node;
    int operation;
    char *name;
    int number;
}

%%

program:
|       vars LCBR calculations RCBR {print_ast($3, 0); delete_ast_node($3);}
;

vars:   VAR vars_list SEMICOLON
;

vars_list:  IDENT
|           IDENT COMMA vars_list
;

calculations:   operator SEMICOLON                  {$$ = create_ast_node_root(); add_child($$, $1);}
|               calculations operator SEMICOLON     {$$ = $1; add_child($$, $2);}
;

operator:   assign                      {$$ = $1; $$->branches[0]->var_val->value = eval($$->branches[1]);}
|           complex_op                  {$$ = $1;}
|           PRINT LBR IDENT RBR         {printf("%s = %d\n", $3, 0);}
;

assign:     IDENT ASSIGN expr           {$$ = create_ast_node_op('A'); add_child($$, create_ast_node_var(create_variable($1, 0))); add_child($$, $3);}
;

expr:       unary_op sub_expr           {$$ = create_ast_node_op($1); add_child($$, $2);}
|           sub_expr                    {$$ = $1;}
;

sub_expr:   LBR expr RBR                {$$ = $2;}
|           operand                     {$$ = $1;}
|           sub_expr binary_op sub_expr {$$ = create_ast_node_op($2); add_child($$, $1); add_child($$, $3);}
;

complex_op: cycle_op                    {$$ = $1;}
|           comb_op                     {$$ = $1;}
;

cycle_op:   WHILE expr DO operator      {$$ = create_ast_node_op('C'); add_child($$, $2); add_child($$, $4);}
;

comb_op:    LCBR calculations RCBR      {$$ = $2;}
;

unary_op:   MINUS   {$$ = '-';}
|           NOT     {$$ = 'N';}
;

binary_op:  MINUS   {$$ = '-';}
|           PLUS    {$$ = '+';}
|           MUL     {$$ = '*';}
|           DIV     {$$ = '/';}
|           LESS    {$$ = '<';}
|           MORE    {$$ = '>';}
|           EQUALS  {$$ = 'E';}
;

operand:    IDENT    {$$ = create_ast_node_var(create_variable($1, 0));}
|           NUMBER   {$$ = create_ast_node_int($1);}
;

%%

void yyerror(const char *str) {
	fprintf(stderr, "Error: %s", str);
}

int main() {
    FILE *file = fopen(FILENAME, "w");
    if (file) fclose(file);
	return yyparse();
}