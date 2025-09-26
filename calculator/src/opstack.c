#include "../include/opstack.h"

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