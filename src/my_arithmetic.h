/*
 *	程序名：my_arithmetic.c
 *	作者：陈源源
 *	日期：2013-02-10
 *	功能：包含算术运算操作
 */
#ifndef MY_ARITHMETIC_H
#define MY_ARITHMETIC_H

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
 *	功能：有理数乘以小整数
 *	参数:
 *		a：有理数
 *		b：必须在[-9999,9999]区间
 *		type：结果存放方式，取值以下：
 *			MY_NEW_RES：积作为新的有理数返回
 *			MY_ARG_RES：积放在a
 *	返回值：
 *		非NULL：积
 *		NULL：出错
 */
my_rat* my_rat_multiply_small_int(my_rat* a,int32_t b,my_result_saving_type type);

my_rat* my_rat_multiply_small_int2(my_rat* a,int32_t b,my_result_saving_type type);

#endif
