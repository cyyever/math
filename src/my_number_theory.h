/*
 *	程序名：my_number_theory.h
 *	作者：陈源源
 *	日期：2016-04-04
 *	功能：包含数论相关函数
 */
#ifndef MY_NUMBER_THEORY_H
#define MY_NUMBER_THEORY_H

#include "my_int.h"
#include <cstdint>
#include <vector>

namespace my_math {

/*
 *	功能：判定质数
 *	参数：
 *		num：要判定的数
 *	返回值：
 *		true：是质数
 *		false：不是质数
 */
bool is_prime(uint64_t num);

/*
 *	功能：生成质数
 *	参数：
 *		upper_limit：质数上限
 *	返回值：
 *		质数vector
 */
std::vector<uint64_t> get_primes(uint64_t upper_limit);

/*
 *	功能：计算两个整数的最大公约数
 *	参数：
 *		a，b：整数
 *	返回值：
 *		最大公约数
 */
my_int gcd(const my_int &a, const my_int &b);
}
#endif
