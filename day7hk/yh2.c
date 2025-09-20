#include <string.h>
#include <stdio.h>
/*
 有一张银行卡，密码是gec123456，
程序运行之后，提示请求用户输入密码，
如果密码正确，则输出密码的长度，
如果密码出错，则提示重新输入，
如果输入的次数超过3次，则程序直接退出
*/
int verifyPassword(char *pw_t);//验证密码

int changePassword(char *pw_t);//修改密码

void cib(void);//清空输入缓存

int main(int argc, char const *argv[])
{
  char password[] = "gec123456";

  changePassword(password);

  verifyPassword(password);

  return 0;
}
int verifyPassword(char *pw_t)
{
  int count = 3;
  for (int i = 0; i < count; i++)
  {
    printf("输入密码：\n");

    char inter[18];
    scanf("%s", inter);
    cib;
    if (strcmp(pw_t, inter))
    {
      if (i != 2)
      {
        printf("再次");
      }
    }
    else
    {
      int len = strlen(inter);
      printf("密码的长度：%d", len);
      return 1;
    }
  }
  return 0;
}
int changePassword(char *pw_t)
{

  int flag = 0;
  printf("是否要修改密码（yn）");
  char chose;
  scanf("%c", &chose);
  cib();
  if (chose == 'y')
  {

    if (verifyPassword(pw_t))
    {
      char newPdone[18];

      for (;;)
      {
        printf("请输入新密码：\n");
        scanf("%s",newPdone);
        cib();
        printf("请再次输入新密码：\n");
        char newPdtwo[18];
        scanf("%s",newPdtwo);
        cib();
        if (strcmp(newPdone, newPdtwo))
        {
          printf("两次输入不一致\n");
        }
        else
        {
          strcpy(pw_t,newPdone);
          flag=1;
          break;
        }
      }
    }
    else
    {
      printf("密码错误！");
    }
  }

  return flag;
}
void cib(void)
{
  int c;
  while((c=getchar())!='\n'&&c!=EOF);
}