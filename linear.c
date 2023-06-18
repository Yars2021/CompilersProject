//
// Created by yars on 18.06.23.
//

#include <stdio.h>
#include <stdlib.h>
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

size_t count_var_defs(ast_node *node) {
    size_t num_of_vars = 1;
    ast_node *var = node->branches[0];
    while (var->num_of_branches == 1) {
        if (var->num_of_branches == 1) var = var->branches[0];
        num_of_vars++;
    }
    return num_of_vars;
}

void linearize_op(FILE *linear, ast_node *node, ast_node *prg_root, size_t *buf_num) {
    ssize_t var_index;

    for (size_t i = 0; i < node->num_of_branches; i++)
        linearize_op(linear, node->branches[i], prg_root, buf_num);

    if (node->node_type == NODE_TYPE_OPERATION) {
        if (node->num_of_branches == 1) {
            switch (node->operation) {
                case '-': {
                    switch (node->branches[0]->node_type) {
                        case NODE_TYPE_VARIABLE:
                            var_index = get_var_index(prg_root, node->branches[0]->var_name);
                            if (var_index == VAR_NOT_FOUND) {
                                printf("Undefined variable\n");
                                return;
                            }
                            fprintf(linear,"lw x1, x0, %ld\nsub x1, x0, x1\n", var_index);
                            break;
                        case NODE_TYPE_OPERATION: {
                            var_index = get_var_index(prg_root, node->branches[0]->var_name);
                            if (var_index == VAR_NOT_FOUND) {
                                printf("Undefined variable\n");
                                return;
                            }
                            fprintf(linear, "lw x1, x0, %ld\nsub x1, x0, x1\n", var_index);
                            break;
                        }
                    }
                    break;
                }
                case 'N': {
                    switch (node->branches[0]->node_type) {
                        case NODE_TYPE_VARIABLE:
                            var_index = get_var_index(prg_root, node->branches[0]->var_name);
                            if (var_index == VAR_NOT_FOUND) {
                                printf("Undefined variable\n");
                                return;
                            }
                            fprintf(linear, "lw x1, x0, %ld\nseq x1, x0, x1\n", var_index);
                            break;
                        case NODE_TYPE_OPERATION: {
                            var_index = get_var_index(prg_root, node->branches[0]->var_name);
                            if (var_index == VAR_NOT_FOUND) {
                                printf("Undefined variable\n");
                                return;
                            }
                            fprintf(linear, "lw x1, x0, %ld\nseq x1, x0, x1\n", var_index);
                            break;
                        }
                    }
                    break;
                }
            }
        } else {
            if (node->num_of_branches == 2) {
                switch (node->operation) {
                    case '+': {
                        switch (node->branches[0]->node_type) {
                            case NODE_TYPE_VARIABLE:
                                var_index = get_var_index(prg_root, node->branches[0]->var_name);
                                if (var_index == VAR_NOT_FOUND) {
                                    printf("Undefined variable\n");
                                    return;
                                }
                                fprintf(linear, "lw x1, x0, %ld\n", var_index);
                                break;
                            case NODE_TYPE_LITERAL:
                                fprintf(linear, "li x1, %d\n", node->branches[0]->int_val);
                                break;
                            case NODE_TYPE_OPERATION:
                                fprintf(linear, "lw x1, x0, %ld\n", (*buf_num) - 1);
                                break;
                        }
                        switch (node->branches[1]->node_type) {
                            case NODE_TYPE_VARIABLE:
                                var_index = get_var_index(prg_root, node->branches[1]->var_name);
                                if (var_index == VAR_NOT_FOUND) {
                                    printf("Undefined variable\n");
                                    return;
                                }
                                fprintf(linear, "lw x2, x0, %ld\n", var_index);
                                break;
                            case NODE_TYPE_LITERAL:
                                fprintf(linear, "li x2, %d\n", node->branches[1]->int_val);
                                break;
                            case NODE_TYPE_OPERATION:
                                fprintf(linear, "lw x2, x0, %ld\n", (*buf_num) - 1);
                                break;
                        }
                        fprintf(linear, "add x1, x1, x2\n");
                        break;
                    }
                    case '-': {
                        switch (node->branches[0]->node_type) {
                            case NODE_TYPE_VARIABLE:
                                var_index = get_var_index(prg_root, node->branches[0]->var_name);
                                if (var_index == VAR_NOT_FOUND) {
                                    printf("Undefined variable\n");
                                    return;
                                }
                                fprintf(linear, "lw x1, x0, %ld\n", var_index);
                                break;
                            case NODE_TYPE_LITERAL:
                                fprintf(linear, "li x1, %d\n", node->branches[0]->int_val);
                                break;
                            case NODE_TYPE_OPERATION:
                                fprintf(linear, "lw x1, x0, %ld\n", (*buf_num) - 1);
                                break;
                        }
                        switch (node->branches[1]->node_type) {
                            case NODE_TYPE_VARIABLE:
                                var_index = get_var_index(prg_root, node->branches[1]->var_name);
                                if (var_index == VAR_NOT_FOUND) {
                                    printf("Undefined variable\n");
                                    return;
                                }
                                fprintf(linear, "lw x2, x0, %ld\n", var_index);
                                break;
                            case NODE_TYPE_LITERAL:
                                fprintf(linear, "li x2, %d\n", node->branches[1]->int_val);
                                break;
                            case NODE_TYPE_OPERATION:
                                fprintf(linear, "lw x2, x0, %ld\n", (*buf_num) - 1);
                                break;
                        }
                        fprintf(linear, "sub x1, x1, x2\n");
                        break;
                    }
                    case '*': {
                        switch (node->branches[0]->node_type) {
                            case NODE_TYPE_VARIABLE:
                                var_index = get_var_index(prg_root, node->branches[0]->var_name);
                                if (var_index == VAR_NOT_FOUND) {
                                    printf("Undefined variable\n");
                                    return;
                                }
                                fprintf(linear, "lw x1, x0, %ld\n", var_index);
                                break;
                            case NODE_TYPE_LITERAL:
                                fprintf(linear, "li x1, %d\n", node->branches[0]->int_val);
                                break;
                            case NODE_TYPE_OPERATION:
                                fprintf(linear, "lw x1, x0, %ld\n", (*buf_num) - 1);
                                break;
                        }
                        switch (node->branches[1]->node_type) {
                            case NODE_TYPE_VARIABLE:
                                var_index = get_var_index(prg_root, node->branches[1]->var_name);
                                if (var_index == VAR_NOT_FOUND) {
                                    printf("Undefined variable\n");
                                    return;
                                }
                                fprintf(linear, "lw x2, x0, %ld\n", var_index);
                                break;
                            case NODE_TYPE_LITERAL:
                                fprintf(linear, "li x2, %d\n", node->branches[1]->int_val);
                                break;
                            case NODE_TYPE_OPERATION:
                                fprintf(linear, "lw x2, x0, %ld\n", (*buf_num) - 1);
                                break;
                        }
                        fprintf(linear, "mul x1, x1, x2\n");
                        break;
                    }
                    case '/': {
                        switch (node->branches[0]->node_type) {
                            case NODE_TYPE_VARIABLE:
                                var_index = get_var_index(prg_root, node->branches[0]->var_name);
                                if (var_index == VAR_NOT_FOUND) {
                                    printf("Undefined variable\n");
                                    return;
                                }
                                fprintf(linear, "lw x1, x0, %ld\n", var_index);
                                break;
                            case NODE_TYPE_LITERAL:
                                fprintf(linear, "li x1, %d\n", node->branches[0]->int_val);
                                break;
                            case NODE_TYPE_OPERATION:
                                fprintf(linear, "lw x1, x0, %ld\n", (*buf_num) - 1);
                                break;
                        }
                        switch (node->branches[1]->node_type) {
                            case NODE_TYPE_VARIABLE:
                                var_index = get_var_index(prg_root, node->branches[1]->var_name);
                                if (var_index == VAR_NOT_FOUND) {
                                    printf("Undefined variable\n");
                                    return;
                                }
                                fprintf(linear, "lw x2, x0, %ld\n", var_index);
                                break;
                            case NODE_TYPE_LITERAL:
                                fprintf(linear, "li x2, %d\n", node->branches[1]->int_val);
                                break;
                            case NODE_TYPE_OPERATION:
                                fprintf(linear, "lw x2, x0, %ld\n", (*buf_num) - 1);
                                break;
                        }
                        fprintf(linear, "div x1, x1, x2\n");
                        break;
                    }
                    case '>': {
                        switch (node->branches[0]->node_type) {
                            case NODE_TYPE_VARIABLE:
                                var_index = get_var_index(prg_root, node->branches[0]->var_name);
                                if (var_index == VAR_NOT_FOUND) {
                                    printf("Undefined variable\n");
                                    return;
                                }
                                fprintf(linear, "lw x1, x0, %ld\n", var_index);
                                break;
                            case NODE_TYPE_LITERAL:
                                fprintf(linear, "li x1, %d\n", node->branches[0]->int_val);
                                break;
                            case NODE_TYPE_OPERATION:
                                fprintf(linear, "lw x1, x0, %ld\n", (*buf_num) - 1);
                                break;
                        }
                        switch (node->branches[1]->node_type) {
                            case NODE_TYPE_VARIABLE:
                                var_index = get_var_index(prg_root, node->branches[1]->var_name);
                                if (var_index == VAR_NOT_FOUND) {
                                    printf("Undefined variable\n");
                                    return;
                                }
                                fprintf(linear, "lw x2, x0, %ld\n", var_index);
                                break;
                            case NODE_TYPE_LITERAL:
                                fprintf(linear, "li x2, %d\n", node->branches[1]->int_val);
                                break;
                            case NODE_TYPE_OPERATION:
                                fprintf(linear, "lw x2, x0, %ld\n", (*buf_num) - 1);
                                break;
                        }
                        fprintf(linear, "sge x3, x1, x2\nseq x4, x1, x2\nslt x1, x4, x3\n");
                        break;
                    }
                    case '<': {
                        switch (node->branches[0]->node_type) {
                            case NODE_TYPE_VARIABLE:
                                var_index = get_var_index(prg_root, node->branches[0]->var_name);
                                if (var_index == VAR_NOT_FOUND) {
                                    printf("Undefined variable\n");
                                    return;
                                }
                                fprintf(linear, "lw x1, x0, %ld\n", var_index);
                                break;
                            case NODE_TYPE_LITERAL:
                                fprintf(linear, "li x1, %d\n", node->branches[0]->int_val);
                                break;
                            case NODE_TYPE_OPERATION:
                                fprintf(linear, "lw x1, x0, %ld\n", (*buf_num) - 1);
                                break;
                        }
                        switch (node->branches[1]->node_type) {
                            case NODE_TYPE_VARIABLE:
                                var_index = get_var_index(prg_root, node->branches[1]->var_name);
                                if (var_index == VAR_NOT_FOUND) {
                                    printf("Undefined variable\n");
                                    return;
                                }
                                fprintf(linear, "lw x2, x0, %ld\n", var_index);
                                break;
                            case NODE_TYPE_LITERAL:
                                fprintf(linear, "li x2, %d\n", node->branches[1]->int_val);
                                break;
                            case NODE_TYPE_OPERATION:
                                fprintf(linear, "lw x2, x0, %ld\n", (*buf_num) - 1);
                                break;
                        }
                        fprintf(linear, "slt x1, x1, x2\n");
                        break;
                    }
                    case 'E': {
                        switch (node->branches[0]->node_type) {
                            case NODE_TYPE_VARIABLE:
                                var_index = get_var_index(prg_root, node->branches[0]->var_name);
                                if (var_index == VAR_NOT_FOUND) {
                                    printf("Undefined variable\n");
                                    return;
                                }
                                fprintf(linear, "lw x1, x0, %ld\n", var_index);
                                break;
                            case NODE_TYPE_LITERAL:
                                fprintf(linear, "li x1, %d\n", node->branches[0]->int_val);
                                break;
                            case NODE_TYPE_OPERATION:
                                fprintf(linear, "lw x1, x0, %ld\n", (*buf_num) - 1);
                                break;
                        }
                        switch (node->branches[1]->node_type) {
                            case NODE_TYPE_VARIABLE:
                                var_index = get_var_index(prg_root, node->branches[1]->var_name);
                                if (var_index == VAR_NOT_FOUND) {
                                    printf("Undefined variable\n");
                                    return;
                                }
                                fprintf(linear, "lw x2, x0, %ld\n", var_index);
                                break;
                            case NODE_TYPE_LITERAL:
                                fprintf(linear, "li x2, %d\n", node->branches[1]->int_val);
                                break;
                            case NODE_TYPE_OPERATION:
                                fprintf(linear, "lw x2, x0, %ld\n", (*buf_num) - 1);
                                break;
                        }
                        fprintf(linear, "seq x1, x1, x2\n");
                        break;
                    }
                }
            }
        }
        fprintf(linear, "sw x0, %ld, x1\naddi x1, x0, 0\naddi x2, x0, 0\naddi x3, x0, 0\naddi x4, x0, 0\n", *buf_num);
        (*buf_num)++;
    }
}

