/*
* rm和rm -r命令的自定义实现
* @author：谢小鹏、梁亮、徐璟逸
*/
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<dirent.h>
#include<unistd.h>
#include<string.h>

// 删除指定文件
int delete_file(char *fileName);

// 删除指定目录
int delete_directory(char *dirName);

int main(int argc, char *argv[])
{
    // 没有参数
    if (argc == 1)
    {
        fprintf(stdout, "参数数量错误，请检查输入\n");
    }
    // 至少一个参数
    else
    {
        int i = 1;
        // 检查是否有-r参数
        if (strcmp(argv[1], "-r") == 0)
        {
            // 参数数量小于等于2，无法删除目录
            if (argc <= 2)
            {
                fprintf(stdout, "参数数量错误，请检查输入\n");
                return 0;
            }

            i = 2; // 从第三个参数开始处理
            for (; i < argc; ++i)
            {
                if (delete_directory(argv[i]) == -1)
                {
                    fprintf(stdout, "删除目录%s失败\n", argv[i]);
                }
            }
        }
        else
        {
            for (; i < argc; ++i)
            {
                if (delete_file(argv[i]) == -1)
                {
                    fprintf(stdout, "删除文件%s失败\n", argv[i]);
                }
            }
        }
    }
}

// 删除指定文件
int delete_file(char *fileName)
{
    char file_path[128];
    strcpy(file_path, fileName);
    struct stat st;
    // 找不到文件
    if (lstat(file_path, &st) == -1)
    {
        return -1;
    }
    // 是否为常规文件
    if (S_ISREG(st.st_mode))
    {
        // unlink失败
        if (unlink(file_path) == -1)
        {
            return -1;
        }
    }
    // 是否为文件夹
    else if (S_ISDIR(st.st_mode))
    {
        fprintf(stdout, "无法删除：%s 为一个目录\n", fileName);
        return -1;
    }
    return 0;
}

// 删除指定目录
int delete_directory(char *dirName)
{
    DIR *dir;
    struct dirent *entry;
    char file_path[128];

    if ((dir = opendir(dirName)) == NULL)
    {
        fprintf(stdout, "无法打开目录：%s\n", dirName);
        return -1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(file_path, sizeof(file_path), "%s/%s", dirName, entry->d_name);

        if (delete_file(file_path) == -1)
        {
            if (delete_directory(file_path) == -1)
            {
                fprintf(stdout, "删除目录%s失败\n", file_path);
            }
        }
    }

    closedir(dir);

    if (rmdir(dirName) == -1)
    {
        return -1;
    }

    return 0;
}
