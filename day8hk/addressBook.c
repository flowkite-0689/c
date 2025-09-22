#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cib(void){
  int c;
  while((c=getchar())!='\n'&&c!=EOF);
}

struct person
{
  char phone[12]; // 增加1位用于字符串结束符
  int student_id;
  char name[11];  // 增加1位用于字符串结束符
  int age;
};

void printAllperson(struct person *p, int count)
{
  if(count == 0) {
    printf("通讯录为空！\n");
    return;
  }

  printf("\n=== 通讯录信息 (共%d个联系人) ===\n", count);
  for(int i=0; i<count; i++)
  {
    printf("%d. 学号：%d, 姓名：%s, 年龄：%d, 电话：%s\n",
           i+1, p[i].student_id, p[i].name, p[i].age, p[i].phone);
  }
}

// 按姓名查找联系人
void searchByName(struct person *p, int count)
{
  char name[11];
  printf("请输入要查找的姓名：");
  scanf("%10s", name);
  cib();

  int found = 0;
  printf("\n=== 查找结果 ===\n");
  for(int i=0; i<count; i++)
  {
    if(strcmp(p[i].name, name) == 0)
    {
      printf("找到联系人 - 学号：%d, 姓名：%s, 年龄：%d, 电话：%s\n",
             p[i].student_id, p[i].name, p[i].age, p[i].phone);
      found++;
    }
  }

  if(!found)
  {
    printf("未找到姓名为 %s 的联系人\n", name);
  } else {
    printf("共找到 %d 个联系人\n", found);
  }
}

// 按手机号查找联系人
void searchByPhone(struct person *p, int count)
{
  char phone[12];
  printf("请输入要查找的手机号：");
  scanf("%11s", phone);
  cib();

  int found = 0;
  printf("\n=== 查找结果 ===\n");
  for(int i=0; i<count; i++)
  {
    if(strcmp(p[i].phone, phone) == 0)
    {
      printf("找到联系人 - 学号：%d, 姓名：%s, 年龄：%d, 电话：%s\n",
             p[i].student_id, p[i].name, p[i].age, p[i].phone);
      found++;
    }
  }

  if(!found)
  {
    printf("未找到手机号为 %s 的联系人\n", phone);
  } else {
    printf("共找到 %d 个联系人\n", found);
  }
}

// 保存数据到文件
void saveToFile(struct person *p, int count)
{
  FILE *file = fopen("addressbook.dat", "wb");
  if(file == NULL) {
    printf("❌ 无法创建数据文件！\n");
    return;
  }

  // 先写入联系人数量
  fwrite(&count, sizeof(int), 1, file);
  // 再写入所有联系人数据
  fwrite(p, sizeof(struct person), count, file);

  fclose(file);
  printf("✅ 数据已保存到文件！\n");
}

// 从文件加载数据
int loadFromFile(struct person *p)
{
  FILE *file = fopen("addressbook.dat", "rb");
  if(file == NULL) {
    printf("ℹ️  无历史数据，将创建新的通讯录\n");
    return 0;
  }

  int count;
  fread(&count, sizeof(int), 1, file);
  fread(p, sizeof(struct person), count, file);

  fclose(file);
  printf("✅ 已从文件加载 %d 个联系人\n", count);
  return count;
}

// 显示菜单
void showMenu()
{
  printf("\n=== 通讯录管理系统 ===\n");
  printf("1. 添加联系人\n");
  printf("2. 显示所有联系人\n");
  printf("3. 按姓名查找\n");
  printf("4. 按手机号查找\n");
  printf("5. 保存数据到文件\n");
  printf("6. 从文件加载数据\n");
  printf("7. 退出系统\n");
  printf("请选择操作 (1-7): ");
}

int main()
{
  struct person p[100];
  int count = 0;
  int choice;
  int exitProgram = 0;

  // 初始化结构体数组
  for(int i=0; i<100; i++) {
    p[i].student_id = 0;
    p[i].age = 0;
    p[i].name[0] = '\0';
    p[i].phone[0] = '\0';
  }

  printf("欢迎使用通讯录管理系统！\n");

  // 程序启动时自动尝试加载数据
  int loadedCount = loadFromFile(p);
  if(loadedCount > 0) {
    count = loadedCount;
  }

  while(!exitProgram) {
    showMenu();

    if(scanf("%d", &choice) != 1) {
      printf("输入错误，请输入数字1-7！\n");
      cib();
      continue;
    }
    cib();

    switch(choice) {
      case 1:  // 添加联系人
        if(count < 100) {
          printf("\n=== 添加联系人 ===\n");

          printf("输入学号：");
          while(scanf("%d", &p[count].student_id) != 1) {
            printf("输入错误，请输入一个整数：");
            cib();
          }
          cib();

          printf("输入姓名（最多10个字符）：");
          scanf("%10s", p[count].name);
          cib();

          printf("输入年龄：");
          while(scanf("%d", &p[count].age) != 1) {
            printf("输入错误，请输入一个整数：");
            cib();
          }
          cib();

          printf("输入手机号码（最多11位）：");
          scanf("%11s", p[count].phone);
          cib();

          count++;
          printf("✅ 联系人添加成功！当前共有 %d 个联系人\n", count);
        } else {
          printf("❌ 通讯录已满（最多100个联系人），无法添加新联系人！\n");
        }
        break;

      case 2:  // 显示所有联系人
        printAllperson(p, count);
        break;

      case 3:  // 按姓名查找
        if(count > 0) {
          searchByName(p, count);
        } else {
          printf("通讯录为空，请先添加联系人！\n");
        }
        break;

      case 4:  // 按手机号查找
        if(count > 0) {
          searchByPhone(p, count);
        } else {
          printf("通讯录为空，请先添加联系人！\n");
        }
        break;

      case 5:  // 保存数据到文件
        saveToFile(p, count);
        break;

      case 6:  // 从文件加载数据
        count = loadFromFile(p);
        break;

      case 7:  // 退出
        printf("是否保存数据到文件？(y/n): ");
        char saveChoice;
        scanf(" %c", &saveChoice);
        cib();
        if(saveChoice == 'y' || saveChoice == 'Y') {
          saveToFile(p, count);
        }
        printf("感谢使用通讯录管理系统！再见！\n");
        exitProgram = 1;
        break;

      default:
        printf("无效的选择，请输入1-7之间的数字！\n");
    }

    if(!exitProgram) {
      printf("\n按回车键继续...");
      cib();
    }
  }

  return 0;
}
