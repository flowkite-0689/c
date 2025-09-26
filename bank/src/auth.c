#include "../include/auth.h"
#include "../include/account.h"
#include <windows.h>

void cib(void)
{
  int c;
  while((c=getchar())!='\n'&&c!=EOF);
}

int verifyPassword(bAL* head, int account, char *pw_t)
{
    bAD data=searchByAccount(head,account)->data;
    if (strcmp(pw_t,data.password))
    {
      return 0;
    }
    return 1;
}

int verifyRoot(char *pw)
{
  if (strcmp(pw,rootpassword))
  {
    return 0;
  }
  return 1;
}

void userLogin()
{
    system("cls");
    printf("用户登录功能\n");
    printf("请输入账号：");
    int account;
    scanf("%d", &account);
    cib();

    printf("请输入密码：");
    char password[18];
    scanf("%17s", password);
    cib();

    // 这里需要从文件读取账户列表进行验证
    bAL *head = readListFromFile("accounts.dat");
    if (head == NULL) {
        printf("账户文件不存在或为空\n");
        system("pause");
        return;
    }

    if (verifyPassword(head, account, password)) {
        printf("登录成功！\n");
        system("pause");
        // 这里应该进入用户操作菜单
    } else {
        printf("账号或密码错误！\n");
        system("pause");
    }
}

int changePassword(bAL* head, int account, char *pw_t)
{
    bAL *node = searchByAccount(head, account);
    if (node == NULL) {
        printf("账户不存在\n");
        system("pause");
        return 0;
    }

    strcpy(node->data.password, pw_t);
    writeListToFile(head, "accounts.dat");
    printf("密码修改成功！\n");
    system("pause");
    return 1;
}
