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

} // namespace cyy::math
