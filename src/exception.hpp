/*!
 * \file exception.hpp
 *
 */

#pragma once

#include <stdexcept>

namespace cyy::math::exception {

  class no_integer : public std::invalid_argument {
  public:
    using invalid_argument::invalid_argument;
  };

  class divided_by_zero : public std::invalid_argument {
  public:
    using invalid_argument::invalid_argument;
  };
} // namespace cyy::math::exception
