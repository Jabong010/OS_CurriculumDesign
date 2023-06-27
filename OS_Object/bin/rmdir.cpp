/*
* rmdir命令的自定义实现
* @author：谢小鹏、梁亮、徐璟逸
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#define PATH_SIZE 4096

// 递归删除目录及其内容
void delete_directory(const char *path);

int main(int argc, char const *argv[])
{
    // 验证参数数量
    if (argc != 2)
    {
        fprintf(stdout, "参数错误: 参数数量错误\n");
        return 0;
    }

    // 验证路径的合法性
    if (access(argv[1], F_OK) != 0)
    {
        fprintf(stdout, "参数错误: 无效的路径\n");
        return 0;
    }

    // 删除目录
    delete_directory(argv[1]);

    return 0;
}

void delete_directory(const char *path)
{
    DIR *dir;
    struct dirent *entry;
    char file_path[PATH_SIZE];

    // 打开目录
    dir = opendir(path);
    if (dir == NULL)
    {
        fprintf(stdout, "无法打开目录：%s\n", path);
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // 跳过当前目录和上级目录
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // 构建文件的完整路径
        snprintf(file_path, PATH_SIZE, "%s/%s", path, entry->d_name);

        // 递归删除子目录或删除文件
        if (entry->d_type == DT_DIR)
        {
            delete_directory(file_path);
        }
        else if (entry->d_type == DT_REG || entry->d_type == DT_LNK)
        {
            if (unlink(file_path) != 0)
            {
                fprintf(stderr, "无法删除文件：%s (%s)\n", file_path, strerror(errno));
            }
            else
            {
                fprintf(stdout, "已删除文件：%s\n", file_path);
            }
        }
    }

    // 关闭目录
    closedir(dir);

    // 删除目录本身
    if (rmdir(path) != 0)
    {
        fprintf(stderr, "无法删除目录：%s (%s)\n", path, strerror(errno));
    }
    else
    {
        fprintf(stdout, "已删除目录：%s\n", path);
    }
}
