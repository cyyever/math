/*
 *	程序名：my_arithmetic.cpp
 *	作者：陈源源
 *	日期：2016-04-12
 *	功能：算术相关的函数
 */

#include "my_arithmetic.h"
#include <iostream>
#include <stdexcept>

namespace my_math {

uint64_t abs(int64_t num) {
  uint64_t tmp;
  if (num >= 0)
    return (uint64_t)num;

  if (num == INT64_MIN) {
    tmp = INT64_MAX;
    tmp++;
  } else
    tmp = -num;
  return tmp;
}

/*
 *	功能：计算base的power次的乘方
 * 	参数：
 *		base：底数
 *		power：指数
 * 	返回值：
 * 		乘方
 */
my_int power(const my_int &base, uint64_t power) {
  my_int res = 1;
  my_int tmp_base = base;

  while (power) {
    if (power & 1)
      res *= tmp_base;
    power >>= 1;
    tmp_base *= tmp_base;
  }
  return res;
}

/*
 *	功能：计算base的power次的乘方
 * 	参数：
 *		base：底数
 *		power：指数
 * 	返回值：
 * 		乘方
 */
my_int power(const my_int &base, const my_int &power) {
  if (power < 0)
    throw std::invalid_argument(string("power is less than zero"));
  my_int res = 1;
  my_int tmp_base = base;
  my_int tmp_power = power;

  while (tmp_power != 0) {
    if (tmp_power.is_odd())
      res *= tmp_base;
    tmp_power /= 2;
    tmp_base *= tmp_base;
  }
  return res;
}

/*
 *	功能：计算n的阶乘
 * 	参数：
 *		n：参数
 * 	返回值：
 * 		阶乘
 */
my_int factorial(uint64_t n) {
  my_int res = 1;
  while (n >= 2) {
    res *= n;
    n--;
  }
  return res;
}

/*
 *	功能：计算基于base的对数
 * 	参数：
 *		n：整数
 *		base：底数
 *		digit_num：要计算的小数点后的位数，多余的给截断
 * 	返回值：
 * 		对数
 */
my_rat log(const my_int &n, uint64_t base, size_t digit_num) {
  my_int p, q, min_power, max_power, tmp, tmp_power;
  my_rat n_power;
  if (n <= 0)
    throw std::invalid_argument("n is less than zero");
  if (base <= 1)
    throw std::invalid_argument("base is less than or equal to one");
  if (n == 1)
    return my_rat(0, 1);
  q = power(10, digit_num);
  n_power = my_rat(power(n, q), 1);

  //通过二分法找出对数
  //我们基于10的基数估计一个粗糙的上界
  max_power = n_power.numerator().digit_num();
  if (base < 10)
    max_power *= 4;

  while (min_power <= max_power) {
    tmp = (max_power + min_power) / 2;
    tmp_power = power(base, tmp);
    auto compare_res = n_power.compare(tmp_power);
    if (compare_res >= 0)
      p += tmp;
    if (compare_res == 0)
      break;
    else if (compare_res > 0) {
      n_power /= tmp_power;
      max_power -= tmp;
      min_power = 1;
    } else
      max_power = tmp - 1;
  }
  return my_rat(p, q);
}
}
