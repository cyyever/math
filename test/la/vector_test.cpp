/*!
 * \file vector_test.cpp
 *
 * \brief 测试vector类
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "src/la/vector.hpp"

TEST_CASE("init") {
  std::vector<int> vec{1, 2, 3};
  cyy::math::la::vector_view v(vec.data(), vec.size());
}
