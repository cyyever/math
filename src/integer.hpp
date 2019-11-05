/*!
 * \file integer.hpp
 *
 * \brief integer related classes
 */
#pragma once

#include <cinttypes>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace cyy::math {

  class integer final {
  public:
    integer() { digits.push_back(0); }

    template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    integer(T num) {

      if constexpr (std::is_unsigned<T>::value) {
        do {
          digits.push_back(num & (base - 1));
          num /= base;
        } while (num);
        return;
      }

      bool add_one = false;
      std::make_unsigned_t<T> unsigned_num = 0;
      if (num < 0) {
        non_negative = false;
        if (num == std::numeric_limits<T>::min()) {
          add_one = true;
          num += 1;
        }
        num = -num;
      }
      unsigned_num = static_cast<decltype(unsigned_num)>(num);
      do {
        digits.push_back(unsigned_num & (base - 1));
        unsigned_num /= base;
      } while (unsigned_num);
      if (add_one) {
        this->operator-=(1);
      }
    }

    integer(std::string_view str);

    integer(const integer &) = default;
    integer &operator=(const integer &) = default;

    integer(integer &&) noexcept = default;
    integer &operator=(integer &&) noexcept = default;
    ~integer() noexcept = default;

    operator std::string() const;
    std::string to_string() const;
    /* operator bool() const { return !is_zero(); } */
    bool diffrent_sign(const integer &rhs) const {
      return non_negative != rhs.non_negative;
    }
    /* uint64_t digit_num() const; */

    bool is_odd() const { return digits.back() & 1; }
    bool is_zero() const { return digits.back() == 0; }
    bool is_abs_one() const { return digits.back() == 1 && digits.size() == 1; }

    integer operator-() const;
    integer &operator+=(const integer &rhs);
    integer &operator++();   // prefix
    integer operator++(int); // suffix
    integer &operator--();   // prefix
    integer operator--(int); // suffix
    integer &operator-=(const integer &rhs);
    integer &operator*=(uint32_t rhs);
    integer &operator*=(const integer &rhs);
    integer &operator/=(uint32_t rhs);
    uint32_t operator%(uint32_t b);

    int compare(const integer &rhs) const;

  private:
    void normalize();

  private:
    std::vector<uint32_t> digits;
    static const uint64_t base = static_cast<uint64_t>(1) << 32;
    bool non_negative{true};
  };

  inline bool operator==(const integer &a, const integer &b) {
    return a.compare(b) == 0;
  }

  inline bool operator!=(const integer &a, const integer &b) {
    return !(a == b);
  }

  inline bool operator<(const integer &a, const integer &b) {
    return a.compare(b) < 0;
  }

  inline bool operator<=(const integer &a, const integer &b) {
    return a.compare(b) <= 0;
  }

  inline bool operator>(const integer &a, const integer &b) {
    return !(a <= b);
  }

  inline bool operator>=(const integer &a, const integer &b) {
    return !(a < b);
  }
  inline integer operator+(const integer &a, const integer &b) {
    return integer(a) += b;
  }
  inline integer operator-(const integer &a, const integer &b) {
    return integer(a) -= b;
  }
  inline integer operator*(const integer &a, const integer &b) {
    return integer(a) *= b;
  }
  /* integer operator/(const integer &a, uint64_t b); */
  /* integer operator/(const integer &a, int64_t b); */
  /* integer operator/(const integer &a, int b); */
  /* integer operator/(const integer &a, const integer &b); */
  /* integer operator%(const integer &a, uint64_t b); */
  /* integer operator%(const integer &a, int64_t b); */
  /* integer operator%(const integer &a, int b); */
  /* integer operator%(const integer &a, const integer &b); */

  inline std::ostream &operator<<(std::ostream &os, const integer &a) {
    os << a.to_string();
    return os;
  }
} // namespace cyy::math
