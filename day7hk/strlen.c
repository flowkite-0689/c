#include <stdio.h>


int strlen1(char *str){
  int i =0;
  for(i;str[i]!='\0';i++);
  return i;
}


int main(int argc, char const *argv[])
{
  
  char s[18]="hhel jhs";
  
  printf("%d\n",strlen1(s));
  return 0;
}
