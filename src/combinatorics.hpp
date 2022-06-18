/*!
 * \file combinatorics.hpp
 *
 * \brief 包含组合学相关函数
 */
#pragma once

#include <cstdint>
#include <memory>
#include <range/v3/all.hpp>
#include <vector>

#include "exception.hpp"
#include "integer.hpp"
namespace cyy::math {

  /*
   *	功能：生成系数C(n,k)
   *	参数：
   *		n,k：参数
   *	返回值：
   *		C(n,k)
   */
  integer binomial_coefficient(uint64_t n, uint64_t k);

  /*
   *	功能：生成在n个数中选k个数的所有组合
   *	参数：
   *		n,k：参数
   *	返回值：
   *		第一个组合
   */
  inline auto all_combinations(uint64_t n, uint64_t k) {
    if (k > n)
      throw exception::out_of_range("k>n");
    if (k == 0)
      throw exception::out_of_range("k is 0");
    auto res = std::make_shared<std::vector<uint64_t>>();
    return ranges::views::generate([=]() {
             if (res->empty()) {
               res->resize(k);
               for (uint64_t i = 0; i < k; i++)
                 (*res)[i] = i + 1;
               return *res;
             }
             bool find_false = false;
             uint64_t a = n;
             uint64_t cnt = 0;
             while (!res->empty()) {
               if (find_false) {
                 auto tmp = res->back();
                 res->pop_back();
                 cnt++;
                 for (uint64_t j = 0; j < cnt; j++) {
                   res->push_back(tmp + j + 1);
                 }
                 return *res;
               }
               if (res->back() == a) {
                 res->pop_back();
                 a--;
                 cnt++;
                 continue;
               } else {
                 find_false = true;
               }
             }
             return *res;
           }) |
           std::views::take_while([](auto const &v) { return !v.empty(); });
  }
} // namespace cyy::math
