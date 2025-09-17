#include <stdio.h>
int main(int argc, char const *argv[])
{
  int num = 0;
  printf("请输入一个数字：\n");
  scanf("%d", &num);
  // if (num >= 0 && num <= 100)
  // {
  //   printf("samll");
  // }
  // if (num >= 101 && num <= 200)
  // {
  //   printf("middle");
  // }
  // if (num >= 201 && num <= 300)
  // {
  //   printf("large");
  // }
  // if (num < 0 || num > 300)
  // {
  //   printf("unknown");
  // }

  // 二
  // if (num >= 0 && num <= 100)
  // {
  //   printf("samll");
  // }
  // else
  // {
  //   if (num >= 101 && num <= 200)
  //   {
  //     printf("middle");
  //   }
  //   else
  //   {
  //     if (num >= 201 && num <= 300)
  //     {
  //       printf("large");
  //     }
  //     else
  //     {
  //       printf("unknown");
  //     }
  //   }
  // }

  // 三
  if (num >= 0 && num <= 100)
  {
    printf("samll");
  }
  else if (num >= 101 && num <= 200)
  {
    printf("middle");
  }
  else if (num >= 201 && num <= 300)
  {
    printf("large");
  }
  else
  {
    printf("unknown");
  }

  return 0;
}
