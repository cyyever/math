/*
 *	程序名：combinatorics_test.cpp
 *	作者：陈源源
 *	日期：2016-03-28
 *	功能：测试integer类
 */
#include <doctest/doctest.h>

#include "../src/combinatorics.hpp"

TEST_CASE("binomial_coefficient") {
  CHECK_EQ(cyy::math::binomial_coefficient(100, 50),
           cyy::math::integer("100891344545564193334812497256"));
}
/* TEST_CASE("all_combinations") { */
/*   auto combination_seq = cyy::math::all_combinations(3, 2); */
/*   auto it = combination_seq.begin(); */
/*   REQUIRE(it != combination_seq.end()); */
/*   REQUIRE_EQ(*it, std::vector<uint64_t>{1, 2}); */
/*   it++; */
/*   REQUIRE(it != combination_seq.end()); */
/*   REQUIRE_EQ(*it, std::vector<uint64_t>{1, 3}); */
/*   it++; */
/*   REQUIRE(it != combination_seq.end()); */
/*   REQUIRE_EQ(*it, std::vector<uint64_t>{2, 3}); */
/*   it++; */
/*   REQUIRE(it == combination_seq.end()); */
/* } */
