#include <stdio.h>
int main(int argc, char const *argv[])
{
  int x=0;
  int sum =0;
  printf("输入数据：\n");
  scanf("%d",&x);
  do
  {
    sum+=x;
    x--;
  } while (x>0);
  printf("%d\n",sum);
  return 0;
}
