#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>

int main()
{
  unsigned int green_color = (0 << 24) | (0 << 16) | (255 << 8) | (0 << 0);
  unsigned int black_color = (0 << 24) | (0 << 16) | (0 << 8) | (0 << 0);
  int fd = open("/dev/fb0", O_RDWR);
  if (fd < 0)
  {
    perror("open fd0 fail");
    return -1;
  }

  struct fb_var_screeninfo vinfo;
  if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo))
  {
    perror("ioctl FBIOGET_VSCREENINFO fail");
    close(fd);
    return -1;
  }

  int width = vinfo.xres;
  int height = vinfo.yres;
  int bpp = vinfo.bits_per_pixel / 8; // 每像素字节数
  if (bpp != 4)
  {
    fprintf(stderr, "%d bits",bpp);
    close(fd);
    return -1;
  }

  // 映射帧缓冲区
  unsigned int *p = mmap(
      NULL,                 // 系统自动找空位
      width * height * bpp, // 空间大小
      PROT_READ | PROT_WRITE,
      MAP_SHARED,
      fd,
      0);
  if (p == (void *)-1)
  {
    perror("mmap fail");
    close(fd);
    return -1;
  }

  // 清屏
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      p[y * width + x] = black_color;
    }
  }

  // 进度条动画
  for (int x = 0; x < width; x++)
  {
    // 绘制进度条
    for (int y = height / 3; y < height / 3 * 2; y++)
    {
      p[y * width + x] = green_color;
    }

    // 计算并显示进度百分比
    int progress = (x * 100) / width;
    printf("Progress: %d%%\r", progress);
    fflush(stdout);

    usleep(10000); //
  }
printf("Progress: %d%%\r", 100);
  printf("\nProgress complete!\n");

  munmap(p, width * height * bpp);
  close(fd);

  return 0;
}
