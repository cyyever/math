/*!
 * \file prime.cpp
 *
 * \brief primes related functions
 */

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>

#include "config.hpp"
#include "exception.hpp"
#include "prime.hpp"

namespace cyy::math {

  uint64_t primes::at(size_t index) {
    if (index < in_memory_primes.size()) {
      return in_memory_primes[index];
    }

    if (index >= prime_count) {
      throw cyy::math::exception::out_of_range("over prime range");
    }
    seek(index);
    uint64_t prime;
    data_file >> prime;
    if (!data_file) {
      throw cyy::math::exception::internal_error("stream error");
    }
    return prime;
  }
  void primes::open_data_file() {
    if (data_file) {
      return;
    }
    data_file.open(std::filesystem::path(INSTALLED_DATA_DIR) / "prime.txt");
    if (!data_file) {
      data_file.open(std::filesystem::path(DATA_DIR) / "prime.txt");
    }
    if (!data_file) {
      throw cyy::math::exception::no_data_file("prime.txt");
    }
    data_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  }
  void primes::seek(size_t index) {
    open_data_file();
    data_file.seekg(static_cast<std::iostream::pos_type>(index * line_width));
    if (!data_file) {
      throw cyy::math::exception::internal_error("stream error");
    }
  }
  std::vector<uint64_t> primes::in_memory_primes{

  };

} // namespace cyy::math
