#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void cib()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

typedef struct person
{

  char phone[12];
  char name[10];
  int id;
  int age;

} ps;

int searchByphone(ps *p, int count);
int serachByName(ps *p, int count);
int addPerson(ps *p, int *pcount);
int updatePerson(ps *p, int count);
int deletPerson(ps *p, int *count);
void showMenu();
void printPerson(ps ps);
void printAll(ps *p, int count);
void saveToFile(ps *p, int count);
int loadFromFile(ps *p);

int main(int argc, char const *argv[])
{
  ps p[100];
  int count = 0;
  int forflag = 1;
  count = loadFromFile(p);
  for (; forflag;)
  {
    showMenu();
    int flag;
    scanf("%d", &flag);
    cib();
    switch (flag)
    {
    case 1:

      addPerson(p, &count);
      break;
    case 2:
      printAll(p, count);

      break;
    case 3:
      int index = serachByName(p, count);
      if (index != -1)
      {
        printPerson(p[index]);
      }
      else
      {
        printf("没有找到");
      }

      break;
    case 4:
      index = searchByphone(p, count);

      if (index != -1)
      {
        printPerson(p[index]);
      }
      else
      {
        printf("没有找到");
      }

      break;
    case 5:
      saveToFile(p, count);

      break;
    case 6:
      loadFromFile(p);

      break;
    case 9:
      printf("是否要将文件保存：");
      char c = getchar();
      if (c == 'y')
      {
        saveToFile(p, count);
      }

      forflag = 0;
      printf("感谢使用.");

      break;
    case 8:
      updatePerson(p, count);
      break;
    case 7:
      deletPerson(p, &count);
      break;
    default:

      break;
    }
  }
  return 0;
}

void saveToFile(ps *p, int count)
{
  FILE *file = fopen("addressBookv2.dat", "wb");
  if (file == NULL)
  {
    printf("无文件|！\n");
    return;
  }
  fwrite(&count, sizeof(int), 1, file);

  fwrite(p, sizeof(ps), count, file);
  fclose(file);
  printf("已保存到文件\n");
}

int loadFromFile(ps *p)
{
  FILE *file = fopen("addressBookv2.dat", "rb");
  if (file == NULL)
  {
    printf("无历史记录！");
    return 0;
  }
  int count;
  fread(&count, sizeof(int), 1, file);
  fread(p, sizeof(ps), count, file);

  fclose(file);
  printf("已加载%d个联系人\n", count);
  return count;
}
void showMenu()
{
  printf("\n---通讯录管理系统---\n");
  printf("1、添加联系人\n");
  printf("2、显示所有联系人\n");
  printf("3、按姓名查找\n");
  printf("4、按手机号查找\n");
  printf("5、保存数据到文件\n");
  printf("6、从文件加载数据\n");
  printf("7、删除联系人\n");
  printf("8、修改联系人\n");
  printf("9、退出系统\n");
  printf("请选择操作（1-9）：");
}

void printPerson(ps ps)
{
  printf("姓名:%9s\t,\t电话:%11s\t,\t学号:%3d,\t年龄:%3d\n", ps.name, ps.phone, ps.id, ps.age);
}

void printAll(ps *p, int count)
{
  for (int i = 0; i < count; i++)
  {
    printf("%d\t",i);
    printPerson(p[i]);
  }
}

int addPerson(ps *p, int *pcount)
{
  if (*pcount == 100)
  {
    printf("已满，无法添加！");
    return 0;
  }

  printf("---添加联系人---");
  printf("输入第%d位的信息\n", *pcount);
  printf("输入学号：\n");
  scanf("%d", &p[*pcount].id);
  cib();

  printf("输入姓名：\n");
  scanf("%s", &p[*pcount].name);
  cib();

  printf("输入电话：\n");
  scanf("%s", &p[*pcount].phone);
  cib();

  printf("输入年龄：\n");
  scanf("%d", &p[*pcount].age);
  cib();

  (*pcount)++;
  printf("添加联系人成功，当前有%d个联系人。", *pcount);
  return 1;
}
int deletPerson(ps *p, int *count)
{

  printAll(p, *count);
  printf("输入要删除的联系人的序号：\n");
  int index = 0;
  scanf("%d", &index);
  cib();
  if (index < 0 || index >= *count)
  {
    printf("没有该序号的联系人，删除失败。");
    return 0;
  }

  for (int i = index; i < (*count) - 1; i++)
  {
    p[i] = p[i + 1];
  }
  (*count)--;
  return 1;
}

int updatePerson(ps *p, int count)
{
  printAll(p, count);
  printf("输入要修改的联系人的序号：\n");
  int index = 0;
  scanf("%d", &index);
  cib();
  if (index < 0 || index >= count)
  {
    printf("没有该序号的联系人，修改失败。");
    return 0;
  }
  printf("输入学号：\n");
  scanf("%d", &p[index].id);
  cib();

  printf("输入姓名：\n");
  scanf("%s", &p[index].name);
  cib();

  printf("输入电话：\n");
  scanf("%s", &p[index].phone);
  cib();

  printf("输入年龄：\n");
  scanf("%d", &p[index].age);
  cib();
  return index;
}
int serachByName(ps *p, int count)
{
  printf("输入要查找的联系人的名字：");
  char name[10];
  scanf("%s", name);
  cib();
  for (int i = 0; i < count; i++)
  {
    if (!strcmp(name, p[i].name))
    {
      return i;
    }
  }
  return -1;
}
int searchByphone(ps *p, int count)
{
  printf("输入要查找的联系人的电话号码：");
  char phone[12];
  scanf("%s", phone);
  cib();
  for (int i = 0; i < count; i++)
  {
    if (!strcmp(phone, p[i].phone))
    {
      return i;
    }
  }
  return -1;
}