/*
 *	程序名：my_number_theory.cpp
 *	作者：陈源源
 *	日期：2016-04-04
 *	功能：包含数论相关函数
 */

#include "number_theory.hpp"

#include <iostream>
#include <stdexcept>

namespace cyy::math {
  /*
   *	功能：计算两个整数的最大公约数
   *	参数：
   *		a，b：整数
   *	返回值：
   *		最大公约数
   */
  integer gcd(const integer &a, const integer &b) {
    integer p, q, tmp, m;
    int cmp_res;

    if (a == 0 && b == 0)
      throw std::invalid_argument("a and b are zero");
    p = a;
    p.set_non_negative();
    q = b;
    q.set_non_negative();

    if (p < q) {
      std::swap(p, q);
    }
    //用欧几里得算法找到最大公约数

    while (1) {
      m = p % q;
      if (m == 0)
        break;
      p = std::move(q);
      q = m;
    }
    return q;
  }
} // namespace cyy::math
