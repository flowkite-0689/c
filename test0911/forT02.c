#include <stdio.h>
int main()
{
  int x = 0;
  int sum = 0;
  printf("输入大于0的整型数\n");
  scanf("%d", &x);
  for (x; x > 0; x--)
  {
    sum += x;
  }
  printf(" sum=%d\n", sum);
  return 0;
}