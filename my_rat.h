/*
 *	程序名：my_rat.h
 *	作者：陈源源
 *	日期：2013-02-09
 *	功能：有理数相关定义和函数
 */
#ifndef MY_RAT_H
#define MY_RAT_H

#include <stdlib.h>
#include "my_math.h"

//有理数数据节点
struct my_node_s
{
	int32_t data;
	struct my_node_s* prev;	//上一个
	struct my_node_s* next; //下一个
};
typedef struct my_node_s* my_node;

//有理数表示成 (sign) (lsn ... msn)*(10^5)^power，10^5进制
struct my_rat_s
{
	my_node lsn;		//最低node
	my_node msn;		//最低node
	ssize_t power; 		//指数
	int8_t sign; 		//1-正数 0-未初始化 -1-负数
	size_t used_node_num;	//已使用节点数量
	size_t total_node_num;	//总节点数量
};
typedef struct my_rat_s* my_rat;

#define MY_RAT_TOTAL_NODE_NUM(n) ((n)?((n)->total_node_num):0)
#define MY_RAT_USED_NODE_NUM(n) ((n)?((n)->used_node_num):0)
#define MY_RAT_FREE_NODE_NUM(n) ((n)?((n)->total_node_num-(n)->used_node_num):0)
#define MY_RAT_INIT(n) do {(n)->msn=(n)->lsn;(n)->used_node_num=0;(n)->sign=0;(n)->power=0;} while(0)
#define MY_RAT_HAS_INITED(n) ((n)->sign !=0)


/*
 *	功能：释放有理数
 *	参数：
 *		n：要释放的有理数
 *	返回值：
 * 		无
 */
void my_rat_free(my_rat n);

/*
 * 	功能:把字符串转换为my_rat
 * 	参数：
 *		n:	
 *			NULL：新建有理数并返回
 *			非NULL：保存有理数于此
 *		str:要转换的字符串
 *	返回值：
 * 		非NULL：返回生成的有理数
 * 		NULL：出错
 */
my_rat my_rat_from_str(my_rat n,const char* str);

/*
 *	功能：把my_rat转换为字符串
 *	参数：
 *		n：要处理的有理数
 *	返回值：
 * 		非NULL：字符串
 * 		NULL：出错
 */
char* my_rat_to_str(my_rat n);
#endif
