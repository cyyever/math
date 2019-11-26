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
  class no_rational : public std::invalid_argument {
  public:
    using invalid_argument::invalid_argument;
  };

  class divided_by_zero : public std::invalid_argument {
  public:
    using invalid_argument::invalid_argument;
  };
  class no_data_file : public std::runtime_error {
  public:
    using runtime_error::runtime_error;
  };

  class out_of_range : public std::logic_error {
  public:
    using logic_error::logic_error;
  };

  class invalid_type : public std::runtime_error {
  public:
    using runtime_error::runtime_error;
  };
} // namespace cyy::math::exception
