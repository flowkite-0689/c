#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// 最大历史记录数量
#define MAX_HISTORY 100
// 时辰名称
const char* hour_names[12] = {"子时", "丑时", "寅时", "卯时", "辰时", "巳时", 
                             "午时", "未时", "申时", "酉时", "戌时", "亥时"};

// 小六壬掌诀数组
const char* positions[6] = {"大安", "留连", "速喜", "赤口", "小吉", "空亡"};

// 掌诀简短含义说明
const char* explanations[6] = {
    "大安事事昌，求谋在东方，失物去不远，宅舍保安康",
    "留连事难成，求谋日未明，官事只宜缓，去者未回程",
    "速喜喜来临，求财向南行，失物申午未，逢人路上寻",
    "赤口主口舌，官非切要防，失物急去寻，行人有惊慌",
    "小吉最吉昌，路上好商量，阴人来报喜，失物在坤方",
    "空亡事不祥，阴人多乖张，求财无利益，行人有灾殃"
};

// 掌诀详细解释
const char* detailed_explanations[6] = {
    "大安：大吉之兆。万事顺利，平安吉祥。\n"
    "运势：目前运势正盛，凡事都能顺利进行，适合谋划大事。\n"
    "财运：财运平稳，可以尝试在东方寻找商机。\n"
    "感情：感情稳定发展，单身者有机会遇到心仪对象。\n"
    "事业：工作顺利，能得到上级赏识，有晋升机会。",
    
    "留连：拖延之兆。事情进展缓慢，需耐心等待。\n"
    "运势：目前运势停滞，凡事不宜急躁，需循序渐进。\n"
    "财运：财运不佳，投资需谨慎，避免冒险。\n"
    "感情：感情发展缓慢，容易出现误会，需多沟通。\n"
    "事业：工作遇到瓶颈，进展缓慢，需耐心克服困难。",
    
    "速喜：喜庆之兆。事情能快速解决，有喜事临门。\n"
    "运势：运势向好，事情能迅速得到解决，喜讯将至。\n"
    "财运：财运亨通，南方有利，可把握投资机会。\n"
    "感情：感情进展顺利，可能有惊喜出现。\n"
    "事业：工作效率高，能快速完成任务，得到认可。",
    
    "赤口：口舌之兆。需防是非争执，注意言行。\n"
    "运势：运势下滑，容易发生口角是非，需谨言慎行。\n"
    "财运：财运不佳，容易有金钱纠纷，避免借贷。\n"
    "感情：感情容易出现争吵，需控制情绪，避免冲突。\n"
    "事业：工作上易与同事发生矛盾，需注意人际关系。",
    
    "小吉：小吉之兆。凡事顺利，有小惊喜。\n"
    "运势：运势不错，凡事都能顺利解决，有小吉小利。\n"
    "财运：财运尚可，可在西南方向寻找机会。\n"
    "感情：感情稳定，可能会收到对方的小礼物或惊喜。\n"
    "事业：工作顺利，能得到同事帮助，进展顺利。",
    
    "空亡：空亡之兆。事情难成，需谨慎行事。\n"
    "运势：运势低迷，凡事不易成功，不宜做重大决定。\n"
    "财运：财运不佳，投资容易失利，宜保守。\n"
    "感情：感情容易出现变数，需多加留意。\n"
    "事业：工作上容易出现失误，需仔细检查，避免出错。"
};

// 历史记录结构体
typedef struct {
    int month;
    int day;
    int hour;
    int result;
    time_t timestamp; // 记录占卜时间
} FortuneRecord;

// 全局历史记录数组和计数器
FortuneRecord history[MAX_HISTORY];
int history_count = 0;

// 函数声明
void print_menu();
int calculate_fortune(int month, int day, int hour);
void display_result(int month, int day, int hour, int result);
void add_to_history(int month, int day, int hour, int result);
void show_history();
void random_fortune();
void manual_fortune();
int get_valid_input(int min, int max, const char* prompt);

int main() {
    int choice;
    srand(time(NULL)); // 初始化随机数种子
    
    printf("========================================\n");
    printf("          小六壬占卜程序 v2.0           \n");
    printf("========================================\n");
    printf("小六壬是中国传统占卜术之一，据传为诸葛亮所创。\n");
    printf("本程序仅供娱乐参考，请勿过度迷信。\n\n");
    
    do {
        print_menu();
        choice = get_valid_input(1, 4, "请选择操作(1-4): ");
        
        switch(choice) {
            case 1:
                manual_fortune();
                break;
            case 2:
                random_fortune();
                break;
            case 3:
                show_history();
                break;
            case 4:
                printf("感谢使用，再见！\n");
                break;
        }
        
        if (choice != 4) {
            printf("\n按回车键继续...");
            getchar(); // 消耗之前的换行符
            getchar(); // 等待用户按键
            system("cls || clear"); // 清屏，兼容Windows和Linux
        }
        
    } while (choice != 4);
    
    return 0;
}

// 显示菜单
void print_menu() {
    printf("\n请选择占卜方式：\n");
    printf("1. 手动输入日期时间占卜\n");
    printf("2. 随机时间占卜\n");
    printf("3. 查看历史记录\n");
    printf("4. 退出程序\n");
}

// 计算占卜结果
int calculate_fortune(int month, int day, int hour) {
    int start = 0; // 大安起始位置为0
    
    // 月上起日
    int mid = (start + month - 1) % 6;
    
    // 日上起日
    mid = (mid + day - 1) % 6;
    
    // 日上起时
    return (mid + hour - 1) % 6;
}

