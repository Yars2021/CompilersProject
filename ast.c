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
    return node;
}

ast_node *create_ast_node_op(int operation) {
    ast_node *node = create_ast_node();
    node->node_type = 0;
    node->operation = operation;
    return node;
}

ast_node *create_ast_node_root() {
    ast_node *node = create_ast_node();
    node->node_type = 4;
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

void print_ast(ast_node *node, size_t tabs) {
    if (!node) return;



    char *node_type;
    switch (node->node_type) {
        case 0:
            node_type = "operation";
            break;
        case 1:
            node_type = "literal";
            break;
        case 2:
            node_type = "variable";
            break;
        case 4:
            node_type = "operation_root";
            break;
    }

    FILE *file = fopen(FILENAME, "r+");

    fprintf(file, "{\n\tnode_type: %s\n", node_type);

    switch (node->node_type) {
        case 0:
            fprintf(file, "\toperation: %c\n", node->operation);
            break;
        case 1:
            fprintf(file, "\tint_value: %d\n", node->int_val);
            break;
        case 2:
            fprintf(file, "\tvariable:\n\t{\n\t\tvar_name: %s\n\t\tvar_value: %d\n\t}\n", node->var_val->name, node->var_val->value);
            break;
    }

    fprintf(file, "\tnum_of_branches: %zd\n\tbranches:\n\t{\n", node->num_of_branches);

    for (size_t i = 0; i < node->num_of_branches; i++)
        print_ast(node->branches[i], tabs + 1);

    fprintf(file, "\t}\n}\n");

    fclose(file);
}

void delete_ast_node(ast_node *node) {
    if (!node) return;
    if (node->node_type == 2) delete_variable(node->var_val);
    for (size_t i = 0; i < node->num_of_branches; i++)
        delete_ast_node(node->branches[i]);
}

int eval(ast_node *node) {
    if (!node) return 0;
    switch (node->node_type) {
        case 0:
            switch (node->operation) {
                case '+':
                    if (node->num_of_branches != 2) return 0;
                    else return eval(node->branches[0]) + eval(node->branches[1]);
                case '-':
                    if (node->num_of_branches == 1)
                        return -eval(node->branches[0]);
                    else
                        if (node->num_of_branches == 2)
                            return eval(node->branches[0]) - eval(node->branches[1]);
                    else return 0;
                case '*':
                    if (node->num_of_branches != 2) return 0;
                    else return eval(node->branches[0]) * eval(node->branches[1]);
                case '/':
                    if (node->num_of_branches != 2) return 0;
                    else return eval(node->branches[0]) / eval(node->branches[1]);
                case 'N':
                    if (node->num_of_branches != 1) return 0;
                    else return !eval(node->branches[0]);
                case '>':
                    if (node->num_of_branches != 2) return 0;
                    else return eval(node->branches[0]) > eval(node->branches[1]);
                case '<':
                    if (node->num_of_branches != 2) return 0;
                    else return eval(node->branches[0]) < eval(node->branches[1]);
                case 'E':
                    if (node->num_of_branches != 2) return 0;
                    else return eval(node->branches[0]) == eval(node->branches[1]);
                case 'A':
                    if (node->num_of_branches != 2) return 0;
                    else return eval(node->branches[1]);
            }
            break;
        case 1:
            return node->int_val;
        case 2:
            return node->var_val->value;
    }
    return 0;
}