/*!
 * \file integer.hpp
 *
 * \brief integer related classes
 */

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>
#include <regex>

/* #include "my_arithmetic.h" */
#include "exception.hpp"
#include "integer.hpp"

namespace cyy::math {

  integer::integer(std::string_view str) {
    static const std::regex integer_regex("^[+-]?(0|[1-9][0-9]*)$");
    if (!std::regex_match(str.begin(), str.end(), integer_regex)) {
      throw cyy::math::exception::no_integer(std::string(str));
    }
    bool should_be_negative = str.starts_with('-');
    if (should_be_negative) {
      str.remove_prefix(1);
    }

    digits.push_back(0);
    for (auto c : str) {
      operator*=(10);
      operator+=(c - '0');
    }
    if (should_be_negative && !is_zero()) {
      non_negative = false;
    }
  }

  void integer::normalize() {
    while (digits.back() == 0 && digits.size() > 1) {
      digits.pop_back();
    }
    if (is_zero()) {
      non_negative = true;
    }
  }

  /*
   *	功能：比较和另一个整数的大小
   * 	参数：
   *		rhs：另一个整数
   * 	返回值：
   * 		>0：大于另一个整数
   * 		0：两个整数相等
   * 		<0：小于另一个整数
   */
  int integer::compare(const integer &rhs) const {
    if (non_negative != rhs.non_negative) {
      if (non_negative && !rhs.non_negative)
        return 1;
      else
        return -1;
    }

    int res;
    if (digits.size() < rhs.digits.size())
      res = -1;
    else if (digits.size() > rhs.digits.size())
      res = 1;
    else {
      res = 0;
      auto it = digits.crbegin();
      auto it2 = rhs.digits.crbegin();
      for (; it != digits.crend() && it2 != rhs.digits.crend(); it++, it2++) {
        if (*it < *it2) {
          res = -1;
          break;
        } else if (*it > *it2) {
          res = 1;
          break;
        }
      }
    }
    if (!non_negative) //负数
      res = -res;
    return res;
  }
  integer integer::operator-() const {
    auto res = *this;
    if (!res.is_zero())
      res.non_negative = !res.non_negative;
    return res;
  }

  integer &integer::operator+=(const integer &rhs) {
    if (rhs.is_zero()) {
      return *this;
    }
    if (this == &rhs) {
      return operator*=(2);
    }

    if (diffrent_sign(rhs)) //符号不同，转换成减法
    {
      bool changed_sign = false;
      if (!non_negative) {
        non_negative = true;
        changed_sign = true;
      }
      operator-=(rhs);
      if (changed_sign) {
        non_negative = false;
      }
      normalize();
      return *this;
    }

    if (digits.size() < rhs.digits.size()) {
      digits.resize(rhs.digits.size() + 1, 0);
    }

    uint64_t sum = 0;
    size_t i = 0;
    for (; i < rhs.digits.size(); i++) {
      sum += static_cast<uint64_t>(digits[i]) +
             static_cast<uint64_t>(rhs.digits[i]);
      digits[i] = static_cast<uint32_t>(sum & (base - 1));
      sum >>= 32;
    }
    while (sum) {
      sum += static_cast<uint64_t>(digits[i]);
      digits[i] = static_cast<uint32_t>(sum & (base - 1));
      sum >>= 32;
      i++;
    }
    normalize();
    return *this;
  }

  integer &integer::operator-=(const integer &rhs) {
    if (rhs.is_zero()) {
      return *this;
    }
    if (this == &rhs) {
      *this = 0;
      return *this;
    }

    //转换成加法
    if (non_negative && !rhs.non_negative) {
      non_negative = false;
      operator+=(rhs);
      non_negative = true;
      normalize();
      return *this;
    }
    if (!non_negative && rhs.non_negative) {
      non_negative = true;
      operator+=(rhs);
      non_negative = false;
      normalize();
      return *this;
    }
    bool changed_sign = false;
    if (!non_negative && !rhs.non_negative) {
      non_negative = true;
      changed_sign = true;
    }

    std::vector<int64_t> diffrence(std::max(digits.size(), rhs.digits.size()),
                                   0);
    size_t i = 0;
    for (; i < std::min(digits.size(), rhs.digits.size()); i++) {
      diffrence[i] =
          static_cast<int64_t>(digits[i]) - static_cast<int64_t>(rhs.digits[i]);
    }
    for (; i < digits.size(); i++) {
      diffrence[i] = static_cast<int64_t>(digits[i]);
    }
    for (; i < rhs.digits.size(); i++) {
      diffrence[i] = -static_cast<int64_t>(rhs.digits[i]);
    }
    while (diffrence.back() == 0 && diffrence.size() > 1) {
      diffrence.pop_back();
    }
    non_negative = true;
    if (diffrence.back() < 0) {
      non_negative = false;
      for (auto &num : diffrence) {
        num = -num;
      }
    }
    digits.resize(diffrence.size());
    for (i = 0; i < diffrence.size(); i++) {
      if (diffrence[i] < 0) {
        diffrence[i + 1]--;
        diffrence[i] += static_cast<int64_t>(base);
      }
      digits[i] = static_cast<uint32_t>(diffrence[i]);
    }
    if (changed_sign) {
      non_negative = !non_negative;
    }

    normalize();
    return *this;
  }

