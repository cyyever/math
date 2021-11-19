/*!
 * \file integer.hpp
 *
 * \brief integer related classes
 */
#pragma once

#include <boost/container_hash/hash.hpp>
#include <compare>
#include <cstdint>
#include <limits>
#include <ranges>
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace cyy::math {

  class integer final {
  public:
    integer() : digits{0} {}

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
    integer &operator*=(const integer &rhs);
    integer operator/(const integer &rhs) const;
    integer &operator/=(uint32_t rhs);
    integer &operator/=(const integer &rhs);
    int64_t operator%(uint32_t b);
    integer operator%(const integer &rhs) const;
    integer &operator%=(const integer &rhs);
    integer &multiply(uint32_t count);

    void change_sign() {
      if (digits.back() == 0) {
        non_negative = true;
        return;
      }
      non_negative = !non_negative;
    }
    std::strong_ordering operator<=>(const integer &rhs) const noexcept;
    bool operator==(const integer &rhs) const noexcept {
      normalize();
      rhs.normalize();
      return non_negative == rhs.non_negative && digits == rhs.digits;
    }

    std::pair<integer, integer> div(integer divisor) const;
    auto const &get_digits() const { return digits; }

  private:
    struct const_view {
      explicit const_view(const integer &n)
          : non_negative(n.non_negative),
            digits(n.digits.data(), n.digits.size()) {}
      bool non_negative;
      std::span<const uint32_t> digits;
      void change_sign() {
        if (digits.back() == 0) {
          non_negative = true;
          return;
        }
        non_negative = !non_negative;
      }
    };

  private:
    integer &operator+=(const_view rhs);
    integer &operator-=(const_view rhs);
    void normalize() const;

  private:
    mutable std::vector<uint32_t> digits;
    mutable bool non_negative{true};
    static constexpr uint64_t base =
        static_cast<uint64_t>(std::numeric_limits<uint32_t>::max()) + 1;
    static constexpr uint64_t mask = base - 1;
  };

  inline integer operator+(integer a, const integer &b) { return a += b; }
  inline integer operator-(integer a, const integer &b) { return a -= b; }
  inline integer operator*(integer a, const integer &b) { return a *= b; }
  inline integer operator/(integer a, uint32_t b) { return a /= b; }

  inline std::ostream &operator<<(std::ostream &os, const integer &a) {
    os << a.to_string();
    return os;
  }
} // namespace cyy::math

namespace std {
  template <> struct hash<cyy::math::integer> {
    std::size_t operator()(const cyy::math::integer &x) const noexcept {
      return boost::hash_range(std::begin(x.get_digits()),
                               std::end(x.get_digits()));
    }
  };
} // namespace std
