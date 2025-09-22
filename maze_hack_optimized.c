/**
 * @file maze_hack_optimized.c
 * @brief 迷宫骇客游戏 - 优化版本
 * @version 2.0
 * @date 2025
 *
 * 优化特性：
 * - 改进的迷宫生成算法
 * - 更好的敌人AI
 * - 性能优化
 * - 游戏平衡性调整
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#ifdef _WIN32
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)
#else
#include <unistd.h>
#define sleep_ms(ms) usleep(ms * 1000)
#endif

#define WIDTH 40
#define HEIGHT 20
#define MAX_ENEMIES 8
#define MAX_ITEMS 12

#define WALL '#'
#define FLOOR '.'
#define PLAYER '@'
#define ENEMY 'E'
#define TREASURE '$'
#define POTION 'P'
#define EXIT '>'
#define DOOR '+'

typedef enum { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } Direction;

typedef struct {
    int x, y;
    short health, attack, defense;
} Entity;

typedef struct {
    char map[HEIGHT][WIDTH];
    Entity player;
    Entity enemies[MAX_ENEMIES];
    int enemy_count;
    int items[MAX_ITEMS][2];
    int item_count;
    int level;
    int score;
    int turns;
} GameState;

GameState game;

void initialize_game();
void generate_maze();
void place_entities();
void draw_map();
void move_player(Direction dir);
void move_enemies();
void check_collisions();
void game_loop();
void clear_screen();
void show_game_over();
void show_level_complete();

void initialize_game() {
    srand(time(NULL));

    game.player.x = 1;
    game.player.y = 1;
    game.player.health = 100 + (game.level * 10);
    game.player.attack = 10 + (game.level * 2);
    game.player.defense = 5 + game.level;

    game.enemy_count = 5 + (game.level / 2);
    if (game.enemy_count > MAX_ENEMIES) game.enemy_count = MAX_ENEMIES;

    game.item_count = 8 + (game.level / 3);
    if (game.item_count > MAX_ITEMS) game.item_count = MAX_ITEMS;

    generate_maze();
    place_entities();
}

void generate_maze() {
    // 初始化迷宫
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            game.map[y][x] = (x == 0 || x == WIDTH-1 || y == 0 || y == HEIGHT-1) ? WALL : FLOOR;
        }
    }

    // 生成更好的迷宫结构
    for (int y = 2; y < HEIGHT-2; y += 2) {
        for (int x = 2; x < WIDTH-2; x += 2) {
            if (rand() % 3 > 0) {
                game.map[y][x] = WALL;
                if (x+1 < WIDTH-1) game.map[y][x+1] = WALL;
                if (y+1 < HEIGHT-1) game.map[y+1][x] = WALL;
            }
        }
    }

    game.map[1][1] = FLOOR;
    game.map[HEIGHT-2][WIDTH-2] = EXIT;
}

void place_entities() {
    // 放置敌人
    for (int i = 0; i < game.enemy_count; i++) {
        int x, y, attempts = 0;
        do {
            x = 3 + rand() % (WIDTH - 6);
            y = 3 + rand() % (HEIGHT - 6);
            attempts++;
        } while (game.map[y][x] != FLOOR && attempts < 30);

        if (attempts < 30) {
            game.enemies[i].x = x;
            game.enemies[i].y = y;
            game.enemies[i].health = 25 + (game.level * 5);
            game.enemies[i].attack = 6 + (game.level * 2);
            game.enemies[i].defense = 3 + game.level;
        }
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

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void draw_map() {
    clear_screen();
    printf("=== 迷宫骇客游戏 v2.0 ===\n");
    printf("等级: %d | 分数: %d | 生命: %d\n\n", game.level, game.score, game.player.health);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            char c = game.map[y][x];
            if (x == game.player.x && y == game.player.y) c = PLAYER;
            else {
                for (int i = 0; i < game.enemy_count; i++) {
                    if (game.enemies[i].health > 0 && game.enemies[i].x == x && game.enemies[i].y == y) {
                        c = ENEMY;
                        break;
                    }
                }
                if (c == FLOOR) {
                    for (int i = 0; i < game.item_count; i++) {
                        if (game.items[i][0] == x && game.items[i][1] == y) {
                            c = (rand() % 2) ? TREASURE : POTION;
                            break;
                        }
                    }
                }
            }
            printf("%c", c);
        }
        printf("\n");
    }
    printf("\n控制: WASD移动 | Q退出 | R重新开始\n");
}

void move_player(Direction dir) {
    int nx = game.player.x, ny = game.player.y;
    switch (dir) {
        case DIR_UP: ny--; break;
        case DIR_DOWN: ny++; break;
        case DIR_LEFT: nx--; break;
        case DIR_RIGHT: nx++; break;
    }
    if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && game.map[ny][nx] != WALL) {
        game.player.x = nx;
        game.player.y = ny;
        game.turns++;
    }
}

void move_enemies() {
    for (int i = 0; i < game.enemy_count; i++) {
        if (game.enemies[i].health <= 0) continue;

        int dx = 0, dy = 0;
        int dist = abs(game.enemies[i].x - game.player.x) + abs(game.enemies[i].y - game.player.y);

        if (dist < 10) {
            if (game.enemies[i].x < game.player.x) dx = 1;
            else if (game.enemies[i].x > game.player.x) dx = -1;
            if (game.enemies[i].y < game.player.y) dy = 1;
            else if (game.enemies[i].y > game.player.y) dy = -1;
        } else {
            dx = (rand() % 3) - 1;
            dy = (rand() % 3) - 1;
        }

        int nx = game.enemies[i].x + dx, ny = game.enemies[i].y + dy;
        if (nx > 0 && nx < WIDTH-1 && ny > 0 && ny < HEIGHT-1 && game.map[ny][nx] != WALL) {
            game.enemies[i].x = nx;
            game.enemies[i].y = ny;
        }
    }
}

void check_collisions() {
    // 敌人碰撞
    for (int i = 0; i < game.enemy_count; i++) {
        if (game.enemies[i].health > 0 && game.enemies[i].x == game.player.x && game.enemies[i].y == game.player.y) {
            int damage = game.player.attack - game.enemies[i].defense;
            if (damage < 1) damage = 1;
            game.enemies[i].health -= damage;

            if (game.enemies[i].health <= 0) {
                printf("\n击败敌人! +20分\n");
                game.score += 20;
            } else {
                int edamage = game.enemies[i].attack - game.player.defense;
                if (edamage < 1) edamage = 1;
                game.player.health -= edamage;
                printf("\n战斗! 你受到%d伤害\n", edamage);
            }
            sleep_ms(1000);
        }
    }

    // 物品碰撞
    for (int i = 0; i < game.item_count; i++) {
        if (game.items[i][0] == game.player.x && game.items[i][1] == game.player.y) {
            if (rand() % 2) {
                printf("\n获得宝藏! +15分\n");
                game.score += 15;
            } else {
                printf("\n获得药水! +25生命\n");
                game.player.health += 25;
            }
            game.items[i][0] = -1;
            sleep_ms(800);
        }
    }

    // 出口检查
    if (game.map[game.player.y][game.player.x] == EXIT) {
        printf("\n通关第%d层! +60分\n", game.level);
        game.score += 60;
        game.level++;
        initialize_game();
    }
}

void game_loop() {
    char input;
    while (1) {
        draw_map();
        if (game.player.health <= 0) {
            printf("\n游戏结束! 最终分数: %d\n", game.score);
            break;
        }

        input = getch();
        switch (input) {
            case 'w': case 'W': move_player(DIR_UP); break;
            case 's': case 'S': move_player(DIR_DOWN); break;
            case 'a': case 'A': move_player(DIR_LEFT); break;
            case 'd': case 'D': move_player(DIR_RIGHT); break;
            case 'q': case 'Q': return;
            case 'r': case 'R': game.level = 1; game.score = 0; initialize_game(); continue;
            default: continue;
        }

        move_enemies();
        check_collisions();
    }
}

int main() {
    printf("迷宫骇客游戏 - 优化版\n");
    printf("按任意键开始...\n");
    getch();

    game.level = 1;
    game.score = 0;
    initialize_game();
    game_loop();

    printf("感谢游玩!\n");
    return 0;
}
