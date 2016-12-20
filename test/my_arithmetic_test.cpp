/*
 *	程序名：my_arithmetic_test.cpp
 *	作者：陈源源
 *	日期：2016-04-12
 *	功能：测试算术相关函数
 */
#include <cassert>
#include <iostream>
#include <my_math.h>

using namespace std;
using namespace my_math;

int main(int argc, char **argv) {
  my_int a;

  assert(my_int(UINT64_MAX) == power(2, 64) - 1);
  assert(my_int(UINT64_MAX) == power(2, my_int(64)) - 1);

  a = 2;
  for (auto i = 2; i <= 50; i++, a *= i)
    assert(factorial(i) == a);

  assert(log(519432, 2, 4) == my_rat(189865, 10000));
  return 0;
}
