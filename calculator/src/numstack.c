#include "../include/numstack.h"

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
