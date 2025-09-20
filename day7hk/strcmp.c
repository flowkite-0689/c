#include <stdio.h>

int strcmp(char *str1, char *str2)
{
  int i;
  for (i = 0; str1[i] == str2[i]; i++)
  {
    if (str1[i] == '\0')
    {
      return 0;
    }

    /* code */
  }
  return str1[i] - str2[i];
}

int main(int argc, char const *argv[])
{
  char str[] = "AB";
  char sss[] = "AA";

  int a = strcmp(str, sss);
  printf("%d\n", a);

  return 0;
}
