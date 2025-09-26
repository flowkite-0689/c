#include "../include/auth.h"
#include "../include/account.h"
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

}