/*!
 * \file rational.cpp
 *
 * \brief
 */

#include "irrational.hpp"
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

} // namespace cyy::math