void linearize_cycle(FILE *linear, ast_node *node, ast_node *prg_root, size_t *buf_num, size_t *label_num, size_t *print_num) {
    if (!node || node->node_type != NODE_TYPE_OPERATION || node->operation != 'C') return;
    fprintf(linear,"CYCLE_START%zd:\n", *label_num);
    linearize_op(linear, node->branches[0], prg_root, buf_num);
    fprintf(linear, "lw x1, x0, %zd\nbne x1, x0, 1\njal x0, CYCLE_EXIT%zd\n", (*buf_num) - 1, *label_num);
    linearize_op_root(linear, node->branches[1], prg_root, buf_num, label_num, print_num);
    fprintf(linear, "jal x0, CYCLE_START%zd\nCYCLE_EXIT%zd:\n", *label_num, *label_num);
    (*label_num)++;
}

void linearize_op_root(FILE *linear, ast_node *node, ast_node *prg_root, size_t *buf_num, size_t *label_num, size_t *print_num) {
    ssize_t var_index, var_index0, var_index1;

    for (size_t i = 0; i < node->num_of_branches; i++) {
        switch (node->branches[i]->node_type) {
            case NODE_TYPE_OP_ROOT:
                linearize_op_root(linear, node->branches[i], prg_root, buf_num, label_num, print_num);
                break;
            case NODE_TYPE_OPERATION: {
                switch (node->branches[i]->operation) {
                    case 'C':
                        linearize_cycle(linear, node->branches[i], prg_root, buf_num, label_num, print_num);
                        break;
                    case 'A': {
                        switch (node->branches[i]->branches[0]->node_type) {
                            case NODE_TYPE_LITERAL:
                                var_index = get_var_index(prg_root, node->branches[i]->branches[1]->var_name);
                                fprintf(linear, "li x1, %d\nsw x0, %ld, x1\n", node->branches[i]->branches[0]->int_val, var_index);
                                break;
                            case NODE_TYPE_VARIABLE:
                                var_index0 = get_var_index(prg_root, node->branches[i]->branches[0]->var_name);
                                var_index1 = get_var_index(prg_root, node->branches[i]->branches[1]->var_name);
                                if (var_index0 == VAR_NOT_FOUND || var_index1 == VAR_NOT_FOUND) {
                                    printf("Undefined variable\n");
                                    return;
                                }
                                fprintf(linear, "lw x1, x0, %ld\nsw x0, %ld, x1\n", var_index0, var_index1);
                                break;
                            case NODE_TYPE_OPERATION:
                                linearize_op(linear, node->branches[i]->branches[0], prg_root, buf_num);
                                var_index = get_var_index(prg_root, node->branches[i]->branches[1]->var_name);
                                if (var_index == VAR_NOT_FOUND) {
                                    printf("Undefined variable\n");
                                    return;
                                }
                                fprintf(linear, "lw x1, x0, %ld\nsw x0, %ld, x1\n", (*buf_num) - 1, var_index);
                                break;
                        }
                        break;
                    }
                    case 'O':
                        var_index = get_var_index(prg_root, node->branches[i]->branches[0]->var_name);
                        if (var_index == VAR_NOT_FOUND) {
                            printf("Undefined variable\n");
                            return;
                        }
                        fprintf(linear, PRINT_FORMAT, var_index, *print_num, *print_num);
                        (*print_num)++;
                        break;
                }
                break;
            }
        }
    }
}

