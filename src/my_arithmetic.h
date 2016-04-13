/*
 *	程序名：my_arithmetic.h
 *	作者：陈源源
 *	日期：2016-04-12
 *	功能：算术相关的函数
 */
#ifndef MY_ARITHMETIC_H
#define MY_ARITHMETIC_H

#include "my_int.h"
#include "my_rat.h"

namespace my_math
{

uint64_t abs(int64_t num);

my_int power(const my_int &base,uint64_t power);
my_int power(const my_int &base,const my_int &power);
my_int factorial(uint64_t n);
my_rat log(const my_int &n,uint64_t base,size_t digit_num=0);

}
#endif
