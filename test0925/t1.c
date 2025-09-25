#include <stdio.h>

typedef struct
{
  int a;
  char b;
} ab;

int main(int argc, char const *argv[])
{
  ab one={10};
  one.a = 1;
  one.b = 'A';
  printf("%d", one.a);
  printf("%c", one.b);

  return 0;
}
