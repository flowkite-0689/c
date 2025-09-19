#include <stdio.h>

void qk()
{
  int c;
  
  while((c=getchar())=='\n'&&c!=EOF);
 
  
}

int main(int argc, char const *argv[])
{
  char a,b;
 scanf("%c",&a);
 qk();
 scanf("%c",&b);
   return 0;
}
