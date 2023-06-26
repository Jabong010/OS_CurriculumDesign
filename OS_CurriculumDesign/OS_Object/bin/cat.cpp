/*
* cat命令的自定义实现
* @author：谢小鹏、梁亮、徐璟逸
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFF_SIZE 4096

//读取fd_in文件里的内容并写入到fd_out文件中
void filecopy(int fd_in, int fd_out);

//main
int main(int argc, char *argv[])
{
	int fd_in = STDIN_FILENO;    //设置Standard input(键盘输入)
	int fd_out = STDOUT_FILENO;  //设置Standard output(屏幕输出)
	
	if (argc == 1)   //一个参数，代表用户只输入了cat命令，后面并未接文件
	{
		filecopy(fd_in, fd_out);
		close(fd_in);
	}
	else            //多个参数，cat命令后面接了一个或多个文件
	{
		for(int i = 1; i < argc; ++i)
		{
			fd_in = open(argv[i], O_RDONLY);  //以读的方式打开输入文件
			if(fd_in < 0)
			{
				fprintf(stderr, "打开%s文件时发生错误！\n", argv[i]);
				continue;
			}
			filecopy(fd_in, fd_out);
			close(fd_in);
		}
	}
	return 0;
}

//读取fd_in文件里的内容并写入到fd_out文件中
void filecopy(int fd_in, int fd_out)
{
	char buffer[BUFF_SIZE];
	int bytesRead=-1;
	
	while(bytesRead = read(fd_in, buffer, BUFF_SIZE))
	{
		if(write(fd_out, buffer, bytesRead) != bytesRead)
		{
			fprintf(stderr, "写入时发生错误！\n");
			exit(1);
		}                                                                                                                                                 
	}
	//当读入出现问题
	if(bytesRead<0)
	{
		fprintf(stderr, "读入时发生错误！\n");
  		exit(1);
	}
}