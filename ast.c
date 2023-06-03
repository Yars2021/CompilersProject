#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

char *alloc_string(char *str) {
    if (!str) return NULL;
    char *string = (char*) malloc(sizeof(char) * strlen(str) + 1);
    memset(string, 0, strlen(str) + 1);
    memcpy(string, str, strlen(str) + 1);
    return string;
}

variable *create_variable(char *name, int value) {
    if (!name) return NULL;
    variable *var = (variable*) malloc(sizeof(variable));
    var->name = alloc_string(name);
    var->value = value;
    return var;
}

void delete_variable(variable *var) {
    if (!var || !var->name) return;
    var->value = 0;
    free(var->name);
    free(var);
}

ast_node *create_ast_node() {
    ast_node *node = (ast_node*) malloc(sizeof(ast_node));
    node->num_of_branches = 0;
    node->branches = NULL;
    return node;
}

ast_node *create_ast_node_lit(int int_value) {
    ast_node *node = create_ast_node();
    node->node_type = NODE_TYPE_LITERAL;
    node->int_val = int_value;
    return node;
}

ast_node *create_ast_node_var(char *name) {
    if (!name) return NULL;
    ast_node *node = create_ast_node();
    node->node_type = NODE_TYPE_VARIABLE;
    node->var_name = alloc_string(name);
    return node;
}

ast_node *create_ast_node_op(int operation) {
    ast_node *node = create_ast_node();
    node->node_type = NODE_TYPE_OPERATION;
    node->operation = operation;
    return node;
}

ast_node *create_ast_node_var_def(char *name) {
    if (!name) return NULL;
    ast_node *node = create_ast_node();
    node->node_type = NODE_TYPE_VAR_DEF;
    node->var = create_variable(name, 0);
    return node;
}

ast_node *create_ast_node_root() {
    ast_node *node = create_ast_node();
    node->node_type = NODE_TYPE_OP_ROOT;
    return node;
}

ast_node *create_ast_node_program_root() {
    ast_node *node = create_ast_node();
    node->node_type = NODE_TYPE_PROGRAM_ROOT;
    return node;
}

void add_child(ast_node *node, ast_node *child) {
    node->num_of_branches++;
    ast_node **new_nodes = (ast_node**) malloc(sizeof(ast_node*) * node->num_of_branches);
    for (size_t i = 0; i < node->num_of_branches - 1; i++)
        new_nodes[i] = node->branches[i];
    new_nodes[node->num_of_branches - 1] = child;
    if (node->branches) free(node->branches);
    node->branches = new_nodes;
}

void print_ast(FILE *file, ast_node *node, size_t tabs) {
    if (!node) return;

    char *tabs_line = (char*) malloc(tabs + 1);
    memset(tabs_line, '\t', tabs);
    tabs_line[tabs] = 0;

    char *node_type;
    switch (node->node_type) {
        case NODE_TYPE_PROGRAM_ROOT:
            node_type = "program_root";
            break;
        case NODE_TYPE_OP_ROOT:
            node_type = "operation_root";
            break;
        case NODE_TYPE_OPERATION:
            node_type = "operation";
            break;
        case NODE_TYPE_LITERAL:
            node_type = "literal";
            break;
        case NODE_TYPE_VARIABLE:
            node_type = "variable";
            break;
        case NODE_TYPE_VAR_DEF:
            node_type = "variable_def";
            break;
    }

    fprintf(file, "%s{\n%s\tnode_type: %s,\n", tabs_line, tabs_line, node_type);

    switch (node->node_type) {
        case NODE_TYPE_OPERATION:
            fprintf(file, "%s\toperation: %c,\n", tabs_line, node->operation);
            break;
        case NODE_TYPE_LITERAL:
            fprintf(file, "%s\tint_value: %d,\n", tabs_line, node->int_val);
            break;
        case NODE_TYPE_VARIABLE:
            fprintf(file, "%s\tvariable_name: %s,\n", tabs_line, node->var_name);
            break;
        case NODE_TYPE_VAR_DEF:
            fprintf(file, "%s\tvariable_name: %s,\n%s\tvariable_value: %d,\n", tabs_line, node->var->name, tabs_line, node->var->value);
            break;
    }

    fprintf(file, "%s\tnum_of_branches: %zd,\n%s\tbranches:\n%s\t[\n", tabs_line, node->num_of_branches, tabs_line, tabs_line);

    for (size_t i = 0; i < node->num_of_branches; i++)
        print_ast(file, node->branches[i], tabs + 2);

    fprintf(file, "%s\t]\n%s}\n", tabs_line, tabs_line);

    free(tabs_line);
}

void delete_ast_node(ast_node *node) {
    if (!node) return;
    if (node->node_type == NODE_TYPE_VARIABLE) free(node->var_name);
    if (node->node_type == NODE_TYPE_VAR_DEF) delete_variable(node->var);
    for (size_t i = 0; i < node->num_of_branches; i++)
        delete_ast_node(node->branches[i]);
    node->num_of_branches = 0;
    free(node->branches);
    free(node);
}

int eval_literals(ast_node *node) {
    if (!node) return 0;
    if (node->node_type == NODE_TYPE_LITERAL) return node->int_val;
    if (node->node_type != NODE_TYPE_OPERATION) return 0;
    switch (node->operation) {
        case '+':
            if (node->num_of_branches != 2) return 0;
            else return eval_literals(node->branches[0]) + eval_literals(node->branches[1]);
        case '-':
            if (node->num_of_branches == 1)
                return -eval_literals(node->branches[0]);
            else
                if (node->num_of_branches == 2)
                    return eval_literals(node->branches[0]) - eval_literals(node->branches[1]);
            else return 0;
        case '*':
            if (node->num_of_branches != 2) return 0;
            else return eval_literals(node->branches[0]) * eval_literals(node->branches[1]);
        case '/':
            if (node->num_of_branches != 2) return 0;
            else return eval_literals(node->branches[0]) / eval_literals(node->branches[1]);
        case 'N':
            if (node->num_of_branches != 1) return 0;
            else return !eval_literals(node->branches[0]);
        case '>':
            if (node->num_of_branches != 2) return 0;
            else return eval_literals(node->branches[0]) > eval_literals(node->branches[1]);
        case '<':
            if (node->num_of_branches != 2) return 0;
            else return eval_literals(node->branches[0]) < eval_literals(node->branches[1]);
        case 'E':
            if (node->num_of_branches != 2) return 0;
            else return eval_literals(node->branches[0]) == eval_literals(node->branches[1]);
        case 'A':
            if (node->num_of_branches != 2) return 0;
            else return eval_literals(node->branches[1]);
    }
    return 0;
}

void try_eval(ast_node *node) {
    if (!node) return;
//    if (node->node_type == NODE_TYPE_OPERATION) {
//        for (size_t i = 0; i < node->num_of_branches; i++)
//            if (node->branches[i]->node_type != NODE_TYPE_LITERAL) return;
//        int eval_res = eval_literals(node);
//        for (size_t i = 0; i < node->num_of_branches; i++)
//            delete_ast_node(node->branches[i]);
//        node->node_type = NODE_TYPE_LITERAL;
//        node->num_of_branches = 0;
//        node->int_val = eval_res;
//    }
}