/*!
 * \file arithmetic_test.cpp
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <vector>

#include "../src/arithmetic.hpp"
#include "../src/integer.hpp"
using namespace cyy::math;

TEST_CASE("exponent") {
  CHECK_EQ(exponent(integer(2), (uint32_t)64) - 1, integer(UINT64_MAX));
}
TEST_CASE("factorial") {
  integer a = 2;
  for (uint32_t i = 2; i <= 50; i++, a *= i)
    CHECK_EQ(factorial(i) , a);
}
