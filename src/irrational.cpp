/*!
 * \file rational.cpp
 *
 * \brief
 */

#include "irrational.hpp"

namespace cyy::math {

  cyy::math::rational get_e(const cyy::math::rational &err) {
    integer n = 1;
    cyy::math::rational sum = 1;
    integer m = 1;

    while (1 / m >= err) {
      ++n;
      m *= n;

      sum += 1 / m;
    }
    return sum;
  }
} // namespace cyy::math
