#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
  unsigned int Germany_red_color = (0 << 24) | (200 << 16) | (16 << 8) | (46 << 0);
  unsigned int Germany_black_color = (0 << 24) | (0 << 16) | (0 << 8) | (0 << 0);
  unsigned int Germany_aurum_color = (0 << 24) | (255 << 16) | (204 << 8) | (0 << 0);

  //1.打开屏幕文件
  int fd =open("/dev/fb0",O_RDWR);
  if (fd < 0)
  {
    perror("open /dev/fb0 fail");
    return -1;
  }
  //2.写入颜色数据
  for (int i = 0; i < 1024*600; i++)
  {
    if (i<1024*200)
    {
      write(fd,&Germany_black_color,4);
    }else
    if (i<1024*400)
    {
      write(fd,&Germany_red_color,4);
    }else
    {
      write(fd,&Germany_aurum_color,4);
    }
    
    
  }
  close(fd);
  
  
  return 0;
}
