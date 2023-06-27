/*
* mv命令的自定义实现
* @author：谢小鹏、梁亮、徐璟逸
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILENAME_LENGTH 100

// 获取文件名
char* getFileName(const char* fileName);

// main
int main(int argc, char* argv[])
{
    struct stat st;
    if (argc != 3)
    {
        fprintf(stderr, "参数错误:参数数量错误\n");
        return 1;  // 返回非零值表示错误
    }
    // 查看参数1相关
    if (stat(argv[1], &st) == -1 || S_ISDIR(st.st_mode))
    {
        fprintf(stderr, "参数错误:参数1不是文件或者该文件不存在\n");
        return 1;  // 返回非零值表示错误
    }
    // 查看参数2相关
    if (stat(argv[2], &st) != -1)
    {
        // 参数2是目录，构造新的文件路径
        if (S_ISDIR(st.st_mode))
        {
            if (strlen(argv[2]) + strlen(getFileName(argv[1])) + 2 > MAX_FILENAME_LENGTH)
            {
                fprintf(stderr, "目标路径过长，无法构造新文件路径\n");
                return 1;  // 返回非零值表示错误
            }
            strcat(argv[2], "/");
            strcat(argv[2], getFileName(argv[1]));
        }
        else
        {
            // 参数2是已存在的文件，提示用户是否覆盖
            printf("参数2所指文件已存在，是否覆盖？(y/n): ");
            char choice;
            scanf(" %c", &choice);
            if (choice != 'y' && choice != 'Y')
            {
                printf("操作已取消\n");
                return 1;  // 返回非零值表示错误
            }
        }
    }
    // 直接使用 rename
    if (rename(argv[1], argv[2]) == -1)
    {
        fprintf(stderr, "移动失败\n");
        return 1;  // 返回非零值表示错误
    }
    return 0;
}

// 获取文件名
char* getFileName(const char* fileName)
{
    char tp[MAX_FILENAME_LENGTH];
    int i, j = 0;
    for (i = strlen(fileName) - 1; i >= 0; --i)
    {
        if (fileName[i] == '/')
        {
            break;
        }
    }
    for (i++; i < strlen(fileName); ++i)
    {
        tp[j++] = fileName[i];
    }
    tp[j] = '\0';
    char* name = (char*)malloc((strlen(tp) + 1) * sizeof(char));
    strcpy(name, tp);
    return name;
}