/*
 *	程序名：my_rat_test.cpp
 *	作者：陈源源
 *	日期：2016-04-06
 *	功能：测试my_rat类
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
	vector<string> invalid_rational_str={"p/q","abc","1/0"};
	vector<string> valid_rational_str={to_string(INT64_MAX)+"/"+to_string(INT64_MAX),to_string(INT64_MIN)+"/"+to_string(INT64_MAX)};

	for(auto it=invalid_rational_str.begin();it!=invalid_rational_str.end();it++)
	{
		try
		{
			my_rat a(*it);
		}
		catch(invalid_argument &e)
		{
			cout<<"create rational failed:"<<e.what()<<endl;
		}
	}

	for(auto it=valid_rational_str.begin();it!=valid_rational_str.end();it++)
	{
		my_rat a(*it);
		assert(a==*it);
	}

	return 0;
}
