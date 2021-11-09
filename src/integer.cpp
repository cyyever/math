/*!
 * \file integer.hpp
 *
 * \brief integer related classes
 */

#include <algorithm>
#include <cassert>
#include <compare>
#include <cstdlib>
#include <range/v3/all.hpp>
#include <ranges>
#include <regex>

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
    if (should_be_negative && (*this) != 0) {
      non_negative = false;
    }
  }

  void integer::normalize() {
    while (digits.back() == 0 && digits.size() > 1) {
      digits.pop_back();
    }
    if (digits.back() == 0) {
      non_negative = true;
    }
  }

  std::strong_ordering integer::operator<=>(const integer &rhs) const noexcept {
    if (non_negative != rhs.non_negative) {
      if (non_negative && !rhs.non_negative)
        return std::strong_ordering::greater;
      else
        return std::strong_ordering::less;
    }

    int res;
    if (digits.size() < rhs.digits.size())
      res = -1;
    else if (digits.size() > rhs.digits.size())
      res = 1;
    else {
      res = 0;
      for (auto [a, b] :
           ranges::views::zip(digits, rhs.digits) | ::ranges::views::reverse) {
        if (a < b) {
          res = -1;
          break;
        } else if (a > b) {
          res = 1;
          break;
        }
      }
    }
    if (!non_negative) //负数
      res = -res;
    if (res > 0) {
      return std::strong_ordering::greater;
    } else if (res == 0) {
      return std::strong_ordering::equal;
    }
    return std::strong_ordering::less;
  }
  integer integer::operator-() const {
    auto res = *this;
    if (res != 0)
      res.non_negative = !res.non_negative;
    return res;
  }

  integer &integer::operator+=(const integer &rhs) {
    if (diffrent_sign(rhs)) //符号不同，转换成减法
    {
      if (!non_negative) {
        non_negative = true;
        operator-=(rhs);
        non_negative = false;
      } else {
        auto v = const_view(rhs);
        v.change_sign();
        operator-=(v);
      }
      normalize();
      return *this;
    }
    return operator+=(const_view(rhs));
  }

  integer &integer::operator+=(const_view rhs) {
    if (digits.size() <= rhs.digits.size()) {
      digits.resize(rhs.digits.size() + 1, 0);
    }

    uint64_t sum = 0;
    size_t i = 0;
    for (; i < static_cast<size_t>(rhs.digits.size()); i++) {
      sum += static_cast<uint64_t>(digits[i]) +
             static_cast<uint64_t>(rhs.digits[i]);
      digits[i] = static_cast<uint32_t>(sum & mask);
      sum >>= 32;
    }
    while (sum) {
      sum += static_cast<uint64_t>(digits[i]);
      digits[i] = static_cast<uint32_t>(sum & mask);
      sum >>= 32;
      i++;
    }
    normalize();
    return *this;
  }

  integer &integer::operator-=(const integer &rhs) {
    if (rhs == 0) {
      return *this;
    }
    if (this == &rhs) {
      *this = 0;
      return *this;
    }

    //转换成加法
    if (diffrent_sign(rhs)) {
      non_negative = !non_negative;
      operator+=(rhs);
      non_negative = !non_negative;
      normalize();
      return *this;
    }
    return operator-=(const_view(rhs));
  }
  integer &integer::operator-=(const_view rhs) {
    bool changed_sign = false;
    if (!non_negative && !rhs.non_negative) {
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

  integer &integer::multiply(uint32_t count) {
    auto whole_block_num = count / 32;
    auto remain_shift_num = count % 32;
    if (remain_shift_num) {
      digits.push_back(0);
      for (auto it = digits.rbegin() + 1; it != digits.rend(); it++) {
        uint64_t tmp = static_cast<uint64_t>(*it) << remain_shift_num;
        *(it - 1) |= static_cast<uint32_t>(tmp >> 32);
        *it = static_cast<uint32_t>(tmp & mask);
      }
    }
    if (whole_block_num) {
      digits.insert(digits.begin(), whole_block_num, 0);
    }
    normalize();
    return *this;
  }

  integer &integer::operator*=(const integer &rhs) {
    if (rhs == 0) {
      *this = 0;
      return *this;
    }
    if (rhs.is_abs_one()) {
      non_negative = (non_negative == rhs.non_negative);
      return *this;
    }

    std::vector<uint32_t> result_digits(digits.size() + rhs.digits.size() + 1,
                                        0);
    for (size_t i = 0; i < digits.size(); i++) {
      for (size_t j = 0; j < rhs.digits.size(); j++) {
        uint64_t result = static_cast<uint64_t>(digits[i]) * rhs.digits[j];
        auto k = i + j;
        do {
          result += result_digits[k];
          result_digits[k] = static_cast<uint32_t>(result & mask);
          result >>= 32;
          k++;
        } while (result);
      }
    }
    digits = std::move(result_digits);
    non_negative = (non_negative == rhs.non_negative);
    normalize();
    return *this;
  }

  integer &integer::operator/=(uint32_t rhs) {
    if (rhs == 0) {
      throw cyy::math::exception::divided_by_zero("");
    }

    if (*this == 0 || rhs == 1)
      return *this;

    uint64_t res = 0;
    for (auto &digit : std::ranges::reverse_view(digits)) {
      res = (res << 32) + digit;
      digit = static_cast<uint32_t>(res / rhs);
      res %= rhs;
    }
    normalize();
    return *this;
  }

  int64_t integer::operator%(uint32_t b) {
    if (b == 0) {
      throw cyy::math::exception::divided_by_zero("");
    }

    if (*this == 0 || b == 1)
      return 0;

    int64_t res = 0;

    for (auto digit : std::ranges::reverse_view(digits)) {
      res = ((res << 32) + digit) % b;
    }
    if (!non_negative) {
      res = -res;
    }
    return res;
  }

  integer integer::operator/(const integer &rhs) const {
    return div(rhs).first;
  }
  integer integer::operator%(const integer &rhs) const {
    return div(rhs).second;
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
    while (tmp != 0) {
      auto decimal_digit = tmp.operator%(10);
      tmp /= static_cast<uint32_t>(10);
      int_str.push_back(static_cast<char>('0' + decimal_digit));
    }
    if (int_str.empty()) {
      int_str.push_back('0');
    }
    if (!non_negative) {
      int_str.push_back('-');
    }
    std::ranges::reverse(int_str);
    return int_str;
  }

  integer &integer::operator/=(const integer &rhs) {
    *this = div(rhs).first;
    return *this;
  }
  integer &integer::operator%=(const integer &rhs) {
    *this = div(rhs).second;
    return *this;
  }

  std::pair<integer, integer> integer::div(integer divisor) const {
    if (divisor == 0)
      throw exception::divided_by_zero("divide");

    auto remainder = *this;
    bool real_divisor_non_negative = divisor.non_negative;
    remainder.non_negative = true;
    divisor.non_negative = true;

    integer res;
    while (remainder >= divisor) {
      auto zero_digit_num = remainder.digits.size() - divisor.digits.size();
      auto divisor_top_digit = divisor.digits.back();
      divisor_top_digit++;
      if (divisor_top_digit == 0) {
        if (zero_digit_num == 0) {
          break;
        }
        divisor_top_digit = 1;
        zero_digit_num--;
      }
      auto top_digit = remainder.digits.back() / divisor_top_digit;
      if (top_digit == 0) {
        if (zero_digit_num == 0) {
          break;
        }
        zero_digit_num--;
        top_digit = static_cast<uint32_t>(
            (static_cast<uint64_t>(remainder.digits.back()) << 32) /
            static_cast<uint64_t>(divisor_top_digit));
      }
      assert(top_digit != 0);
      auto tmp = divisor * top_digit;
      tmp.digits.insert(tmp.digits.begin(), zero_digit_num, 0);
      remainder -= tmp;
      integer partial_res;
      partial_res.digits.resize(zero_digit_num + 1);
      partial_res.digits.back() = top_digit;
      res += partial_res;
    }
    while (remainder >= divisor) {
      remainder -= divisor;
      ++res;
    }
    res.non_negative = (non_negative == real_divisor_non_negative);
    remainder.non_negative = non_negative;
    return {std::move(res), std::move(remainder)};
  }

} // namespace cyy::math
