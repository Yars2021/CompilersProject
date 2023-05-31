//
// Created by yars on 31.05.23.
//

#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define FILENAME "/home/yars/labs/compilers_labs/compilers_project/ast.txt"

typedef struct {
    char *name;
    int value;
} variable;

typedef struct {
    size_t num_of_branches;
    ast_node **branches;
    int node_type;  // 0 - op, 1 - int, 2 - var
    union {
        int operation; // A - assign, N - not, E - equals
        int int_val;
        variable *var_val;
    };
} ast_node;

variable *create_variable(char *name, int value);
void delete_variable(variable *var);
ast_node *create_ast_node_int(int int_value);
ast_node *create_ast_node_var(variable *var);
ast_node *create_ast_node_op(int operation);
void add_child(ast_node *node, ast_node *child);
void print_ast(FILE *file, ast_node *node, size_t tabs);
void delete_ast_node(ast_node *node);
int eval(ast_node *node);

#endif //AST_H