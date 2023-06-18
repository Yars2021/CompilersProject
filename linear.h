//
// Created by yars on 18.06.23.
//

#ifndef COMPILERSPROJECT_LINEAR_H
#define COMPILERSPROJECT_LINEAR_H

#include "ast.h"

#define DEFAULT_LINEAR_PATH "/home/yars/work/labs/compilers/CompilersProject/lin.txt"
#define VAR_NOT_FOUND (-1)

#define PRINT_FORMAT \
"li x1, 0\n\
lw x1, x0, %ld\n\
bge x1, x0, PCC%zd\n\
addi x2, x0, -1\n\
mul x1, x1, x2\n\
addi x2, x0, 45\n\
ewrite x2\n\
PCC%zd:\n\
li x4, 10\n\
li x3, 1000000000\n\
div x2, x1, x3\n\
addi x2, x2, 48\n\
ewrite x2\n\
li x3, 100000000\n\
div x2, x1, x3\n\
rem x2, x2, x4\n\
addi x2, x2, 48\n\
ewrite x2\n\
li x3, 10000000\n\
div x2, x1, x3\n\
rem x2, x2, x4\n\
addi x2, x2, 48\n\
ewrite x2\n\
li x3, 1000000\n\
div x2, x1, x3\n\
rem x2, x2, x4\n\
addi x2, x2, 48\n\
ewrite x2\n\
li x3, 100000\n\
div x2, x1, x3\n\
rem x2, x2, x4\n\
addi x2, x2, 48\n\
ewrite x2\n\
li x3, 10000\n\
div x2, x1, x3\n\
rem x2, x2, x4\n\
addi x2, x2, 48\n\
ewrite x2\n\
li x3, 1000\n\
div x2, x1, x3\n\
rem x2, x2, x4\n\
addi x2, x2, 48\n\
ewrite x2\n\
li x3, 100\n\
div x2, x1, x3\n\
rem x2, x2, x4\n\
addi x2, x2, 48\n\
ewrite x2\n\
div x2, x1, x4\n\
rem x2, x2, x4\n\
addi x2, x2, 48\n\
ewrite x2\n\
rem x2, x1, x4\n\
addi x2, x2, 48\n\
ewrite x2\n\
addi x2, x0, 10\n\
ewrite x2\n\
addi x1, x0, 0\n\
addi x2, x0, 0\n\
addi x3, x0, 0\n\
addi x4, x0, 0\n"

ssize_t get_var_index(ast_node *node, char *var_name);
size_t count_var_defs(ast_node *node);
void linearize_op(FILE *linear, ast_node *node, ast_node *prg_root, size_t *buf_num);
void linearize_cycle(FILE *linear, ast_node *node, ast_node *prg_root, size_t *buf_num, size_t *label_num, size_t *print_num);
void linearize_op_root(FILE *linear, ast_node *node, ast_node *prg_root, size_t *buf_num, size_t *label_num, size_t *print_num);
void linearize(const char *path, ast_node *node);

#endif //COMPILERSPROJECT_LINEAR_H