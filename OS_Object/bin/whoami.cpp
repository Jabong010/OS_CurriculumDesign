/*
* whoami命令的自定义
* @author：谢小鹏、梁亮、徐璟逸
*/
#include<stdio.h>
#include<unistd.h>
#include<pwd.h>

int main(int argc, char* argv[])
{
	int u;
	struct passwd* pass;
	pass = getpwuid(getuid());
	const char *optstring = "ngGu";
	while ((u = getopt(argc, argv, optstring)) != -1)
	{
		switch(u)
		{
          case 'n':
		    printf("%s\n",pass->pw_name);
		    break;
		  case 'g':
		    printf("%s\n",pass->pw_passwd);
		    break;
		  case 'G':
		    printf("主目录:%s\n",pass->pw_dir);
			printf("用户描述:%s\n",pass->pw_gecos);
			printf("用户名:%s\n",pass->pw_name);
			printf("Shell路径:%s\n",pass->pw_shell);
			printf("密码:%s\n",pass->pw_passwd);
			printf("用户ID:%ld\n",(long)pass->pw_uid);
			printf("组ID:%ld\n",(long)pass->pw_gid);
			break;
		  case 'u':
		    printf("%u\n",pass->pw_uid);
		     

	    }
	return 0;
	}
}