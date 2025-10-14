

#include <stdio.h>
#include <fcntl.h>



int main(int argc,char *argv[])
{
  int fd = open("/home/flowkite/2.txt", O_CREAT | O_RDWR,0777);
  if(fd < 0)
  {
    printf("打开文件失败\n");
    return -1;
  }
  else 
  {
    printf("打开文件成功\n");
  }
  return 0;
}