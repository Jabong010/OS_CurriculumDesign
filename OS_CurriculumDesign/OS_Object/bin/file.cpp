/*
* file命令的自定义实现
* @author：谢小鹏、梁亮、徐璟逸
*/
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

//获取文件类型
const char *getFileType(const char *fileName);
//获取文件权限字符串
const char *getFilePermissions(const struct stat *st);
//获取文件所有者用户名
const char *getFileOwner(const struct stat *st);
//获取文件最后修改时间
const char *getFileGroup(const struct stat *st);
//获取文件大小
off_t getFileSize(const struct stat *st);
//获取文件最后访问时间
const char *getFileAccessTime(const struct stat *st);
//获取文件最后修改时间
const char *getFileModificationTime(const struct stat *st);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "参数数量错误，请检查是否输入错误");
		return 1;
	}

	const char *fileName = argv[1];
	struct stat st;
	if (stat(fileName, &st) == 0)
	{ // 若是文件或目录，则获取其文件信息并打印出来
		const char *fileType = getFileType(fileName);
		const char *filePermissions = getFilePermissions(&st);
		const char *fileOwner = getFileOwner(&st);
		const char *fileGroup = getFileGroup(&st);
		off_t fileSize = getFileSize(&st);
		const char *fileAccessTime = getFileAccessTime(&st);
		const char *fileModificationTime = getFileModificationTime(&st);

		printf("FileName(文件名):              %s\n", fileName);
		printf("Type(文件类型):                %s\n", fileType);
		printf("Permissions(文件权限):         %s\n", filePermissions);
		printf("Owner(文件所属者名):           %s\n", fileOwner);
		printf("Group(文件所属组名):           %s\n", fileGroup);
		printf("Size(文件大小):                %ld bytes\n", (long)fileSize);
		printf("Last accessed(上次访问时间):   %s\n", fileAccessTime);
		printf("Last modified(上次修改时间):   %s\n", fileModificationTime);
	}
	else
	{
		fprintf(stderr, "File not found: %s\n", fileName);
		return 1;
	}

	return 0;
}

//获取文件类型
const char *getFileType(const char *fileName)
{
	struct stat st;
	if (stat(fileName, &st) == 0)
	{
		if (S_ISREG(st.st_mode))
			return "regular file";
		else if (S_ISDIR(st.st_mode))
			return "directory";
		else if (S_ISCHR(st.st_mode))
			return "character device";
		else if (S_ISBLK(st.st_mode))
			return "block device";
		else if (S_ISFIFO(st.st_mode))
			return "FIFO/pipe";
		else if (S_ISLNK(st.st_mode))
			return "symbolic link";
		else if (S_ISSOCK(st.st_mode))
			return "socket";
	}
	return "unknow";
}

//获取文件权限字符串
const char *getFilePermissions(const struct stat *st)
{
	static char permissions[10];
	strcpy(permissions, "----------");
	mode_t mode = st->st_mode;
	if (S_ISDIR(mode))
		permissions[0] = 'd';
	else if (S_ISLNK(mode))
		permissions[0] = 'l';

	if (mode & S_IRUSR)
		permissions[1] = 'r';
	if (mode & S_IWUSR)
		permissions[2] = 'w';
	if (mode & S_IXUSR)
		permissions[3] = 'x';

	if (mode & S_IRGRP)
		permissions[4] = 'r';
	if (mode & S_IWGRP)
		permissions[5] = 'w';
	if (mode & S_IXGRP)
		permissions[6] = 'x';

	if (mode & S_IROTH)
		permissions[7] = 'r';
	if (mode & S_IWOTH)
		permissions[8] = 'w';
	if (mode & S_IXOTH)
		permissions[9] = 'x';

	return permissions;
}

//获取文件所有者用户名
const char *getFileOwner(const struct stat *st)
{
	struct passwd *pw = getpwuid(st->st_uid);
	if (pw != NULL)
		return pw->pw_name;
	return "unknown";
}

//获取文件所有者所属组名
const char *getFileGroup(const struct stat *st)
{
	struct group *grp = getgrgid(st->st_gid);
	if (grp != NULL)
		return grp->gr_name;
	return "unknown";
}

//获取文件大小
off_t getFileSize(const struct stat *st)
{
	return st->st_size;
}

//获取文件最后访问时间
const char *getFileAccessTime(const struct stat *st)
{
	static char accessTime[20];
	time_t t = st->st_atime;
	struct tm *tm = localtime(&t);
	strftime(accessTime, sizeof(accessTime), "%Y-%m-%d %H:%M:%S", tm);
	return accessTime;
}

//获取文件最后修改时间
const char *getFileModificationTime(const struct stat *st)
{
	static char modTime[20];
	time_t t = st->st_mtime;
	struct tm *tm = localtime(&t);
	strftime(modTime, sizeof(modTime), "%Y-%m-%d %H:%M:%S", tm);
	return modTime;
}
