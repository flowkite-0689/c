#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <math.h>

#define SIZE 4
#define THRESHOLD 50  // 触摸滑动阈值

// 2048游戏颜色定义 (ARGB格式)
unsigned int colors[] = {
    0xFFCCCCCC,  // 0
    0xFFEEE4DA,  // 2
    0xFFEDE0C8,  // 4
    0xFFF2B179,  // 8
    0xFFF59563,  // 16
    0xFFF67C5F,  // 32
    0xFFF65E3B,  // 64
    0xFFEDCF72,  // 128
    0xFFEDCC61,  // 256
    0xFFEDC850,  // 512
    0xFFEDC53F,  // 1024
    0xFFEDC22E   // 2048
};

int board[SIZE][SIZE];
int score = 0;
int fb_fd, touch_fd;
unsigned int *fb_buffer;
int screen_width = 1024, screen_height = 600;

// 初始化framebuffer
int init_framebuffer() {
    fb_fd = open("/dev/fb0", O_RDWR);
    if (fb_fd < 0) {
        perror("open /dev/fb0 fail");
        return -1;
    }

    fb_buffer = mmap(NULL, screen_width * screen_height * 4,
                    PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if (fb_buffer == (void *)-1) {
        perror("mmap fail");
        close(fb_fd);
        return -1;
    }
    return 0;
}

// 初始化触摸屏
int init_touch() {
    touch_fd = open("/dev/input/event6", O_RDONLY);
    if (touch_fd < 0) {
        perror("open touch device fail");
        return -1;
    }
    return 0;
}

// 绘制2048游戏界面
void draw_board() {
    // 清屏
    for (int y = 0; y < screen_height; y++) {
        for (int x = 0; x < screen_width; x++) {
            fb_buffer[y * screen_width + x] = 0xFFEEEEEE; // 灰色背景
        }
    }

    // 计算每个格子的大小
    int cell_width = screen_width / (SIZE + 2);
    int cell_height = screen_height / (SIZE + 2);
    int board_start_x = (screen_width - SIZE * cell_width) / 2;
    int board_start_y = (screen_height - SIZE * cell_height) / 2;

    // 绘制游戏板
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int value = board[i][j];
            int color_idx = value ? (int)(log2(value)) : 0;
            if (color_idx > 11) color_idx = 11;

            // 绘制格子
            for (int y = 0; y < cell_height; y++) {
                for (int x = 0; x < cell_width; x++) {
                    int px = board_start_x + j * cell_width + x;
                    int py = board_start_y + i * cell_height + y;
                    fb_buffer[py * screen_width + px] = colors[color_idx];
                }
            }

            // 绘制数字
            if (value > 0) {
                char num_str[10];
                sprintf(num_str, "%d", value);
                int text_x = board_start_x + j * cell_width + cell_width/2 - 10;
                int text_y = board_start_y + i * cell_height + cell_height/2;

                // 简单数字绘制
                for (int c = 0; c < strlen(num_str); c++) {
                    int px = text_x + c * 10;
                    int py = text_y;
                    if (px >= 0 && px < screen_width && py >= 0 && py < screen_height) {
                        fb_buffer[py * screen_width + px] = 0xFF000000; // 黑色文字
                    }
                }
            }
        }
    }
}

// 2048游戏逻辑 (从2048v2.c移植)
void add_num() {
    int empty[SIZE * SIZE][2];
    int count = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                empty[count][0] = i;
                empty[count][1] = j;
                count++;
            }
        }
    }
    if (count) {
        int index = rand() % count;
        int num = (rand() % 10 < 9) ? 2 : 4;
        board[empty[index][0]][empty[index][1]] = num;
    }
}

// 触摸屏输入处理 (从Swipedetection.c移植)
int get_swipe_direction() {
    struct input_event ev;
    int start_x = -1, start_y = -1, end_x = -1, end_y = -1;
    int touch_active = 0;

    while (1) {
        if (read(touch_fd, &ev, sizeof(ev)) != sizeof(ev)) {
            perror("read ev fail");
            return -1;
        }

        if (ev.type == EV_KEY && ev.code == BTN_TOUCH) {
            if (ev.value == 1) {
                touch_active = 1;
            } else if (ev.value == 0 && touch_active) {
                touch_active = 0;
                break;
            }
        }

        if (touch_active && ev.type == EV_ABS) {
            if (ev.code == ABS_X) {
                if (start_x == -1) start_x = ev.value;
                end_x = ev.value;
            } else if (ev.code == ABS_Y) {
                if (start_y == -1) start_y = ev.value;
                end_y = ev.value;
            }
        }
    }

    if (start_x == -1 || start_y == -1 || end_x == -1 || end_y == -1) {
        return -1;
    }

    int delta_x = abs(end_x - start_x);
    int delta_y = abs(end_y - start_y);

    if (delta_x < THRESHOLD && delta_y < THRESHOLD) {
        return 4; // 点击
    }
    if (delta_x > delta_y && delta_x > THRESHOLD) {
        return (end_x > start_x) ? 1 : 3; // 右:左 (对应2048v2.c的方向编码)
    }
    if (delta_y > THRESHOLD) {
        return (end_y > start_y) ? 2 : 0; // 下:上
    }

    return 4; // 默认视为点击
}

