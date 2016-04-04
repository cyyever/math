/*
 *	程序名：my_combinaiorics_test.cpp
 *	作者：陈源源
 *	日期：2016-04-04
 *	功能：测试组合学相关函数
 */
#include <cassert>
#include <iostream>
#include <string>
#include <my_math.h>

using namespace std;
using namespace my_math;

int main(int argc, char** argv)
{
	assert(binomial_coefficient(100,50)==std::string("100891344545564193334812497256"));
	return 0;
}
