/*
* top命令的自定义实现
* @author：谢小鹏、梁亮、徐璟逸
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFF_SIZE 4096

// 读取fd_in文件里的内容并写入到fd_out文件中
void filecopy(int fd_in, int fd_out, const char *filename, int isLastFile);

// main
int main(int argc, char *argv[])
{
    int fd_in = STDIN_FILENO;    // 设置Standard input(键盘输入) 文件描述符
    int fd_out = STDOUT_FILENO;  // 设置Standard output(屏幕输出)

    if (argc == 1)   // 一个参数，代表用户只输入了cat命令，后面并未接文件
    {
        filecopy(fd_in, fd_out, "Standard Input", 1);
        close(fd_in);
    }
    else if (argc == 4 && strcmp(argv[2], ">") == 0)  // 四个参数且第三个参数是">"，执行文件内容写入操作
    {
        fd_in = open(argv[1], O_RDONLY);  // 以读的方式打开输入文件
        if (fd_in < 0)
        {
            fprintf(stderr, "打开%s文件时发生错误！\n", argv[1]);
            exit(1);
        }

        fd_out = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);  // 打开输出文件
        if (fd_out < 0)
        {
            fprintf(stderr, "打开%s文件时发生错误！\n", argv[3]);
            exit(1);
        }

        filecopy(fd_in, fd_out, argv[0], 1);
        close(fd_in);
        close(fd_out);
    }
    else if (argc == 4 && strcmp(argv[2], ">>") == 0)  // 四个参数且第三个参数是">>"，执行文件内容追加操作
    {
        fd_in = open(argv[1], O_RDONLY);  // 以读的方式打开输入文件
        if (fd_in < 0)
        {
            fprintf(stderr, "打开%s文件时发生错误！\n", argv[0]);
            exit(1);
        }

        fd_out = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);  // 打开输出文件，追加方式写入
        if (fd_out < 0)
        {
            fprintf(stderr, "打开%s文件时发生错误！\n", argv[3]);
            exit(1);
        }

        filecopy(fd_in, fd_out, argv[0], 1);
        close(fd_in);
        close(fd_out);
    }
    else            // 多个参数，cat命令后面接了一个或多个文件
    {
        for (int i = 1; i < argc; ++i)
        {
            fd_in = open(argv[i], O_RDONLY);  // 以读的方式打开输入文件
            if (fd_in < 0)
            {
                fprintf(stderr, "123打开%s文件时发生错误！\n", argv[i]);
                
                continue;
            }
            int isLastFile = (i == argc - 1); // 判断是否是最后一个文件
            filecopy(fd_in, fd_out, argv[i], isLastFile);
            close(fd_in);
        }
    }
    return 0;
}

// 读取fd_in文件里的内容并写入到fd_out文件中
void filecopy(int fd_in, int fd_out, const char *filename, int isLastFile)
{
    char buffer[BUFF_SIZE];
    int bytesRead = -1;

    if (!isLastFile) {
        write(fd_out, "\n", 1);  // 在追加前插入一个空行
    }

    while ((bytesRead = read(fd_in, buffer, BUFF_SIZE)))
    {
        if (write(fd_out, buffer, bytesRead) != bytesRead)
        {
            fprintf(stderr, "写入时发生错误！\n");
            exit(1);
        }
    }

    if (!isLastFile) {
        write(fd_out, "\033[31m\n================================\033[0m", strlen("\033[31m\n================================\033[0m"));
    }

    write(fd_out, "\n", 1);

    if (bytesRead < 0)
    {
        fprintf(stderr, "读入时发生错误！\n");
        exit(1);
    }
}

