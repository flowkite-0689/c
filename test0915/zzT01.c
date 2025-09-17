#include <stdio.h>

int main(int argc, char const *argv[])
{
  int a = 10;
  int *p = &a;

  printf("p=%d\n", p);
  printf("*p=%d", *p);
  return 0;
}
