#include <stdio.h>

int menu();
int secondaryMenu();
void cib(void);
int main(int argc, char const *argv[])
{
  menu();
  return 0;
}
/*
********菜单*********
*****功能1:xxxx*****
*****功能2:xxxx*****
*****功能3:xxxx*****


********功能1*********
*****功能1.1:xxxx*****
*****功能1.2:xxxx*****
*****功能1.3:xxxx*****

*/
int menu()
{
  int flag = 0;
  for (; flag == 0;)
  {
    printf("********菜单*********\n");
    printf("*****功能1:xxxx*****\n");
    printf("*****功能2:xxxx*****\n");
    printf("*****功能3:xxxx*****\n");
    printf("=----------------------------=\n");
    printf("请输入你的选择： \n");
    char choose;

    scanf("%c", &choose);
    cib();
    switch (choose)
    {
    case '1':
      secondaryMenu();
      break;

    default:
      flag = 1;
      break;
    }
  }
  return 0;
}
void cib(void)
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}
int secondaryMenu()
{
  int flag = 0;
  for (; flag == 0;)
  {
    printf("*********功能1********\n");
    printf("*****功能1.1:xxxx*****\n");
    printf("*****功能1.2:xxxx*****\n");
    printf("*****功能1.3:xxxx*****\n");
    printf("=----------------------------=\n");
    printf("请输入你的选择： \n");
    char choose;

    scanf("%c", &choose);
    cib();
    switch (choose)
    {
    case '1':

      break;

    default:
      flag = 1;
      break;
    }
  }

  return 0;
}