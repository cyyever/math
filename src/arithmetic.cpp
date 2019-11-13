/*!
 * \file arithmetic.cpp
 *
 * \brief ������صĺ���
 */

#include "arithmetic.hpp"
namespace cyy::math {

  /*
   *	���ܣ�����n�Ľ׳�
   * 	������
   *		n������
   * 	����ֵ��
   * 		�׳�
   */
  integer factorial(uint32_t n) {
    integer res = 1;
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
  /* my_rat log(const integer &n, uint64_t base, size_t digit_num) { */
  /*   integer p, q, min_power, max_power, tmp, tmp_power; */
  /*   my_rat n_power; */
  /*   if (n <= 0) */
  /*     throw std::invalid_argument("n is less than zero"); */
  /*   if (base <= 1) */
  /*     throw std::invalid_argument("base is less than or equal to one"); */
  /*   if (n == 1) */
  /*     return my_rat(0, 1); */
  /*   q = power(10, digit_num); */
  /*   n_power = my_rat(power(n, q), 1); */

  /*   //ͨ�����ַ��ҳ����� */
  /*   //���ǻ���10�Ļ�������һ���ֲڵ��Ͻ� */
  /*   max_power = n_power.numerator().digit_num(); */
  /*   if (base < 10) */
  /*     max_power *= 4; */

  /*   while (min_power <= max_power) { */
  /*     tmp = (max_power + min_power) / 2; */
  /*     tmp_power = power(base, tmp); */
  /*     auto compare_res = n_power.compare(tmp_power); */
  /*     if (compare_res >= 0) */
  /*       p += tmp; */
  /*     if (compare_res == 0) */
  /*       break; */
  /*     else if (compare_res > 0) { */
  /*       n_power /= tmp_power; */
  /*       max_power -= tmp; */
  /*       min_power = 1; */
  /*     } else */
  /*       max_power = tmp - 1; */
  /*   } */
  /*   return my_rat(p, q); */
  /* } */
} // namespace cyy::math
