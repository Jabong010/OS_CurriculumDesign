/*
* whoami命令的自定义实现
* @author：谢小鹏、梁亮、徐璟逸
*/
#include<stdio.h>
#include<unistd.h>
#include<pwd.h>

int main(int argc, char* argv[])
{
	struct passwd* pass;          //struct passwd 是一个定义了用户信息的结构体类型。
	                              //它通常用于存储与用户相关的数据，如用户名、用户ID、主目录路径等。
	pass = getpwuid(getuid());    //getuid()函数返回当前用户的用户ID（UID）
	                              //getpwuid()函数接受一个UID作为参数，并返回一个指向 passwd 结构的指针，其中包含了与该UID关联的用户信息。
	printf("%s\n",pass->pw_name);
	return 0;
}
