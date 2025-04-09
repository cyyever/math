/*!
 * \file irrational_test.cpp
 *
 * \brief 测试irrational类
 */
#include <doctest/doctest.h>

#include "irrational.hpp"
using namespace std;
using namespace cyy::math;

TEST_CASE("e") {
  CHECK_EQ(stold(e_natural()
                     .value_within_error(rational(1, 1000000))
                     .to_string(rational(1, 1000000))),
           doctest::Approx(M_E));
}
TEST_CASE("root") {
  CHECK_EQ(stold(square_root(2)
                     .value_within_error(rational(1, 1000000))
                     .to_string(rational(1, 1000000))),
           doctest::Approx(1.414213));
}
