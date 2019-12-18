/*!
 * \file rational.cpp
 *
 * \brief
 */

#include <regex>

#include "exception.hpp"
#include "number_theory.hpp"
#include "prime.hpp"
#include "rational.hpp"

namespace cyy::math {

  rational::rational(integer numerator, integer denominator)
      : p(std::move(numerator)), q(std::move(denominator)) {
    if (q == 0)
      throw exception::divided_by_zero("denominator");
    if (p == 0) {
      p = 0;
      q = 1;
    }
    if (q < 0) {
      p.change_sign();
      q.change_sign();
    }
  }
  rational::rational(std::string_view str) {
    static const std::regex rational_regex(
        "^[+-]?(0|[1-9][0-9]*)(/(0|[1-9][0-9]*))?$");
    if (!std::regex_match(str.begin(), str.end(), rational_regex)) {
      throw cyy::math::exception::no_rational(std::string(str));
    }
    auto pos = str.find('/');
    if (pos == std::string_view::npos) {
      p = str;
      q = 1;
      return;
    }
    p = str.substr(0, pos);
    q = str.substr(pos + 1);
  }

  /*
   *	功能：比较和另一个有理数的大小
   * 	参数：
   *		rhs：另一个有理数
   * 	返回值：
   * 		>0：大于另一个有理数
   * 		0：两个有理数相等
   * 		<0：小于另一个有理数
   */
  int rational::compare(const rational &rhs) const {
    return (p * rhs.q).compare(q * rhs.p);
  }

  rational rational::operator-() const {
    if (p == 0) {
      return *this;
    }
    return {-p, q};
  }
  rational &rational::operator+=(const rational &rhs) {
    if (this == &rhs) {
      p *= 2;
      return *this;
    }

    integer tmp_p = p * rhs.q + q * rhs.p;
    q *= rhs.q;
    p = std::move(tmp_p);
    normalize();
    return *this;
  }

  rational &rational::operator-=(const rational &rhs) {
    if (this == &rhs) {
      q = 0;
      p = 1;
      return *this;
    }

    integer tmp_p = p * rhs.q - q * rhs.p;
    p = tmp_p;
    q *= rhs.q;
    normalize();
    return *this;
  }

  rational &rational::operator*=(const rational &rhs) {
    p *= rhs.p;
    q *= rhs.q;
    normalize();
    return *this;
  }

  rational &rational::operator/=(const rational &rhs) {
    if (rhs == 0)
      throw exception::divided_by_zero("denominator");

    if (this == &rhs) {
      p = 1;
      q = 1;
      return *this;
    }

    p *= rhs.q;
    q *= rhs.p;
    normalize();
    return *this;
  }

  rational &rational::operator/=(const integer &rhs) {
    if (rhs == 0)
      throw exception::divided_by_zero("denominator");

    q *= rhs;
    normalize();
    return *this;
  }

  void rational::normalize() {
    if (q < 0) {
      p.change_sign();
      q.change_sign();
    }
  }

  rational &rational::simplify() {
    auto gcd_int = gcd(p, q);
    p /= gcd_int;
    q /= gcd_int;
    return *this;
  }

  std::string rational::to_string(const rational &abs_error) const {
    if (q == 1) {
      return p.to_string();
    }
    if (abs_error <= 0) {
      throw ::cyy::math::exception::out_of_range("abs_error <=0");
    }

    auto new_p = p;
    std::string res;
    if (new_p < 0) {
      new_p.change_sign();
      res.push_back('-');
    }

    auto [quotient, remainder] = new_p.div(q);
    res += quotient.to_string();
    rational cur_abs_error = 1;
    if (abs_error < cur_abs_error) {
      res.push_back('.');
    }

    while (abs_error < cur_abs_error) {
      remainder *= 10;
      std::tie(quotient, remainder) = remainder.div(q);
      res += quotient.to_string();
      cur_abs_error /= cyy::math::integer(10);
    }
    return res;
  }
} // namespace cyy::math
