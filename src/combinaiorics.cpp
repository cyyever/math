/*!
 * \file combinaiorics.cpp
 *
 * \brief 包含组合学相关函数
 */

#include "combinaiorics.hpp"
#include "arithmetic.hpp"

#include <iostream>
#include <stdexcept>

namespace cyy::math {

  /*
   *	功能：生成伯努利系数C(n,k)
   *	参数：
   *		n,k：参数
   *	返回值：
   *		C(n,k)
   */
  integer binomial_coefficient(uint64_t n, uint64_t k) {

    if (k > n)
      throw std::out_of_range("k>n");

    if (n < k * 2)
      k = n - k;
    if (k == 0)
      return 1;

    integer numerator = 1;
    for (auto i = n - k + 1; i <= n; i++) {
      numerator *= integer(i);
    }

    return numerator / factorial(k);
  }

#if 0
  /*
   *	功能：生成在n个数中选k个数的所有组合
   *	参数：
   *		n,k：参数
   *	返回值：
   *		第一个组合
   */
  std::vector<bool> my_next_combination(uint64_t n, uint64_t k) {
    if (k > n)
      throw std::out_of_range("k>n");

    auto res = std::vector<bool>(n, false);
    for (uint64_t i = 0; i < k; i++)
      res[i] = true;
    return res;
  }

  /*
   *	功能：生成在n个数中选k个数的所有组合
   *	参数：
   *		n,k：参数
   *		prev_combination：上一次调用此函数返回的组合
   *	返回值：
   *		返回下一个组合，如果组合已经全部生成完毕，则vector为空
   */
  std::vector<bool>
  my_next_combination(uint64_t n, uint64_t k,
                      const std::vector<bool> &prev_combination) {
    std::vector<bool>::size_type i, j, cnt;
    uint8_t find_false;

    if (k > n)
      throw std::out_of_range("k>n");

    auto res = prev_combination;
    if (prev_combination.size() != n)
      throw std::invalid_argument("wrong prev_combination");

    cnt = 0;
    find_false = 0;
    i = n - 1;
    while (1) {
      if (res[i] == true) {
        cnt++;
        res[i] = false;
        if (find_false) {
          i++;
          for (j = 0; j < cnt; j++)
            res[i + j] = true;

          return res;
        }
      } else
        find_false = 1;
      if (i == 0)
        break;
      i--;
    }
    res.resize(0);
    return res;
  }
#endif
} // namespace cyy::math
