#include <stdio.h>

void cib()
{
  int c;
  while((c=getchar())!='\n'&&c!=EOF);
}
struct person
{
  char phone[12];
  char name[10];
  int id;
   int age;

};

int main(int argc, char const *argv[])
{
  struct person p[3];
  for (int i = 0; i < 3; i++)
  {
    printf("输入第%d位的信息\n");
    printf("输入学号：\n");
    scanf("%d",&p[i].id);cib();
    printf("输入姓名：\n");
    scanf("%s",&p[i].name);cib();
    printf("输入电话：\n");
    scanf("%s",&p[i].phone);cib();
    printf("输入年龄：\n");
    scanf("%d",&p[i].age);
  }
  for (int i = 0; i < 3; i++)
  {
    printf("");
  }
  

  return 0;
}

