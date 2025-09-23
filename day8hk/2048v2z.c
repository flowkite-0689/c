#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define SIZE 4  // 游戏棋盘大小 4x4

// ANSI 颜色定义 - 用于控制台输出颜色
#define COLOR_RESET "\033[0m"          // 重置颜色
#define COLOR_2 "\033[38;5;226m"       // 数字2的颜色 (黄色)
#define COLOR_4 "\033[38;5;45m"        // 数字4的颜色 (蓝色)
#define COLOR_8 "\033[38;5;220m"       // 数字8的颜色 (金色)
#define COLOR_16 "\033[1;38;5;33m"     // 数字16的颜色 (亮蓝色)
#define COLOR_32 "\033[1;38;5;214m"    // 数字32的颜色 (橙色)
#define COLOR_64 "\033[1;38;5;21m"     // 数字64的颜色 (深蓝色)
#define COLOR_128 "\033[1;38;5;208m"   // 数字128的颜色 (亮橙色)
#define COLOR_256 "\033[1;4;38;5;17m"  // 数字256的颜色 (下划线深蓝色)
#define COLOR_512 "\033[1;4;38;5;202m" // 数字512的颜色 (下划线红色)
#define COLOR_1024 "\033[1;4;38;5;93m" // 数字1024的颜色 (下划线紫色)
#define COLOR_2048 "\033[1;4;5;38;5;196m" // 数字2048的颜色 (闪烁红色)

int board[SIZE][SIZE]; // 游戏棋盘
int score = 0;         // 游戏分数

// 函数声明
void addNum();     // 在空白位置添加数字(2或4)
void printBoard(); // 打印游戏界面
void init();       // 初始化游戏
int move(int);     // 处理移动逻辑
int isGameover();  // 判断游戏是否结束

/**
 * 主函数 - 游戏入口点
 */
int main(int argc, char const *argv[])
{
  srand(time(NULL)); // 初始化随机数种子

  init(); // 初始化游戏

  int flagsx = 1; // 控制是否需要刷新界面

  // 游戏主循环
  for (;;)
  {
    // 如果需要刷新界面
    if (flagsx)
    {
      system("cls||clear"); // 清屏
      printf("合成%s\t2048\t%s以获取胜利\n", COLOR_2048, COLOR_RESET);
      printf("\t分数：%d\n\twsad(上下左右)，q(退出)\n", score);
      printBoard(); // 打印棋盘
    }

    int fx = -1; // 移动方向
    char input = getch(); // 获取用户输入

    // 处理方向输入
    if (input == 'a' || input == 'A')
    {
      fx = 0; // 左
    }
    else if (input == 'd' || input == 'D')
    {
      fx = 1; // 右
    }
    else if (input == 'w' || input == 'W')
    {
      fx = 2; // 上
    }
    else if (input == 's' || input == 'S')
    {
      fx = 3; // 下
    }
    else if (input == 'q' || input == 'Q')
    {
      printf("退出游戏~\n");
      break; // 退出游戏
    }

    // 如果移动成功
    if (move(fx))
    {
      flagsx = 1; // 需要刷新界面
      addNum();   // 添加新数字
    }
    else
    {
      flagsx = 0; // 不需要刷新界面
    }

    // 检查游戏是否结束
    if (isGameover())
    {
      printf("游戏结束！！\n");
      break;
    }
  }

  return 0;
}

/**
 * 在空白位置随机添加数字(2或4)
 * 90%概率添加2，10%概率添加4
 */
void addNum()
{
  int empty[SIZE * SIZE][2]; // 存储空白位置坐标
  int count = 0;             // 空白位置计数

  // 遍历棋盘，记录所有空白位置
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      if (board[i][j] == 0)
      {
        empty[count][0] = i;
        empty[count][1] = j;
        count++;
      }
    }
  }

  // 如果有空白位置
  if (count)
  {
    int index = rand() % count; // 随机选择一个空白位置
    int num = (rand() % 10 < 9) ? 2 : 4; // 90%概率为2，10%概率为4
    board[empty[index][0]][empty[index][1]] = num; // 放置数字
  }
}

