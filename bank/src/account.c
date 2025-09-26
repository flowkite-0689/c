#include "../include/account.h"

bAL *createNode(int account, const char *password, int balance)
{
  bAL *newNode = (bAL *)malloc(sizeof(bAL));
  if (newNode == NULL)
  {
    printf("内存分配失败\n");
    exit(1);
  }

  newNode->data.account = account;
  strcpy(newNode->data.password, password);
  newNode->data.balance = balance;
  newNode->next = NULL;
  return newNode;
}

void insertAtHead(bAL **head, int account, const char *password, int balance)
{
  bAL *newNode = createNode(account, password, balance);
  newNode->next = *head;
  *head = newNode;
}

void traverseList(bAL *head)
{
  bAL *current = head;

  printf("当前链表数据：\n");
  while (current != NULL)
  {
    printf("账号：%d\t,\t密码：%s\t\n", current->data.account, current->data.password);
    current = current->next;
  }
}
bAL *searchByAccount(bAL *head, int account)
{
  bAL *current = head;
  while (current != NULL)
  {
    if (current->data.account == account)
    {
      return current;
    }
    current = current->next;
  }

  return NULL;
}

void deleteNode(bAL **head, int account)
{
  bAL *current = *head;
  bAL *prev = NULL;
  while (current != NULL && current->data.account != account)
  {
    prev = current;
    current = current->next;
  }
  if (current == NULL)
  {

    printf("未找到账号为：%d的账户\n", account);
    return;
  }
  if (prev == NULL)
  {
    *head = current->next;
  }
  else
  {
    prev->next = current->next;
  }

  free(current);
  printf("账户%d已删除\n", account);
}

void writeListToFile(bAL *head, const char *filename)
{
  FILE *file = fopen(filename, "wb");
  if (file == NULL)
  {
    printf("文件打开失败！");
    return;
  }

  bAL *current = head;
  while (current != NULL)
  {
    fwrite(&(current->data), sizeof(bAD), 1, file);
    current = current->next;
  }

  fclose(file);

  printf("已写入文件%s\n", filename);
}

bAL *readListFromFile(const char *filename)
{
  FILE *file = fopen(filename, "rb");
  if (file == NULL)
  {
    printf("打开文件失败");
    return NULL;
  }

  bAL *head = NULL;
  bAL *tail = NULL;
  bAD tempData;

  while (fread(&tempData, sizeof(bAD), 1, file) == 1)
  {
    int account = tempData.account;
    char password[18];
    strcpy(password, tempData.password);
    int balance = tempData.balance;
    insertAtHead(&head, account, password, balance);
  }
  fclose(file);
  printf("从文件读取列表成功\n");
  return head;
}