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
   *	功能：生成伯努利系数C(n,k)
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
  inline auto combination(uint64_t n, uint64_t k) {
    if (k > n)
      throw exception::out_of_range("k>n");
    auto res = std::make_shared<std::vector<bool>>();

    return ranges::views::generate([=]() {
             if (res->empty()) {
               res->resize(n, false);
               for (uint64_t i = 0; i < k; i++)
                 (*res)[i] = true;
               return *res;
             }
             std::vector<bool>::size_type i, j, cnt;
             uint8_t find_false;
             cnt = 0;
             find_false = 0;
             i = n - 1;
             while (true) {
               if ((*res)[i] == true) {
                 cnt++;
                 (*res)[i] = false;
                 if (find_false) {
                   i++;
                   for (j = 0; j < cnt; j++)
                     (*res)[i + j] = true;
                   return *res;
                 }
               } else
                 find_false = 1;
               if (i == 0)
                 break;
               i--;
             }
             res->resize(0);
             return *res;
           }) |
           ranges::views::take_while([](auto const &v) { return !v.empty(); });
  }
} // namespace cyy::math
