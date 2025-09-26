#ifndef __AUTH_H
#define __AUTH_H

#include "account.h"
void cib(void);
int verifyPassword(bAL* head, int account, char *pw_t);//验证密码
int verifyRoot(char *pw);//验证管理员密码
int changePassword(bAL* head, int account,char *pw_t);//修改密码
void userLogin(void);//用户登录


#endif
