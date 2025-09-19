#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 小六壬掌诀数组
const char* positions[6] = {"大安", "留连", "速喜", "赤口", "小吉", "空亡"};

// 掌诀含义说明
const char* explanations[6] = {
    "大安事事昌，求谋在东方，失物去不远，宅舍保安康",
    "留连事难成，求谋日未明，官事只宜缓，去者未回程",
    "速喜喜来临，求财向南行，失物申午未，逢人路上寻",
    "赤口主口舌，官非切要防，失物急去寻，行人有惊慌",
    "小吉最吉昌，路上好商量，阴人来报喜，失物在坤方",
    "空亡事不祥，阴人多乖张，求财无利益，行人有灾殃"
};

int main() {
    int month, day, hour;
    
    printf("========== 小六壬占卜程序 ==========\n");
    printf("温馨提示：不急不占、无事不占、不动不占\n\n");
    
    // 获取用户输入
    printf("请输入当前月份(农历1-12): ");
    scanf("%d", &month);
    
    printf("请输入当前日期(农历1-30): ");
    scanf("%d", &day);
    
    printf("\n请选择时辰:\n");
    printf("1. 【子】23:00-01:00\n");
    printf("2. 【丑】01:00-03:00\n");
    printf("3. 【寅】03:00-05:00\n");
    printf("4. 【卯】05:00-07:00\n");
    printf("5. 【辰】07:00-09:00\n");
    printf("6. 【巳】09:00-11:00\n");
    printf("7. 【午】11:00-13:00\n");
    printf("8. 【未】13:00-15:00\n");
    printf("9. 【申】15:00-17:00\n");
    printf("10.【酉】17:00-19:00\n");
    printf("11.【戌】19:00-21:00\n");
    printf("12.【亥】21:00-23:00\n");
    printf("请输入时辰编号(1-12): ");
    scanf("%d", &hour);
    
    // 输入验证
    if(month < 1 || month > 12 || day < 1 || day > 30 || hour < 1 || hour > 12) {
        printf("输入错误！请确保月份(1-12)、日期(1-30)和时辰(1-12)在有效范围内。\n");
        return 1;
    }
    
    // 小六壬算法：月上起日，日上起时
    int start = 0; // 大安起始位置为0
    
    // 月上起日
    int mid = (start + month - 1) % 6;

    // 日上起日
    mid = (mid + day - 1) % 6;
    
    // 日上起时
    int result = (mid + hour - 1) % 6;
    
    // 显示结果
    printf("\n========== 占卜结果 ==========\n");
    printf("农历 %d月%d日 ", month, day);
    
    // 显示时辰名称
    switch(hour) {
        case 1: printf("子时"); break;
        case 2: printf("丑时"); break;
        case 3: printf("寅时"); break;
        case 4: printf("卯时"); break;
        case 5: printf("辰时"); break;
        case 6: printf("巳时"); break;
        case 7: printf("午时"); break;
        case 8: printf("未时"); break;
        case 9: printf("申时"); break;
        case 10: printf("酉时"); break;
        case 11: printf("戌时"); break;
        case 12: printf("亥时"); break;
    }
    
    printf(" 的占卜结果:\n");
    printf("掌诀：%s\n", positions[result]);
    printf("含义：%s\n", explanations[result]);
    
    printf("\n===============================\n");
    printf("注意：此占卜结果仅供参考，请勿过度依赖。\n");
    
    return 0;
}