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
typedef struct my_node_s
{
	int32_t data;
	struct my_node_s *prev;	//上一个
	struct my_node_s *next; //下一个
}my_node;

//有理数表示成 (sign) (lsn ... msn)*(10^5)^power，10^5进制
typedef struct
{
	my_node *lsn;		//最低node
	my_node *msn;		//最高node
	ssize_t power; 		//指数
	int8_t sign; 		//1-正数 0-未初始化 -1-负数
	size_t used_node_num;	//已使用节点数量
	size_t total_node_num;	//总节点数量
}my_rat;

//舍入模式
typedef enum{
	MY_TRUNC=0,	//截断
}my_round_mode;

#define MY_RAT_TOTAL_NODE_NUM(rat) ((rat)?((rat)->total_node_num):0)
#define MY_RAT_USED_NODE_NUM(rat) ((rat)?((rat)->used_node_num):0)
#define MY_RAT_FREE_NODE_NUM(rat) ((rat)?((rat)->total_node_num-(rat)->used_node_num):0)
#define MY_RAT_INIT(rat) do {(rat)->msn=(rat)->lsn;(rat)->used_node_num=0;(rat)->sign=0;(rat)->power=0;} while(0)
#define MY_RAT_HAS_INITED(rat) ((rat)->sign!=0)
#define MY_RAT_DIGIT_NUM(node) ((node)->data>=1000? 4 : ((node)->data>=100 ? 3: ((node)->data>=10?2:1)))

/*
 *	功能：增加有理数的节点
 *	参数：
 *		n：要处理的有理数
 *		num：增加的节点数
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
int my_rat_add_node(my_rat *n,size_t num);

/*
 *	功能：去除有理数的两端的0数据节点
 *	参数：
 *		n：要处理的有理数
 *	返回值：
 *		无
 */
void my_rat_strip_zero_end_nodes(my_rat* n);

/*
 *	功能：释放有理数
 *	参数：
 *		n：要释放的有理数
 *	返回值：
 * 		无
 */
void my_rat_free(my_rat *n);

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
my_rat *my_rat_from_str(my_rat *n,const char *str);

/*
 *	功能：把my_rat转换为字符串
 *	参数：
 *		n：要处理的有理数
 *	返回值：
 * 		非NULL：字符串
 * 		NULL：出错
 */
char *my_rat_to_str(my_rat *n);

/*
 * 	功能：把int64_t转换为有理数
 * 	参数：
 *		n：	
 *			NULL：新建有理数并返回
 *			非NULL：保存有理数于此
 *		num：要处理的数字
 *	返回值：
 * 		非NULL：返回生成的有理数
 * 		NULL：出错
 */
my_rat *my_rat_from_int64(my_rat *n,int64_t num);

/*
 *	功能：复制有理数b
 *	参数：
 *		dest：目的有理数，取以下值：
 *			NULL：返回新建的副本
 *			非NULL：副本保存于此
 *		src：源有理数
 *	返回值：
 *		非NULL：返回副本
 *		NULL：出错
 */
my_rat* my_rat_copy(my_rat* dest,my_rat* src);

/*
 *	功能：减少有理数指数部分，作为0增加到整数部分
 *	参数：
 *		n：要处理的有理数
 *		delta:指数减少量
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
int my_rat_reduce_power(my_rat *n,size_t delta);

/*
 *	功能：舍入有理数到指定小数位数
 *	参数：
 *		n：要处理的有理数
 *		fraction：小数位数
 *		round_mode：舍入模式
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
int my_rat_round(my_rat *n,ssize_t fraction,my_round_mode mode);
#endif
