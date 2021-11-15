/*!
 * \file rational.cpp
 *
 * \brief
 */

#include "irrational.hpp"
#include "arithmetic.hpp"
#include "rational.hpp"

namespace cyy::math {
  rational e_natural::value_within_error(const cyy::math::rational &err) {
    integer n = 1;
    rational sum = 2;
    integer m = 1;
    while (1 / m >= err) {
      ++n;
      m *= n;
      sum += 1 / m;
    }
    return sum;
  }

  rational square_root::value_within_error(const cyy::math::rational &err) {
    rational root = value / rational(2);
    while (abs(exponent(root, 2) - value) > err) {
      root = (root + value / root) / rational(2);
    }
    return root;
  }

} // namespace cyy::math