/**
 * 初始化游戏
 * 清空棋盘并添加两个初始数字
 */
void init()
{
  // 清空棋盘
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      board[i][j] = 0;
    }
  }

  // 添加两个初始数字
  addNum();
  addNum();
}

/**
 * 打印游戏界面
 * 使用不同颜色显示不同数字
 */
void printBoard()
{
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      if (board[i][j] == 0)
      {
        printf("\t.\t"); // 空白位置显示点
      }
      else
      {
        // 根据数字值设置不同的颜色
        switch (board[i][j])
        {
        case 2:
          printf("%s\t2\t%s", COLOR_2, COLOR_RESET);
          break;
        case 4:
          printf("%s\t4\t%s", COLOR_4, COLOR_RESET);
          break;
        case 8:
          printf("%s\t8\t%s", COLOR_8, COLOR_RESET);
          break;
        case 16:
          printf("%s\t16\t%s", COLOR_16, COLOR_RESET);
          break;
        case 32:
          printf("%s\t32\t%s", COLOR_32, COLOR_RESET);
          break;
        case 64:
          printf("%s\t64\t%s", COLOR_64, COLOR_RESET);
          break;
        case 128:
          printf("%s\t128\t%s", COLOR_128, COLOR_RESET);
          break;
        case 256:
          printf("%s\t256\t%s", COLOR_256, COLOR_RESET);
          break;
        case 512:
          printf("%s\t512\t%s", COLOR_512, COLOR_RESET);
          break;
        case 1024:
          printf("%s\t1024\t%s", COLOR_1024, COLOR_RESET);
          break;
        case 2048:
          printf("%s\t2048\t%s", COLOR_2048, COLOR_RESET);
          break;
        default:
          printf("\t%d\t", board[i][j]); // 默认颜色显示
        }
      }
      printf(" ");
    }
    printf("\n");
    printf("\n\n\n"); // 行间距
  }
}

/**
 * 处理移动逻辑
 * @param direction 移动方向: 0-左, 1-右, 2-上, 3-下
 * @return 1表示移动成功，0表示移动失败
 */
