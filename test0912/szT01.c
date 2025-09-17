#include <stdio.h>
int main(int argc, char const *argv[])
{
  int arr[4]={0};
  printf("输入四个整型数：\n");
  for (int i = 0; i < 4; i++)
  {
    scanf("%d",&arr[i]);
  }

  int max = arr[0];
  int min = arr[0];
  for (int i = 0; i < 4; i++)
  {
    if (arr[i]>max)
    {
      max=arr[i];
    }
    if (arr[i]<min)
    {
      min=arr[i];
    }
    
    
  }
  printf("max=%d\n",max);
  printf("min=%d\n",min);
  

  return 0;
}
