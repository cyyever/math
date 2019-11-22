/*!
 * \file rational.cpp
 *
 * \brief
 */

#include <algorithm>
#include <cstdlib>
#include <iostream>

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

  void rational::normalize() {
    if (q < 0) {
      p.change_sign();
      q.change_sign();
    }
  }

  rational &rational::simplify() {
    auto prime_c = primes();

    for (auto prime : prime_c.till(100)) {
      while (p % prime == 0 && q % prime == 0) {
        p /= prime;
        q /= prime;
      }
    }
    return *this;
    auto gcd_int = gcd(p, q);
    p /= gcd_int;
    q /= gcd_int;
    return *this;
  }

  std::string rational::to_string() const {
    if (p == 0) {
      return p.to_string();
    }
    return p.to_string() + "/" + q.to_string();
  }
} // namespace cyy::math
