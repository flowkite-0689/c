/**
 * @file maze_hack.c
 * @brief 迷宫骇客游戏 - 经典的roguelike风格迷宫冒险游戏
 * @version 1.0
 * @date 2025
 *
 * 功能特性：
 * - 随机迷宫生成
 * - 玩家角色移动和探索
 * - 敌人AI和战斗系统
 * - 物品收集和使用
 * - 关卡进度系统
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// 游戏常量定义
#define WIDTH 40
#define HEIGHT 20
#define MAX_ENEMIES 10
#define MAX_ITEMS 15

// 地图图例
#define WALL '#'
#define FLOOR '.'
#define PLAYER '@'
#define ENEMY 'E'
#define TREASURE '$'
#define POTION 'P'
#define EXIT '>'

// 方向枚举
typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

// 游戏实体结构体
typedef struct {
    int x;
    int y;
    int health;
    int attack;
    int defense;
} Entity;

// 游戏状态结构体
typedef struct {
    char map[HEIGHT][WIDTH];
    Entity player;
    Entity enemies[MAX_ENEMIES];
    int enemy_count;
    int items[MAX_ITEMS][2]; // [x,y]坐标
    int item_count;
    int level;
    int score;
    int turns;
} GameState;

GameState game;

// 函数声明
void initialize_game();
void generate_maze();
void place_entities();
void draw_map();
void move_player(Direction dir);
void move_enemies();
void check_collisions();
void game_loop();
void clear_screen();

/**
 * @brief 初始化游戏状态
 */
void initialize_game() {
    srand(time(NULL));

    game.player.x = 1;
    game.player.y = 1;
    game.player.health = 100;
    game.player.attack = 10;
    game.player.defense = 5;

    game.level = 1;
    game.score = 0;
    game.turns = 0;
    game.enemy_count = 5;
    game.item_count = 8;

    generate_maze();
    place_entities();
}

/**
 * @brief 生成随机迷宫
 */
void generate_maze() {
    // 初始化墙壁
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH-1 || y == 0 || y == HEIGHT-1) {
                game.map[y][x] = WALL;
            } else {
                game.map[y][x] = (rand() % 4 == 0) ? WALL : FLOOR;
            }
        }
    }

    // 确保玩家位置是地板
    game.map[1][1] = FLOOR;
    game.map[HEIGHT-2][WIDTH-2] = EXIT;
}

/**
 * @brief 放置实体（敌人、物品等）
 */
void place_entities() {
    // 放置敌人
    for (int i = 0; i < game.enemy_count; i++) {
        int x, y;
        do {
            x = 2 + rand() % (WIDTH - 4);
            y = 2 + rand() % (HEIGHT - 4);
        } while (game.map[y][x] != FLOOR ||
                (x == game.player.x && y == game.player.y));

        game.enemies[i].x = x;
        game.enemies[i].y = y;
        game.enemies[i].health = 20 + rand() % 30;
        game.enemies[i].attack = 5 + rand() % 10;
        game.enemies[i].defense = 2 + rand() % 5;
    }

    // 放置物品
    for (int i = 0; i < game.item_count; i++) {
        int x, y;
        do {
            x = 2 + rand() % (WIDTH - 4);
            y = 2 + rand() % (HEIGHT - 4);
        } while (game.map[y][x] != FLOOR);

        game.items[i][0] = x;
        game.items[i][1] = y;
    }
}

/**
 * @brief 清屏函数（跨平台）
 */
void clear_screen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

/**
 * @brief 绘制地图和游戏界面
 */
void draw_map() {
    clear_screen();

    printf("=== 迷宫骇客游戏 ===\n");
    printf("等级: %d | 分数: %d | 生命: %d | 回合: %d\n\n",
           game.level, game.score, game.player.health, game.turns);

    // 绘制地图
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            char display_char = game.map[y][x];

            // 检查玩家位置
            if (x == game.player.x && y == game.player.y) {
                display_char = PLAYER;
            }
            // 检查敌人位置
            else {
                for (int i = 0; i < game.enemy_count; i++) {
                    if (game.enemies[i].health > 0 &&
                        x == game.enemies[i].x && y == game.enemies[i].y) {
                        display_char = ENEMY;
                        break;
                    }
                }
            }
            // 检查物品位置
            for (int i = 0; i < game.item_count; i++) {
                if (game.items[i][0] != -1 &&
                    x == game.items[i][0] && y == game.items[i][1]) {
                    display_char = (rand() % 2 == 0) ? TREASURE : POTION;
                    break;
                }
            }

            printf("%c", display_char);
        }
        printf("\n");
    }

    printf("\n控制: WASD移动 | Q退出 | R重新开始\n");
    printf("图例: @玩家 E敌人 $宝藏 P药水 >出口\n");
}

