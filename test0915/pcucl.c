#include <stdio.h>
void swap(int *a,int *b)
{
  int temp = *a;
  *a = *b;
  *b =temp;

}

int main(int argc, char const *argv[])
{
  int a =10 ;
  int b = 12;
  printf("交换前： a= %d,b = %d\n",a,b);
  swap(&a,&b);
  printf("交换后： a= %d,b = %d\n",a,b);
  return 0;
}