  integer &integer::operator*=(uint32_t rhs) {
    if (rhs == 0) {
      *this = 0;
      return *this;
    }
    if (rhs == 1) {
      return *this;
    }

    uint64_t result = 0;
    for (auto &digit : digits) {
      result += static_cast<uint64_t>(digit) * rhs;
      digit = static_cast<uint32_t>(result & (base - 1));
      result >>= 32;
    }
    if (result) {
      digits.push_back(static_cast<uint32_t>(result));
    }
    normalize();
    return *this;
  }

  integer &integer::operator*=(const integer &rhs) {
    if (rhs.is_zero()) {
      *this = 0;
      return *this;
    }
    if (rhs.is_abs_one()) {
      non_negative = (non_negative == rhs.non_negative);
      return *this;
    }

    std::vector<uint32_t> result_digits(
        std::max(digits.size(), rhs.digits.size()) + 1, 0);
    for (size_t i = 0; i < digits.size(); i++) {
      for (size_t j = 0; j < rhs.digits.size(); j++) {
        uint64_t result = static_cast<uint64_t>(digits[i]) * rhs.digits[j];
        auto k = i + j;
        do {
          result += result_digits[k];
          result_digits[k] = static_cast<uint32_t>(result & (base - 1));
          result >>= 32;
          k++;
        } while (result);
      }
    }
    digits = std::move(result_digits);
    normalize();
    return *this;
  }

  integer &integer::operator/=(uint32_t rhs) {
    if (rhs == 0) {
      throw cyy::math::exception::divided_by_zero("");
    }

    if (this->is_zero() || rhs == 1)
      return *this;

    uint64_t res = 0;
    for (auto &digit : ranges::reverse_view(digits)) {
      res = (res << 32) + digit;
      digit = static_cast<uint32_t>(res / rhs);
      res %= rhs;
    }
    normalize();
    return *this;
  }

  uint32_t integer::operator%(uint32_t b) {
    if (b == 0) {
      throw cyy::math::exception::divided_by_zero("");
    }

    if (is_zero() || b == 1)
      return 0;

    uint64_t res = 0;

    for (auto digit : ranges::reverse_view(digits)) {
      res = ((res << 32) + digit) % b;
    }
    if (!non_negative) {
      res = b - res;
    }
    return static_cast<uint32_t>(res);
  }

  integer &integer::operator++() {
    operator+=(1);
    return *this;
  }

  integer &integer::operator--() {
    operator-=(1);
    return *this;
  }

  integer integer::operator++(int) {
    integer tmp = *this;
    operator+=(1);
    return tmp;
  }

  integer integer::operator--(int) {
    integer tmp = *this;
    operator-=(1);
    return tmp;
  }
  std::string integer::to_string() const {
    std::string int_str;
    int_str.reserve(digits.size() * 8);

    auto tmp = *this;
    tmp.non_negative = true;
    while (tmp) {
      auto decimal_digit = tmp.operator%(10);
      tmp /= 10;
      int_str.push_back(static_cast<char>('0' + decimal_digit));
    }
    if (int_str.empty()) {
      int_str.push_back('0');
    }
    if (!non_negative) {
      int_str.push_back('-');
    }
    ranges::reverse(int_str);
    return int_str;
  }
#if 0

  integer &integer::operator/=(const integer &rhs) {
    integer quotient, tmp, low_bound, high_bound;
    uint8_t org_sign = sign;
    int compare_res = 0;
    if (rhs.is_zero())
      throw std::invalid_argument("divided by zero");

    if (this == &rhs) {
      *this = 1;
      return *this;
    }

    if (this->is_zero())
      return *this;
    if (rhs.is_abs_one()) {
      if (rhs.sign == 0)
        sign = 1 - sign;
      return *this;
    }

    //通过二分法找出来，注意这边我们转成正数
    sign = 1;
    high_bound = *this;

    while (low_bound <= high_bound) {
      integer res = (high_bound + low_bound) / 2;
      tmp = res * rhs;
      if (rhs.sign == 0)
        tmp.sign = 1 - tmp.sign;

      compare_res = compare(tmp);
      if (compare_res >= 0)
        quotient = std::move(res);
      if (compare_res == 0)
        break;
      else if (compare_res > 0)
        low_bound = res + 1;
      else
        high_bound = res - 1;
    }

    *this = std::move(quotient);

    if (is_zero())
      sign = 1;
    else
      sign = !(org_sign ^ rhs.sign);
    return *this;
  }

#endif
} // namespace cyy::math
