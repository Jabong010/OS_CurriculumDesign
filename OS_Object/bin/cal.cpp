/*
* cal命令的自定义实现
* @author：谢小鹏、梁亮、徐璟逸
*/
#include <stdio.h>
#include <time.h>
 
void showCal(int year, int month, int day);

int main()
{
	time_t t;
	struct tm *current_time;

	time(&t); // 获取时间戳 1970.1.1至今秒数
	current_time = localtime(&t); // gmtime()也使用UTC，0时区

	int year = current_time->tm_year + 1900; // tm_year是从1900年开始计算的，所以year要加上1900
	int month = current_time->tm_mon + 1;	 // tm_mon是从0开始计算月数，所以要加1
	int day = current_time->tm_mday;

	showCal(year, month, day);

	return 0;
}

void showCal(int year, int month, int day)
{
	struct tm date = {0};
	date.tm_year = year - 1900;  // 还原成时间戳的year时间
	date.tm_mon = month - 1;     // 还原成tm结构体下从0开始计数的月份大小
	date.tm_mday = 1;            // 设置本月开始天数，从第一天开始

	mktime(&date);               // 根据时间更新成员变量，如星期几等等

	int start_day = date.tm_wday; // 一周中的第几天 周日为1
	int total_days = 0;

	switch (month)
	{
	case 2:
		if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
			total_days = 29;
		else
			total_days = 28;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		total_days = 30;
		break;
	default:
		total_days = 31;
		break;
	}
	printf("\n");
	printf("     %02d月 %d\n", month, year);
	printf("日 一 二 三 四 五 六\n");

	for (int i = 0; i < start_day; i++)
		printf("   ");

	for (int i = 1; i <= total_days; i++)
	{
		if (i == day)
			printf("\033[1;40;32m%2d\033[0m ", i); // 今日时期高亮显示
		else
			printf("%2d ", i);

		if ((i + start_day) % 7 == 0)
			printf("\n");
	}
	printf("\n\n");
}
