/*!
 * \file rational_test.cpp
 *
 * \brief 测试rational类
 */
#include <cmath>
#include <doctest/doctest.h>

#include "../src/irrational.hpp"
using namespace std;
using namespace cyy::math;

TEST_CASE("e") {
  CHECK_EQ(stold(irrational::get_e(rational(1, 1000000))
                     .to_string(rational(1, 1000000))),
           doctest::Approx(M_E));
}
