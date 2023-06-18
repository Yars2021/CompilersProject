%{
#include <stdio.h>
#include "ast.h"
#include "linear.h"

extern int yylex();

void yyerror(const char *ast_path, const char *linear_path, const char *str);
%}

%parse-param {char *ast_path}
%parse-param {char *linear_path}

%token VAR WHILE DO PRINT
%token ASSIGN NOT
%token MINUS PLUS MUL DIV LESS MORE EQUALS
%token LCBR RCBR LBR RBR
%token SEMICOLON COMMA
%token IDENT NUMBER

%left LESS MORE EQUALS
%left PLUS MINUS
%left MUL DIV

%type <node> program vars vars_list calculations operator assign complex_op comb_op cycle_op expr operand
%type <var_name> IDENT
%type <number> NUMBER

%union {
    ast_node *node;
    char var_name[256];
    int number;
}

%%

program:                            {;}
|       vars LCBR calculations RCBR {
                                        FILE *ast_file = fopen(ast_path, "w");
                                        FILE *linear_file = fopen(linear_path, "w");

                                        $$ = create_ast_node_program_root();
                                        add_child($$, $1);
                                        add_child($$, $3);
                                        print_ast(ast_file, $$, 0);

                                        ast_node *linear = create_ast_node_program_root();
                                        linearize_op($$, linear);
                                        print_ast(linear_file, linear, 0);

                                        delete_ast_node($$);
                                        delete_ast_node(linear);

                                        if (linear_file) fclose(linear_file);
                                        if (ast_file) fclose(ast_file);
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

assign:     IDENT ASSIGN expr           {$$ = create_ast_node_op('A'); try_eval($3); add_child($$, $3); add_child($$, create_ast_node_var($1));}
;

expr:       operand             {$$ = $1;}
|           LBR expr RBR        {$$ = $2;}
|           MINUS expr          {$$ = create_ast_node_op('-'); add_child($$, $2); try_eval($2);}
|           NOT expr            {$$ = create_ast_node_op('N'); add_child($$, $2); try_eval($2);}
|           expr MUL expr       {$$ = create_ast_node_op('*'); add_child($$, $1); add_child($$, $3); try_eval($1); try_eval($3);}
|           expr DIV expr       {$$ = create_ast_node_op('/'); add_child($$, $1); add_child($$, $3); try_eval($1); try_eval($3);}
|           expr PLUS expr      {$$ = create_ast_node_op('+'); add_child($$, $1); add_child($$, $3); try_eval($1); try_eval($3);}
|           expr MINUS expr     {$$ = create_ast_node_op('-'); add_child($$, $1); add_child($$, $3); try_eval($1); try_eval($3);}
|           expr LESS expr      {$$ = create_ast_node_op('<'); add_child($$, $1); add_child($$, $3); try_eval($1); try_eval($3);}
|           expr MORE expr      {$$ = create_ast_node_op('>'); add_child($$, $1); add_child($$, $3); try_eval($1); try_eval($3);}
|           expr EQUALS expr    {$$ = create_ast_node_op('E'); add_child($$, $1); add_child($$, $3); try_eval($1); try_eval($3);}
;

complex_op: cycle_op                    {$$ = $1;}
|           comb_op                     {$$ = $1;}
;

cycle_op:   WHILE expr DO operator      {$$ = create_ast_node_op('C'); add_child($$, $2); add_child($$, $4);}
;

comb_op:    LCBR calculations RCBR      {$$ = $2;}
;

operand:    IDENT    {$$ = create_ast_node_var($1);}
|           NUMBER   {$$ = create_ast_node_lit($1);}
;

%%

void yyerror(const char *ast_path, const char *linear_path, const char *str) {
	fprintf(stderr, "Error: %s", str);
}

int main() {
	return yyparse(FILENAME, LINEAR);
}