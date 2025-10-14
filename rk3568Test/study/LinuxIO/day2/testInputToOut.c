#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/mman.h>
#include <string.h>

// 使用 #define 定义常量
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 600

int getXY(int *x, int *y);         // 获取被触控的坐标
int turnColour(const char *color); // 改变屏幕颜色

int main()
{
  char current_color[16] = "red";
  while (1)
  {
    int x = 0, y = 0;
    if (getXY(&x, &y) < 0)
    {
      continue;
    }

    char new_color[16] = "red";
    if (x < (SCREEN_WIDTH / 2))
    {
      strcpy(new_color, "green");
    }
    if (strcmp(current_color, new_color) != 0)
    {
      if (turnColour(new_color) < 0)
      {
        fprintf(stderr, "设置颜色失败\n");
        break;
      }
      strcpy(current_color, new_color);
    }
    usleep(100000);
  }
}

int turnColour(const char *color)
{
  static int fd = -1;
  static unsigned int *p = NULL;
  unsigned int green_color = (255 << 24) | (0 << 16) | (255 << 8) | (0 << 0);
  unsigned int red_color = (255 << 24) | (255 << 16) | (0 << 8) | (0 << 0);
  if (fd < 0)
  {
    /* code */ int fd = open("/dev/fb0", O_RDWR);
    if (fd < 0)
    {
      perror("open fb0 fail");
      return -1;
    }
    p = mmap(
        NULL,                             // 系统自动寻找合适的内存
        SCREEN_WIDTH * SCREEN_HEIGHT * 4, // 映射的字节大小
        PROT_READ | PROT_WRITE,           // 权限
        MAP_SHARED,                       // 内存进程共享
        fd,                               // 要映射的文件描述符
        0);                               // 偏移量

    if (p == (void *)-1)
    {
      perror("mmap fail");
      close(fd);
      return -1;
    }
  }

  // 写入颜色
  unsigned int selected_color = (strcmp(color, "green") == 0) ? green_color : red_color;
  for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++)
  {

    *(p + i) = selected_color;
  }
  // 回收资源
  munmap(p, SCREEN_HEIGHT * SCREEN_WIDTH * 4);
  close(fd);
  return 0;
}

int getXY(int *x, int *y)
{
  struct input_event input_info = {0}; // 保存触摸屏事件信息

  int fd = open("/dev/input/event6", O_RDWR); // 打开触摸屏文件
  if (fd < 0)
  {
    perror("打开触摸屏文件失败");
    return -1;
  }

  read(fd, &input_info, sizeof(input_info));
  // 触摸屏绝对位移，x轴坐标发生改变
  if (EV_ABS == input_info.type && ABS_X == input_info.code)
  {
    *x = input_info.value; // 保存x轴坐标
  }
  // 触摸屏绝对位移，y轴坐标发生改变
  if (EV_ABS == input_info.type && ABS_Y == input_info.code)
  {
    *y = input_info.value; // 保存y轴坐标
  }
  close(fd);
  return 0;
}