/*
 *	程序名：my_number_theory.cpp
 *	作者：陈源源
 *	日期：2016-04-04
 *	功能：包含数论相关函数
 */

#include "my_number_theory.h"

#include <iostream>
#include <stdexcept>

const uint64_t cached_prime_upper_limit = 10000000;
static std::vector<uint64_t> cached_primes(cached_prime_upper_limit + 1, 0);

namespace my_math {
/*
 *	功能：缓存质数
 *	参数：
 *		无
 *	返回值：
 *		无
 */
static void cache_primes(void) {
  uint64_t i, j;

  if (cached_primes.back() == 0) {
    //标识质数
    for (i = 2; i <= cached_prime_upper_limit; i++)
      cached_primes[i] = 1;
    for (i = 2; i <= cached_prime_upper_limit; i++) {
      if (cached_primes[i])
        for (j = i * 2; j <= cached_prime_upper_limit; j += i)
          cached_primes[j] = 0;
    }
    //重新定位
    for (i = 2, j = 0; i <= cached_prime_upper_limit; i++) {
      if (cached_primes[i])
        cached_primes[j++] = i;
    }
    cached_primes.resize(j);
  }
  return;
}

/*
 *	功能：判定质数
 *	参数：
 *		num：要判定的数
 *	返回值：
 *		true：是质数
 *		false：不是质数
 */
bool is_prime(uint64_t num) {
  decltype(cached_primes.size()) i, j, k;
  uint64_t square, prime;

  cache_primes();

  if (num == UINT64_MAX)
    return false;
  if (num <= 1)
    return false;
  if ((num & 1) == 0 && num != 2)
    return false;
  if (num <= cached_primes.back()) {
    i = 0;
    j = cached_primes.size() - 1;

    while (i <= j) {
      k = (i + j) / 2;

      if (num == cached_primes[k])
        return true;
      else if (num < cached_primes[k])
        j = k - 1;
      else
        i = k + 1;
    }
    return false;
  }

  for (i = 1; i < 100; i++) {
    if ((num % cached_primes[i]) == 0)
      return false;
  }

  i = 1;
  if (num > UINT32_MAX)
    j = (uint64_t)(UINT32_MAX) + 1;
  else
    j = num;
  while (i <= j) {
    k = (i + j) / 2;
    square = k * k;
    if (num == square)
      return false;
    else if (num < square)
      j = k - 1;
    else
      i = k + 1;
  }
  for (i = 100; i < cached_primes.size(); i++) {
    if (cached_primes[i] > j)
      return true;
    if ((num % cached_primes[i]) == 0)
      return false;
  }

  for (prime = cached_prime_upper_limit; prime <= j; prime += 2) {
    if ((num % prime) == 0)
      return false;
  }
  return true;
}

/*
 *	功能：生成质数
 *	参数：
 *		upper_limit：质数上限
 *	返回值：
 *		质数vector
 */
std::vector<uint64_t> get_primes(uint64_t upper_limit) {
  uint64_t prime;

  cache_primes();

  std::vector<uint64_t> res;

  auto it = cached_primes.begin();
  for (; it != cached_primes.end(); it++) {
    if (*it > upper_limit)
      break;
    res.push_back(*it);
  }

  if (it == cached_primes.end()) {
    for (prime = cached_prime_upper_limit; prime <= upper_limit; prime += 2) {
      if (is_prime(prime))
        res.push_back(prime);
    }
  }
  return res;
}

/*
 *	功能：计算两个整数的最大公约数
 *	参数：
 *		a，b：整数
 *	返回值：
 *		最大公约数
 */
my_int gcd(const my_int &a, const my_int &b) {
  my_int p, q, tmp, m;
  int cmp_res;

  if (a == 0 && b == 0)
    throw std::invalid_argument("a and b are zero");
  p = (a >= 0) ? a : -a;
  q = (b >= 0) ? b : -b;

  if (p <= 1)
    return q;
  if (q <= 1)
    return p;

  //用欧几里得算法找到最大公约数
  cmp_res = p.compare(q);
  if (cmp_res < 0) {
    tmp = std::move(p);
    p = std::move(q);
    q = std::move(tmp);
  }

  while (1) {
    m = p % q;
    if (m == 0)
      break;
    p = std::move(q);
    q = m;
  }
  return q;
}
}
