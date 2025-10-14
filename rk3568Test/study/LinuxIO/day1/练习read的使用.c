#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
  int ret = 0;
  int fd = open("/home/flowkite/1.txt", O_RDWR);
  char buf[1024] = {0};
  if (fd < 0)
  {
    perror("打开文件失败");
    return -1;
  }
  ret = read(fd, buf, sizeof(buf));
  printf("读到的文件的内容为\n%s\n", buf);
  ret = read(fd, buf, sizeof(buf));
  printf("读到的文件的内容为\n%s\n", buf);
  close(fd);
  return 0;
}