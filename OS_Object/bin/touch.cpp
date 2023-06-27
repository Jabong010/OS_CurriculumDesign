/*
* touch命令以及touch -acm的自定义实现
* @author：谢小鹏、梁亮、徐璟逸
*/
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <utime.h>
#include <time.h> // 添加time.h头文件

#define CH_ATIME 1
#define CH_MTIME 2
//touch -a用于更新文件的访问时间，如果文件不存在则创建文件。
//touch -c用于更新文件的时间戳，但不创建新文件。
//touch -m用于更新文件的修改时间，如果文件不存在则创建文件

// 定义创建文件时的默认权限
#define DEFAULT_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

// 标志文件access time和modify time的改变情况
static int change_times;

// 是否只更新时间而不创建文件 -c
static bool update_only;

//utimbuf 是一个定义在 <utime.h> 头文件中的结构体，用于存储文件的访问时间和修改时间。
static struct utimbuf new_times;    

// touch命令核心模块，用于创建或更新文件的时间戳
static bool mytouch(const char *file);

int main(int argc, char **argv) {
    int c;
    bool success = true;
    change_times = 0;
    update_only = false;

    // 解析命令行参数，目前只支持-a、-c、-m三个选项
    while ((c = getopt(argc, argv, "acm")) != -1) {
        switch (c) {
            case 'a':
                change_times |= CH_ATIME;
                break;
            case 'c':
                update_only = true;
                break;
            case 'm':
                change_times |= CH_MTIME;
                break;
            default:
                printf("Invalid option: -%c\n", optopt);
                return EXIT_FAILURE;
        }
    }

    if (change_times == 0) {  //如果没有解析到任何选项，即 change_times 仍为 0
	                          //则将 change_times 设置为 CH_ATIME | CH_MTIME，表示同时修改访问时间和修改时间。
        change_times = CH_ATIME | CH_MTIME;
    }

    // 设置新的时间戳
    time_t current_time = time(NULL); //获得当前时间
    new_times.actime = (change_times & CH_ATIME) ? current_time : -1;  //如果change_time的访问时间位为1,则更新文件访问时间为当前时间
    new_times.modtime = (change_times & CH_MTIME) ? current_time : -1; //如果change_time的修改时间位为1,则更新文件修改时间为当前时间
                                                                       //如果是-1，在更新文件时间时就不会进行更改
    // 检查是否提供了文件名
    if (optind == argc) {
        printf("Missing file operand\n");
        return EXIT_FAILURE;
    }

    // 针对每个文件名调用mytouch函数
    for (; optind < argc; ++optind) {
        success &= mytouch(argv[optind]);
    }

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

// touch命令核心模块，用于创建或更新文件的时间戳
static bool mytouch(const char *file) {
    int fd = -1;
    if (!update_only) {
        fd = open(file, O_CREAT | O_WRONLY, DEFAULT_MODE);
        if (fd == -1) {
            perror("Error creating file");
            return false;
        }
        close(fd);
    }
    //utime() 函数用于修改文件的访问时间和修改时间
    if (utime(file, &new_times) == -1) {
        perror("Error updating file timestamps");
        return false;
    }
    return true;
}
