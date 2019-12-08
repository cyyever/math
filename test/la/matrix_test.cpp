/*!
 * \file matrix_test.cpp
 *
 * \brief 测试matrix类
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "src/la/matrix.hpp"

TEST_CASE("init") {
  std::vector<int> m{1, 2, 3, 4};
  cyy::math::la::matrix_view v(m.data(), 2, 2);
}
