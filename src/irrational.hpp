/*!
 * \file irrational.hpp
 *
 * \brief
 */
#pragma once

#include "rational.hpp"

namespace cyy::math {
  // we implements irrational as a convergent sequence
  class irrational {
  public:
    virtual rational value_within_error(const rational &err) = 0;
    virtual ~irrational() = default;
  };
  class e_natural final : public irrational {
  public:
    rational value_within_error(const rational &err) override;
  };
  class square_root final : public irrational {
  public:
    square_root(rational value_) : value(std::move(value_)) {}
    rational value_within_error(const rational &err) override;

  private:
    rational value;
  };
} // namespace cyy::math
