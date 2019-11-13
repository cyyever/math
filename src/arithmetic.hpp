/*!
 * \file arithmetic.hpp
 *
 * \brief 算术相关的函数
 */
#pragma once

#include <type_traits>

#include "integer.hpp"
namespace cyy::math {

  template <typename BASE_T, typename POWER_T,
            typename = std::enable_if_t<std::conjunction_v<
                std::is_integral_v<POWER_T>, std::is_unsigned_v<POWER_T>>>>
  BASE_T exponent(BASE_T base, POWER_T power) {
    BASE_T res = 1;
    auto tmp_base = base;

    while (power != 0) {
      if (power % 2 == 1) {
        res *= tmp_base;
      }
      power /= 2;
      tmp_base *= tmp_base;
    }
    return res;
  }
  integer factorial(uint32_t n);
  /* my_rat log(const my_int &n, uint64_t base, size_t digit_num = 0); */
} // namespace cyy::math
