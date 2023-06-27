/*
* cp命令的自定义实现
* @author：谢小鹏、梁亮、徐璟逸
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFF_SIZE 4096
#define COPY_MODE 0644

int main(int argc, char *argv[]) {
    int fd_in, fd_out, bytes;
    char buf[BUFF_SIZE];
    
    if (argc != 3) {
        fprintf(stderr, "参数数量错误\n");
        return 1;  // 返回非零值表示错误
    }
    
    // 打开原文件
    fd_in = open(argv[1], O_RDONLY);
    if (fd_in == -1) {
        perror("打开文件失败");
        return 1;
    }
    
    // 新建复制文件
    fd_out = creat(argv[2], COPY_MODE);
    if (fd_out == -1) {
        perror("创建文件失败");
        close(fd_in);
        return 1;
    }
    
    // 复制文件内容
    while ((bytes = read(fd_in, buf, BUFF_SIZE)) > 0) {
        ssize_t bytes_written = write(fd_out, buf, bytes);
        if (bytes_written == -1 || bytes_written != bytes) {
            perror("复制中出现错误");
            close(fd_in);
            close(fd_out);
            return 1;
        }
    }
    
    if (bytes == -1) {
        perror("读取文件内容错误");
        close(fd_in);
        close(fd_out);
        return 1;
    }
    
    // 关闭文件
    if (close(fd_in) == -1 || close(fd_out) == -1) {
        perror("关闭文件出现错误");
        return 1;
    }
    
    printf("cp success\n");
    return 0;
}
