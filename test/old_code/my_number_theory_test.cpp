/*
 *	程序名：my_number_theory_test.cpp
 *	作者：陈源源
 *	日期：2016-04-04
 *	功能：测试数论相关函数
 */
#include <cassert>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "../src/my_math.h"

using namespace std;
using namespace my_math;

int main(int argc, char **argv) {
  vector<uint64_t> primes = {2, 3, 5, 7, 23, 252097800623};
  vector<uint64_t> non_primes = {252097800625};

  for (auto it = primes.begin(); it != primes.end(); it++)
    assert(is_prime(*it) == true);
  for (auto it = non_primes.begin(); it != non_primes.end(); it++)
    assert(is_prime(*it) == false);

  primes = get_primes(100);
  for (auto it = primes.begin(); it != primes.end(); it++)
    cout << *it << endl;

  try {
    gcd(0, 0);
  } catch (invalid_argument &e) {
    cout << "gcd failed:" << e.what() << endl;
  }
  assert(gcd(-3, 7) == 1);
  assert(gcd(-14, 77) == 7);
  assert(gcd(-5, 5) == 5);
  return 0;
}
