#ifndef __OPSTACK_H
#define __OPSTACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

/**
 * @brief 定义运算符栈（用于中缀转后缀）
 */
typedef struct {
    char **data;
    int top;
    int capacity;
} OpStack;

/**
 * @brief 获取运算符优先级
 */
int get_precedence(char op);

OpStack* create_op_stack(int capacity);
bool op_stack_is_empty(OpStack* stack);
bool op_stack_is_full(OpStack* stack);

void op_push(OpStack* stack, char* op);
char* op_pop(OpStack* stack);
char* op_peek(OpStack* stack);
void free_op_stack(OpStack* stack) ;
double apply_operation(char op, double a, double b) ;

#endif