/*
 *	程序名：combinatorics_test.cpp
 *	作者：陈源源
 *	日期：2016-03-28
 *	功能：测试integer类
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "../src/combinatorics.hpp"
using namespace std;
using namespace cyy::math;

TEST_CASE("binomial_coefficient") {
  assert(binomial_coefficient(100, 50) == integer("100891344545564193334812497256"));
  assert(binomial_coefficient(5, 2) ==
         integer("10"));
}
