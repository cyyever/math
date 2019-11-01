/*
 *	��������integer.cpp
 *	���ߣ���ԴԴ
 *	���ڣ�2016-03-28
 *	���ܣ����������ͺ���
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
#if 0



  /*
   *	���ܣ��ȽϺ���һ�������Ĵ�С
   * 	������
   *		rhs����һ������
   * 	����ֵ��
   * 		>0��������һ������
   * 		0�������������
   * 		<0��С����һ������
   */
  int integer::compare(const integer &rhs) const {
    int res;
    if (sign < rhs.sign)
      return -1;
    else if (sign > rhs.sign)
      return 1;

    if (digits.size() < rhs.digits.size())
      res = -1;
    else if (digits.size() > rhs.digits.size())
      res = 1;
    else {
      res = 0;
      auto it = --digits.end();
      auto it2 = --rhs.digits.cend();
      for (;; it--, it2--) {
        if (*it < *it2) {
          res = -1;
          break;
        } else if (*it > *it2) {
          res = 1;
          break;
        }
        if (it == digits.begin())
          break;
      }
    }
    if (sign == 0) //����
      res = -res;
    return res;
  }

  /*
   *	���ܣ���ȡ������λ��
   * 	������
   * 		��
   * 	����ֵ��
   * 		λ��
   */
  uint64_t integer::digit_num() const {
    return my_digit_num * (digits.size() - 1) +
           std::to_string(digits.back()).size();
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

  integer operator-(const integer &a) {
    integer b = a;
    if (!b.is_zero())
      b.sign = 1 - b.sign;
    return b;
  }

  integer &integer::operator+=(int64_t rhs) {
    if (rhs >= 0)
      return operator+=((uint64_t)rhs);
    else
      return operator-=(abs(rhs));
  }

  integer &integer::operator+=(int rhs) { return operator+=((int64_t)rhs); }

  integer &integer::operator+=(uint64_t rhs) {
    if (sign == 0) //���Ų�ͬ��ת���ɼ���
    {
      sign = 1 - sign;
      operator-=(rhs);
      if (!is_zero())
        sign = 1 - sign;
      return *this;
    }
    unsigned __int128 tmp, carry;
    auto it = digits.begin();

    tmp = (unsigned __int128)(*it) + rhs;

    if (tmp >= my_base) {
      carry = 1;
      tmp -= my_base;
      if (tmp >= my_base) {
        carry += (tmp / my_base);
        *it = tmp % my_base;
      } else
        *it = tmp;

      it++;

      while (it != digits.end()) {
        tmp = *it + carry;
        if (tmp >= my_base) {
          carry = 1;
          tmp -= my_base;
          if (tmp >= my_base) {
            carry += (tmp / my_base);
            *it = tmp % my_base;
          } else
            *it = tmp;
        } else {
          *it = tmp;
          carry = 0;
          break;
        }
        it++;
      }

      if (carry > 0) {
        if (carry < my_base)
          digits.push_back(carry);
        else {
          while (carry) {
            digits.push_back(carry % my_base);
            carry /= my_base;
          }
        }
      }
    } else
      *it = tmp;
    return *this;
  }

  integer &integer::operator+=(const integer &rhs) {
    if (this == &rhs) {
      integer tmp_this = *this;
      return operator+=(tmp_this);
    }

    if (sign != rhs.sign) //���Ų�ͬ��ת���ɼ���
    {
      sign = 1 - sign;
      operator-=(rhs);
      if (!is_zero())
        sign = 1 - sign;
      return *this;
    }
    uint8_t carry = 0;

    auto it = digits.begin();
    auto it2 = rhs.digits.cbegin();
    for (; it != digits.end() && it2 != rhs.digits.cend(); it++, it2++) {
      *it += *it2 + carry;
      if ((*it) >= my_base) {
        carry = 1;
        *it -= my_base;
      } else
        carry = 0;
    }
    if (it2 != rhs.digits.cend()) {
      do {
        int64_t my_digit = *it2 + carry;

        if (my_digit >= my_base) {
          carry = 1;
          my_digit -= my_base;
        } else
          carry = 0;
        digits.push_back(my_digit);
        it2++;
      } while (it2 != rhs.digits.cend());
    } else if (carry == 1 && it != digits.end()) {
      do {
        *it += 1;
        if ((*it) >= my_base)
          *it -= my_base;
        else {
          carry = 0;
          break;
        }
        it++;
      } while (it != digits.end());
    }

    if (carry == 1)
      digits.push_back(1);
    return *this;
  }

  integer operator+(const integer &a, uint64_t b) { return integer(a) += b; }

  integer operator+(const integer &a, int64_t b) { return integer(a) += b; }

  integer operator+(const integer &a, int b) { return integer(a) += b; }

  integer operator+(const integer &a, const integer &b) {
    return integer(a) += b;
  }

  integer &integer::operator-=(uint64_t rhs) {
    if (sign == 0) //���Ų�ͬ��ת���ɼӷ�
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
        //�����Ļ�ǰ�漸��my_digit�Ǹ������������·�ת�����Ų���λ
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

    //ȥ��ǰ���0
    while (digits.back() == 0 && digits.size() > 1)
      digits.pop_back();

    //���������ȵĸ���������������Ҫ��������Ϊ��
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

  integer &integer::operator-=(const integer &rhs) {
    if (this == &rhs) {
      *this = 0;
      return *this;
    }

    if (sign != rhs.sign) //���Ų�ͬ��ת���ɼӷ�
    {
      sign = 1 - sign;
      operator+=(rhs);
      if (!is_zero())
        sign = 1 - sign;
      return *this;
    }

    uint8_t carry = 0;

    auto it = digits.begin();
    auto it2 = rhs.digits.cbegin();
    for (; it != digits.end() && it2 != rhs.digits.cend(); it++, it2++) {
      *it -= (*it2 + carry);
      if ((*it) < 0) {
        *it += my_base;
        carry = 1;
      } else
        carry = 0;
    }
    if (it2 != rhs.digits.cend()) {
      do {
        int64_t my_digit = -(*it2 + carry);

        if (my_digit < 0) {
          my_digit += my_base;
          carry = 1;
        } else
          carry = 0;
        digits.push_back(my_digit);
        it2++;
      } while (it2 != rhs.digits.cend());
    } else if (carry == 1) {
      for (; it != digits.end(); it++) {
        *it -= 1;
        if ((*it) < 0)
          *it += my_base;
        else {
          carry = 0;
          break;
        }
      }
    }

    if (carry == 1) {
      digits.push_back(-1);
      //�����Ļ�ǰ�漸��my_digit�Ǹ������������·�ת�����Ų���λ
      carry = 0;
      for (auto it = digits.begin(); it != digits.end(); it++) {
        *it = -(*it + carry);
        if (*it < 0) {
          *it += my_base;
          carry = 1;
        } else
          carry = 0;
      }
      sign = 1 - sign;
    }

    //ȥ��ǰ���0
    while (digits.back() == 0 && digits.size() > 1)
      digits.pop_back();

    //���������ȵĸ���������������Ҫ��������Ϊ��
    if (is_zero())
      sign = 1;
    return *this;
  }

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
    if (rhs == -1) //������ã�Ϊ��������ܣ������ж�
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

    //ȥ��ǰ���0
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

    //ͨ�����ַ��ҳ�����ע���������ת������
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
   *	���ܣ���鴫����ַ����Ƿ����������Ƿ�ƥ�� ^[+-]?[1-9][0-9]*$
   * 	������
   *		str��Ҫ�����ַ���
   * 	����ֵ��
   *		true����
   *		false������
   */
  bool integer::is_valid_int_str(const string &str) {
      return true;
    else
      return false;
  }
#endif
} // namespace cyy::math
