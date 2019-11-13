/*!
 * \file rational.hpp
 *
 * \brief
 */
#pragma once

#include <cinttypes>
#include <string>

#include "integer.hpp"

namespace cyy::math {

  class rational final {
  public:
    rational() : p(0), q(1) {}

    template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    rational(T num) : rational(integer(num), integer(1)) {}
    rational(integer numerator, integer denominator);

    rational(integer a) : p(std::move(a)), q(1) {}
    rational(const rational &) = default;
    rational &operator=(const rational &) = default;

    rational(rational &&) noexcept = default;
    rational &operator=(rational &&) noexcept = default;
    ~rational() noexcept = default;

    std::string to_string() const;

    rational &operator+=(const rational &rhs);
    rational &operator-=(const rational &rhs);
    rational &operator*=(const rational &rhs);
    rational &operator/=(const rational &rhs);
    void normalize();
    /* rational &simplify(); */

    const integer &numerator() const { return p; }
    const integer &denominator() const { return q; }
    /* bool is_abs_one() const { return *this != 0 && p == q; } */
    int compare(const rational &rhs) const;

  private:
  private:
    integer p; //分子，有理数的符号由它体现
    integer q; //分母，恒为正数
  };

  inline bool operator==(const rational &a, const rational &b) {
    return a.compare(b) == 0;
  }

  inline bool operator!=(const rational &a, const rational &b) {
    return !(a == b);
  }

  inline bool operator<(const rational &a, const rational &b) {
    return a.compare(b) < 0;
  }

  inline bool operator<=(const rational &a, const rational &b) {
    return a.compare(b) <= 0;
  }

  inline bool operator>(const rational &a, const rational &b) {
    return !(a <= b);
  }

  inline bool operator>=(const rational &a, const rational &b) {
    return !(a < b);
  }
  inline rational operator+(const rational &a, const rational &b) {
    return rational(a) += b;
  }
  inline rational operator-(const rational &a, const rational &b) {
    return rational(a) -= b;
  }
  inline rational operator*(const rational &a, const rational &b) {
    return rational(a) *= b;
  }

} // namespace cyy::math
