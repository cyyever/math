/*
 *	程序名：my_log.h
 *	作者：陈源源
 *	日期：2013-02-09
 *	功能：日志相关函数
 */
#ifndef MY_LOG_H
#define MY_LOG_H

/*
 * 	功能：写入日志到syslog
 * 	参数：
 * 		fmt：格式字符串
 * 		...：可变参数
 * 	返回值：
 * 		无
 */
void _my_log(const char* fmt,...);

#define my_log(fmt,...) _my_log("{%s=>%s=>%d} "fmt,__FILE__,__func__,__LINE__,##__VA_ARGS__)

#endif
