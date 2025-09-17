#include <stdio.h>
#include <stdlib.h>
int main(int argc, char const *argv[])
{
  int flag = 0;
  int flag1 = 0;
  do
  {
    printf("--------------一级菜单--------------\n");
    printf("--------------功能1-------------\n");
    printf("--------------功能2--------------\n");
    printf("--------------功能3--------------\n");
    printf("--------------4退出--------------\n");
    printf("请输入选择：\n");
    scanf("%d", &flag);
    switch (flag)
    {
    case 1:
      do
      {
        printf("--------------菜单（一）,二级菜单----------------\n");
        printf("--------------功能1.1-------------\n");
        printf("--------------功能1.2--------------\n");
        printf("--------------功能1.3--------------\n");
        printf("--------------4退出--------------\n");
        printf("请输入选择：\n");
        scanf("%d", &flag1);
        switch (flag1)
        {
        case 1:
          printf("--------------功能1.1-------------\n");
          system("pause");
          break;
        case 2:
          printf("--------------功能1.2--------------\n");
          system("pause");
          break;
        case 3:
          printf("--------------功能1.3--------------\n");
          system("pause");
          break;
        case 4:
          printf("--------------4退出--------------\n");

          break;
        default:
          printf("输入不正确，重新输入");
          break;
        }
        system("cls");

      } while (flag1 != 4);

      system("pause");
      break;
    case 2:
      printf("--------------功能2--------------\n");
      system("pause");
      break;
    case 3:
      printf("--------------功能3--------------\n");
      system("pause");
      break;
    case 4:
      printf("--------------4退出--------------\n");
      system("pause");
      break;
    default:
      printf("输入不正确，重新输入");
      break;
    }
    system("cls");
  } while (flag != 4);

  return 0;
}
