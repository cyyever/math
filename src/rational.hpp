/*!
 * \file rational.hpp
 *
 * \brief
 */
#pragma once

#include <string>

#include "integer.hpp"
#include <boost/functional/hash.hpp>

namespace cyy::math {

  class rational final {
  public:
    rational() : p(0), q(1) {}

    template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    rational(T num) : rational(integer(num), integer(1)) {}
    rational(integer numerator, integer denominator);
    rational(std::string_view str);

    rational(integer a) : p(std::move(a)), q(1) {}
    rational(const rational &) = default;
    rational &operator=(const rational &) = default;

    rational(rational &&) noexcept = default;
    rational &operator=(rational &&) noexcept = default;
    ~rational() noexcept = default;

    std::string to_string(const rational &abs_error = rational(1, 1000)) const;

    rational operator-() const;
    rational &operator+=(const rational &rhs);
    rational &operator-=(const rational &rhs);
    rational &operator*=(const rational &rhs);
    rational &operator/=(const rational &rhs);
    rational &operator/=(const integer &rhs);
    void normalize();
    rational &simplify();

    const integer &numerator() const { return p; }
    const integer &denominator() const { return q; }
    integer round_zero() const { return p / q; }
    std::strong_ordering operator<=>(const rational &rhs) const noexcept;

    bool operator==(const rational &rhs) const noexcept {
      return (*this <=> rhs) == std::strong_ordering::equal;
    }

    rational reciprocal() const { return rational(q, p); }

  private:
    integer p; //分子，有理数的符号由它体现
    integer q; //分母，恒为正数
  };

  inline rational operator+(const rational &a, const rational &b) {
    return rational(a) += b;
  }
  inline rational operator-(const rational &a, const rational &b) {
    return rational(a) -= b;
  }
  inline rational operator*(const rational &a, const rational &b) {
    return rational(a) *= b;
  }
  inline rational operator/(const rational &a, const rational &b) {
    return rational(a) /= b;
  }
  inline rational operator/(const rational &a, const integer &b) {
    return rational(a) /= b;
  }

  inline std::ostream &operator<<(std::ostream &os, const rational &a) {
    os << a.to_string();
    return os;
  }
} // namespace cyy::math

namespace std {
  template <> struct hash<cyy::math::rational> {
    std::size_t operator()(const cyy::math::rational &x) const noexcept {
      std::size_t seed = 0;
      boost::hash_combine(seed, std::hash<cyy::math::integer>()(x.numerator()));
      boost::hash_combine(seed,
                          std::hash<cyy::math::integer>()(x.denominator()));
      return seed;
    }
  };
} // namespace std
