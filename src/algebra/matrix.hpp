/*!
 * \file matrix.hpp
 *
 * \brief matrix related classes
 */
#pragma once

#include "vector.hpp"
#include <Eigen/Core>
#include <Eigen/LU>
namespace cyy::math::la {
  template <typename number_type = cyy::math::rational>
  using matrix = Eigen::Matrix<number_type, cyy::math::rational, Eigen::Dynamic,
                               Eigen::Dynamic>;
  template <typename number_type = cyy::math::rational>
  using vector = Eigen::Matrix<number_type, Eigen::Dynamic, 1>;
} // namespace cyy::math::la
