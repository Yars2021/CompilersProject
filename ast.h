//
// Created by yars on 31.05.23.
//

#ifndef AST_H
#define AST_H

#include <stdio.h>

#define DEFAULT_AST_PATH "/home/yars/work/labs/compilers/CompilersProject/ast.txt"

#define NODE_TYPE_PROGRAM_ROOT (-1)
#define NODE_TYPE_OP_ROOT 0
#define NODE_TYPE_OPERATION 1
#define NODE_TYPE_LITERAL 2
#define NODE_TYPE_VARIABLE 3
#define NODE_TYPE_VAR_DEF 4

typedef struct variable {
    char *name;
    int value;
} variable;

typedef struct ast_node {
    size_t num_of_branches;
    struct ast_node **branches;
    int node_type;
    union {
        int operation; // A - assign, N - not, E - equals, O - output
        int int_val;
        char *var_name;
        variable *var;
    };
} ast_node;

char *alloc_string(char *str);
variable *create_variable(char *name, int value);
void delete_variable(variable *var);
ast_node *create_ast_node_lit(int int_value);
ast_node *create_ast_node_var(char *name);
ast_node *create_ast_node_op(int operation);
ast_node *create_ast_node_var_def(char *name);
ast_node *create_ast_node_root();
ast_node *create_ast_node_program_root();
void add_child(ast_node *node, ast_node *child);
void print_ast(FILE *file, ast_node *node, size_t tabs);
void delete_ast_node(ast_node *node);
int eval_literals(ast_node *node);
void try_eval(ast_node *node);

#endif //AST_H