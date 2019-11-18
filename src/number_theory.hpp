/*
 *	程序名：my_number_theory.h
 *	作者：陈源源
 *	日期：2016-04-04
 *	功能：包含数论相关函数
 */
#pragma once

#include "integer.hpp"
#include <cstdint>
#include <vector>

namespace cyy::math {

  /*
   *	功能：计算两个整数的最大公约数
   *	参数：
   *		a，b：整数
   *	返回值：
   *		最大公约数
   */
  integer gcd(const integer &a, const integer &b);
} // namespace cyy::math
