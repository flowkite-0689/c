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
int get_precedence(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

// --- 数字栈操作 ---
NumStack* create_num_stack(int capacity) {
    NumStack* stack = (NumStack*)malloc(sizeof(NumStack));
    stack->data = (double*)malloc(capacity * sizeof(double));
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

bool num_stack_is_empty(NumStack* stack) { return stack->top == -1; }
bool num_stack_is_full(NumStack* stack) { return stack->top == stack->capacity - 1; }

void num_push(NumStack* stack, double value) {
    if (num_stack_is_full(stack)) {
        printf("数字栈已满，无法压入元素\n");
        return;
    }
    stack->data[++stack->top] = value;
}

double num_pop(NumStack* stack) {
    if (num_stack_is_empty(stack)) {
        printf("数字栈为空，无法弹出元素\n");
        return 0;
    }
    return stack->data[stack->top--];
}

double num_peek(NumStack* stack) {
    if (num_stack_is_empty(stack)) return 0;
    return stack->data[stack->top];
}

void free_num_stack(NumStack* stack) {
    free(stack->data);
    free(stack);
}

// --- 运算符栈操作 ---
OpStack* create_op_stack(int capacity) {
    OpStack* stack = (OpStack*)malloc(sizeof(OpStack));
    stack->data = (char**)malloc(capacity * sizeof(char*));
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

bool op_stack_is_empty(OpStack* stack) { return stack->top == -1; }
bool op_stack_is_full(OpStack* stack) { return stack->top == stack->capacity - 1; }

void op_push(OpStack* stack, char* op) {
    if (op_stack_is_full(stack)) {
        printf("运算符栈已满，无法压入元素\n");
        return;
    }
    stack->data[++stack->top] = op;
}

char* op_pop(OpStack* stack) {
    if (op_stack_is_empty(stack)) {
        printf("运算符栈为空，无法弹出元素\n");
        return NULL;
    }
    return stack->data[stack->top--];
}

char* op_peek(OpStack* stack) {
    if (op_stack_is_empty(stack)) return NULL;
    return stack->data[stack->top];
}

void free_op_stack(OpStack* stack) {
    // 注意：这里不释放栈中存储的字符串指针
    // 因为它们指向tokenize分配的内存，由调用者统一释放
    free(stack->data);
    free(stack);
}

/**
 * @brief 应用运算符进行计算
 */
double apply_operation(char op, double a, double b) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                printf("错误：除零错误\n");
                return 0;
            }
            return a / b;
        case '^': return pow(a, b);
        default:
            printf("错误：无效的运算符 '%c'\n", op);
            return 0;
    }
}

/**
 * @brief 将表达式分割成令牌（数字和运算符）
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
 * @brief 将中缀表达式转换为后缀表达式（逆波兰表示法）
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

/**
 * @brief 主函数 - 计算器主界面
 */
int main() {
    printf("欢迎使用C语言表达式计算器！\n");
    printf("支持运算符: +, -, *, /, ^, ( )\n");
    printf("输入 'exit' 退出程序\n\n");
    
    char input[100];
    while (1) {
        printf("请输入表达式: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // 移除换行符
        
        if (strcmp(input, "exit") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        double result = evaluate_expression(input);
        printf("结果: %.2f\n\n", result);
    }
    
    printf("感谢使用计算器！\n");
    return 0;
}
