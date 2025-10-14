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

  unsigned int Germany_red_color = (0 << 24) | (200 << 16) | (16 << 8) | (46 << 0);
  unsigned int Germany_black_color = (0 << 24) | (0 << 16) | (0 << 8) | (0 << 0);
  unsigned int Germany_aurum_color = (0 << 24) | (255 << 16) | (204 << 8) | (0 << 0);
  unsigned int  all_Germany_color[]={Germany_aurum_color,Germany_black_color,Germany_red_color};
  // 1.打开屏幕文件
  int fd = open("/dev/fb0", O_RDWR);
  if (fd < 0)
  {
    perror("open /dev/fb0 fail");
    return -1;
  }

  // 2. 获取帧缓冲区信息
  struct fb_var_screeninfo vinfo;
  if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0)
  {
    perror("ioctl FBIOGET_VSCREENINFO fail");
    close(fd);
    return -1;
  }

  int width = vinfo.xres;             // 分辨率宽度
  int height = vinfo.yres;            // 分辨率高度
  int bpp = vinfo.bits_per_pixel / 8; // 每像素字节数
  if (bpp != 4)
  {
    fprintf(stderr, "Unsupported pixel format: %d bits per pixel\n", vinfo.bits_per_pixel);
    close(fd);
    return -1;
  }

  // 3. 映射帧缓冲区
  unsigned int *p = mmap(
      NULL,
      width * height * bpp,
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
  // 颜色循环逻辑优化
  int flag = 0;
  while (1)
  {
    for (int y = 0; y < height; y++)
    {
      // 计算当前行属于哪个条带 (0, 1, 2)
      int stripe = y / (height / 3);
      if (stripe > 2) stripe = 2; // 防止越界

      // 使用模运算
      int color_index = (flag + stripe) % 3;

      for (int x = 0; x < width; x++)
      {
        p[y * width + x] = all_Germany_color[color_index];
      }
    }

    // 更新标志位
    flag = (flag + 1) % 3;

    usleep(2000000); 
  }

  // 清理
  munmap(p, width * height * 4);
  close(fd);

  return 0;
}
