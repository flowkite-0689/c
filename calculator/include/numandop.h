#ifndef __NUMANDOP_H
#define __NUMANDOP_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "../include/opstack.h"
#include "../include/numstack.h"
/**
 * @brief 将表达式分割成令牌（数字和运算符）
 */
char** tokenize(const char* expr, int* token_count);

/**
 * @brief 判断字符串是否为数字
 */
bool is_number(const char* s);

/**
 * @brief 将中缀表达式转换为后缀表达式（逆波兰表示法）
 */
char** infix_to_postfix(char** tokens, int token_count, int* postfix_count) ;


/**
 * @brief 计算后缀表达式的值
 */
double evaluate_postfix(char** postfix, int postfix_count);
/**
 * @brief 计算表达式的值（主入口函数）
 */
double evaluate_expression(const char* expr);
#endif