typedef struct code_line {
    char *data;
    struct code_line *next;
} code_line;

void linearize(const char *path, ast_node *node) {
    if (!node || node->node_type != NODE_TYPE_PROGRAM_ROOT || node->num_of_branches != 2) return;

    FILE *file = fopen(path, "w");

    size_t var_num = count_var_defs(node), buf_num = var_num, label_num = 0, print_num = 0;
    linearize_op_root(file, node->branches[1], node, &buf_num, &label_num, &print_num);
    fprintf(file, "addi x0, x0, 0\n");

    if (file) fclose(file);

    file = fopen(path, "r");

    code_line *start = (code_line*) malloc(sizeof(code_line)), *line;
    char *str;
    size_t len = 0, num_of_lines = 0;

    line = start;
    while (getline(&str, &len, file) != -1) {
        line->data = alloc_string(str);
        line->next = (code_line*) malloc(sizeof(code_line));
        line = line->next;
        num_of_lines++;
    }

    if (file) fclose(file);

    file = fopen(path, "w");

    for (size_t i = 0; i < buf_num; i++) fprintf(file, "addi x0, x0, 0\n");
    for (size_t i = 0; i < buf_num; i++) fprintf(file, "sw x0, %zd, x0\n", i);

    for (size_t i = 0; i < num_of_lines; i++) {
        if (start) fprintf(file, "%s", start->data);
        line = start;
        start = start->next;
        if (line) {
            if (line->data) free(line->data);
            free(line);
        }
    }

    if (file) fclose(file);
}