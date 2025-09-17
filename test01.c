#include <stdio.h>
#include <string.h>

int BToD(const char *binary)
{
  int decimal = 0;
  int length = strlen(binary);
  for (int i = 0; i < length; i++)
  {
    if (binary[i] != 1 || binary[i] != 0)
    {
      printf("并非二进制数");
      return -1;
    }
    decimal =decimal*2+(binary[i] - '0');
  }
  return decimal;
}
int main()
{
  char binary[32];
  printf("请输入二进制数：");
  scanf("%s",binary);
  
}
