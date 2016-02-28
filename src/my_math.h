/*
 *	程序名：my_math.h
 *	作者：陈源源
 *	日期：2013-02-09
 *	功能：公用的定义
 */
#ifndef MY_MATH_H
#define MY_MATH_H

//通用函数返回值
typedef enum
{
	MY_SUCC,
	MY_ERROR,
}my_return_value;

//计算结果保存类型
typedef enum
{
	MY_NEW_RES,	//计算结果作为新的数据返回
	MY_ARG_RES,	//计算结果放在函数参数
}my_result_saving_type;

//舍入模式
typedef enum{
	MY_TRUNC=0,	//截断
}my_round_mode;

#define MY_MAX(a,b) ((a)>(b)?(a):(b))
#define MY_MIN(a,b) ((a)<(b)?(a):(b))
#define MY_INT64_MIN_STR_LEN 20
#define MY_UNSIGNED_MOD_4(a) ((a)&3)

#endif
