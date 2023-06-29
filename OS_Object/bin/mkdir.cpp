/*
* cat命令的自定义实现
* @author：谢小鹏、梁亮、徐璟逸
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    if (argc == 1) 
    {
        fprintf(stderr, "参数错误: 参数数量错误\n");
        return 1;
    }

    for(int i = 1; i < argc; i++)
    {
        const char* dirName = argv[i];
        
        if (mkdir(dirName, 0700) == -1) //0700二进制存取所有者、组、其他用户权限
        {
            fprintf(stderr, "文件夹 \"%s\" 创建失败\n", dirName);
            return 1;
        }
        else
        {
            printf("文件夹 \"%s\" 创建成功\n", dirName);
        }
    }
    
    return 0;
}
