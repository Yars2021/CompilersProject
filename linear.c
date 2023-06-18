//
// Created by yars on 18.06.23.
//

#include <string.h>
#include "linear.h"

ssize_t get_var_index(ast_node *node, char *var_name) {
    if (!node || !var_name || node->node_type != NODE_TYPE_PROGRAM_ROOT || node->num_of_branches != 2) return VAR_NOT_FOUND;
    ast_node *var = node->branches[0];
    for (ssize_t index = 0;; index++)
        if (strcmp(var_name, var->var->name) == 0) return index;
        else
            if (var->num_of_branches == 0) return VAR_NOT_FOUND;
            else var = var->branches[0];
}

void linearize_op(ast_node *node, ast_node *result) {
    for (size_t i = 0; i < node->num_of_branches; i++)
        linearize_op(node->branches[i], result);

    switch (node->node_type) {
        case NODE_TYPE_VAR_DEF:
            add_child(result, create_ast_node_var_def(node->var->name));
            break;
        case NODE_TYPE_OPERATION:
            add_child(result, create_ast_node_op(node->operation));
            break;
        case NODE_TYPE_VARIABLE:
            add_child(result, create_ast_node_var(node->var_name));
            break;
        case NODE_TYPE_LITERAL:
            add_child(result, create_ast_node_lit(node->int_val));
            break;
    }
}



void linearize(ast_node *node) {
    if (!node || node->node_type != NODE_TYPE_PROGRAM_ROOT || node->num_of_branches != 2) return;
    linearize_op(node->branches[1], NULL);
}