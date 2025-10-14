#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
int main(int argc, char *argv)
{
  int fd = open("/home/flowkite/1.txt",O_RDWR);
  if (fd < 0)
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