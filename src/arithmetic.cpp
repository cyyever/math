/*!
 * \file arithmetic.cpp
 *
 * \brief 算术相关的函数
 */

#include "arithmetic.hpp"
#include "exception.hpp"
namespace cyy::math {

  rational ln(const rational &a) {
    if (a <= 0) {
      throw exception::out_of_range("a<0");
    }
    if (a == 1) {
      return 0;
    }
    uint64_t n = 30;

    rational res = 0;
    rational delta = (a - 1) /  n;
    rational x = 1;
    for (uint64_t i = 1; i <= n; i++) {
      x += delta;
      x.reciprocal();
      res += x.reciprocal();
    }
    res *= delta;
    //res.simplify();
    return res;
  }

} // namespace cyy::math
