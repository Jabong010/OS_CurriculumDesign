/*
* 简单实现whoami命令
*/
#include<stdio.h>
#include<unistd.h>
#include<pwd.h>

int main(int argc, char* argv[])
{
	struct passwd* pass;
	pass = getpwuid(getuid());
	printf("%s\n",pass->pw_name);
	return 0;
}
