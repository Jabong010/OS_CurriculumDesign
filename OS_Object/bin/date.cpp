/*
* date命令的自定义实现
* @author：谢小鹏、梁亮、徐璟逸
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main()
{
    time_t t;
    struct tm *p;
    char current_time[80];
    
    time(&t);
    p = localtime(&t);
    
    // 设置时区
    setenv("TZ", "CST-8", 1); //设置时区TZ，将时区设置为CST-8(东八区的时区) China Standard Time
    tzset();                  //更新时区，根据环境变量 TZ 的值重新初始化时区信息，
	                          //使程序在后续的时间操作中基于新的时区设置进行计算和显示

    // 格式化日期和时间后存进current_time sizeof存储字节数保证不会越界
    strftime(current_time,sizeof(current_time), "%Y年 %m月 %d日 星期%u %H:%M:%S %Z", p);
    
    printf("%s\n", current_time);
    return 0;
}

