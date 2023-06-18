//
// Created by yars on 18.06.23.
//

#ifndef COMPILERSPROJECT_LINEAR_H
#define COMPILERSPROJECT_LINEAR_H

#include "ast.h"

#define VAR_NOT_FOUND (-1)

ssize_t get_var_index(ast_node *node, char *var_name);
void linearize_op(ast_node *node, ast_node *result);
void linearize(ast_node *node);

#endif //COMPILERSPROJECT_LINEAR_H