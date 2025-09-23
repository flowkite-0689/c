#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define SIZE 4

// 修正颜色定义格式
#define COLOR_RESET "\033[0m"
#define COLOR_2 "\033[38;5;226m"
#define COLOR_4 "\033[38;5;45m"
#define COLOR_8 "\033[38;5;220m"
#define COLOR_16 "\033[1;38;5;33m"
#define COLOR_32 "\033[1;38;5;214m"
#define COLOR_64 "\033[1;38;5;21m"
#define COLOR_128 "\033[1;38;5;208m"
#define COLOR_256 "\033[1;4;38;5;17m"
#define COLOR_512 "\033[1;4;38;5;202m"
#define COLOR_1024 "\033[1;4;38;5;93m"
#define COLOR_2048 "\033[1;4;5;38;5;196m"

int board[SIZE][SIZE];
int score = 0;

// 函数
void addNum();     // 添加数字
void printBoard(); // 打印界面
void init();       // 初始化
int move(int);     // 移动
int isGameover();  // 游戏结束判定

int main(int argc, char const *argv[])
{

  init();
  // printBoard();
  for (;;)
  {
    system("cls||clear");
    printf("合成%s\t2048\t%s以获取胜利\n", COLOR_2048, COLOR_RESET);
    printf("\t分数：%d\n\twsad(上下左右)，q(退出)\n", score);
    printBoard();
    int fx = -1;
    char input = getch();
    if (input == 'a' || input == 'A')
    {
      fx = 0;
    }
    else if (input == 'd' || input == 'D')
    {
      fx = 1;
    }
    else if (input == 'w' || input == 'W')
    {
      fx = 2;
    }
    else if (input == 's' || input == 'S')
    {
      fx = 3;
    }
    else
    {
      printf("退出游戏~\n```");
      break;
    }
    if (move(fx))
    {
      addNum();
    }
    if (isGameover())
    {
      printf("游戏结束！！\n```");
      break;
    }
  }
  return 0;
}
void addNum()
{
  int empty[SIZE * SIZE][2];
  int count = 0;
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
  if (count)
  {
    int index = rand() % count;
    int num = (rand() % 10 < 9) ? 2 : 4;
    board[empty[index][0]][empty[index][1]] = num;
  }
}

void init()
{
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      board[i][j] = 0;
    }
  }
  addNum();
  addNum();
}

void printBoard()
{
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      if (board[i][j] == 0)
      {
        printf("\t.\t");
      }
      else
      {
        // 根据数字值设置颜色
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
          printf("\t%d\t", board[i][j]); // 默认颜色
        }
      }
      printf(" ");
    }
    printf("\n");
    printf("\n\n\n");
  }
}

int move(int direction)
{
  /*
  0 左
  1 右
  2 上
  3 下
  */

  int he[SIZE * SIZE / 2][2]; // 已合并的位置
  int count = 0;
  int moved = 0;
  if (direction == 0) // 左移
  {
    for (int i = 0; i < SIZE; i++)
    {
      for (int j = 1; j < SIZE; j++)
      {
        if (board[i][j] != 0)
        {
          int k = j;
          while (k > 0 && board[i][k - 1] == 0)
          {
            board[i][k - 1] = board[i][k];
            board[i][k] = 0;
            k--;
            moved = 1;
          }
          if (k > 0 && board[i][k - 1] == board[i][k])
          {
            int flag = 0;
            for (int z = 0; z < count; z++)
            {
              if (he[z][0] == i && he[z][1] == (k - 1))
              {
                flag = 1; // 判断已合并过
              }
            }
            if (!flag)
            {
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
  else if (direction == 1) // 右移
  {
    for (int i = 0; i < SIZE; i++)
    {
      for (int j = SIZE - 2; j >= 0; j--)
      {
        if (board[i][j] != 0)
        {
          int k = j;
          while (k < SIZE - 1 && board[i][k + 1] == 0)
          {
            board[i][k + 1] = board[i][k];
            board[i][k] = 0;
            k++;
            moved = 1;
          }
          if (k < SIZE - 1 && board[i][k + 1] == board[i][k])
          {
            int flag = 0;
            for (int z = 0; z < count; z++)
            {
              if (he[z][0] == i && he[z][1] == (k + 1))
              {
                flag = 1; // 判断已合并过
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
  else if (direction == 2) // 上移
  {
    for (int j = 0; j < SIZE; j++)
    {
      for (int i = 1; i < SIZE; i++)
      {
        if (board[i][j] != 0)
        {
          int k = i;
          while (k > 0 && board[k - 1][j] == 0)
          {
            board[k - 1][j] = board[k][j];
            board[k][j] = 0;
            k--;
            moved = 1;
          }
          if (k > 0 && board[k - 1][j] == board[k][j])
          {
            int flag = 0;
            for (int z = 0; z < count; z++)
            {
              if (he[z][0] == (k - 1) && he[z][1] == j)
              {
                flag = 1; // 判断已合并过
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
  else if (direction == 3) // 下移
  {
    for (int j = 0; j < SIZE; j++)
    {
      for (int i = SIZE - 2; i >= 0; i--)
      {
        if (board[i][j] != 0)
        {
          int k = i;
          while (k < SIZE - 1 && board[k + 1][j] == 0)
          {
            board[k + 1][j] = board[k][j];
            board[k][j] = 0;
            k++;
            moved = 1;
          }
          if (k < SIZE - 1 && board[k + 1][j] == board[k][j])
          {
            int flag = 0;
            for (int z = 0; z < count; z++)
            {
              if (he[z][0] == (k + 1) && he[z][1] == j)
              {
                flag = 1; // 判断已合并过
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
  return moved;
}

int isGameover()
{

  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      if (board[i][j] == 2048)
      {
        printf("合成了2048\nWe are the champion!!!\n");
        return 1;
      }
    }
  }
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {

      if (!board[i][j])
      {
        return 0;
      }
    }
  }
  for (int i = 0; i < SIZE - 1; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      if (board[i + 1][j] == board[i][j])
      {
        return 0;
      }
    }
  }

  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE - 1; j++)
    {
      if (board[i][j + 1] == board[i][j])
      {
        return 0;
      }
    }
  }

  return 1;
}
