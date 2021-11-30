/*!
 * \file prime.hpp
 *
 * \brief primes related functions
 */
#pragma once

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>
#include <ranges>
#include <vector>

#include "exception.hpp"
#include "integer.hpp"
namespace cyy::math {

  class primes final {
  public:
    primes() = default;
    primes(const primes &) = delete;
    primes &operator=(const primes &) = delete;

    primes(primes &&) = default;
    primes &operator=(primes &&) = default;
    ~primes() noexcept = default;

    class iterator {
    public:
      using value_type = uint64_t;
      using difference_type = ptrdiff_t;
      using pointer = value_type *;
      using reference = const value_type &;

      iterator(const primes *ptr_, size_t index_) : ptr(ptr_), index(index_) {}
      iterator(const iterator &) = default;
      iterator &operator=(const iterator &) = default;

      iterator(iterator &&) noexcept = default;
      iterator &operator=(iterator &&) noexcept = default;
      ~iterator() noexcept = default;

      inline iterator &operator+=(difference_type rhs) {
        index = static_cast<size_t>(static_cast<difference_type>(index) + rhs);
        return *this;
      }
      inline iterator &operator-=(difference_type rhs) {
        index = static_cast<size_t>(static_cast<difference_type>(index) - rhs);
        return *this;
      }
      inline uint64_t operator*() const { return ptr->at(index); }

      inline iterator &operator++() {
        ++index;
        return *this;
      }
      inline iterator &operator--() {
        --index;
        return *this;
      }
      inline iterator operator++(int) {
        iterator tmp(*this);
        ++index;
        return tmp;
      }
      inline iterator operator--(int) {
        iterator tmp(*this);
        --index;
        return tmp;
      }
      inline difference_type operator-(const iterator &rhs) const {
        return index - rhs.index;
      }
      inline iterator operator+(difference_type rhs) const {
        return iterator(ptr, index + rhs);
      }
      inline iterator operator-(difference_type rhs) const {
        return iterator(ptr, index - rhs);
      }
      inline auto operator==(const iterator &rhs) const noexcept {
        return index == rhs.index && ptr == rhs.ptr;
      }

    private:
      const primes *ptr{nullptr};
      size_t index{0};
    };

    iterator begin() const noexcept { return iterator(this, 0); }
    iterator end() const noexcept { return iterator(this, prime_count); }

    iterator cbegin() const noexcept { return iterator(this, 0); }
    iterator cend() const noexcept { return iterator(this, prime_count); }
    auto get_view() const {
      return std::ranges::views::iota(static_cast<size_t>(0), prime_count) |
             std::ranges::views::transform(
                 [this](auto idx) { return at(idx); });
    }

    uint64_t at(size_t index) const;
    auto till(uint64_t upper_bound) -> auto {
      if (upper_bound > max_prime) {
        throw exception::out_of_range("more than max prime");
      }
      seek(in_memory_primes.size());

      return ranges::views::concat(in_memory_primes,
                                   ranges::istream_view<uint64_t>(data_file)) |
             ranges::views::take_while(
                 [upper_bound](auto i) { return i <= upper_bound; });
    }
    auto from(uint64_t lower_bound) -> auto {
      if (lower_bound > max_prime) {
        throw exception::out_of_range("more than max prime");
      }
      seek(in_memory_primes.size());

      return ranges::views::concat(in_memory_primes,
                                   ranges::istream_view<uint64_t>(data_file)) |
             ranges::views::drop_while(
                 [lower_bound](auto i) { return i < lower_bound; });
    }

    bool has(uint64_t num) const {
      if (num > max_prime) {
        for (const auto p : *this) {
          if (num % p == 0) {
            return false;
          }
          if (cyy::math::integer(p) * p > num) {
            return true;
          }
        }
        throw exception::out_of_range("more than max prime");
      }
      return std::binary_search(begin(), end(), num);
    }

  private:
    void open() const;
    void seek(size_t index) const;

  private:
    mutable std::ifstream data_file;
    mutable size_t file_index{0};
    static constexpr size_t prime_count = 50000000;
    static constexpr uint64_t max_prime = 982451653;
    static constexpr size_t line_width = 10;
    static inline std::vector<uint64_t> in_memory_primes;
  };

} // namespace cyy::math
