/*
* 简单实现pwd命令
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
       	printf("allocation error1\n");
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
                		printf("allocation error2\n");
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
