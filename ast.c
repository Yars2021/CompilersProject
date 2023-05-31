#include "ast.h"

char *alloc_string(char *str) {
    if (!string) return NULL;
    char *string = (char*) malloc(sizeof(char) * strlen(str) + 1);
    memset(string, 0, strlen(str) + 1);
    memcpy(string, str, strlen(str) + 1);
    return string;
}

variable *create_variable(char *name, int value) {
    if (!name) return NULL;
    variable *var = (variable*) malloc(sieof(variable));
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

ast_node *create_ast_node_int(int int_value) {
    ast_node *node = create_ast_node();
    node->node_type = 1;
    node->int_val = int_value;
    return node;
}

ast_node *create_ast_node_var(variable *var) {
    if (!var) return NULL;
    ast_node *node = create_ast_node();
    node->node_type = 2;
    node->var_val = var;
}

ast_node *create_ast_node_op(int operation) {
    ast_node *node = create_ast_node();
    node->node_type = 0;
    node->operation = operation;
    return node;
}

void add_child(ast_node *node, ast_node *child) {
    if (!node || !child) return;
    node->num_of_branches++;
    ast_node **new_nodes = (ast_node**) malloc(sizeof(ast_node*) * node->num_of_branches);
    for (size_t i = 0; i < node->num_of_branches - 1; i++)
        new_nodes[i] = node->branches[i];
    new_nodes[node->num_of_branches - 1] = child;
    if (node->branches) free(node->branches);
    node->branches = new_nodes;
}

void print_ast(ast_node *node) {

}

void delete_ast_node(ast_node *node) {
    if (!node) return;
    if (node->node_type == 2) delete_variable(node->var_val);
    for (size_t i = 0; i < node->num_of_branches; i++)
        delete_ast_node(node->branches[i]);
}