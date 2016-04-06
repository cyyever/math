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

	/*
	for(auto i=-200;i<=200;i++)
	{
		for(auto j=-200;j<=200;j++)
		{
			assert((i<j)==(my_rat(i)<my_rat(j)));
			assert((i==j)==(my_rat(i)==my_rat(j)));
			assert((i>j)==(my_rat(i)>my_rat(j)));
			assert(i+j==(my_rat(i)+j));
			assert(i+j==(my_rat(i)+my_rat(j)));
			assert(i-j==(my_rat(i)-j));
			assert(i-j==(my_rat(i)-my_rat(j)));
			assert(i*j==(my_rat(i)*my_rat(j)));
			if(j==0)
				continue;
	//		assert(i/j==(my_rat(i)/j));
	//		assert(i/j==(my_rat(i)/my_rat(j)));
			assert(i%j==(my_rat(i)%j));
			assert(i%j==(my_rat(i)%my_rat(j)));
		}
	}
	assert(my_rat(INT64_MAX)+UINT64_MAX==string("27670116110564327422"));
	assert(my_rat(INT64_MAX)-UINT64_MAX==string("-9223372036854775808"));

	my_rat a;
	a=0;
	assert(++a==1);
	assert(--a==0);
	assert((a++)==0 && a==1);
	assert((a--)==1 && a==0);

	a=2;
	assert(a*UINT64_MAX==string("36893488147419103230"));
	assert(a*INT64_MIN==string("-18446744073709551616"));

	assert(my_rat("44277496045533614223360000")/my_rat("479001600")==my_rat("92437052497389600"));
	assert(my_rat("44277496045533614223360000")/479001600==my_rat("92437052497389600"));
	*/
	return 0;
}
