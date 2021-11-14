/*!
 * \file vector_test.cpp
 *
 * \brief 测试vector类
 */
#include <doctest/doctest.h>

#include "src/algebra/vector.hpp"

TEST_CASE("init") {
  std::vector<int> vec{1, 2, 3};
  cyy::math::la::vector_view v(vec.data(), vec.size());
}
