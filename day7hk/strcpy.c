#include <stdlib.h>
#include <stdio.h>
char *strcpy(char *dest, char *arr)
{
  char *dest_p = dest;
  int i;
  for ( i= 0; arr[i] != '\0'; i++)
  {

    dest[i] = arr[i];
    /* code */
  }
  dest[i] = '\0';
  return dest_p;
}
char *strncpy(char *dest, const char *src, size_t n)
{
  int i = 0;
  char *dest_p = dest;
  for (i = 0; i < n && src[i] != '\0'; i++)
  {
    dest[i] = src[i];
  }
if(i<n){
  for (i; i < n; i++)
  {
    dest[i] = '\0';
  }
  return dest_p;
}
}
int main(int argc, char const *argv[])
{

  char str[] = "hello!!";

  char *str1 = (char *)malloc(20);

 // strncpy
  strncpy(str1, str, 2);
  puts(str1);

  strcpy(str1, str);
  puts(str1);


  free(str1);

  return 0;
}
