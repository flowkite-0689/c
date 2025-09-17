#include <stdio.h>

int main(int a,char *b[])
{

  printf("%d\n",a);
  for (int i = 0; i < a; i++)
  {
    printf("%s\n ",b[i]);
  }
  return 0;
}