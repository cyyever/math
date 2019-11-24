/*!
 * \file arithmetic.cpp
 *
 * \brief 算术相关的函数
 */

#include "arithmetic.hpp"
#include "exception.hpp"
namespace cyy::math {
  /*
  rational ln(const rational &a) {
    if (a <= 0) {
      throw exception::out_of_range("a<0");
    }
    if (a == 1) {
      return 0;
    }
    uint64_t n = 200;

    rational res = 0;
    rational delta = (a - 1) / n;
    rational x = 1;
    for (uint64_t i = 1; i <= n; i++) {
      x += delta;
      auto tmp = x.reciprocal();
      res += (tmp + x.reciprocal()) / 2;
    }
    res *= delta;
    return res;
  }
*/

  /*
   *	功能：计算基于base的对数
   * 	参数：
   *		n：整数
   *		base：底数
   *		digit_num：要计算的小数点后的位数
   * 	返回值：
   * 		对数
   */
  /* rational log(const integer &n, uint64_t base, size_t digit_num) { */
  /*   integer p, q, min_power, max_power, tmp, tmp_power; */
  /*   if (n <= 0) */
  /*     throw exception::out_of_range("n<0"); */
  /*   if (base <= 1) */
  /*     throw exception::out_of_range("base<=1"); */
  /*   if (n == 1) */
  /*     return 0; */

  /*   q = power(10, digit_num); */
  /*   n_power = rational(power(n, q), 1); */
  /*   rational n_power; */

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
  /*   return rational(p, q); */
  /* } */
} // namespace cyy::math
