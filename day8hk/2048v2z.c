/**
 * @file 2048v2.c
 * @brief 2048游戏实现
 * @version 2.0
 * @date 2024
 *
 * 实现经典的2048游戏，支持上下左右移动操作
 * 使用WSAD键控制方向，Q键退出游戏
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define SIZE 4 // 游戏棋盘大小

int board[SIZE][SIZE]; // 游戏棋盘
int score = 0;         // 游戏分数

// 函数声明
void addNum();           // 在空白位置添加数字(2或4)
void printBoard();       // 打印游戏界面
void init();             // 初始化游戏棋盘
int move(int direction); // 移动方块并合并相同数字
int isGameover();        // 判断游戏是否结束

/**
 * @brief 主函数 - 游戏主循环
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return int 程序退出状态
 */
int main(int argc, char const *argv[])
{
  srand(time(NULL)); // 初始化随机数种子
  init();            // 初始化游戏

  for (;;) // 游戏主循环
  {
    system("cls||clear"); // 清屏
    printf("分数：%d\nwsad(上下左右)，q(退出)\n", score);
    printBoard(); // 打印当前棋盘

    int fx = -1;          // 移动方向
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
    else
    {
      printf("退出游戏~");
      break; // 退出游戏
    }

    // 执行移动操作
    if (move(fx))
    {
      addNum(); // 如果发生了移动，添加新数字
    }

    // 检查游戏是否结束
    if (isGameover())
    {
      printf("游戏结束！！");
      break;
    }
  }
  return 0;
}

/**
 * @brief 在空白位置随机添加数字2或4
 * 80%概率添加2，20%概率添加4
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

  // 如果有空白位置，随机选择一个添加数字
  if (count)
  {
    int index = rand() % count;          // 随机选择空白位置
    int num = (rand() % 10 < 8) ? 2 : 4; // 80%概率为2，20%概率为4
    board[empty[index][0]][empty[index][1]] = num;
  }
}

/**
 * @brief 初始化游戏棋盘
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
 * @brief 打印游戏棋盘
 * 数字0显示为点号"."，其他数字正常显示
 */
void printBoard()
{
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      if (board[i][j] == 0)
      {
        printf("\t.\t"); // 空白位置显示点号
      }
      else
      {
        printf("\t%d\t", board[i][j]); // 显示数字
      }
      printf(" ");
    }
    printf("\n\n"); // 行间隔
  }
}

/**
 * @brief 移动方块并合并相同数字
 * @param direction 移动方向: 0-左, 1-右, 2-上, 3-下
 * @return int 是否发生了移动(1-是, 0-否)
 */
int move(int direction)
{
  int he[SIZE * SIZE / 2][2]; // 记录已合并的位置，防止重复合并
  int count = 0;              // 已合并位置计数
  int moved = 0;              // 移动标志

  /*
  方向说明:
  0 - 左移
  1 - 右移
  2 - 上移
  3 - 下移
  */

  if (direction == 0)
  { // 左移
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
            // 检查该位置是否已经合并过
            for (int z = 0; z < count; z++)
            {
              if (he[z][0] == i && he[z][1] == (k - 1))
              {
                flag = 1; // 已合并过，跳过
              }
            }
            if (!flag)
            { // 可以合并
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
  else if (direction == 1)
  { // 右移
    for (int i = 0; i < SIZE; i++)
    {
      for (int j = SIZE - 2; j >= 0; j--)
      {
        if (board[i][j] != 0)
        {
          int k = j;
          // 向右移动
          while (k < SIZE - 1 && board[i][k + 1] == 0)
          {
            board[i][k + 1] = board[i][k];
            board[i][k] = 0;
            k++;
            moved = 1;
          }
          // 检查合并
          if (k < SIZE - 1 && board[i][k + 1] == board[i][k])
          {
            int flag = 0;
            for (int z = 0; z < count; z++)
            {
              if (he[z][0] == i && he[z][1] == (k + 1))
              {
                flag = 1;
              }
            }
            if (!flag)
            {
              board[i][k + 1] *= 2;
              score += board[i][k + 1];
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
  else if (direction == 2)
  { // 上移
    for (int j = 0; j < SIZE; j++)
    {
      for (int i = 1; i < SIZE; i++)
      {
        if (board[i][j] != 0)
        {
          int k = i;
          // 向上移动
          while (k > 0 && board[k - 1][j] == 0)
          {
            board[k - 1][j] = board[k][j];
            board[k][j] = 0;
            k--;
            moved = 1;
          }
          // 检查合并
          if (k > 0 && board[k - 1][j] == board[k][j])
          {
            int flag = 0;
            for (int z = 0; z < count; z++)
            {
              if (he[z][0] == (k - 1) && he[z][1] == j)
              {
                flag = 1;
              }
            }
            if (!flag)
            {
              board[k - 1][j] *= 2;
              score += board[k - 1][j];
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
  else if (direction == 3)
  { // 下移
    for (int j = 0; j < SIZE; j++)
    {
      for (int i = SIZE - 2; i >= 0; i--)
      {
        if (board[i][j] != 0)
        {
          int k = i;
          // 向下移动
          while (k < SIZE - 1 && board[k + 1][j] == 0)
          {
            board[k + 1][j] = board[k][j];
            board[k][j] = 0;
            k++;
            moved = 1;
          }
          // 检查合并
          if (k < SIZE - 1 && board[k + 1][j] == board[k][j])
          {
            int flag = 0;
            for (int z = 0; z < count; z++)
            {
              if (he[z][0] == (k + 1) && he[z][1] == j)
              {
                flag = 1;
              }
            }
            if (!flag)
            {
              board[k + 1][j] *= 2;
              score += board[k + 1][j];
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
  return moved; // 返回是否发生了移动
}

/**
 * @brief 判断游戏是否结束
 * @return int 1-游戏结束, 0-游戏继续
 * 游戏结束条件: 棋盘已满且没有可以合并的相邻方块
 */
int isGameover()
{
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

  // 检查垂直方向是否有可以合并的方块
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

  // 检查水平方向是否有可以合并的方块
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

  return 1; // 游戏结束
}
