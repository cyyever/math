/*
 *	程序名：my_number_theory_test.cpp
 *	作者：陈源源
 *	日期：2016-04-04
 *	功能：测试数论相关函数
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "../src/number_theory.hpp"
using namespace cyy::math;

TEST_CASE("gcd") {
  CHECK_EQ(cyy::math::gcd(-3, 7), 1);
  CHECK_EQ(cyy::math::gcd(-14, 77), 7);
  CHECK_EQ(cyy::math::gcd(-5, 5), 5);
  CHECK_EQ(cyy::math::gcd(0, -10), 10);
}
