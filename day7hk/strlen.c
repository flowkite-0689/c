#include <stdio.h>


int strlen(char *str){
  int i =0;
  for(i;str[i]!='\0';i++);
  return i;
}


int main(int argc, char const *argv[])
{
  
  char s[18]="hhel jhs";
  
  printf("%d",strlen(s));
  return 0;
}
