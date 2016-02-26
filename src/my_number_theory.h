/*
 *	程序名：my_number_theory.h
 *	作者：陈源源
 *	日期：2016-02-11
 *	功能：包含数论相关函数
 */
#ifndef MY_NUMBER_THEORY_H
#define MY_NUMBER_THEORY_H

#include <inttypes.h>
#include "my_rat.h"

/*
 *	功能：判定质数
 *	参数：
 *		num：要判定的数
 *	返回值：
 *		1：是质数
 *		0：不是质数
 */
int my_is_prime(uint64_t num);

/*
 *	功能：生成质数
 *	参数：
 *		upper_limit：质数上限
 *	返回值：
 *		NULL：失败
 *		非NULL：质数数组，以零结尾
 */
uint64_t *my_primes(uint64_t upper_limit);

/*
 *	功能：生成伯努利系数C(n,k)
 *	参数：
 *		n,k：参数
 *	返回值：
 *		非NULL：C(n,k)
 *		NULL：失败
 */
my_rat *my_binomial(int64_t n,uint32_t k);
#endif
