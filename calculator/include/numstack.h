#ifndef __numstack_H
#define __numstack_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
/**
 * @brief 定义数字栈（用于计算）
 */
typedef struct {
    double *data;
    int top;
    int capacity;
} NumStack;

NumStack* create_num_stack(int capacity);
bool num_stack_is_empty(NumStack* stack);
bool num_stack_is_full(NumStack* stack);
void num_push(NumStack* stack, double value);
double num_pop(NumStack* stack);
double num_peek(NumStack* stack);
void free_num_stack(NumStack* stack);


#endif