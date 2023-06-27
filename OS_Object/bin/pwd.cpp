/*
* pwd命令的自定义实现
* @author：谢小鹏、梁亮、徐璟逸
*/
#include<stdio.h>
#include<stdlib.h>
#include<cstring>
#include<unistd.h>
#include<pwd.h>

int main(int argc, char* argv[])
{
	int bufsize = 128;
	char *buffer = (char*)malloc(sizeof(char)*bufsize);
    	if (!buffer)
    	{
       	fprintf(stderr,"malloc内存分配发生错误\n");
       	exit(1);
    	}
    	while (1)
    	{
       	if(getcwd(buffer, bufsize) == NULL)
       		{
            		bufsize += bufsize;
            		buffer = (char*)realloc(buffer, sizeof(char)*bufsize);
            		if (!buffer)
            		{
                		fprintf(stderr,"realloc内存分配发送错误\n");
                		exit(1);
                	}
        	}
        	else
        	{
            		printf("%s\n", buffer);
            		free(buffer);
            		break;
        	}
    	}
	return 0;
}
