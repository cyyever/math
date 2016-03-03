/*
 *	程序名：my_arithmetic.c
 *	作者：陈源源
 *	日期：2013-02-10
 *	功能：包含算术运算操作
 */
#ifndef MY_ARITHMETIC_H
#define MY_ARITHMETIC_H

#include <inttypes.h>
#include "my_rat.h"

/*
 *	功能：比较有理数的大小
 *	参数：
 *		a，b：要比较的有理数
 *		cmp_res：比较结果：
 *			>0：a>b
 *			0：a=b
 *			<0：a<b
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
int my_rats_cmp(my_rat* a,my_rat* b,int32_t* cmp_res);

/*
 *	功能：比较有理数的绝对值大小
 *	参数：
 *		a，b：要比较的有理数
 *		cmp_res：比较结果：
 *			>0：|a|>|b|
 *			0：|a|=|b|
 *			<0：|a|<|b|
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
int my_rats_cmp_abs(my_rat* a,my_rat* b,int* cmp_res);

/*
 *	功能：有理数加法
 *	参数：
 *		a,b：要相加的有理数
 *		saving_type：结果存放方式，取值以下：
 *			MY_NEW_RES：和作为新的有理数返回
 *			MY_ARG_RES：和放在a
 *	返回值：
 *		非NULL：和
 *		NULL：出错
 */
my_rat *my_rats_add(my_rat *a,my_rat *b,my_result_saving_type saving_type);

/*
 *	功能：有理数乘法
 *	参数：
 *		a,b：要相乘的有理数
 *		saving_type：结果存放方式，取值以下：
 *			MY_NEW_RES：积作为新的有理数返回
 *			MY_ARG_RES：积放在a
 *	返回值：
 *		非NULL：和
 *		NULL：出错
 */
my_rat *my_rats_multiply(my_rat *a,my_rat *b,my_result_saving_type saving_type);

/*
 *	功能：有理数乘以int64
 *	参数:
 *		a：有理数
 *		b：乘数
 *		saving_type：结果存放方式，取值以下：
 *			MY_NEW_RES：积作为新的有理数返回
 *			MY_ARG_RES：积放在a
 *	返回值：
 *		非NULL：积
 *		NULL：出错
 */
my_rat *my_rat_multiply_int64(my_rat *a,int64_t b,my_result_saving_type saving_type);

/*
 *	功能：有理数的平方
 *	参数:
 *		a：有理数
 *		saving_type：结果存放方式，取值以下：
 *			MY_NEW_RES：指数作为新的有理数返回
 *			MY_ARG_RES：指数放在a
 *	返回值：
 *		非NULL：平方
 *		NULL：出错
 */
my_rat *my_rat_square(my_rat *a,my_result_saving_type saving_type);

/*
 *	功能：有理数对uint64的指数
 *	参数:
 *		a：有理数
 *		power：指数
 *		saving_type：结果存放方式，取值以下：
 *			MY_NEW_RES：指数作为新的有理数返回
 *			MY_ARG_RES：指数放在a
 *	返回值：
 *		非NULL：指数
 *		NULL：出错
 */
my_rat *my_rat_exp_uint64(my_rat *a,uint64_t power,my_result_saving_type saving_type);

/*
 *	功能：非负整数模uint64
 *	参数:
 *		a:非负整数
 *		b:除数
 *		remainder：保留余数
 *	返回值:
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
int my_mod_uint64(my_rat *a,uint64_t b,uint64_t *remainder);
	
/*
 *	功能：有理数除以uint32
 *	参数:
 *		a:有理数
 *		b:除数
 *		fraction：保留的小数位数
 *		round_mode：舍入模式
 *		restype:结果存放方式
 *	返回值:
 *		成功:商
 *		失败:NULL
 */
my_rat *my_divide_uint32(my_rat *a,uint32_t b,ssize_t fraction,my_round_mode round_mode,my_result_saving_type saving_type);

/*
 *	功能：计算传入整数的阶乘
 *	参数：
 *		n：计算n!
 *	返回值：
 *		非NULL：对应的阶乘
 *		NULL：出错
 */
my_rat *my_factorial(int64_t n);

/*
 *	功能：如果有理数是一个正整数，获取其每位上的数的和
 *	参数：
 *		n：要处理的有理数
 *		digit_sum：保存和
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
int my_rat_sum_digits(my_rat *n,uint64_t *digit_sum);

/*
 *	功能：如果有理数是一个正整数，获取其数位的数量
 *	参数：
 *		n：要处理的有理数
 *		digit_num：保存数量
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
int my_rat_digit_num(my_rat *n,uint64_t *digit_num);
#endif
