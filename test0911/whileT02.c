#include <stdio.h>

int main(int argc, char *argv[])
{
  int x = 0;
  printf("输入数据：\n");
  scanf("%d", &x);
  int count = 0;
  while (x > 0)
  {
    count += x;
    x--;
  }
  printf("1+2+3+4+...+x 结果：%d\n", count);
  return 0;
}