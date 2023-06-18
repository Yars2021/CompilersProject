//
// Created by yars on 18.06.23.
//

#ifndef COMPILERSPROJECT_LINEAR_H
#define COMPILERSPROJECT_LINEAR_H

#include "ast.h"

#define VAR_NOT_FOUND (-1)

ssize_t get_var_index(ast_node *node, char *var_name);
void linearize_op(ast_node *node, size_t *buf_num);
void linearize_cycle(ast_node *node, size_t *buf_num, size_t *label_num);
void linearize_var_defs(ast_node *node);
void linearize_op_root(ast_node *node, size_t *buf_num, size_t *label_num);
void linearize(ast_node *node);

#endif //COMPILERSPROJECT_LINEAR_H