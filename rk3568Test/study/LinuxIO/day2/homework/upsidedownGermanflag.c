#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

int main() {
    // 德国国旗颜色
    unsigned int Germany_black_color = (0 << 24) | (0 << 16) | (0 << 8) | (0 << 0);
    unsigned int Germany_red_color = (0 << 24) | (200 << 16) | (16 << 8) | (46 << 0);
    unsigned int Germany_aurum_color = (0 << 24) | (255 << 16) | (204 << 8) | (0 << 0);

    // 1. 打开帧缓冲区
    int fd = open("/dev/fb0", O_RDWR);
    if (fd < 0) {
        perror("open /dev/fb0 fail");
        return -1;
    }

    // 2. 获取帧缓冲区信息
    struct fb_var_screeninfo vinfo;
    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("ioctl FBIOGET_VSCREENINFO fail");
        close(fd);
        return -1;
    }

    int width = vinfo.xres;  // 分辨率宽度
    int height = vinfo.yres; // 分辨率高度
    int bpp = vinfo.bits_per_pixel / 8; // 每像素字节数
    if (bpp != 4) {
        fprintf(stderr, "Unsupported pixel format: %d bits per pixel\n", vinfo.bits_per_pixel);
        close(fd);
        return -1;
    }

    // 3. 映射帧缓冲区
    unsigned int *p = mmap(NULL, width * height * bpp, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == (void *)-1) {
        perror("mmap fail");
        close(fd);
        return -1;
    }

    // 4. 绘制德国国旗（三条垂直条纹）
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned int select_color;
            if (x < width / 3) { // 左1/3列：黑色
                select_color = Germany_black_color;
            } else if (x < 2 * width / 3) { // 中间1/3列：红色
                select_color = Germany_red_color;
            } else { // 右1/3列：金色
                select_color = Germany_aurum_color;
            }
            p[y * width + x] = select_color;
        }
    }

    // 5. 清理
    munmap(p, width * height * bpp);
    close(fd);
    return 0;
}