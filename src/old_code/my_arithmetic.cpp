/*
 *	��������my_arithmetic.cpp
 *	���ߣ���ԴԴ
 *	���ڣ�2016-04-12
 *	���ܣ�������صĺ���
 */

#include "my_arithmetic.h"
#include <iostream>
#include <stdexcept>

namespace my_math {

uint64_t abs(int64_t num) {
  uint64_t tmp;
  if (num >= 0)
    return (uint64_t)num;

  if (num == INT64_MIN) {
    tmp = INT64_MAX;
    tmp++;
  } else
    tmp = -num;
  return tmp;
}

/*
 *	���ܣ�����base��power�εĳ˷�
 * 	������
 *		base������
 *		power��ָ��
 * 	����ֵ��
 * 		�˷�
 */
my_int power(const my_int &base, uint64_t power) {
  my_int res = 1;
  my_int tmp_base = base;

  while (power) {
    if (power & 1)
      res *= tmp_base;
    power >>= 1;
    tmp_base *= tmp_base;
  }
  return res;
}

/*
 *	���ܣ�����base��power�εĳ˷�
 * 	������
 *		base������
 *		power��ָ��
 * 	����ֵ��
 * 		�˷�
 */
my_int power(const my_int &base, const my_int &power) {
  if (power < 0)
    throw std::invalid_argument(string("power is less than zero"));
  my_int res = 1;
  my_int tmp_base = base;
  my_int tmp_power = power;

  while (tmp_power != 0) {
    if (tmp_power.is_odd())
      res *= tmp_base;
    tmp_power /= 2;
    tmp_base *= tmp_base;
  }
  return res;
}

/*
 *	���ܣ�����n�Ľ׳�
 * 	������
 *		n������
 * 	����ֵ��
 * 		�׳�
 */
my_int factorial(uint64_t n) {
  my_int res = 1;
  while (n >= 2) {
    res *= n;
    n--;
  }
  return res;
}

/*
 *	���ܣ��������base�Ķ���
 * 	������
 *		n������
 *		base������
 *		digit_num��Ҫ�����С������λ��������ĸ��ض�
 * 	����ֵ��
 * 		����
 */
my_rat log(const my_int &n, uint64_t base, size_t digit_num) {
  my_int p, q, min_power, max_power, tmp, tmp_power;
  my_rat n_power;
  if (n <= 0)
    throw std::invalid_argument("n is less than zero");
  if (base <= 1)
    throw std::invalid_argument("base is less than or equal to one");
  if (n == 1)
    return my_rat(0, 1);
  q = power(10, digit_num);
  n_power = my_rat(power(n, q), 1);

  //ͨ�����ַ��ҳ�����
  //���ǻ���10�Ļ�������һ���ֲڵ��Ͻ�
  max_power = n_power.numerator().digit_num();
  if (base < 10)
    max_power *= 4;

  while (min_power <= max_power) {
    tmp = (max_power + min_power) / 2;
    tmp_power = power(base, tmp);
    auto compare_res = n_power.compare(tmp_power);
    if (compare_res >= 0)
      p += tmp;
    if (compare_res == 0)
      break;
    else if (compare_res > 0) {
      n_power /= tmp_power;
      max_power -= tmp;
      min_power = 1;
    } else
      max_power = tmp - 1;
  }
  return my_rat(p, q);
}
}
