#ifndef __ACCOUNT_H
#define __ACCOUNT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define rootpassword "root123456"
typedef struct bankAccountData
{
  int account;
  char password[18];
  int balance;
} bAD;

typedef struct bankAccountList
{
  bAD data;
  struct bankAccountList *next;
} bAL;
// 银行账户

// 创建新节点
bAL *createNode(int account, const char *password,int );

// 在链表头插入新节点
void insertAtHead(bAL **head, int account, const char *password,int);

// 遍历全部节点
void traverseList(bAL *head);

// 按账户查找
bAL *searchByAccount(bAL *head, int account);

// 按账号删除节点
void deleteNode(bAL **head, int account);

// 写入文件账户列表
void writeListToFile(bAL *head, const char *filename);

// 从文件读取账户列表
bAL *readListFromFile(const char *filename);
#endif