/*
* vi命令的自定义实现
* @author：谢小鹏、梁亮、徐璟逸
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_LINE_LENGTH 1000
bool is_LineFeed=true; //用于判断是否要换行，若是最后一行输入则不用换行
void appendLine(int fd, const char* line);
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "参数数量错误\n");
        return 1;
    }
    bool file_isExist = true; //文件是否存在的标志，若存在则是true
    const char* fileName = argv[1];  //获取文件姓名
    struct stat st1;  //存储文件信息的对象
    if (stat(fileName, &st1) == -1)
        file_isExist = false;
        
    int fd = open(fileName, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        fprintf(stderr, "无法打开文件 %s\n", fileName);
        return 1;
    }
    struct stat st;
    if (stat(fileName, &st) == -1) {
        fprintf(stderr, "无法获取文件信息 %s\n", fileName);
        return 1;
    }
    // 检查文件类型
    if (S_ISDIR(st.st_mode)) {
        printf("%s 是一个目录，无法编辑\n", fileName);
        close(fd);
        return 1;
    }
    printf("欢迎使用简单文本编辑器（输入':q'保存并退出）\n");
    if (!file_isExist) {
        printf("%s文件不存在,已成功创建\n", fileName);
        printf("请输入内容\n");
    } else {
        printf("%s文件存在,是否覆盖该文件内容(y/n)\n", fileName);
        char op;
        scanf(" %c", &op);
        getchar();
        if (op == 'n' || op == 'N') {
            // 显示文件内容
            printf("原文件内容：\n");
            char buffer[MAX_LINE_LENGTH];
            ssize_t bytesRead;
            while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
                buffer[bytesRead] = '\0';
                printf("%s", buffer);
            }
            printf("请输入内容(以追加的形式)\n");
            // 将文件指针移动到文件末尾
            lseek(fd, 0, SEEK_END);
        }
        else
        {
            close(fd);
            if(remove(fileName)!=0)
            perror("remove");
            fd = open(fileName, O_RDWR | O_CREAT, 0644);
            printf("下面输入该文件新内容\n");
        }
    }
    while (1) {
        printf("> ");
        char inputLine[MAX_LINE_LENGTH];
        fgets(inputLine, sizeof(inputLine), stdin);
        // 检查是否输入保存命令
        if (strcmp(inputLine, ":q\n") == 0) {
            close(fd);
            printf("文件已保存\n");
            is_LineFeed=false;
            break;
        }
        // 移除换行符
        inputLine[strcspn(inputLine, "\n")] = '\0';
        // 添加新行
        appendLine(fd, inputLine);
    }
    return 0;
}
void appendLine(int fd, const char* line) {
    write(fd, line, strlen(line));
    if(is_LineFeed)
    write(fd, "\n", 1);
}

