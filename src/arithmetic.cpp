/*!
 * \file arithmetic.cpp
 *
 * \brief 算术相关的函数
 */

#include "arithmetic.hpp"
namespace cyy::math {

  /*
   *	功能：计算n的阶乘
   * 	参数：
   *		n：参数
   * 	返回值：
   * 		阶乘
   */
  integer factorial(uint32_t n) {
    integer res = 1;
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
  /* my_rat log(const integer &n, uint64_t base, size_t digit_num) { */
  /*   integer p, q, min_power, max_power, tmp, tmp_power; */
  /*   my_rat n_power; */
  /*   if (n <= 0) */
  /*     throw std::invalid_argument("n is less than zero"); */
  /*   if (base <= 1) */
  /*     throw std::invalid_argument("base is less than or equal to one"); */
  /*   if (n == 1) */
  /*     return my_rat(0, 1); */
  /*   q = power(10, digit_num); */
  /*   n_power = my_rat(power(n, q), 1); */

  /*   //通过二分法找出对数 */
  /*   //我们基于10的基数估计一个粗糙的上界 */
  /*   max_power = n_power.numerator().digit_num(); */
  /*   if (base < 10) */
  /*     max_power *= 4; */

  /*   while (min_power <= max_power) { */
  /*     tmp = (max_power + min_power) / 2; */
  /*     tmp_power = power(base, tmp); */
  /*     auto compare_res = n_power.compare(tmp_power); */
  /*     if (compare_res >= 0) */
  /*       p += tmp; */
  /*     if (compare_res == 0) */
  /*       break; */
  /*     else if (compare_res > 0) { */
  /*       n_power /= tmp_power; */
  /*       max_power -= tmp; */
  /*       min_power = 1; */
  /*     } else */
  /*       max_power = tmp - 1; */
  /*   } */
  /*   return my_rat(p, q); */
  /* } */
} // namespace cyy::math
