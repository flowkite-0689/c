#include <stdio.h>

int main()
{
  printf("char--%d\n",sizeof(char));
  printf("int--%d\n",sizeof(int));
  printf("float--%d\n",sizeof(float));
  printf("double--%d\n",sizeof(double));
  printf("bool--%d\n",sizeof(_Bool));

  printf("long --%d\n",sizeof(long));
  printf("long long --%d\n",sizeof(long long));
  return 0;
}