/**
 * @brief 移动玩家
 * @param dir 移动方向
 */
void move_player(Direction dir) {
    int new_x = game.player.x;
    int new_y = game.player.y;

    switch (dir) {
        case DIR_UP:    new_y--; break;
        case DIR_DOWN:  new_y++; break;
        case DIR_LEFT:  new_x--; break;
        case DIR_RIGHT: new_x++; break;
    }

    // 检查移动是否有效
    if (new_x >= 0 && new_x < WIDTH && new_y >= 0 && new_y < HEIGHT) {
        if (game.map[new_y][new_x] != WALL) {
            game.player.x = new_x;
            game.player.y = new_y;
            game.turns++;
        }
    }
}

/**
 * @brief 敌人移动AI
 */
void move_enemies() {
    for (int i = 0; i < game.enemy_count; i++) {
        if (game.enemies[i].health <= 0) continue;

        // 简单AI：随机移动或向玩家移动
        int dx = 0, dy = 0;
        if (rand() % 3 > 0) { // 2/3概率向玩家移动
            if (game.enemies[i].x < game.player.x) dx = 1;
            else if (game.enemies[i].x > game.player.x) dx = -1;

            if (game.enemies[i].y < game.player.y) dy = 1;
            else if (game.enemies[i].y > game.player.y) dy = -1;
        } else { // 随机移动
            dx = (rand() % 3) - 1;
            dy = (rand() % 3) - 1;
        }

        int new_x = game.enemies[i].x + dx;
        int new_y = game.enemies[i].y + dy;

        if (new_x >= 1 && new_x < WIDTH-1 && new_y >= 1 && new_y < HEIGHT-1 &&
            game.map[new_y][new_x] != WALL) {
            game.enemies[i].x = new_x;
            game.enemies[i].y = new_y;
        }
    }
}

/**
 * @brief 检查碰撞（玩家与敌人、物品等）
 */
void check_collisions() {
    // 检查敌人碰撞
    for (int i = 0; i < game.enemy_count; i++) {
        if (game.enemies[i].health > 0 &&
            game.enemies[i].x == game.player.x &&
            game.enemies[i].y == game.player.y) {

            // 战斗逻辑
            int damage = game.player.attack - game.enemies[i].defense;
            if (damage < 1) damage = 1;

            game.enemies[i].health -= damage;
            printf("\n你对敌人造成了%d点伤害！", damage);

            if (game.enemies[i].health <= 0) {
                printf(" 敌人被击败！\n");
                game.score += 20;
            }

            #ifdef _WIN32
            Sleep(1000);
            #else
            sleep(1);
            #endif
        }
    }

    // 检查物品碰撞
    for (int i = 0; i < game.item_count; i++) {
        if (game.items[i][0] != -1 &&
            game.items[i][0] == game.player.x &&
            game.items[i][1] == game.player.y) {

            if (rand() % 2 == 0) {
                printf("\n找到了宝藏！+10分\n");
                game.score += 10;
            } else {
                printf("\n找到了治疗药水！+20生命\n");
                game.player.health += 20;
                if (game.player.health > 100) game.player.health = 100;
            }

            game.items[i][0] = -1; // 标记为已收集
            game.items[i][1] = -1;

            #ifdef _WIN32
            Sleep(1000);
            #else
            sleep(1);
            #endif
        }
    }

    // 检查出口
    if (game.map[game.player.y][game.player.x] == EXIT) {
        printf("\n恭喜！你完成了第%d层！\n", game.level);
        game.level++;
        game.score += 50;
        initialize_game();
    }
}

/**
 * @brief 游戏主循环
 */
void game_loop() {
    char input;
    int game_over = 0;

    while (!game_over) {
        draw_map();

        if (game.player.health <= 0) {
            printf("\n游戏结束！你的最终得分：%d\n", game.score);
            break;
        }

        input = getch();

        switch (input) {
            case 'w': case 'W': move_player(DIR_UP); break;
            case 's': case 'S': move_player(DIR_DOWN); break;
            case 'a': case 'A': move_player(DIR_LEFT); break;
            case 'd': case 'D': move_player(DIR_RIGHT); break;
            case 'q': case 'Q': game_over = 1; continue;
            case 'r': case 'R': initialize_game(); continue;
            default: continue;
        }

        move_enemies();
        check_collisions();
    }
}

/**
 * @brief 主函数
 */
int main() {
    printf("欢迎来到迷宫骇客游戏！\n");
    printf("探索迷宫，击败敌人，收集宝藏，找到出口！\n");
    printf("按任意键开始游戏...\n");
    getch();

    initialize_game();
    game_loop();

    printf("感谢游玩！\n");
    return 0;
}
