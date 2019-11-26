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
#include <utility>
#include <vector>

namespace cyy::math {

  class integer final {
  public:
    integer() { digits.push_back(0); }

    template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    integer(T num) {
      bool add_one = false;
      if constexpr (!std::is_unsigned<T>::value) {
        if (num < 0) {
          non_negative = false;
          if (num == std::numeric_limits<T>::min()) {
            add_one = true;
            num += 1;
          }
          num = -num;
        }
      }
      auto unsigned_num = static_cast<std::make_unsigned_t<T>>(num);
      if (unsigned_num < base) {
        digits.push_back(static_cast<uint32_t>(unsigned_num));
      } else {
        do {
          digits.push_back(unsigned_num & mask);
          unsigned_num =
              static_cast<decltype(unsigned_num)>(unsigned_num / base);
        } while (unsigned_num);
      }
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

    std::string to_string() const;
    bool diffrent_sign(const integer &rhs) const {
      return non_negative != rhs.non_negative;
    }

    void set_non_negative() { non_negative = true; }
    bool is_odd() const { return digits[0] & 1; }
    bool is_abs_one() const { return digits[0] == 1 && digits.size() == 1; }

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
    integer &operator/=(const integer &rhs);
    integer &operator%=(const integer &rhs);
    int64_t operator%(uint32_t b);
    integer operator%(const integer &rhs);
    integer &multiply_2(uint32_t count);

    void change_sign() {
      if (digits.back() == 0) {
        non_negative = true;
        return;
      }
      non_negative = !non_negative;
    }
    int compare(const integer &rhs) const;

    std::pair<integer, integer> div(integer divisor) const;

  private:
    void normalize();

  private:
    std::vector<uint32_t> digits;
    static constexpr uint64_t base =
        static_cast<uint64_t>(std::numeric_limits<uint32_t>::max()) + 1;
    static constexpr uint64_t mask = base - 1;
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
  inline integer operator*(const integer &a, uint32_t b) {
    return integer(a) *= b;
  }
  inline integer operator/(const integer &a, uint32_t b) {
    return integer(a) /= b;
  }
  inline integer operator/(const integer &a, const integer &b) {
    return integer(a) /= b;
  }

  inline std::ostream &operator<<(std::ostream &os, const integer &a) {
    os << a.to_string();
    return os;
  }
} // namespace cyy::math
