/*!
 * \file template.hpp
 *
 * \brief
 */

#include "integer.hpp"
#include "rational.hpp"

namespace cyy::math::type {
  template <typename T>
  constexpr bool is_numerical_type = (std::is_integral_v<T> ||
                                      std::is_same_v<T, ::cyy::math::integer>);
}
