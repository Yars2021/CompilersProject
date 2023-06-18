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

void linearize_op(ast_node *node, size_t *buf_num) {
    for (size_t i = 0; i < node->num_of_branches; i++)
        linearize_op(node->branches[i], buf_num);

    if (node->node_type == NODE_TYPE_OPERATION) {
        if (node->num_of_branches == 1) {
            printf("BUF%zd = %c ", *buf_num, node->operation);
            switch (node->branches[0]->node_type) {
                case NODE_TYPE_VARIABLE:
                    printf("%s\n", node->branches[0]->var_name);
                    break;
                case NODE_TYPE_OPERATION:
                    break;
            }
        } else {
            if (node->num_of_branches == 2) {
                printf("BUF%zd = ", *buf_num);
                switch (node->branches[0]->node_type) {
                    case NODE_TYPE_VARIABLE:
                        printf("%s ", node->branches[0]->var_name);
                        break;
                    case NODE_TYPE_LITERAL:
                        printf("%d ", node->branches[0]->int_val);
                        break;
                    case NODE_TYPE_OPERATION:
                        printf("BUF%zd ", (*buf_num) - 1);
                        break;
                }

                printf("%c ", node->operation);

                switch (node->branches[1]->node_type) {
                    case NODE_TYPE_VARIABLE:
                        printf("%s\n", node->branches[1]->var_name);
                        break;
                    case NODE_TYPE_LITERAL:
                        printf("%d\n", node->branches[1]->int_val);
                        break;
                    case NODE_TYPE_OPERATION:
                        printf("BUF%zd\n", (*buf_num) - 1);
                        break;
                }
            }
        }
        (*buf_num)++;
    }
}

void linearize_cycle(ast_node *node, size_t *buf_num, size_t *label_num) {
    if (!node || node->node_type != NODE_TYPE_OPERATION || node->operation != 'C') return;
    printf("CYCLE_START%zd:\n", *label_num);
    linearize_op(node->branches[0], buf_num);
    printf("CMP BUF%zd 0\nJE CYCLE_EXIT%zd\n", (*buf_num) - 1, *label_num);
    linearize_op_root(node->branches[1], buf_num, label_num);
    printf("JMP CYCLE_START%zd\nCYCLE_EXIT%zd:\n", *label_num, *label_num);
    (*label_num)++;
}

void linearize_var_defs(ast_node *node) {
    ast_node *var = node->branches[0];
    printf("VAR_DEF %s\n", var->var->name);
    while (var->num_of_branches == 1) {
        if (var->num_of_branches == 1) var = var->branches[0];
        printf("VAR_DEF %s\n", var->var->name);
    }
}

void linearize_op_root(ast_node *node, size_t *buf_num, size_t *label_num) {
    for (size_t i = 0; i < node->num_of_branches; i++) {
        switch (node->branches[i]->node_type) {
            case NODE_TYPE_OP_ROOT:
                linearize_op_root(node->branches[i], buf_num, label_num);
                break;
            case NODE_TYPE_OPERATION: {
                switch (node->branches[i]->operation) {
                    case 'C':
                        linearize_cycle(node->branches[i], buf_num, label_num);
                        break;
                    case 'A': {
                        switch (node->branches[i]->branches[0]->node_type) {
                            case NODE_TYPE_LITERAL:
                                printf("%s = %d\n", node->branches[i]->branches[1]->var_name, node->branches[i]->branches[0]->int_val);
                                break;
                            case NODE_TYPE_VARIABLE:
                                printf("%s = %s\n", node->branches[i]->branches[1]->var_name, node->branches[i]->branches[0]->var_name);
                                break;
                            case NODE_TYPE_OPERATION:
                                linearize_op(node->branches[i]->branches[0], buf_num);
                                printf("%s = BUF%zd\n", node->branches[i]->branches[1]->var_name, (*buf_num) - 1);
                                break;
                        }
                        break;
                    }
                    case 'O':
                        printf("print(%s)\n", node->branches[i]->branches[0]->var_name);
                        break;
                }
                break;
            }
        }
    }
}

void linearize(ast_node *node) {
    if (!node || node->node_type != NODE_TYPE_PROGRAM_ROOT || node->num_of_branches != 2) return;

    size_t buf_num = 0, label_num = 0;

    linearize_var_defs(node);
    linearize_op_root(node->branches[1], &buf_num, &label_num);

    printf("FINISH\n");
}