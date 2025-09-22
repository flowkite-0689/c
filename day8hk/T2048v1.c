/**
 * @file T2048v1.c
 * @brief 2048游戏实现
 * @version 1.0
 * @date 2025
 *
 * 使用WASD控制方向，Q退出游戏
 * 合并相同数字的方块获得分数
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define SIZE 4  ///< 游戏棋盘大小

int board[SIZE][SIZE];  ///< 游戏棋盘
int score = 0;           ///< 游戏得分

/**
 * @brief 在随机空位置添加一个方块
 */
void addRandomTile();

/**
 * @brief 初始化游戏棋盘
 *
 * 清空棋盘并添加两个初始方块
 */
void initialize() {
    // 清空棋盘
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }
    srand(time(0));  // 初始化随机数种子
    // 添加两个初始方块
    addRandomTile();
    addRandomTile();
}

void addRandomTile() {
    int emptyCells[SIZE * SIZE][2];  // 存储空位置的坐标
    int count = 0;

    // 统计所有空位置
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                emptyCells[count][0] = i;
                emptyCells[count][1] = j;
                count++;
            }
        }
    }

    // 如果有空位置，随机选择一个添加方块
    if (count > 0) {
        int index = rand() % count;
        int value = (rand() % 10 < 9) ? 2 : 4;  // 90%概率生成2，10%概率生成4
        board[emptyCells[index][0]][emptyCells[index][1]] = value;
    }
}

/**
 * @brief 打印游戏棋盘
 *
 * 清屏后显示当前得分和棋盘状态
 */
void printBoard() {
    system("cls");  // 清屏
    printf("2048 Game - Score: %d\n\n", score);

    // 打印棋盘
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                printf(".\t");  // 空位置显示点号
            } else {
                printf("%d\t", board[i][j]);  // 有数字的位置显示数字
            }
        }
        printf("\n\n");  // 行间空行
    }
}

/**
 * @brief 移动方块
 * @param direction 移动方向：0-左，1-右，2-上，3-下
 * @return int 是否发生了移动（1-是，0-否）
 */
int moveTiles(int direction) {
    int moved = 0;  // 移动标志

    // 向左移动
    if (direction == 0) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 1; j < SIZE; j++) {
                if (board[i][j] != 0) {
                    int k = j;
                    // 向左移动直到遇到边界或其他方块
                    while (k > 0 && board[i][k - 1] == 0) {
                        board[i][k - 1] = board[i][k];
                        board[i][k] = 0;
                        k--;
                        moved = 1;
                    }
                    // 检查是否可以合并
                    if (k > 0 && board[i][k - 1] == board[i][k]) {
                        board[i][k - 1] *= 2;
                        score += board[i][k - 1];  // 增加得分
                        board[i][k] = 0;
                        moved = 1;
                    }
                }
            }
        }
    }
    // 向右移动
    else if (direction == 1) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = SIZE - 2; j >= 0; j--) {
                if (board[i][j] != 0) {
                    int k = j;
                    // 向右移动直到遇到边界或其他方块
                    while (k < SIZE - 1 && board[i][k + 1] == 0) {
                        board[i][k + 1] = board[i][k];
                        board[i][k] = 0;
                        k++;
                        moved = 1;
                    }
                    // 检查是否可以合并
                    if (k < SIZE - 1 && board[i][k + 1] == board[i][k]) {
                        board[i][k + 1] *= 2;
                        score += board[i][k + 1];  // 增加得分
                        board[i][k] = 0;
                        moved = 1;
                    }
                }
            }
        }
    }
    // 向上移动
    else if (direction == 2) {
        for (int j = 0; j < SIZE; j++) {
            for (int i = 1; i < SIZE; i++) {
                if (board[i][j] != 0) {
                    int k = i;
                    // 向上移动直到遇到边界或其他方块
                    while (k > 0 && board[k - 1][j] == 0) {
                        board[k - 1][j] = board[k][j];
                        board[k][j] = 0;
                        k--;
                        moved = 1;
                    }
                    // 检查是否可以合并
                    if (k > 0 && board[k - 1][j] == board[k][j]) {
                        board[k - 1][j] *= 2;
                        score += board[k - 1][j];  // 增加得分
                        board[k][j] = 0;
                        moved = 1;
                    }
                }
            }
        }
    }
    // 向下移动
    else if (direction == 3) {
        for (int j = 0; j < SIZE; j++) {
            for (int i = SIZE - 2; i >= 0; i--) {
                if (board[i][j] != 0) {
                    int k = i;
                    // 向下移动直到遇到边界或其他方块
                    while (k < SIZE - 1 && board[k + 1][j] == 0) {
                        board[k + 1][j] = board[k][j];
                        board[k][j] = 0;
                        k++;
                        moved = 1;
                    }
                    // 检查是否可以合并
                    if (k < SIZE - 1 && board[k + 1][j] == board[k][j]) {
                        board[k + 1][j] *= 2;
                        score += board[k + 1][j];  // 增加得分
                        board[k][j] = 0;
                        moved = 1;
                    }
                }
            }
        }
    }
    return moved;
}

/**
 * @brief 检查游戏是否结束
 * @return int 1-游戏结束，0-游戏继续
 */
int isGameOver() {
    // 检查是否有空位置
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                return 0;  // 有空位置，游戏继续
            }
            // 检查水平方向是否可以合并
            if (i < SIZE - 1 && board[i][j] == board[i + 1][j]) {
                return 0;  // 可以合并，游戏继续
            }
            // 检查垂直方向是否可以合并
            if (j < SIZE - 1 && board[i][j] == board[i][j + 1]) {
                return 0;  // 可以合并，游戏继续
            }
        }
    }
    return 1;  // 没有空位置且无法合并，游戏结束
}

/**
 * @brief 主函数
 * @return int 程序退出码
 */
int main() {
    initialize();  // 初始化游戏

    // 游戏主循环
    while (1) {
        printBoard();  // 显示棋盘

        // 检查游戏是否结束
        if (isGameOver()) {
            printf("Game Over! Final Score: %d\n", score);
            break;
        }

        // 获取用户输入
        char input = getch();
        int direction = -1;

        // 处理方向输入
        if (input == 'a' || input == 'A') {
            direction = 0; // 向左移动
        } else if (input == 'd' || input == 'D') {
            direction = 1; // 向右移动
        } else if (input == 'w' || input == 'W') {
            direction = 2; // 向上移动
        } else if (input == 's' || input == 'S') {
            direction = 3; // 向下移动
        } else if (input == 'q' || input == 'Q') {
            break;  // 退出游戏
        }

        // 执行移动操作
        if (direction != -1) {
            if (moveTiles(direction)) {
                addRandomTile();  // 移动成功，添加新方块
            }
        }
    }

    return 0;
}
