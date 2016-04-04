/*
 *	程序名：my_combinaiorics.h
 *	作者：陈源源
 *	日期：2016-04-04
 *	功能：包含组合学相关函数
 */
#ifndef MY_COMBINAIORICS_H
#define MY_COMBINAIORICS_H

#include <cstdint>
#include "my_int.h"

namespace my_math
{

/*
 *	功能：生成伯努利系数C(n,k)
 *	参数：
 *		n,k：参数
 *	返回值：
 *		C(n,k)
 */
my_int binomial_coefficient(uint64_t n,uint32_t k);

/*
 *	功能：生成组合
 *	参数：
 *		n,k：在n个数中选k个数的组合
 *		prev_combination：如果*prev_combination非NULL，把第一个组合写入此，否则包括上一次调用此函数生成的组合，此函数把下一个组合写入此
 *	返回值：
 *		非NULL：返回*prev_combination
 *		NULL：组合已经全部生成完毕
 */
//uint8_t *my_next_combination(uint64_t n,uint64_t k,uint8_t **prev_combination);

}
#endif
