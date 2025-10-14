#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>

// 使用 #define 定义常量
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 600

// 全局变量用于framebuffer和触摸设备
static int fb_fd = -1;
static unsigned int *fb_ptr = NULL;
static int touch_fd = -1;

int initFramebuffer();           // 初始化framebuffer
int getTouch(int *x, int *y);    // 获取触摸坐标
int turnColour(const char *color); // 改变屏幕颜色
void cleanup();                  // 清理资源

int main()
{
    char current_color[16] = "red";

    // 初始化framebuffer
    if (initFramebuffer() < 0) {
        fprintf(stderr, "初始化framebuffer失败\n");
        return -1;
    }

    printf("程序启动，点击屏幕左半边显示绿色，右半边显示红色\n");
    printf("按Ctrl+C退出程序\n");

    while (1) {
        int x = 0, y = 0;

        // 获取触摸坐标
        if (getTouch(&x, &y) < 0) {
            continue;
        }

        printf("触摸坐标: x=%d, y=%d\n", x, y);

        char new_color[16] = "red";
        if (x < (SCREEN_WIDTH / 2)) {
            strcpy(new_color, "green");
            printf("左半边触摸 - 切换到绿色\n");
        } else {
            printf("右半边触摸 - 切换到红色\n");
        }

        // 只有颜色改变时才更新屏幕
        if (strcmp(current_color, new_color) != 0) {
            if (turnColour(new_color) < 0) {
                fprintf(stderr, "设置颜色失败\n");
                break;
            }
            strcpy(current_color, new_color);
        }

        usleep(50000); // 减少延迟提高响应性
    }

    cleanup();
    return 0;
}

int initFramebuffer()
{
    fb_fd = open("/dev/fb0", O_RDWR);
    if (fb_fd < 0) {
        perror("打开framebuffer失败");
        return -1;
    }

    fb_ptr = mmap(
        NULL,                             // 系统自动寻找合适的内存
        SCREEN_WIDTH * SCREEN_HEIGHT * 4, // 映射的字节大小
        PROT_READ | PROT_WRITE,           // 权限
        MAP_SHARED,                       // 内存进程共享
        fb_fd,                           // 要映射的文件描述符
        0);                              // 偏移量

    if (fb_ptr == (void *)-1) {
        perror("mmap失败");
        close(fb_fd);
        fb_fd = -1;
        return -1;
    }

    return 0;
}

int turnColour(const char *color)
{
    if (fb_ptr == NULL) {
        fprintf(stderr, "framebuffer未初始化\n");
        return -1;
    }

    unsigned int green_color = (255 << 24) | (0 << 16) | (255 << 8) | (0 << 0); // ARGB: 不透明绿色
    unsigned int red_color =  (255 << 24) | (255 << 16) | (0 << 8) | (0 << 0);   // ARGB: 不透明红色

    // 选择颜色
    unsigned int selected_color = (strcmp(color, "green") == 0) ? green_color : red_color;

    // 填充整个屏幕
    for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
        *(fb_ptr + i) = selected_color;
    }

    return 0;
}

int getTouch(int *x, int *y)
{
    struct input_event input_info = {0};
    static int temp_x = -1, temp_y = -1;

    // 打开触摸屏设备（只打开一次）
    if (touch_fd < 0) {
        touch_fd = open("/dev/input/event6", O_RDONLY | O_NONBLOCK);
        if (touch_fd < 0) {
            perror("打开触摸屏文件失败");
            return -1;
        }
    }

    // 读取触摸事件
    while (read(touch_fd, &input_info, sizeof(input_info)) > 0) {
        if (input_info.type == EV_ABS) {
            if (input_info.code == ABS_X) {
                temp_x = input_info.value;
            } else if (input_info.code == ABS_Y) {
                temp_y = input_info.value;
            }
        } else if (input_info.type == EV_KEY && input_info.code == BTN_TOUCH) {
            // 触摸按下事件
            if (input_info.value == 1 && temp_x >= 0 && temp_y >= 0) {
                *x = temp_x;
                *y = temp_y;
                return 0; // 成功获取触摸坐标
            }
        }
    }

    return -1; // 没有有效触摸事件
}

void cleanup()
{
    if (fb_ptr != NULL) {
        munmap(fb_ptr, SCREEN_HEIGHT * SCREEN_WIDTH * 4);
        fb_ptr = NULL;
    }
    if (fb_fd >= 0) {
        close(fb_fd);
        fb_fd = -1;
    }
    if (touch_fd >= 0) {
        close(touch_fd);
        touch_fd = -1;
    }
    printf("资源清理完成\n");
}
