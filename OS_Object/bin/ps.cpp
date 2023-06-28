/*
* ps命令及ps axjf、ps aux的自定义实现
* @author：谢小鹏、梁亮、徐璟逸
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LEN 20

// 信息结构体
typedef struct ps_info {
    char pname[MAX_LEN];
    char user[MAX_LEN];
    int pid;
    int ppid;
    char state;
    struct ps_info* next;
} mps;

// 获取信息
mps* trav_dir(char dir[]);

// 读取信息
int read_info(char d_name[], struct ps_info* p1);

// 根据进程uid获取进程的所有者user
void uid_to_name(uid_t uid, struct ps_info* p1);

// 判断name是否为纯数字
int is_num(const char* name);

// 显示
void print_ps(struct ps_info* head);

// 执行命令
void execute_ps(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    if (argc == 1) {
        mps* head;
        head = trav_dir("/proc/");
        if (head == NULL) {
            printf("traverse dir error\n");
        } else {
            print_ps(head);
            mps* current = head;
            while (current != NULL) {
                mps* next = current->next;
                free(current);
                current = next;
               }
              }
    } else {
        execute_ps(argc, argv);
    }
    return 0;
}

mps* trav_dir(char dir[]) {
    DIR* dir_ptr;
    mps* head = NULL;
    mps* tail = NULL;
    struct dirent* direntp;
    struct stat infobuf;

    if ((dir_ptr = opendir(dir)) == NULL) {
        fprintf(stderr, "dir error %s\n", dir);
    } else {
        while ((direntp = readdir(dir_ptr)) != NULL) {
            if (is_num(direntp->d_name) == 0) {
                mps* p1 = (struct ps_info*)malloc(sizeof(struct ps_info));
                if (p1 == NULL) {
                    printf("malloc error!\n");
                    exit(0);
                }
                if (read_info(direntp->d_name, p1) != 0) {
                    printf("read_info error\n");
                    exit(0);
                }
                if (head == NULL) {
                    head = p1;
                    tail = p1;
                } else {
                    tail->next = p1;
                    tail = p1;
                }
            }
        }
        closedir(dir_ptr);
    }
    if (tail != NULL) {
        tail->next = NULL;
    }
    return head;
}

int read_info(char d_name[], struct ps_info* p1) {
    char dir[20];
    struct stat infobuf;

    sprintf(dir, "%s/%s", "/proc/", d_name);
    chdir("/proc");
    if (stat(d_name, &infobuf) == -1) {
        fprintf(stderr, "stat error %s\n", d_name);
    } else {
        uid_to_name(infobuf.st_uid, p1);
    }

    chdir(dir);
    FILE* fd = fopen("stat", "r");
    if (fd == NULL) {
        printf("open the file is error!\n");
        exit(0);
    }
    if (fscanf(fd, "%d %s %c %d\n", &(p1->pid), p1->pname, &(p1->state), &(p1->ppid)) == 4) {
        fclose(fd);
        return 0;
    } else {
        fclose(fd);
        return -1;
    }
}

void uid_to_name(uid_t uid, struct ps_info* p1) {
    struct passwd* pw_ptr;
    static char numstr[10];

    if ((pw_ptr = getpwuid(uid)) == NULL) {
        sprintf(numstr, "%d", uid);
        strcpy(p1->user, numstr);
    } else {
        strcpy(p1->user, pw_ptr->pw_name);
    }
}

int is_num(const char* name) {
    int i, len;
    len = strlen(name);
    if (len == 0) {
        return -1;
    }
    for (i = 0; i < len; i++) {
        if (name[i] < '0' || name[i] > '9') {
            return -1;
        }
    }
    return 0;
}

void print_ps(struct ps_info* head) {
    printf("USER\t\tPID\tPPID\tSTATE\tPNAME\n");
    for (struct ps_info* list = head; list != NULL; list = list->next) {
        printf("%s\t\t%d\t%d\t%c\t%s\n", list->user, list->pid, list->ppid, list->state, list->pname);
    }
}

void execute_ps(int argc, char* argv[]) {
    if (strcmp(argv[1], "axjf") == 0) {
        // 执行 ps axjf 命令
        char* new_argv[] = {"ps", "axjf", NULL};
        execvp("ps", new_argv);
    } else if (strcmp(argv[1], "aux") == 0) {
        // 执行 ps aux 命令
        char* new_argv[] = {"ps", "aux", NULL};
        execvp("ps", new_argv);
    } else {
        // 执行其他命令
        execvp(argv[0], argv);
    }
}
