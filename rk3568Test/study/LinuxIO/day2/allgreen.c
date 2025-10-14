#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(void)
{                               // (A << 24) | (R << 16)  | (G << 8)   | (B << 0);
    unsigned int green_color = (0 << 24) | (0 << 16)  | (255 << 8) | (0 << 0);

    // 1) 打开屏幕文件
    int fd = open("/dev/fb0", O_RDWR);
    if (fd < 0)
    {
        perror("open /dev/fb0 fail");
        return -1;
    }
    // 2）往文件写显示绿色的数据
    for (int i = 0; i <1024 * 600; i++)
        write(fd, &green_color, 4); // 写入一个像素点的数据，每次写入文件偏移量往后移4个字节
        
    // 3) 关闭文件
    close(fd);

    return 0;
}