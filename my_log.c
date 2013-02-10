/*
 *	程序名：my_log.c
 *	作者：陈源源
 *	日期：2013-02-09
 *	功能：日志相关函数
 */
#include <stdarg.h>
#include <syslog.h>

/*
 * 	功能：写入日志到syslog
 * 	参数：
 * 		fmt：格式字符串
 * 		...：可变参数
 * 	返回值：
 * 		无
 */
void _my_log(const char* fmt,...)
{
	va_list ap;
	if(!fmt)
		return;
	va_start(ap,fmt);
	openlog("ln",LOG_PID,LOG_LOCAL0);
	va_start(ap, fmt);
	vsyslog(LOG_ERR,fmt,ap);
	closelog();
}
