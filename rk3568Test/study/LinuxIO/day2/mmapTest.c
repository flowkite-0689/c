#include <sys/mman.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(void)
{
    // (A << 24) | (R << 16)  | (G << 8)   | (B << 0);
    unsigned int red_color = (255 << 24) | (255 << 16) | (0 << 8) | (0 << 0);
    int fd = open("/dev/fb0", O_RDWR);
    if (fd < 0)
    {
        perror("open fail");
        return -1;
    }
    unsigned int *p = mmap(NULL,                   // 系统自动寻找合适的内存映射后的起始地址
                           1024 * 600 * 4,         // 映射的内存字节大小
                           PROT_READ | PROT_WRITE, // 映射内存的权限可读可写
                           MAP_SHARED,             // 映射的内存进程共享
                           fd,                     // 要映射的文件描述符
                           0);                     // 映射后文件偏移量

    if (p == (void *)-1)
    {
        perror("mmap fail");
        close(fd);
        return -1;
    }

    // 2）往文件写显示绿色的数据
    for (int i = 0; i < 1024 * 600; i++)
        *(p + i) = red_color; // 往映射后的内存写入数据，间接往文件写入数据

    // 3) 资源回收
    munmap(p, 1024 * 600 * 4); // 取消映射
    close(fd);                // 关闭文件

    return 0;
}