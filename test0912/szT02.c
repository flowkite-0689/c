#include <stdio.h>
int main(int argc, char const *argv[])
{
  int arr[7]={10,20,30,40,50,60,70};
  for (int i = 0; i < 7; i++)
  {
    
    printf("%p\n",&arr[i]);
  }
  
  return 0;
}
