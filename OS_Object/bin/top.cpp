/*
* top命令的自定义实现
* @author：谢小鹏、梁亮、徐璟逸
*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/select.h>

#define MAX_PROCESSES 20

// 进程信息结构体
typedef struct {
    char pid[256];
    char name[256];
    char state[256];
    char ppid[256];
    char priority[256];
    char threads[256];
    char vm_size[256];
} ProcessInfo;


// 比较函数，用于进程信息的排序
int compare(const void* a, const void* b);
// 清空终端
void clearTerminal();
// 获取进程信息
void getProcessInfo(const char* procPath, ProcessInfo* processInfo);


int main()
{
    ProcessInfo processes[MAX_PROCESSES];
 
    while (1) {
        clearTerminal();
        // 清空进程信息数组
        memset(processes, 0, sizeof(processes));

        // 打开 /proc 目录
        DIR* procDir = opendir("/proc");
        if (procDir == NULL) {
            perror("Failed to open /proc directory");
            exit(1);
        }

        int processCount = 0;
        struct dirent* entry;
        while ((entry = readdir(procDir)) != NULL && processCount < MAX_PROCESSES) {
            // 判断是否为数字命名的目录
            if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
                char procPath[256];
                snprintf(procPath, sizeof(procPath), "/proc/%s", entry->d_name);

                // 获取进程信息
                ProcessInfo processInfo;
                strcpy(processInfo.pid, entry->d_name);
                getProcessInfo(procPath, &processInfo);

                // 将进程信息添加到数组中
                processes[processCount++] = processInfo;
            }
        }

        closedir(procDir);

        // 更新进程数量
        int actualProcessCount = processCount;

        // 对进程信息进行排序
        qsort(processes, actualProcessCount, sizeof(ProcessInfo), compare);

        // 打印进程信息
        printf("PID(进程序号)\tState(进程状态)\tPPID(父进程ID)\tPriority(进程优先级)\tThreads(线程数)\tVMSize(虚拟内存大小)\tProcess Name(进程名)\n");
        printf("--------------------------------------------------------------------------------------------------------------------------\n");
        for (int i = 0; i < actualProcessCount; i++) {
            printf("  %s\t\t    %s\t\t    %s\t\t    %s\t\t\t   %s \t\t   %s\t\t   %s\n", processes[i].pid, processes[i].state,
                   processes[i].ppid, processes[i].priority, processes[i].threads,
                   processes[i].vm_size, processes[i].name);
        }

        // 刷新并等待一秒
        fflush(stdout);
        sleep(1);

        // 检查输入是否为 'q'
        fd_set inputSet;
        struct timeval timeout;
        int selectResult;

        FD_ZERO(&inputSet);
        FD_SET(STDIN_FILENO, &inputSet);

        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        selectResult = select(STDIN_FILENO + 1, &inputSet, NULL, NULL, &timeout);

        if (selectResult > 0) {
            char input;
            if (scanf(" %c", &input) == 1 && input == 'q') {
                break;  // 输入为 'q'，退出循环
            }
        }
    }

    return 0;
}

// 比较函数，用于进程信息的排序
int compare(const void* a, const void* b)
{
    ProcessInfo* infoA = (ProcessInfo*)a;
    ProcessInfo* infoB = (ProcessInfo*)b;

    int pidA = atoi(infoA->pid);
    int pidB = atoi(infoB->pid);

    return pidA - pidB;
}

// 清空终端
void clearTerminal()
{
    printf("\033[2J\033[H");
    fflush(stdout);
}

// 获取进程信息
void getProcessInfo(const char* procPath, ProcessInfo* processInfo)
{
    // 打开进程的状态文件，获取进程信息
    char statPath[256];
    snprintf(statPath, sizeof(statPath), "%s/stat", procPath);
    FILE* statFile = fopen(statPath, "r");
    if (statFile != NULL) {
        fscanf(statFile, "%*d %s %s %s %s %s", processInfo->name, processInfo->state,
               processInfo->ppid, processInfo->priority, processInfo->threads);
        fclose(statFile);
    }

    // 打开进程的状态文件，获取进程的虚拟内存大小
    char statusPath[256];
    snprintf(statusPath, sizeof(statusPath), "%s/status", procPath);
    FILE* statusFile = fopen(statusPath, "r");
    if (statusFile != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), statusFile) != NULL) {
            if (strncmp(line, "VmSize:", 7) == 0) {
                sscanf(line, "%*s %s", processInfo->vm_size);
                break;
            }
        }
        fclose(statusFile);
    }
}

