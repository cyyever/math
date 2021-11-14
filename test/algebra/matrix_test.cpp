/*!
 * \file matrix_test.cpp
 *
 * \brief 测试matrix类
 */
#include <doctest/doctest.h>

#include "algebra/matrix.hpp"
#include "rational.hpp"

/* TEST_CASE("transpose") { */
/*   cyy::math::la::matrix v(2,3); */
/*   std::vector<int> m{1, 2, 3, 4, 5, 6}; */
/*   cyy::math::la::matrix_view v(m.data(), 2, 3); */
/*   std::vector<int> m2{1, 4, 2, 5, 3, 6}; */
/*   cyy::math::la::matrix_view v2(m2.data(), 3, 2); */
/*   CHECK_EQ(v.transpose(), v2); */
/* } */

TEST_CASE("determinant") {
  cyy::math::la::matrix v{{1,2},{3,4}};
  /* std::vector<cyy::math::rational> m{1, 2, 3, 4}; */
  /* cyy::math::la::square_matrix_view v(m.data(), 2); */
  CHECK_EQ(v.determinant(), -2);
}
TEST_CASE("invert") {
  std::vector<cyy::math::rational> m{1, 2, 3, 4};
  cyy::math::la::square_matrix_view v(m.data(), 2);
  std::vector<cyy::math::rational> m2{
      cyy::math::rational(-2), cyy::math::rational(1),
      cyy::math::rational(3, 2), cyy::math::rational(-1, 2)};
  cyy::math::la::square_matrix_view v2(m2.data(), 2);
  REQUIRE(v.invert());
  CHECK_EQ(v.invert().value(), v2.to_matrix());
}
