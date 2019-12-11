/*!
 * \file matrix_test.cpp
 *
 * \brief 测试matrix类
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "src/la/matrix.hpp"
#include "src/rational.hpp"

TEST_CASE("init") {
  std::vector<int> m{1, 2, 3, 4};
  cyy::math::la::matrix_view v(m.data(), 2, 2);
}

TEST_CASE("determinant") {
  std::vector<cyy::math::rational> m{1, 2, 3, 4};
  cyy::math::la::square_matrix_view v(m.data(), 2);
  CHECK_EQ(v.determinant(), -2);
}
TEST_CASE("invert") {
  std::vector<cyy::math::rational> m{1, 2, 3, 4};
  cyy::math::la::square_matrix_view v(m.data(), 2);
  std::vector<cyy::math::rational> m2{
      cyy::math::rational(-2), cyy::math::rational(1),
      cyy::math::rational(3, 2), cyy::math::rational(-1, 2)};
  cyy::math::la::square_matrix_view v2(m2.data(), 2);
  CHECK(v.invert());
  CHECK_EQ(v.invert().value(), v2);
}
