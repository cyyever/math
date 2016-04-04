/*
 *	程序名：my_number_theory_test.cpp
 *	作者：陈源源
 *	日期：2016-04-04
 *	功能：测试数论相关函数
 */
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <cassert>
#include <my_math.h>

using namespace std;
using namespace my_math;

int main(int argc, char** argv)
{
	vector<uint64_t> primes={2,3,5,7,23,252097800623};
	vector<uint64_t> non_primes={252097800625};

	for(auto it=primes.begin();it!=primes.end();it++)
		assert(is_prime(*it)==true);
	for(auto it=non_primes.begin();it!=non_primes.end();it++)
		assert(is_prime(*it)==false);

	return 0;
}
