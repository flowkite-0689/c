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

    

    int width = 1024;  
    int height = 600;
    

    // 3. 映射帧缓冲区
    unsigned int *p = mmap(NULL, width * height * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
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
    munmap(p, width * height * 4);
    close(fd);
    return 0;
}