#include "../include/numandop.h"

/**
 * @brief 将表达式分割成数字和运算符
 */
char** tokenize(const char* expr, int* token_count) {
    int len = strlen(expr);
    char** tokens = (char**)malloc(len * sizeof(char*));
    *token_count = 0;

    char* buffer = (char*)malloc((len + 1) * sizeof(char));
    int buffer_index = 0;

    for (int i = 0; i <= len; i++) {
        if (i < len && isdigit(expr[i])) {
            buffer[buffer_index++] = expr[i];
        } else {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                tokens[*token_count] = (char*)malloc((buffer_index + 1) * sizeof(char));
                strcpy(tokens[*token_count], buffer);
                (*token_count)++;
                buffer_index = 0;
            }

            if (i < len && (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' ||
                           expr[i] == '/' || expr[i] == '^' || expr[i] == '(' || expr[i] == ')')) {
                tokens[*token_count] = (char*)malloc(2 * sizeof(char));
                tokens[*token_count][0] = expr[i];
                tokens[*token_count][1] = '\0';
                (*token_count)++;
            }
        }
    }

    free(buffer);
    return tokens;
}

/**
 * @brief 判断字符串是否为数字
 */
bool is_number(const char* s) {
    char* endptr;
    strtod(s, &endptr);
    return *endptr == '\0';
}

/**
 * @brief 将中缀表达式转换为后缀表达式
 */
char** infix_to_postfix(char** tokens, int token_count, int* postfix_count) {
    char** postfix = (char**)malloc(token_count * sizeof(char*));
    *postfix_count = 0;

    OpStack* op_stack = create_op_stack(token_count);

    for (int i = 0; i < token_count; i++) {
        if (is_number(tokens[i])) {
            postfix[*postfix_count] = (char*)malloc((strlen(tokens[i]) + 1) * sizeof(char));
            strcpy(postfix[*postfix_count], tokens[i]);
            (*postfix_count)++;
        } else if (strcmp(tokens[i], "(") == 0) {
            op_push(op_stack, tokens[i]);
        } else if (strcmp(tokens[i], ")") == 0) {
            while (!op_stack_is_empty(op_stack) && strcmp(op_peek(op_stack), "(") != 0) {
                char* op = op_pop(op_stack);
                postfix[*postfix_count] = (char*)malloc(2 * sizeof(char));
                postfix[*postfix_count][0] = *op;
                postfix[*postfix_count][1] = '\0';
                (*postfix_count)++;
            }
            if (!op_stack_is_empty(op_stack)) op_pop(op_stack); // 弹出左括号
        } else {
            while (!op_stack_is_empty(op_stack) &&
                   get_precedence(*op_peek(op_stack)) >= get_precedence(*tokens[i]) &&
                   strcmp(op_peek(op_stack), "(") != 0) {
                char* op = op_pop(op_stack);
                postfix[*postfix_count] = (char*)malloc(2 * sizeof(char));
                postfix[*postfix_count][0] = *op;
                postfix[*postfix_count][1] = '\0';
                (*postfix_count)++;
            }
            op_push(op_stack, tokens[i]);
        }
    }

    while (!op_stack_is_empty(op_stack)) {
        char* op = op_pop(op_stack);
        postfix[*postfix_count] = (char*)malloc(2 * sizeof(char));
        postfix[*postfix_count][0] = *op;
        postfix[*postfix_count][1] = '\0';
        (*postfix_count)++;
    }

    free_op_stack(op_stack);
    return postfix;
}

/**
 * @brief 计算后缀表达式的值
 */
double evaluate_postfix(char** postfix, int postfix_count) {
    NumStack* num_stack = create_num_stack(postfix_count);

    for (int i = 0; i < postfix_count; i++) {
        if (is_number(postfix[i])) {
            num_push(num_stack, atof(postfix[i]));
        } else {
            double b = num_pop(num_stack);
            double a = num_pop(num_stack);
            num_push(num_stack, apply_operation(*postfix[i], a, b));
        }
    }

    double result = num_pop(num_stack);
    free_num_stack(num_stack);
    return result;
}

/**
 * @brief 计算表达式的值（主入口函数）
 */
double evaluate_expression(const char* expr) {
    int token_count;
    char** tokens = tokenize(expr, &token_count);
    
    int postfix_count;
    char** postfix = infix_to_postfix(tokens, token_count, &postfix_count);
    
    // 释放tokens内存
    for (int i = 0; i < token_count; i++) {
        free(tokens[i]);
    }
    free(tokens);
    
    double result = evaluate_postfix(postfix, postfix_count);
    
    // 释放postfix内存
    for (int i = 0; i < postfix_count; i++) {
        free(postfix[i]);
    }
    free(postfix);
    
    return result;
}
