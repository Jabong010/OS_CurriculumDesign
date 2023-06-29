#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
//读取命令行输入
bool readCMD(char* buf, int buf_len);
// 解析命令行参数
int cutCMD(char* buf, char** argv);
//判断命令类型
int judgeCMD(int argc,char** argv);
//执行命令
void forkCMD(char** argv, bool background);
//将历史命令记录到cmd_list中
void recordCMD(const char* cmd, std::list<const char*>& cmd_list, int max_size);
//系统调用
void runCMD(const char* cmd);
//获取历史命令的数量
int getHistorySize(const std::list<const char*>& cmd_list);
//退出系统
void exit_OurShell();
int main()
{
    const int max_size = 10; //最多记录十条历史命令
    char buf[1024]; //暂存输入的命令
    //存放历史命令的列表
    std::list<const char*> cmd_list; 
    while (true)
    {
        if (!readCMD(buf, sizeof(buf)))
            continue;

        recordCMD(buf, cmd_list, max_size);

        char* MyArgv[32];
        int argc = cutCMD(buf, MyArgv);
        int cmd_type = judgeCMD(argc,MyArgv);
        if (cmd_type == 1) // exit
            exit_OurShell();
        if (cmd_type == 2) // history
        {
            int history_size = getHistorySize(cmd_list);
            printf("Command History:\n");
            for (const char* cmd : cmd_list)
            {
                printf("%s\n", cmd);
            }
            printf("Total commands: %d\n", history_size);
            continue;
        }
        if (cmd_type == 3) // !!
        {
            if (cmd_list.empty())
            {
                printf("No commands in history.\n");
                continue;
            }
            const char* last_cmd = cmd_list.back();
            if (strcmp(last_cmd, "!!") == 0)
            {
                if (cmd_list.size() < 2)
                {
                    printf("No previous command in history.\n");
                    continue;
                }
                last_cmd = *(++cmd_list.rbegin());
            }
            runCMD(last_cmd);
            continue;
        }
        if (cmd_type == 4) // !n
        {
            if (cmd_list.empty())
            {
                printf("No commands in history.\n");
                continue;
            }
            int n = atoi(&MyArgv[0][1]);
            if (n < 1 || n > getHistorySize(cmd_list))
            {
                printf("Invalid history command index.\n");
                continue;
            }
            auto it = cmd_list.begin();
            std::advance(it, n - 1);
            runCMD(*it);
            continue;
        }
        bool background = (cmd_type == 5); // 后台执行
        forkCMD(MyArgv, background);
    }

    return 0;
}
//读取命令行输入
bool readCMD(char* buf, int buf_len)
{
    printf("Our_Shell>");
    fflush(stdout);
    memset(buf, 0, buf_len);
    if (fgets(buf, buf_len, stdin) == nullptr)
    {
        perror("fgets error");
        exit(1);
    }
    //去除换行符
    buf[strcspn(buf, "\n")] = '\0';
    return strlen(buf) > 0;
}
//解析命令行参数
int cutCMD(char* buf, char** argv)
{
    int argc = 0;
    const char* delimiter = " ";
    char* token = strtok(buf, delimiter);
    while (token != nullptr)
    {
        argv[argc++] = token;
        token = strtok(nullptr, delimiter);
    }
    argv[argc] = nullptr;
    //检查最后一个参数是否为 "&"，如果是，则将其设置为 nullptr
    if (argc > 0 && strcmp(argv[argc-1], "&") == 0)
    {
        argv[argc-1] = nullptr;
        argc--;
    }
    return argc;
}
//判断命令类型
int judgeCMD(int argc,char** argv)
{
    if (argv[0] == nullptr)
        return -1;
    if (strcmp(argv[0], "exit") == 0)
        return 1;
    if (strcmp(argv[0], "history") == 0)
        return 2;
    if (strcmp(argv[0], "!!") == 0)
        return 3;
    if (argv[0][0] == '!')
        return 4;
    if (argv[argc-1] != nullptr && strcmp(argv[argc-1], "&") == 0)
        return 5;
    return 0;
}
//执行命令
void forkCMD(char** argv, bool background)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (pid == 0)
    {
        char filename[1024];
        sprintf(filename, "%s%s", "./bin/", argv[0]);

        //子进程
        if (execvp(filename, argv) < 0)
        {
            perror("execvp error");
            exit(1);
        }
    }
    else
    {
        //父进程
        if (!background) {
            int status;
            waitpid(pid, &status, 0);
        }
    }
}
//将历史命令记录到cmd_list中
void recordCMD(const char* cmd, std::list<const char*>& cmd_list, int max_size)
{
    char* cmd_copy = strdup(cmd);  //复制命令字符串
    cmd_list.push_back(cmd_copy);
    if (cmd_list.size() > static_cast<size_t>(max_size))
    {
        const char* oldest_cmd = cmd_list.front();
        cmd_list.pop_front();
        free(const_cast<char*>(oldest_cmd));  //释放最旧的历史命令副本
    }
}
void runCMD(const char* cmd)
{
    if (system(cmd) < 0)
    {
        perror("system error");
        exit(1);
    }
}
//获取历史命令的数量
int getHistorySize(const std::list<const char*>& cmd_list)
{
    return static_cast<int>(cmd_list.size());
}
//退出系统
void exit_OurShell()
{
    printf("GoodBye~Master~~~\n");
    exit(0);
}