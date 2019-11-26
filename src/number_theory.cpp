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
   *		p，q：整数
   *	返回值：
   *		最大公约数
   */
  integer gcd(integer p, integer q) {
    integer tmp, m;

    if (p == 0 && q == 0)
      throw std::invalid_argument("a and b are zero");
    p.set_non_negative();
    q.set_non_negative();

    if (p < q) {
      std::swap(p, q);
    }
    if (q == 0) {
      return p;
    }
    //用欧几里得算法找到最大公约数

    while (true) {
      auto remainder = p % q;
      if (remainder == 0)
        break;
      p = std::move(q);
      q = std::move(remainder);
    }
    return q;
  }
} // namespace cyy::math
