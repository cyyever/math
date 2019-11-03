/*!
 * \file integer.cpp
 *
 * \brief
 */
/*!
 * \file integer.cpp
 *
 * \brief
 */
/*!
 * \file integer.cpp
 *
 * \brief
 */
/*
 *	程序名：integer.cpp
 *	作者：陈源源
 *	日期：2016-03-28
 *	功能：整数相关类和函数
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

    /*
    thadsadsadsadsadsadsaow

    decltype(int_str.size()) i, first_digit_index;
    if (int_str[0] == '-') {
      sign = 0;
      first_digit_index = 1;
    } else
      first_digit_index = 0;

    i = int_str.size();
    while (i - first_digit_index >= my_digit_num) {
      digits.push_back(stoll(int_str.substr(i - my_digit_num, my_digit_num)));
      i -= my_digit_num;
    }
    if (i - first_digit_index > 0)
      digits.push_back(
          stoll(int_str.substr(first_digit_index, i - first_digit_index)));
    return;
    */
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
      // TODO *=2
      return operator+=(*this);
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
    auto it = diffrence.rbegin();
    it++;
    for (; it != diffrence.rend(); it++) {
      if (*it < 0) {
        *(it - 1)--;
        *it += static_cast<int64_t>(base);
      }
    }
    if (changed_sign) {
      non_negative = !non_negative;
    }

    normalize();
    return *this;
  }

  bool operator==(const integer &a, const integer &b) {
    return a.compare(b) == 0;
  }

  bool operator!=(const integer &a, const integer &b) { return !(a == b); }

  bool operator<(const integer &a, const integer &b) {
    return a.compare(b) < 0;
  }

  bool operator<=(const integer &a, const integer &b) {
    return a.compare(b) <= 0;
  }

  bool operator>(const integer &a, const integer &b) { return !(a <= b); }

  bool operator>=(const integer &a, const integer &b) { return !(a < b); }

  integer operator+(const integer &a, const integer &b) {
    return integer(a) += b;
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





  integer &integer::operator-=(uint64_t rhs) {
    if (sign == 0) //符号不同，转换成加法
    {
      sign = 1 - sign;
      operator+=(rhs);
      if (!is_zero())
        sign = 1 - sign;
      return *this;
    }

    __int128 tmp, carry;
    auto it = digits.begin();

    tmp = (__int128)(*it) - rhs;

    if (tmp < 0) {
      carry = 1;
      tmp += my_base;
      if (tmp < 0) {
        carry += ((-tmp) / my_base);
        tmp += (carry - 1) * my_base;
        if (tmp < 0) {
          carry++;
          tmp += my_base;
        }
      }
      *it = tmp;

      it++;

      while (it != digits.end()) {
        tmp = *it - carry;
        if (tmp < 0) {
          carry = 1;
          tmp += my_base;
          if (tmp < 0) {
            carry += ((-tmp) / my_base);
            tmp += (carry - 1) * my_base;
            if (tmp < 0) {
              carry++;
              tmp += my_base;
            }
          }
          *it = tmp;
        } else {
          *it = tmp;
          carry = 0;
          break;
        }
        it++;
      }

      if (carry > 0) {
        //这样的话前面几个my_digit是负数，我们重新翻转正负号并补位
        for (auto it = digits.begin(); it != digits.end(); it++)
          *it = -(*it);

        if (carry < my_base)
          digits.push_back(carry);
        else {
          while (carry) {
            digits.push_back(carry % my_base);
            carry /= my_base;
          }
        }

        carry = 0;
        for (auto it = digits.begin(); it != digits.end(); it++) {
          *it -= carry;
          if (*it < 0) {
            *it += my_base;
            carry = 1;
          } else
            carry = 0;
        }
        sign = 1 - sign;
      }
    } else
      *it = tmp;

    //去除前面的0
    while (digits.back() == 0 && digits.size() > 1)
      digits.pop_back();

    //如果两个相等的负数相减，这边我们要调整符号为正
    if (is_zero())
      sign = 1;
    return *this;
  }

  integer &integer::operator-=(int64_t rhs) {
    if (rhs >= 0)
      return operator-=((uint64_t)rhs);
    else
      return operator+=(abs(rhs));
  }

  integer &integer::operator-=(int rhs) { return operator-=((int64_t)rhs); }


  integer operator-(const integer &a, uint64_t b) { return integer(a) -= b; }

  integer operator-(const integer &a, int64_t b) { return integer(a) -= b; }

  integer operator-(const integer &a, int b) { return integer(a) -= b; }

  integer operator-(const integer &a, const integer &b) {
    return integer(a) -= b;
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

  integer &integer::operator*=(uint64_t rhs) {
    int64_t carry;
    unsigned __int128 tmp_my_digit;

    if (this->is_zero() || rhs == 1)
      return *this;

    if (rhs == 0) {
      *this = 0;
      return *this;
    }

    carry = 0;
    for (auto it = digits.begin(); it != digits.end(); it++) {
      tmp_my_digit = (unsigned __int128)(*it) * rhs + carry;
      if (tmp_my_digit >= integer::my_base) {
        *it = tmp_my_digit % integer::my_base;
        carry = tmp_my_digit / integer::my_base;
      } else {
        *it = tmp_my_digit;
        carry = 0;
      }
    }

    while (carry) {
      if (carry >= integer::my_base) {
        digits.push_back(carry % integer::my_base);
        carry /= integer::my_base;
      } else {
        digits.push_back(carry);
        carry = 0;
      }
    }
    return *this;
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
  /*
   *	功能：检查传入的字符串是否整数，即是否匹配 ^[+-]?[1-9][0-9]*$
   * 	参数：
   *		str：要检查的字符串
   * 	返回值：
   *		true：是
   *		false：不是
   */
  bool integer::is_valid_int_str(const string &str) {
      return true;
    else
      return false;
  }
#endif
} // namespace cyy::math
