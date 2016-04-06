/*
 *	程序名：my_combinaiorics.h
 *	作者：陈源源
 *	日期：2016-04-04
 *	功能：包含组合学相关函数
 */
#ifndef MY_COMBINAIORICS_H
#define MY_COMBINAIORICS_H

#include <vector>
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
 *	功能：生成在n个数中选k个数的所有组合
 *	参数：
 *		n,k：参数
 *	返回值：
 *		第一个组合
 */
std::vector<bool> my_next_combination(uint64_t n,uint64_t k);

/*
 *	功能：生成在n个数中选k个数的所有组合
 *	参数：
 *		n,k：参数
 *		prev_combination：上一次调用此函数返回的组合	
 *	返回值：
 *		返回下一个组合，如果组合已经全部生成完毕，则vector为空
 */
std::vector<bool> my_next_combination(uint64_t n,uint64_t k,const std::vector<bool> &prev_combination);
}
#endif
