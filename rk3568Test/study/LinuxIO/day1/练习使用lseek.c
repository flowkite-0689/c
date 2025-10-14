#include <stdio.h>
#include <fcntl.h>

long GetFileSzie(int fd)
{
  off_t cur =lseek(fd,0,SEEK_CUR);// 获取当前光标的位置
  long size=lseek(fd , 0,SEEK_END);//获取文件的大小
  lseek(fd,cur ,SEEK_SET);
  return size;
}
void main()
{
  int fd =open("/home/flowkite/1.txt",O_RDWR);
  if(fd <0)
  {
    perror("打开文件失败");
    return ;
  }
  lseek(fd,10 ,SEEK_CUR);
  long size=GetFileSzie(fd);
  printf("该文件的大小为%ld个字节\n",size);
}