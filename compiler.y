%{
#include <stdio.h>
#include "ast.h"

extern int yylex();

void yyerror(const char *path, const char *str);
%}

%parse-param {char *path}

%token VAR WHILE DO PRINT
%token ASSIGN NOT
%token MINUS PLUS MUL DIV LESS MORE EQUALS
%token LCBR RCBR LBR RBR
%token SEMICOLON COMMA
%token IDENT NUMBER

%left LESS MORE EQUALS
%left PLUS MINUS
%left MUL DIV

%type <node> program vars vars_list calculations operator assign complex_op comb_op cycle_op expr sub_expr operand
%type <operation> binary_op unary_op
%type <var_name> IDENT
%type <number> NUMBER

%union {
    ast_node *node;
    int operation;
    char var_name[256];
    int number;
}

%%

program:                            {;}
|       vars LCBR calculations RCBR {
                                        FILE *file = fopen(path, "w");

                                        $$ = create_ast_node_program_root();
                                        add_child($$, $1);
                                        add_child($$, $3);
                                        print_ast(file, $$, 0);
                                        delete_ast_node($$);

                                        if (file) fclose(file);
                                    }
;

vars:   VAR vars_list SEMICOLON         {$$ = $2;}
;

vars_list:  IDENT                       {$$ = create_ast_node_var_def($1);}
|           IDENT COMMA vars_list       {$$ = create_ast_node_var_def($1); add_child($$, $3);}
;

calculations:                                       {$$ = create_ast_node_root();}
|               operator SEMICOLON                  {$$ = create_ast_node_root(); add_child($$, $1);}
|               calculations operator SEMICOLON     {$$ = $1; add_child($$, $2);}
;

operator:   assign                      {$$ = $1;}
|           complex_op                  {$$ = $1;}
|           PRINT LBR IDENT RBR         {$$ = create_ast_node_op('O'); add_child($$, create_ast_node_var($3));}
;

assign:     IDENT ASSIGN expr           {$$ = create_ast_node_op('A'); add_child($$, create_ast_node_var($1)); add_child($$, $3);}
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

operand:    IDENT    {$$ = create_ast_node_var($1);}
|           NUMBER   {$$ = create_ast_node_lit($1);}
;

%%

void yyerror(const char *path, const char *str) {
	fprintf(stderr, "Error: %s", str);
}

int main() {
	return yyparse(FILENAME);
}