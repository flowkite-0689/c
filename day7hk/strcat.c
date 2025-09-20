#include <stdio.h>

char *strcat(char *dest, char *str);

int main(int argc, char const *argv[])
{
  char s[50] = "hey,";
  char s2[50] = "hello";
  strcat(s, s2);
  puts(s);

  return 0;
}
char *strcat(char *dest, char *str)
{
  char *dest_p = dest;
  while (*dest)
  {
    dest++;
  };
  while (*str)
  {
    *dest++ = *str++;
  }
  *dest = '\0';
  return dest_p;
}
