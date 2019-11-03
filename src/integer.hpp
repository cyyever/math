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

  //整数表示成 (sign) (my_digit ... my_digit)*10^power，10^10进制
  class integer final {
  public:
    integer() { digits.push_back(0); }

    template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    integer(T num) {

      if constexpr (std::is_unsigned<T>::value) {
        do {
          digits.push_back(num & base);
          num /= base;
        } while (num);
        return;
      }

      bool add_one = false;
      if (num < 0) {
        non_negative = false;
        if (num == std::numeric_limits<T>::min()) {
          add_one = true;
          num += 1;
        }
        num = -num;
      }
      do {
        digits.push_back(num & base);
        num /= base;
      } while (num);
      if (add_one) {
        this->operator+=(1);
      }
    }

    integer(std::string_view str);

    integer(const integer &) = default;
    integer &operator=(const integer &) = default;

    integer(integer &&) noexcept = default;
    integer &operator=(integer &&) noexcept = default;
    ~integer() noexcept = default;

    operator std::string() const;
    operator bool() const { return !is_zero(); }
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
    integer &operator/=(const integer &rhs);
    integer &operator%=(const integer &rhs);

    friend integer operator+(const integer &a, const integer &b);
    friend integer operator*(const integer &a, const integer &b);
    friend integer operator/(const integer &a, const integer &b);
    friend integer operator%(const integer &a, const integer &b);

    int compare(const integer &rhs) const;

  private:
    void normalize();
    void assign_digits(const std::vector<int64_t> &new_digits);

  private:
    /* #ifdef NDEBUG */
    /*     static const size_t my_digit_num = 18; //单个my_digit所包含的位数 */
    /*     static const int64_t my_base = 1000000000000000000LL;
     * //单个my_digit的base */
    /* #else */
    /*     static const size_t my_digit_num = 1; //单个my_digit所包含的位数 */
    /*     static const int64_t my_base = 10;    //单个my_digit的base */
    /* #endif */
    static bool is_valid_int_str(const std::string &str);

    std::vector<uint32_t> digits;
    static const uint64_t base = static_cast<uint64_t>(1) << 32;
    bool non_negative{true};
    uint8_t sign; // 1-正数 0-负数
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
  integer operator-(const integer &a);
  integer operator+(const integer &a, uint64_t b);
  integer operator+(const integer &a, int64_t b);
  integer operator+(const integer &a, int b);
  integer operator+(const integer &a, const integer &b);
  integer operator-(const integer &a, uint64_t b);
  integer operator-(const integer &a, int64_t b);
  integer operator-(const integer &a, int b);
  integer operator-(const integer &a, const integer &b);
  integer operator*(const integer &a, uint64_t b);
  integer operator*(const integer &a, int64_t b);
  integer operator*(const integer &a, int b);
  integer operator*(const integer &a, const integer &b);
  integer operator/(const integer &a, uint64_t b);
  integer operator/(const integer &a, int64_t b);
  integer operator/(const integer &a, int b);
  integer operator/(const integer &a, const integer &b);
  integer operator%(const integer &a, uint64_t b);
  integer operator%(const integer &a, int64_t b);
  integer operator%(const integer &a, int b);
  integer operator%(const integer &a, const integer &b);

  std::ostream &operator<<(std::ostream &os, const integer &a);
} // namespace cyy::math
