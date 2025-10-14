#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
int main()
{
   int x = 0, y = 0;
   struct input_event input_info = {0};        // 保存触摸屏事件信息
   int fd = open("/dev/input/event6", O_RDWR); // 打开触摸屏文件
   while (1)
   {
      read(fd, &input_info, sizeof(input_info));   // 阻塞读取
      // 手松开
       if (EV_KEY == input_info.type && BTN_TOUCH == input_info.code && 0 == input_info.value) 
      {
         printf("手松开触摸屏\n");
      }
      // 触摸屏绝对位移，x轴坐标发生改变
      if (EV_ABS == input_info.type && ABS_X == input_info.code)
      {
         x = input_info.value;   // 保存x轴坐标
      }
      // 触摸屏绝对位移，y轴坐标发生改变
      if (EV_ABS == input_info.type && ABS_Y == input_info.code)
      {
         y = input_info.value;   // 保存y轴坐标
      }
      printf("(%d, %d)\n", x, y);
   }
   close(fd);
   return 0;
}