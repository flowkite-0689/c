#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define SIZE 4

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
    printf("分数：%d\nwsad(上下左右)，q(退出)\n", score);
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
      printf("退出游戏~");
      break;
    }
    if (move(fx))
    {
      addNum();
     
    }
     if (isGameover())
      {
        printf("游戏结束！！");
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
    int num = (rand() % 10 < 8) ? 2 : 4;
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
        printf("\t%d\t", board[i][j]);
      }
      printf(" ");
    }
    printf("\n");
    printf("\n");
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
            board[i][k - 1] *= 2;
            score += board[i][k - 1];
            board[i][k] = 0;
            moved = 1;
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
          while (k < SIZE-1 && board[i][k + 1] == 0)
          {
            board[i][k + 1] = board[i][k];
            board[i][k] = 0;
            k++;
            moved = 1;
          }
          if (k < SIZE -1 && board[i][k + 1] == board[i][k])
          {
            board[i][k + 1] *= 2;
            score += board[i][k + 1];
            board[i][k] = 0;
            moved = 1;
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
            board[k - 1][j] *= 2;
            score += board[k - 1][j];
            board[k][j] = 0;
            moved = 1;
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
          while (k < SIZE-1 && board[k + 1][j] == 0)
          {
            board[k + 1][j] = board[k][j];
            board[k][j] = 0;
            k++;
            moved = 1;
          }
          if (k < SIZE-1 && board[k + 1][j] == board[k][j])
          {
            board[k + 1][j] *= 2;
            score += board[k + 1][j];
            board[k][j] = 0;
            moved = 1;
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