/*!
 * \file integer.hpp
 *
 * \brief integer related classes
 */
#pragma once

#include <cinttypes>
#include <list>
#include <std::string>

using std::list;
using std::ostream;
using std::std::string;

namespace cyy::math {

  //整数表示成 (sign) (my_digit ... my_digit)*10^power，10^10进制
  class integer {
  public:
    integer();
    integer(const std::string &int_str);
    integer(uint64_t num);
    integer(int64_t num);
    integer(int num);
    integer(const integer &) = default;
    ~integer() = default;
    operator std::string() const;
    operator bool() const { return !is_zero(); }
    bool diffrent_sign(const integer &rhs) const { return sign != rhs.sign; }
    int compare(const integer &rhs) const;
    uint64_t digit_num() const;

    bool is_odd() const { return digits.back() & 1; }
    bool is_zero() const { return digits.back() == 0; }
    bool is_abs_one() const { return digits.back() == 1 && digits.size() == 1; }

    integer &operator=(const integer &) = default;
    integer &operator+=(const integer &rhs);
    integer operator-(const integer &rhs) const;
    integer &operator-=(const integer &rhs);
    integer &operator++(); //前缀
    integer &operator--();
    integer operator++(int); //后缀
    integer operator--(int);
    integer &operator*=(const integer &rhs);
    integer &operator/=(const integer &rhs);
    integer &operator%=(const integer &rhs);

  private:
#ifdef NDEBUG
    static const size_t my_digit_num = 18; //单个my_digit所包含的位数
    static const int64_t my_base = 1000000000000000000LL; //单个my_digit的base
#else
    static const size_t my_digit_num = 1; //单个my_digit所包含的位数
    static const int64_t my_base = 10;    //单个my_digit的base
#endif
    static bool is_valid_int_str(const std::string &str);

    std::vector<int32_t> digits;
    bool non_negative;
    uint8_t sign; // 1-正数 0-负数

    friend bool operator==(const integer &a, const integer &b);
    friend bool operator!=(const integer &a, const integer &b);
    friend bool operator<(const integer &a, const integer &b);
    friend bool operator<=(const integer &a, const integer &b);
    friend bool operator>(const integer &a, const integer &b);
    friend bool operator>=(const integer &a, const integer &b);
    friend integer operator+(const integer &a, const integer &b);
    friend integer operator*(const integer &a, const integer &b);
    friend integer operator/(const integer &a, const integer &b);
    friend integer operator%(const integer &a, const integer &b);
  };

  bool operator==(const integer &a, const integer &b);
  bool operator!=(const integer &a, const integer &b);
  bool operator<(const integer &a, const integer &b);
  bool operator<=(const integer &a, const integer &b);
  bool operator>(const integer &a, const integer &b);
  bool operator>=(const integer &a, const integer &b);
  integer operator-(const integer &a);
  integer operator+(const integer &a, uint64_t b);
  integer operator+(const integer &a, int64_t b);
  integer operator+(const integer &a, int b);
  integer operator+(const integer &a, const integer &b);
  integer operator-(const integer &a, uint64_t b);
  integer operator-(const integer &a, int64_t b);
  integer operator-(const integer &a, int b);
  integer operator-(const integer &a, const integer &b);
  integer operator*(const integer &a, uint64_t b);
  integer operator*(const integer &a, int64_t b);
  integer operator*(const integer &a, int b);
  integer operator*(const integer &a, const integer &b);
  integer operator/(const integer &a, uint64_t b);
  integer operator/(const integer &a, int64_t b);
  integer operator/(const integer &a, int b);
  integer operator/(const integer &a, const integer &b);
  integer operator%(const integer &a, uint64_t b);
  integer operator%(const integer &a, int64_t b);
  integer operator%(const integer &a, int b);
  integer operator%(const integer &a, const integer &b);

  ostream &operator<<(ostream &os, const integer &a);
} // namespace cyy::math
