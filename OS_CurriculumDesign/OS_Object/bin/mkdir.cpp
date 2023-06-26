/*
* mkdir命令的自定义实现
* @author：谢小鹏、梁亮、徐璟逸
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "参数错误: 参数数量错误\n");
        return 1;
    }

    const char* dirName = argv[1];

    // 创建文件夹
    if (mkdir(dirName, 0700) == -1) {
        fprintf(stderr, "创建文件夹失败\n");
        return 1;
    }

    printf("文件夹 \"%s\" 创建成功\n", dirName);
    return 0;
}
