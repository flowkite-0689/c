/*
.编写一个程序，功能为复制文件
例如 ./mycp   old_file    new_file
要求：
1）能够完整拷贝文件
2）能显示拷贝用时，及进度
3) 文件拷贝封装成一个函数
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

// 函数声明
int copy_file(const char *src_file, const char *dst_file);

// 文件复制函数实现
int copy_file(const char *src_file, const char *dst_file)
{
    int src_fd, dst_fd;
    struct stat src_stat;
    char buffer[4096];
    ssize_t bytes_read, bytes_written;
    off_t total_size = 0;
    off_t copied_size = 0;
    time_t start_time, end_time;
    
    // 打开源文件
    src_fd = open(src_file, O_RDONLY);
    if (src_fd == -1) {
        perror("打开源文件失败");
        return -1;
    }
    
    // 获取源文件信息
    if (fstat(src_fd, &src_stat) == -1) {
        perror("获取文件信息失败");
        close(src_fd);
        return -1;
    }
    
    total_size = src_stat.st_size;
    
    // 创建目标文件
    dst_fd = open(dst_file, O_WRONLY | O_CREAT | O_TRUNC, src_stat.st_mode);
    if (dst_fd == -1) {
        perror("创建目标文件失败");
        close(src_fd);
        return -1;
    }
    
    // 记录开始时间
    time(&start_time);
    
    printf("开始复制文件: %s -> %s (大小: %ld 字节)\n", src_file, dst_file, total_size);
    
    // 复制文件内容
    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(dst_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("写入文件失败");
            close(src_fd);
            close(dst_fd);
            return -1;
        }
        
        copied_size += bytes_read;
        
        // 显示进度
        int progress = (int)((double)copied_size / total_size * 100);
        printf("\r进度: %d%% [", progress);
        for (int i = 0; i < 50; i++) {
            if (i < progress / 2) {
                printf("#");
            } else {
                printf(" ");
            }
        }
        printf("] %ld/%ld 字节", copied_size, total_size);
        fflush(stdout);
    }
    
    if (bytes_read == -1) {
        perror("读取文件失败");
        close(src_fd);
        close(dst_fd);
        return -1;
    }
    
    // 记录结束时间
    time(&end_time);
    
    // 计算用时
    double time_used = difftime(end_time, start_time);
    
    printf("\n复制完成! 用时: %.2f 秒\n", time_used);
    
    close(src_fd);
    close(dst_fd);
    
    return 0;
}

int main(int argc, char const *argv[])
{
    // 检查参数数量
    if (argc != 3) {
        printf("用法: %s <源文件> <目标文件>\n", argv[0]);
        printf("示例: %s file1.txt file2.txt\n", argv[0]);
        return 1;
    }
    
    // 检查源文件和目标文件是否相同
    if (strcmp(argv[1], argv[2]) == 0) {
        printf("错误: 源文件和目标文件不能相同\n");
        return 1;
    }
    
    // 调用文件复制函数
    if (copy_file(argv[1], argv[2]) == 0) {
        printf("文件复制成功!\n");
        return 0;
    } else {
        printf("文件复制失败!\n");
        return 1;
    }
}
