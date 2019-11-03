/*!
 * \file integer.hpp
 *
 * \brief integer related classes
 */

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <regex>

/* #include "my_arithmetic.h" */
#include "exception.hpp"
#include "integer.hpp"

using std::cout;
using std::endl;

namespace cyy::math {

  integer::integer(std::string_view str) {
    static const std::regex integer_regex("^[+-]?(0|[1-9][0-9]*)$");
    std::smatch m;
    if (!std::regex_match(str.begin(), str.end(), integer_regex)) {
      throw cyy::math::exception::no_integer(std::string(str));
    }
    if (str.starts_with('-')) {
      non_negative = false;
    }
    str.remove_prefix(1);

    uint64_t init_value = 0;
    while (!str.empty()) {
      auto next_value = init_value * 10 + (str[0] - '0');
      if (next_value >= base) {
        break;
      }
      init_value = next_value;
      str.remove_prefix(1);
    }
    digits.push_back(init_value);
    while (!str.empty()) {
      operator*=(10);
      operator+=(str[0] - '0');
      str.remove_prefix(1);
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
    if (sign == 0) //负数
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
    uint32_t carry = 0;

    if (digits.size() < rhs.digits.size()) {
      digits.insert(digits.end(), rhs.digits.size() - digits.size(), 0);
    }
    auto it = digits.begin();

    for (auto it2 = rhs.digits.cbegin(); it2 != rhs.digits.cend();
         it++, it2++) {
      uint64_t sum = static_cast<uint64_t>(*it) + static_cast<uint64_t>(*it2) +
                     static_cast<uint64_t>(carry);
      *it = static_cast<uint32_t>(sum & base);
      carry = static_cast<uint32_t>(sum >> 32);
    }
    if (carry) {
      digits.push_back(carry);
    }
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
      digits[i] = diffrence[i];
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

    std::vector<uint64_t> result_digits(digits.size() + 1, 0);
    for (size_t i = 0; i < digits.size(); i++) {
      uint64_t result =
          result_digits[i] + static_cast<uint64_t>(digits[i]) * rhs;
      result_digits[i] = result & base;
      result_digits[i + 1] = result >> 32;
    }
    digits.resize(result_digits.size());

    for (size_t i = 0; i < digits.size(); i++) {
      digits[i] = result_digits[i];
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

    std::vector<uint64_t> result_digits(
        std::max(digits.size(), rhs.digits.size()) + 1, 0);
    for (size_t i = 0; i < digits.size(); i++) {
      for (size_t j = 0; j < rhs.digits.size(); j++) {
        uint64_t result = result_digits[i + j] +
                          static_cast<uint64_t>(digits[i]) * rhs.digits[j];
        result_digits[i + j] = result & base;
        result_digits[i + j + 1] = result >> 32;
      }
    }
    digits.resize(result_digits.size());

    for (size_t i = 0; i < digits.size(); i++) {
      digits[i] = result_digits[i];
    }
    normalize();
    return *this;
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
#if 0




  /*
   *	功能：获取整数的位数
   * 	参数：
   * 		无
   * 	返回值：
   * 		位数
   */
  uint64_t integer::digit_num() const {
    return my_digit_num * (digits.size() - 1) +
           std::to_string(digits.back()).size();
  }








  integer &integer::operator*=(int64_t rhs) {
    if (rhs == -1) //这个常用，为了提高性能，特殊判定
    {
      if (!this->is_zero())
        sign = 1 - sign;
    } else {
      operator*=(abs(rhs));
      if (rhs < 0 && !this->is_zero())
        sign = 1 - sign;
    }
    return *this;
  }

  integer &integer::operator*=(int rhs) { return operator*=((int64_t)rhs); }

  integer &integer::operator*=(const integer &rhs) {
    if (this == &rhs) {
      integer tmp_this = *this;
      return operator*=(tmp_this);
    }

    if (this->is_zero())
      return *this;
    if (rhs.is_abs_one()) {
      if (rhs.sign == 0)
        sign = 1 - sign;
      return *this;
    }
    if (rhs.is_zero()) {
      *this = rhs;
      return *this;
    }
    decltype(rhs.digits.size()) cnt = 0, i;
    uint8_t new_sign = !(sign ^ rhs.sign);
    integer tmp_this = std::move(*this);

    *this = 0;
    for (auto it = rhs.digits.cbegin(); it != rhs.digits.cend(); it++, cnt++) {
      integer tmp = tmp_this * (*it);
      for (i = 0; i < cnt; i++)
        tmp.digits.push_front(0);
      operator+=(tmp);
    }
    sign = new_sign;
    return *this;
  }

  integer operator*(const integer &a, uint64_t b) { return integer(a) *= b; }

  integer operator*(const integer &a, int64_t b) { return integer(a) *= b; }

  integer operator*(const integer &a, int b) { return integer(a) *= b; }

  integer operator*(const integer &a, const integer &b) {
    return integer(a) *= b;
  }

  integer &integer::operator/=(uint64_t rhs) {
    unsigned __int128 tmp, carry;
    if (rhs == 0)
      throw std::invalid_argument("divided by zero");

    if (this->is_zero() || rhs == 1)
      return *this;

    carry = 0;
    for (auto it = --digits.end();; it--) {
      tmp = *it;
      if (carry)
        tmp += carry * my_base;

      if (tmp >= rhs) {
        *it = tmp / rhs;
        carry = tmp % rhs;
      } else {
        *it = 0;
        carry = tmp;
      }
      if (it == digits.begin())
        break;
    }

    //去除前面的0
    while (digits.back() == 0 && digits.size() > 1)
      digits.pop_back();

    if (is_zero())
      sign = 1;
    return *this;
  }

  integer &integer::operator/=(int64_t rhs) {
    operator/=(abs(rhs));
    if (rhs < 0 && !this->is_zero())
      sign = 1 - sign;
    return *this;
  }

  integer &integer::operator/=(int rhs) { return operator/=((int64_t)rhs); }

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

  integer operator/(const integer &a, uint64_t b) { return integer(a) /= b; }

  integer operator/(const integer &a, int64_t b) { return integer(a) /= b; }

  integer operator/(const integer &a, int b) { return integer(a) /= b; }

  integer operator/(const integer &a, const integer &b) {
    return integer(a) /= b;
  }

  integer &integer::operator%=(uint64_t rhs) {
    *this = (*this) - ((*this) / rhs) * rhs;
    return *this;
  }

  integer &integer::operator%=(int64_t rhs) {
    *this = (*this) - ((*this) / rhs) * rhs;
    return *this;
  }

  integer &integer::operator%=(int rhs) { return operator%=((int64_t)rhs); }

  integer &integer::operator%=(const integer &rhs) {
    *this = (*this) - ((*this) / rhs) * rhs;
    return *this;
  }

  integer operator%(const integer &a, uint64_t b) { return integer(a) %= b; }

  integer operator%(const integer &a, int64_t b) { return integer(a) %= b; }

  integer operator%(const integer &a, int b) { return integer(a) %= b; }

  integer operator%(const integer &a, const integer &b) {
    return integer(a) %= b;
  }

  ostream &operator<<(ostream &os, const integer &a) {
    os << static_cast<string>(a);
    return os;
  }

  integer::operator string() const {
    string int_str;
    uint8_t flag;
    char tmp[my_digit_num + 1];
    if (sign == 0)
      int_str.push_back('-');

    flag = 1;
    for (auto it = --digits.end();; it--) {
      auto tmp = std::to_string(*it);
      if (flag) {
        flag = 0;
      } else {
        int_str.insert(int_str.end(), my_digit_num - tmp.size(), '0');
      }
      int_str.append(tmp);
      if (it == digits.begin())
        break;
    }
    return int_str;
  }
#endif
} // namespace cyy::math