// 显示占卜结果
void display_result(int month, int day, int hour, int result) {
    printf("\n========================================\n");
    printf("            占卜结果详情                \n");
    printf("========================================\n");
    printf("农历 %d月%d日 %s\n\n", month, day, hour_names[hour-1]);
    printf("掌诀：%s\n", positions[result]);
    printf("简释：%s\n\n", explanations[result]);
    
    // 询问是否查看详细解释
    char answer;
    printf("是否查看详细解释？(y/n): ");
    scanf(" %c", &answer);
    
    if (answer == 'y' || answer == 'Y') {
        printf("\n详细解释：\n%s\n", detailed_explanations[result]);
    }
    
    printf("\n========================================\n");
    printf("注意：此占卜结果仅供参考，请勿过度依赖。\n");
}

// 添加到历史记录
void add_to_history(int month, int day, int hour, int result) {
    if (history_count < MAX_HISTORY) {
        history[history_count].month = month;
        history[history_count].day = day;
        history[history_count].hour = hour;
        history[history_count].result = result;
        history[history_count].timestamp = time(NULL);
        history_count++;
        printf("\n已记录本次占卜结果到历史记录。\n");
    } else {
        printf("\n历史记录已满，无法保存本次结果。\n");
    }
}

// 显示历史记录
void show_history() {
    if (history_count == 0) {
        printf("\n暂无历史记录。\n");
        return;
    }
    
    printf("\n========================================\n");
    printf("            占卜历史记录                \n");
    printf("========================================\n");
    printf("序号 | 日期       | 时辰 | 结果 | 占卜时间\n");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < history_count; i++) {
        char time_str[20];
        struct tm* local_time = localtime(&history[i].timestamp);
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M", local_time);
        
        printf("%4d | %d月%d日     | %s  | %s | %s\n",
               i+1,
               history[i].month,
               history[i].day,
               hour_names[history[i].hour-1],
               positions[history[i].result],
               time_str);
    }
    
    // 询问是否查看某条记录的详细信息
    char answer;
    printf("\n是否查看某条记录的详细信息？(y/n): ");
    scanf(" %c", &answer);
    
    if (answer == 'y' || answer == 'Y') {
        int index = get_valid_input(1, history_count, "请输入记录序号: ");
        index--; // 转换为数组索引
        
        printf("\n========================================\n");
        printf("            记录详情                    \n");
        printf("========================================\n");
        char time_str[20];
        struct tm* local_time = localtime(&history[index].timestamp);
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M", local_time);
        printf("占卜时间: %s\n", time_str);
        
        display_result(history[index].month, history[index].day, 
                      history[index].hour, history[index].result);
    }
}

// 随机时间占卜
void random_fortune() {
    printf("\n========== 随机时间占卜 ==========\n");
    printf("系统将随机生成一个日期时间进行占卜\n\n");
    
    int month = rand() % 12 + 1; // 1-12月
    int day;
    
    // 根据月份调整天数，简化处理
    if (month == 2) {
        day = rand() % 29 + 1; // 最多29天
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        day = rand() % 30 + 1; // 30天
    } else {
        day = rand() % 31 + 1; // 31天
    }
    
    int hour = rand() % 12 + 1; // 1-12时辰
    
    printf("随机生成的时间：农历 %d月%d日 %s\n", 
           month, day, hour_names[hour-1]);
    
    int result = calculate_fortune(month, day, hour);
    display_result(month, day, hour, result);
    add_to_history(month, day, hour, result);
}

// 手动输入占卜
void manual_fortune() {
    int month, day, hour;
    
    printf("\n========== 手动输入占卜 ==========\n");
    printf("温馨提示：不急不占、无事不占、不动不占\n\n");
    
    // 获取用户输入
    month = get_valid_input(1, 12, "请输入当前月份(农历1-12): ");
    day = get_valid_input(1, 30, "请输入当前日期(农历1-30): ");
    
    printf("\n请选择时辰:\n");
    for (int i = 0; i < 12; i++) {
        printf("%d. 【%s】", i+1, hour_names[i]);
        switch(i) {
            case 0: printf("23:00-01:00\n"); break;
            case 1: printf("01:00-03:00\n"); break;
            case 2: printf("03:00-05:00\n"); break;
            case 3: printf("05:00-07:00\n"); break;
            case 4: printf("07:00-09:00\n"); break;
            case 5: printf("09:00-11:00\n"); break;
            case 6: printf("11:00-13:00\n"); break;
            case 7: printf("13:00-15:00\n"); break;
            case 8: printf("15:00-17:00\n"); break;
            case 9: printf("17:00-19:00\n"); break;
            case 10: printf("19:00-21:00\n"); break;
            case 11: printf("21:00-23:00\n"); break;
        }
    }
    
    hour = get_valid_input(1, 12, "请输入时辰编号(1-12): ");
    
    int result = calculate_fortune(month, day, hour);
    display_result(month, day, hour, result);
    add_to_history(month, day, hour, result);
}

// 获取有效的输入值
int get_valid_input(int min, int max, const char* prompt) {
    int value;
    int valid;
    
    do {
        printf("%s", prompt);
        valid = scanf("%d", &value);
        
        // 清除输入缓冲区中的错误输入
        if (!valid) {
            printf("输入错误，请输入一个数字！\n");
            while (getchar() != '\n'); // 清空输入缓冲区
        } else if (value < min || value > max) {
            printf("输入超出范围，请输入%d到%d之间的数字！\n", min, max);
            valid = 0;
        }
    } while (!valid);
    
    return value;
}
