#include "../include/account.h"
#include "../include/ui.h"
#include "../include/auth.h"

int main(int argc, char const *argv[])
{
  int forflag0 = 1;
  for (; forflag0;)
  {
    menu();
    printf("请输入选择：");
    char c = getchar();
    cib();
    switch (c)
    {
    case '1':
      printf("1.用户登录\n");
      int forflag1 = 1;
      for (; forflag1;)
      {
        userMenu();

        printf("请输入选择：");
        char c1 = getchar();
        cib();
        switch (c1)
        {
        case '1':
          printf("1.查看余额\n");
          break;
        case '2':
          printf("2.修该密码\n");
          break;
        case '3':
          printf("3.退出系统\n");
          forflag1 = 0;
          break;
        default:
          break;
        }
      }
      break;
    case '2':
      printf("2.管理员登录\n");
      break;
    case '3':
      printf("3.退出系统\n");
      printf("感谢使用");
      system("pause");
      forflag0 = 0;
      break;
    default:

      printf("无效输入");
      break;
    }
  }

  return 0;
}
