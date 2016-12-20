/*
 *	程序名：my_combinaiorics_test.cpp
 *	作者：陈源源
 *	日期：2016-04-04
 *	功能：测试组合学相关函数
 */
#include <cassert>
#include <iostream>
#include <my_math.h>
#include <string>

using namespace std;
using namespace my_math;

int main(int argc, char **argv) {
  assert(binomial_coefficient(100, 50) ==
         std::string("100891344545564193334812497256"));

  auto prev_combination = my_next_combination(5, 2);
  while (1) {
    if (prev_combination.size() == 0)
      break;

    for (auto it = prev_combination.begin(); it != prev_combination.end(); it++)
      cout << (*it == true ? '1' : '0');
    cout << endl;
    prev_combination = my_next_combination(5, 2, prev_combination);
  }
  return 0;
}
