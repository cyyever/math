/*!
 * \file combinatorics.cpp
 *
 * \brief 包含组合学相关函数
 */

#include <iostream>
#include <stdexcept>

#include "arithmetic.hpp"
#include "combinatorics.hpp"
#include "exception.hpp"
namespace cyy::math {

  /*
   *	功能：生成系数C(n,k)
   *	参数：
   *		n,k：参数
   *	返回值：
   *		C(n,k)
   */
  integer binomial_coefficient(uint64_t n, uint64_t k) {

    if (k > n)
      throw exception::out_of_range("k>n");

    if (n < k * 2)
      k = n - k;
    if (k == 0)
      return 1;
    if (k == 1) {
      return n;
    }

    integer numerator = 1;
    for (auto i = n - k + 1; i <= n; i++) {
      numerator *= integer(i);
    }

    return numerator / factorial(k);
  }

} // namespace cyy::math
