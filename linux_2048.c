/**
 * @file linux_2048.c
 * @brief 2048游戏Linux优化版本
 * @version 2.0
 * @date 2025
 *
 * 跨平台兼容，性能优化，支持配置和状态保存
 * 使用WASD控制方向，Q退出游戏
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// 平台检测
#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <sys/stat.h>
#endif

#define SIZE 4
#define CONFIG_FILE ".2048_config"
#define SAVE_FILE ".2048_save"

// 配置结构
typedef struct {
    int use_colors;
    int animation_delay;
    char key_up;
    char key_down;
    char key_left;
    char key_right;
    char key_quit;
} GameConfig;

// 游戏状态结构
typedef struct {
    int board[SIZE][SIZE];
    int score;
    int move_count;
} GameState;

GameState current_game;
GameConfig config;

// 跨平台getch实现
int getch(void) {
    #ifdef _WIN32
        return _getch();
    #else
        struct termios oldt, newt;
        int ch;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        return ch;
    #endif
}

// 清屏函数
void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        printf("\033[H\033[J");
    #endif
}

// 设置颜色
void set_color(int value) {
    if (!config.use_colors) return;

    const char* colors[] = {
        "\033[0m",     // 0: default
        "\033[93m",    // 2: yellow
        "\033[92m",    // 4: green
        "\033[96m",    // 8: cyan
        "\033[94m",    // 16: blue
        "\033[95m",    // 32: magenta
        "\033[91m",    // 64: red
        "\033[97m",    // 128: white
        "\033[103m",   // 256: background yellow
        "\033[102m",   // 512: background green
        "\033[106m",   // 1024: background cyan
        "\033[104m",   // 2048: background blue
    };

    int index = 0;
    int temp = value;
    while (temp > 1 && index < 11) {
        temp /= 2;
        index++;
    }

    if (index < sizeof(colors)/sizeof(colors[0])) {
        printf("%s", colors[index]);
    }
}

// 初始化默认配置
void init_default_config() {
    config.use_colors = 1;
    config.animation_delay = 50;
    config.key_up = 'w';
    config.key_down = 's';
    config.key_left = 'a';
    config.key_right = 'd';
    config.key_quit = 'q';
}

// 加载配置
int load_config() {
    FILE* file = fopen(CONFIG_FILE, "r");
    if (!file) return 0;

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "use_colors=", 11) == 0) {
            config.use_colors = atoi(line + 11);
        } else if (strncmp(line, "animation_delay=", 16) == 0) {
            config.animation_delay = atoi(line + 16);
        } else if (strncmp(line, "key_up=", 7) == 0) {
            config.key_up = line[7];
        } else if (strncmp(line, "key_down=", 9) == 0) {
            config.key_down = line[9];
        } else if (strncmp(line, "key_left=", 9) == 0) {
            config.key_left = line[9];
        } else if (strncmp(line, "key_right=", 10) == 0) {
            config.key_right = line[10];
        } else if (strncmp(line, "key_quit=", 9) == 0) {
            config.key_quit = line[9];
        }
    }

    fclose(file);
    return 1;
}

// 保存配置
void save_config() {
    FILE* file = fopen(CONFIG_FILE, "w");
    if (!file) return;

    fprintf(file, "use_colors=%d\n", config.use_colors);
    fprintf(file, "animation_delay=%d\n", config.animation_delay);
    fprintf(file, "key_up=%c\n", config.key_up);
    fprintf(file, "key_down=%c\n", config.key_down);
    fprintf(file, "key_left=%c\n", config.key_left);
    fprintf(file, "key_right=%c\n", config.key_right);
    fprintf(file, "key_quit=%c\n", config.key_quit);

    fclose(file);
}

// 保存游戏状态
void save_game() {
    FILE* file = fopen(SAVE_FILE, "wb");
    if (!file) return;

    fwrite(&current_game, sizeof(GameState), 1, file);
    fclose(file);
}

// 加载游戏状态
int load_game() {
    FILE* file = fopen(SAVE_FILE, "rb");
    if (!file) return 0;

    fread(&current_game, sizeof(GameState), 1, file);
    fclose(file);
    return 1;
}

// 优化后的添加随机方块
void addRandomTile() {
    int empty_count = 0;
    int empty_positions[SIZE * SIZE][2];

    // 单次遍历统计空位置
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (current_game.board[i][j] == 0) {
                empty_positions[empty_count][0] = i;
                empty_positions[empty_count][1] = j;
                empty_count++;
            }
        }
    }

    if (empty_count > 0) {
        int index = rand() % empty_count;
        int value = (rand() % 10 < 9) ? 2 : 4;
        current_game.board[empty_positions[index][0]][empty_positions[index][1]] = value;
    }
}

// 初始化游戏
void initialize() {
    memset(&current_game, 0, sizeof(GameState));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            current_game.board[i][j] = 0;
        }
    }
    srand(time(0));
    addRandomTile();
    addRandomTile();
}

// 优化后的打印棋盘
void printBoard() {
    clear_screen();
    printf("2048 Game - Score: %d - Moves: %d\n\n", current_game.score, current_game.move_count);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (current_game.board[i][j] == 0) {
                printf("    .    ");
            } else {
                set_color(current_game.board[i][j]);
                printf("%5d    ", current_game.board[i][j]);
                set_color(0); // Reset color
            }
        }
        printf("\n\n");
    }
    printf("Controls: %c↑ %c↓ %c← %c→ | %c: Quit\n",
           config.key_up, config.key_down, config.key_left, config.key_right, config.key_quit);
}

// 优化移动算法 - 单方向移动合并
int moveDirection(int direction) {
    int moved = 0;

    if (direction == 0) { // Left
        for (int i = 0; i < SIZE; i++) {
            int write_pos = 0;
            int last_value = 0;

            for (int j = 0; j < SIZE; j++) {
                if (current_game.board[i][j] != 0) {
                    if (last_value == 0) {
                        last_value = current_game.board[i][j];
                    } else if (last_value == current_game.board[i][j]) {
                        current_game.board[i][write_pos++] = last_value * 2;
                        current_game.score += last_value * 2;
                        last_value = 0;
                        moved = 1;
                    } else {
                        current_game.board[i][write_pos++] = last_value;
                        last_value = current_game.board[i][j];
                    }
                }
            }

            if (last_value != 0) {
                current_game.board[i][write_pos++] = last_value;
            }

            while (write_pos < SIZE) {
                if (current_game.board[i][write_pos] != 0) moved = 1;
                current_game.board[i][write_pos++] = 0;
            }
        }
    }
    else if (direction == 1) { // Right
        for (int i = 0; i < SIZE; i++) {
            int write_pos = SIZE - 1;
            int last_value = 0;

            for (int j = SIZE - 1; j >= 0; j--) {
                if (current_game.board[i][j] != 0) {
                    if (last_value == 0) {
                        last_value = current_game.board[i][j];
                    } else if (last_value == current_game.board[i][j]) {
                        current_game.board[i][write_pos--] = last_value * 2;
                        current_game.score += last_value * 2;
                        last_value = 0;
                        moved = 1;
                    } else {
                        current_game.board[i][write_pos--] = last_value;
                        last_value = current_game.board[i][j];
                    }
                }
            }

            if (last_value != 0) {
                current_game.board[i][write_pos--] = last_value;
            }

            while (write_pos >= 0) {
                if (current_game.board[i][write_pos] != 0) moved = 1;
                current_game.board[i][write_pos--] = 0;
            }
        }
    }
    else if (direction == 2) { // Up
        for (int j = 0; j < SIZE; j++) {
            int write_pos = 0;
            int last_value = 0;

            for (int i = 0; i < SIZE; i++) {
                if (current_game.board[i][j] != 0) {
                    if (last_value == 0) {
                        last_value = current_game.board[i][j];
                    } else if (last_value == current_game.board[i][j]) {
                        current_game.board[write_pos++][j] = last_value * 2;
                        current_game.score += last_value * 2;
                        last_value = 0;
                        moved = 1;
                    } else {
                        current_game.board[write_pos++][j] = last_value;
                        last_value = current_game.board[i][j];
                    }
                }
            }

            if (last_value != 0) {
                current_game.board[write_pos++][j] = last_value;
            }

            while (write_pos < SIZE) {
                if (current_game.board[write_pos][j] != 0) moved = 1;
                current_game.board[write_pos++][j] = 0;
            }
        }
    }
    else if (direction == 3) { // Down
        for (int j = 0; j < SIZE; j++) {
            int write_pos = SIZE - 1;
            int last_value = 0;

            for (int i = SIZE - 1; i >= 0; i--) {
                if (current_game.board[i][j] != 0) {
                    if (last_value == 0) {
                        last_value = current_game.board[i][j];
                    } else if (last_value == current_game.board[i][j]) {
                        current_game.board[write_pos--][j] = last_value * 2;
                        current_game.score += last_value * 2;
                        last_value = 0;
                        moved = 1;
                    } else {
                        current_game.board[write_pos--][j] = last_value;
                        last_value = current_game.board[i][j];
                    }
                }
            }

            if (last_value != 0) {
                current_game.board[write_pos--][j] = last_value;
            }

            while (write_pos >= 0) {
                if (current_game.board[write_pos][j] != 0) moved = 1;
                current_game.board[write_pos--][j] = 0;
            }
        }
    }

    return moved;
}

// 检查游戏结束
int isGameOver() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (current_game.board[i][j] == 0) return 0;
            if (i < SIZE - 1 && current_game.board[i][j] == current_game.board[i + 1][j]) return 0;
            if (j < SIZE - 1 && current_game.board[i][j] == current_game.board[i][j + 1]) return 0;
        }
    }
    return 1;
}

int main() {
    // 初始化配置
    init_default_config();
    load_config();

    printf("2048 Game - Linux Optimized Version\n");
    printf("Press L to load saved game, any other key to start new game: ");

    char choice = getch();
    if (choice == 'l' || choice == 'L') {
        if (!load_game()) {
            printf("\nNo saved game found. Starting new game.\n");
            #ifdef _WIN32
            Sleep(1000);
        #else
            sleep(1);
        #endif
            initialize();
        }
    } else {
        initialize();
    }

    while (1) {
        printBoard();

        if (isGameOver()) {
            printf("\nGame Over! Final Score: %d\n", current_game.score);
            remove(SAVE_FILE); // 删除存档文件
            break;
        }

        char input = getch();
        int direction = -1;

        if (input == config.key_up || input == config.key_up - 32) {
            direction = 2;
        } else if (input == config.key_down || input == config.key_down - 32) {
            direction = 3;
        } else if (input == config.key_left || input == config.key_left - 32) {
            direction = 0;
        } else if (input == config.key_right || input == config.key_right - 32) {
            direction = 1;
        } else if (input == config.key_quit || input == config.key_quit - 32) {
            printf("\nSave game before quitting? (y/n): ");
            char save_choice = getch();
            if (save_choice == 'y' || save_choice == 'Y') {
                save_game();
                printf("Game saved.\n");
            }
            break;
        } else if (input == 'r' || input == 'R') {
            printf("\nRestarting game...\n");
            #ifdef _WIN32
                Sleep(1000);
            #else
                sleep(1);
            #endif
            initialize();
            continue;
        }

        if (direction != -1) {
            if (moveDirection(direction)) {
                current_game.move_count++;
                addRandomTile();
                save_game(); // 自动保存游戏状态
            }
        }
    }

    return 0;
}
