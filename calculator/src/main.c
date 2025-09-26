#include "../include/numandop.h"


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
