/*!
 * \file prime.hpp
 *
 * \brief primes related functions
 */
#pragma once

#include <cinttypes>
#include <fstream>
#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>
#include <vector>

namespace cyy::math {

  class primes final {
  public:
    primes() = default;
    primes(const primes &) = delete;
    primes &operator=(const primes &) = delete;

    primes(primes &&) noexcept = default;
    primes &operator=(primes &&) noexcept = default;
    ~primes() noexcept = default;

    uint64_t at(size_t index);
    auto all() -> auto {
      seek(in_memory_primes.size());
      return ranges::views::concat(in_memory_primes,
                                   ranges::istream_view<uint64_t>(data_file));
    }

    bool has(uint64_t num) {
      if (num <= in_memory_primes.back()) {
        return ranges::binary_search(in_memory_primes, num);
      }
      size_t low = in_memory_primes.size();
      size_t high = prime_count;
      while (low <= high) {
        auto mid = (low + high) / 2;
        auto mid_prime = at(mid);
        if (num == mid_prime) {
          return true;
        }
        if (num < mid_prime) {
          high = mid - 1;
        } else {
          low = mid + 1;
        }
      }
      return false;
    }

  private:
    void open();
    void seek(size_t index);

  private:
    std::ifstream data_file;
    static constexpr size_t prime_count = 50000000;
    static constexpr size_t line_width = 10;
    static std::vector<uint64_t> in_memory_primes;
  };

} // namespace cyy::math
