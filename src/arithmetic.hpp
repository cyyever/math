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
            typename = std::enable_if_t<
              std::is_integral_v<POWER_T>>>
  BASE_T exponent(BASE_T base, POWER_T power) {
      if constexpr (std::is_unsigned<POWER_T>::value) {
    BASE_T res = 1;

    while (power != 0) {
      if (power % 2 == 1) {
        res *= base;
      }
      power /= 2;
      base *= base;
    }
    return res;
      } else {
      bool add_one = false;
      /*
      if (power<0) {
        base=1/base;
        if (power == std::numeric_limits<POWER_T>::min()) {
          add_one = true;
          power += 1;
        }
        power = -power;
      }
      */
    auto res=exponent(base,  std::make_unsigned_t<POWER_T>(power));
    if(add_one) {
      res*=base;
    }

    return res;
      }
  }
  integer factorial(uint32_t n);
  /* my_rat log(const my_int &n, uint64_t base, size_t digit_num = 0); */
} // namespace cyy::math
