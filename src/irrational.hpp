/*!
 * \file irrational.hpp
 *
 * \brief
 */
#pragma once

#include <string>

#include "rational.hpp"

namespace cyy::math {
  // we implements irrational as a convergent sequence
  class irrational {
  public:
    virtual rational value_within_error(const cyy::math::rational &err) = 0;
    virtual ~irrational() = default;
  };
  class e_natural final : public irrational {
  public:
    rational value_within_error(const cyy::math::rational &err) override;
  };

} // namespace cyy::math
