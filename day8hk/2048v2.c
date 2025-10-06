#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 平台检测
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#endif

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

typedef struct historyScore
{
  char name[5];
  int score;
} HS;

// 函数
void addNum();                    // 添加数字
void printBoard();                // 打印界面
void init();                      // 初始化
int move(int);                    // 移动
int isGameover();                 // 游戏结束判定
void saveToFile(HS *, int);       // 保存到文件
int loadFromFile(HS *hs);           // 从文件加载
void starGame(HS *, int);         // 开始游戏
void showHistoryScore(HS *, int); // 查看历史分数
int getch(void);                  // 跨平台getch实现


int main(int argc, char const *argv[])
{
  int count = 0;
  HS hs[100];
  count = loadFromFile(hs);
  srand(time(NULL)); // 添加随机种子
  int forflag0 = 1;
  for (; forflag0;)
  {
    count = loadFromFile(hs);
    system("cls||clear");
    printf("\t\t\t\t2048\t\t\t\n");
    printf("1.开始游戏\n");
    printf("2.查看历史记录\n");
    printf("3.退出游戏\n");
    printf("-----------------------\n");
    char c = getch();
    if (c == '1')
    {
      starGame(hs, count);
    }
    else if (c == '2')
    {
      showHistoryScore(hs, count);
    }
    else if (c == '3')
    {
      printf("退出游戏\n");
      forflag0 = 0;
      system("pause");
    }
    else
    {
      printf("无效输入");
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
  int moved = 0;//判断有无移动
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

void saveToFile(HS *hs, int count)
{
  FILE *file = fopen("2048v2.dat", "wb");
  if (file == NULL)
  {
    printf("无文件");
    return;
  }
  fwrite(&count, sizeof(int), 1, file);
  fwrite(hs, sizeof(HS), count, file);
  fclose(file);
  printf("文件已经保存");
}

int loadFromFile(HS *hs)
{
  int count = 0;
  FILE *file = fopen("2048v2.dat", "rb");
  if (file == NULL)
  {
    printf("无历史记录文件，将创建新文件\n");
    return 0;  // 返回0而不是-1，避免后续处理出错
  }

  // 检查文件读取是否成功
  if (fread(&count, sizeof(int), 1, file) != 1) {
    printf("读取记录数失败\n");
    fclose(file);
    return 0;
  }

  // 限制最大记录数防止溢出
  if (count > 100) count = 100;

  if (fread(hs, sizeof(HS), count, file) != count) {
    printf("读取记录内容失败\n");
    fclose(file);
    return 0;
  }

  fclose(file);
  printf("已加载%d条数据\n", count);
  return count;
}

void starGame(HS *hs, int count)
{
  init();
  // printBoard();
  int flagsx = 1;
  for (;;)
  {

    if (flagsx)
    {
      system("cls||clear");
      printf("合成%s\t2048\t%s以获取胜利\n", COLOR_2048, COLOR_RESET);
      printf("\t分数：%d\n\twsad(上下左右)，q(退出)\n\n\n", score);
      printBoard();
    }

    if (isGameover())
    {
      printf("游戏结束！！\n```");

      printf(" 是否保存记录(yn)\n");
      char c = getch();
      if (c == 'y')
      {
        printf("请输入记录名称(最多4个字符):\t");
        char buf[10];
        scanf("%4s", buf);
        strncpy(hs[count].name, buf, 4);
        hs[count].name[4] = '\0';
        hs[count].score = score;
        count++;
        saveToFile(hs, count);
      }

      break;
    }
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
    else if (input == 'q' || input == 'Q')
    {

      printf(" 是否保存记录(yn)\n");
      char c = getch();
      if (c == 'y')
      {
        printf("请输入记录名称(最多4个字符):\t");
        char buf[10];
        scanf("%4s", buf);
        strncpy(hs[count].name, buf, 4);
        hs[count].name[4] = '\0';
        hs[count].score = score;
        count++;
        saveToFile(hs, count);
      }
      printf("退出游戏~\n```");

      break;
    }
    else
    {
    }
    if (move(fx))
    {
      flagsx = 1;
      addNum();
    }
    else
    {
      flagsx = 0;
    }
  }

  score = 0;
}

void showHistoryScore(HS *hs, int count)
{
  // 按分数排序
  for (int i = 0; i < count-1; i++) {
    for (int j = 0; j < count-i-1; j++) {
      if (hs[j].score < hs[j+1].score) {
        HS temp = hs[j];
        hs[j] = hs[j+1];
        hs[j+1] = temp;
      }
    }
  }

  printf("\n历史记录(按分数排序)：\n");
  printf("排名\t名称\t分数\n");
  printf("----------------\n");
  for (int i = 0; i < count; i++) {
    printf("%d\t%s\t%d\n", i+1, hs[i].name, hs[i].score);
  }
  printf("\n按任意键退出");
  getch(); // 使用跨平台的getch代替system("pause")
}
// 跨平台getch实现
int getch(void)
{
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
