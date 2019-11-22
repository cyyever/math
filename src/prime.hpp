/*!
 * \file prime.hpp
 *
 * \brief primes related functions
 */
#pragma once

#include <cinttypes>
#include <fstream>
#include <iostream>
#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>
#include <vector>

#include "exception.hpp"
namespace cyy::math {

  class primes final {
  public:
    primes() = default;
    primes(const primes &) = delete;
    primes &operator=(const primes &) = delete;

    primes(primes &&) noexcept = default;
    primes &operator=(primes &&) noexcept = default;
    ~primes() noexcept = default;

    class iterator
        : public std::iterator<std::random_access_iterator_tag, uint64_t> {
    public:
      iterator(const primes *ptr_, size_t index_) : ptr(ptr_), index(index_) {}
      iterator(const iterator &) = default;
      iterator &operator=(const iterator &) = default;

      iterator(iterator &&) noexcept = default;
      iterator &operator=(iterator &&) noexcept = default;
      ~iterator() noexcept = default;

      inline iterator &operator+=(difference_type rhs) {
        index += rhs;
        return *this;
      }
      inline iterator &operator-=(difference_type rhs) {
        index -= rhs;
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
      /* friend inline iterator operator+(difference_type lhs, */
      /*                                  const iterator &rhs) { */
      /*   return iterator(lhs + rhs.index); */
      /* } */
      /* friend inline iterator operator-(difference_type lhs, */
      /*                                  const iterator &rhs) { */
      /*   return iterator(lhs - rhs.index); */
      /* } */

      inline bool operator==(const iterator &rhs) const {
        return index == rhs.index;
      }
      inline bool operator!=(const iterator &rhs) const {
        return index != rhs.index;
      }
      inline bool operator>(const iterator &rhs) const {
        return index > rhs.index;
      }
      inline bool operator<(const iterator &rhs) const {
        return index < rhs.index;
      }
      inline bool operator>=(const iterator &rhs) const {
        return index >= rhs.index;
      }
      inline bool operator<=(const iterator &rhs) const {
        return index <= rhs.index;
      }

    private:
      const primes *ptr{nullptr};
      size_t index{0};
    };

    iterator begin() const noexcept { return iterator(this, 0); }
    iterator end() const noexcept { return iterator(this, prime_count); }

    iterator cbegin() const noexcept { return iterator(this, 0); }
    iterator cend() const noexcept { return iterator(this, prime_count); }

    uint64_t at(size_t index) const;
    /* auto all() -> auto { */
    /*   seek(in_memory_primes.size()); */
    /*   return ranges::views::concat(in_memory_primes, */
    /*                                ranges::istream_view<uint64_t>(data_file));
     */
    /* } */
    auto till(uint64_t upper_bound) -> auto {
      if (upper_bound > max_prime) {
        throw exception::out_of_range("more than max prime");
      }
      seek(in_memory_primes.size());
      return ranges::views::take_while(
          /* ranges::views::all(*this), [upper_bound](auto i) { return i <= upper_bound; }); */
          *this, [upper_bound](auto i) { return i <= upper_bound; });
    }

    bool has(uint64_t num) {
      if (num > max_prime) {
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
    static std::vector<uint64_t> in_memory_primes;
  };

} // namespace cyy::math
