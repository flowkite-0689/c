#include <stdlib.h>
#include <stdio.h>
char *strcpy(char* dest,char* arr)
{
  char *dest_p =dest;
  for (int i = 0; arr[i]!='\0'; i++)
  {

    dest[i]=arr[i];
    /* code */
  }
  return dest_p;
  
  
}

int main(int argc, char const *argv[])
{
  
  char  str[]="hello!!";

  char *str1=(char*)malloc(20);
  strcpy(str1,str);
  puts(str1);
  
  return 0;
}