int move(int direction)
{
  int he[SIZE * SIZE / 2][2]; // 存储已合并的位置，防止重复合并
  int count = 0;               // 已合并位置计数
  int moved = 0;               // 是否发生移动的标志

  // 左移逻辑
  if (direction == 0)
  {
    for (int i = 0; i < SIZE; i++)
    {
      for (int j = 1; j < SIZE; j++)
      {
        if (board[i][j] != 0)
        {
          int k = j;
          // 向左移动直到遇到非空位置或边界
          while (k > 0 && board[i][k - 1] == 0)
          {
            board[i][k - 1] = board[i][k];
            board[i][k] = 0;
            k--;
            moved = 1;
          }
          // 检查是否可以合并
          if (k > 0 && board[i][k - 1] == board[i][k])
          {
            int flag = 0;
            // 检查是否已经合并过这个位置
            for (int z = 0; z < count; z++)
            {
              if (he[z][0] == i && he[z][1] == (k - 1))
              {
                flag = 1;
              }
            }
            // 如果没有合并过，则合并
            if (!flag)
            {
              board[i][k - 1] *= 2;
              he[count][0] = i;
              he[count][1] = (k - 1);
              count++;
              score += board[i][k - 1]; // 增加分数
              board[i][k] = 0;
              moved = 1;
            }
          }
        }
      }
    }
  }
  // 右移逻辑
  else if (direction == 1)
  {
    for (int i = 0; i < SIZE; i++)
    {
      for (int j = SIZE - 2; j >= 0; j--)
      {
        if (board[i][j] != 0)
        {
          int k = j;
          // 向右移动直到遇到非空位置或边界
          while (k < SIZE - 1 && board[i][k + 1] == 0)
          {
            board[i][k + 1] = board[i][k];
            board[i][k] = 0;
            k++;
            moved = 1;
          }
          // 检查是否可以合并
          if (k < SIZE - 1 && board[i][k + 1] == board[i][k])
          {
            int flag = 0;
            // 检查是否已经合并过这个位置
            for (int z = 0; z < count; z++)
            {
              if (he[z][0] == i && he[z][1] == (k + 1))
              {
                flag = 1;
              }
            }
            // 如果没有合并过，则合并
            if (!flag)
            {
              board[i][k + 1] *= 2;
              score += board[i][k + 1]; // 增加分数
              he[count][0] = i;
              he[count][1] = (k + 1);
              count++;
              board[i][k] = 0;
              moved = 1;
            }
          }
        }
      }
    }
  }
  // 上移逻辑
  else if (direction == 2)
  {
    for (int j = 0; j < SIZE; j++)
    {
      for (int i = 1; i < SIZE; i++)
      {
        if (board[i][j] != 0)
        {
          int k = i;
          // 向上移动直到遇到非空位置或边界
          while (k > 0 && board[k - 1][j] == 0)
          {
            board[k - 1][j] = board[k][j];
            board[k][j] = 0;
            k--;
            moved = 1;
          }
          // 检查是否可以合并
          if (k > 0 && board[k - 1][j] == board[k][j])
          {
            int flag = 0;
            // 检查是否已经合并过这个位置
            for (int z = 0; z < count; z++)
            {
              if (he[z][0] == (k - 1) && he[z][1] == j)
              {
                flag = 1;
              }
            }
            // 如果没有合并过，则合并
            if (!flag)
            {
              board[k - 1][j] *= 2;
              score += board[k - 1][j]; // 增加分数
              he[count][0] = (k - 1);
              he[count][1] = j;
              count++;
              board[k][j] = 0;
              moved = 1;
            }
          }
        }
      }
    }
  }
  // 下移逻辑
  else if (direction == 3)
  {
    for (int j = 0; j < SIZE; j++)
    {
      for (int i = SIZE - 2; i >= 0; i--)
      {
        if (board[i][j] != 0)
        {
          int k = i;
          // 向下移动直到遇到非空位置或边界
          while (k < SIZE - 1 && board[k + 1][j] == 0)
          {
            board[k + 1][j] = board[k][j];
            board[k][j] = 0;
            k++;
            moved = 1;
          }
          // 检查是否可以合并
          if (k < SIZE - 1 && board[k + 1][j] == board[k][j])
          {
            int flag = 0;
            // 检查是否已经合并过这个位置
            for (int z = 0; z < count; z++)
            {
              if (he[z][0] == (k + 1) && he[z][1] == j)
              {
                flag = 1;
              }
            }
            // 如果没有合并过，则合并
            if (!flag)
            {
              board[k + 1][j] *= 2;
              score += board[k + 1][j]; // 增加分数
              he[count][0] = (k + 1);
              he[count][1] = j;
              count++;
              board[k][j] = 0;
              moved = 1;
            }
          }
        }
      }
    }
  }

  return moved; // 返回是否发生移动
}

/**
 * 判断游戏是否结束
 * @return 1表示游戏结束，0表示游戏继续
 */
int isGameover()
{
  // 检查是否合成2048（胜利条件）
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      if (board[i][j] == 2048)
      {
        printf("合成了2048\nWe are the champion!!!\n");
        return 1; // 游戏胜利结束
      }
    }
  }

  // 检查是否有空白位置
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      if (!board[i][j])
      {
        return 0; // 有空白位置，游戏继续
      }
    }
  }

  // 检查是否可以水平合并
  for (int i = 0; i < SIZE - 1; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      if (board[i + 1][j] == board[i][j])
      {
        return 0; // 可以合并，游戏继续
      }
    }
  }

  // 检查是否可以垂直合并
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE - 1; j++)
    {
      if (board[i][j + 1] == board[i][j])
      {
        return 0; // 可以合并，游戏继续
      }
    }
  }

  return 1; // 无法移动，游戏失败结束
}
