#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int copy_file(const char *src_file, const char *dst_file);

int main(int argc, char const *argv[])
{
    if (argc != 3) {
        printf("参数错误！\n");
        printf("用法： ./mycp src_file dst_file\n");
        return -1;
    }
    if (strcmp(argv[1], argv[2]) == 0) {
        printf("错误: 源文件和目标文件不能相同\n");
        return -1;
    }
    if (copy_file(argv[1], argv[2]) == -1) {
        printf("\n操作失败\n");
        return -1;
    }
    printf("\n操作成功\n");
    return 0;
}

int copy_file(const char *src_file, const char *dst_file)
{
    int src_fd = open(src_file, O_RDONLY);
    if (src_fd == -1) {
        perror("打开文件失败哦！");
        return -1;
    }

    struct stat src_stat;
    if (fstat(src_fd, &src_stat) == -1) {
        perror("获取文件信息出错了");
        close(src_fd);
        return -1;
    }
    off_t total_size = src_stat.st_size;
    off_t copied_size = 0;

    int dst_fd = open(dst_file, O_WRONLY | O_CREAT | O_TRUNC, src_stat.st_mode);
    if (dst_fd == -1) {
        perror("创建文件失败");
        close(src_fd);
        return -1;
    }

    time_t start_time, end_time;
    time(&start_time);
    printf("开始复制文件: %s -> %s (大小: %ld 字节)\n", src_file, dst_file, total_size);

    char buffer[2048];
    ssize_t bytes_read, bytes_written;
    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(dst_fd, buffer, bytes_read);
        if (bytes_written == -1 || bytes_written != bytes_read) {
            perror("文件写入失败");
            close(src_fd);
            close(dst_fd);
            return -1;
        }
        copied_size += bytes_read;
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

    time(&end_time);
    double time_used = difftime(end_time, start_time);
    printf("\n复制完成! 用时: %.2f 秒\n", time_used);

    close(src_fd);
    close(dst_fd);
    return 0;
}