// 移动函数 (从2048v2.c移植)
int move(int direction) {
    int he[SIZE * SIZE / 2][2];
    int count = 0;
    int moved = 0;

    if (direction == 0) { // 左移
        for (int i = 0; i < SIZE; i++) {
            for (int j = 1; j < SIZE; j++) {
                if (board[i][j] != 0) {
                    int k = j;
                    while (k > 0 && board[i][k - 1] == 0) {
                        board[i][k - 1] = board[i][k];
                        board[i][k] = 0;
                        k--;
                        moved = 1;
                    }
                    if (k > 0 && board[i][k - 1] == board[i][k]) {
                        int flag = 0;
                        for (int z = 0; z < count; z++) {
                            if (he[z][0] == i && he[z][1] == (k - 1)) {
                                flag = 1;
                            }
                        }
                        if (!flag) {
                            board[i][k - 1] *= 2;
                            he[count][0] = i;
                            he[count][1] = (k - 1);
                            count++;
                            score += board[i][k - 1];
                            board[i][k] = 0;
                            moved = 1;
                        }
                    }
                }
            }
        }
    }
    else if (direction == 1) { // 右移
        for (int i = 0; i < SIZE; i++) {
            for (int j = SIZE - 2; j >= 0; j--) {
                if (board[i][j] != 0) {
                    int k = j;
                    while (k < SIZE - 1 && board[i][k + 1] == 0) {
                        board[i][k + 1] = board[i][k];
                        board[i][k] = 0;
                        k++;
                        moved = 1;
                    }
                    if (k < SIZE - 1 && board[i][k + 1] == board[i][k]) {
                        int flag = 0;
                        for (int z = 0; z < count; z++) {
                            if (he[z][0] == i && he[z][1] == (k + 1)) {
                                flag = 1;
                            }
                        }
                        if (!flag) {
                            board[i][k + 1] *= 2;
                            he[count][0] = i;
                            he[count][1] = (k + 1);
                            count++;
                            score += board[i][k + 1];
                            board[i][k] = 0;
                            moved = 1;
                        }
                    }
                }
            }
        }
    }
    else if (direction == 2) { // 上移
        for (int j = 0; j < SIZE; j++) {
            for (int i = 1; i < SIZE; i++) {
                if (board[i][j] != 0) {
                    int k = i;
                    while (k > 0 && board[k - 1][j] == 0) {
                        board[k - 1][j] = board[k][j];
                        board[k][j] = 0;
                        k--;
                        moved = 1;
                    }
                    if (k > 0 && board[k - 1][j] == board[k][j]) {
                        int flag = 0;
                        for (int z = 0; z < count; z++) {
                            if (he[z][0] == (k - 1) && he[z][1] == j) {
                                flag = 1;
                            }
                        }
                        if (!flag) {
                            board[k - 1][j] *= 2;
                            he[count][0] = (k - 1);
                            he[count][1] = j;
                            count++;
                            score += board[k - 1][j];
                            board[k][j] = 0;
                            moved = 1;
                        }
                    }
                }
            }
        }
    }
    else if (direction == 3) { // 下移
        for (int j = 0; j < SIZE; j++) {
            for (int i = SIZE - 2; i >= 0; i--) {
                if (board[i][j] != 0) {
                    int k = i;
                    while (k < SIZE - 1 && board[k + 1][j] == 0) {
                        board[k + 1][j] = board[k][j];
                        board[k][j] = 0;
                        k++;
                        moved = 1;
                    }
                    if (k < SIZE - 1 && board[k + 1][j] == board[k][j]) {
                        int flag = 0;
                        for (int z = 0; z < count; z++) {
                            if (he[z][0] == (k + 1) && he[z][1] == j) {
                                flag = 1;
                            }
                        }
                        if (!flag) {
                            board[k + 1][j] *= 2;
                            he[count][0] = (k + 1);
                            he[count][1] = j;
                            count++;
                            score += board[k + 1][j];
                            board[k][j] = 0;
                            moved = 1;
                        }
                    }
                }
            }
        }
    }
    return moved;
}

// 游戏结束检查 (从2048v2.c移植)
int isGameover() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 2048) return 1;
            if (board[i][j] == 0) return 0;
        }
    }
    for (int i = 0; i < SIZE - 1; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i + 1][j] == board[i][j]) return 0;
        }
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE - 1; j++) {
            if (board[i][j + 1] == board[i][j]) return 0;
        }
    }
    return 1;
}

// 主游戏循环
void game_loop() {
    // 初始化游戏板
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }
    add_num();
    add_num();

    while (1) {
        draw_board();

        int direction = get_swipe_direction();
        if (direction == 4) continue; // 忽略点击

        if (move(direction)) {
            add_num();
        }

        if (isGameover()) break;
    }
}

int main() {
    srand(time(NULL));

    if (init_framebuffer() < 0) return -1;
    if (init_touch() < 0) {
        munmap(fb_buffer, screen_width * screen_height * 4);
        close(fb_fd);
        return -1;
    }

    game_loop();

    // 清理
    munmap(fb_buffer, screen_width * screen_height * 4);
    close(fb_fd);
    close(touch_fd);
    return 0;
}
