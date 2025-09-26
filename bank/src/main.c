#include "../include/account.h"
#include "../include/ui.h"
#include "../include/auth.h"
#include <windows.h>

int main(int argc, char const *argv[])
{
  int forflag0 = 1;
  for (; forflag0;)
  {
    system("cls");
    menu();
    printf("请输入选择：");
    char c = getchar();
    cib();
    switch (c)
    {
    case '1':
      printf("1.用户登录\n");
      userLogin();
      break;
    case '2':
      printf("2.管理员登录\n");
      printf("请输入管理员密码：");
      char rootPw[18];
      scanf("%17s", rootPw);
      cib();

      if (verifyRoot(rootPw)) {
          printf("管理员登录成功！\n");
          int forflag2 = 1;
          for (; forflag2;) {
              system("cls");
              rootMenu();
              printf("请输入选择：");
              char c2 = getchar();
              cib();
              switch (c2) {
              case '1':
                  printf("创建账号\n");
                  printf("请输入新账号：");
                  int newAccount;
                  scanf("%d", &newAccount);
                  cib();

                  printf("请输入初始密码：");
                  char newPassword[18];
                  scanf("%17s", newPassword);
                  cib();

                  printf("请输入初始余额：");
                  int balance;
                  scanf("%d", &balance);
                  cib();

                  bAL *head = readListFromFile("accounts.dat");
                  if (head == NULL) {
                      head = createNode(newAccount, newPassword, balance);
                  } else {
                      insertAtHead(&head, newAccount, newPassword, balance);
                  }
                  writeListToFile(head, "accounts.dat");
                  printf("账号创建成功！\n");
                  break;
              case '2':
                  printf("查看账号列表功能\n");
                  bAL *head2 = readListFromFile("accounts.dat");
                  if (head2 != NULL) {
                      traverseList(head2);
                  }
                  break;
              case '3':
                  printf("删除账号功能\n");
                  printf("请输入要删除的账号：");
                  int delAccount;
                  scanf("%d", &delAccount);
                  cib();

                  bAL *head3 = readListFromFile("accounts.dat");
                  if (head3 != NULL) {
                      deleteNode(&head3, delAccount);
                      writeListToFile(head3, "accounts.dat");
                  }
                  break;
              case '4':
                  printf("退出系统\n");
                  forflag2 = 0;
                  break;
              default:
                  printf("无效输入\n");
                  break;
              }
          }
      } else {
          printf("管理员密码错误！\n");
      }
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
