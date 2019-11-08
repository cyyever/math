/*!
 * \file prime.hpp
 *
 * \brief primes related functions
 */
#pragma once

#include <cinttypes>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

namespace cyy::math {

  class primes final {
  public:
    uint64_t at(size_t index);

  private:
    void load_data_file();
  };

} // namespace cyy::math
