/*
 *	程序名：my_int_test.cpp
 *	作者：陈源源
 *	日期：2016-03-28
 *	功能：测试my_int类
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
	vector<string> invalid_interger_str={"aaa","0123"};
	vector<string> valid_interger_str={"1","-1","0","-0","123456789012345678901","-12345678901234567890",to_string(UINT64_MAX),to_string(INT64_MIN)};

	for(auto it=invalid_interger_str.begin();it!=invalid_interger_str.end();it++)
	{
		try
		{
			my_int a(*it);
		}
		catch(invalid_argument &e)
		{
			cout<<"create interger failed:"<<e.what()<<endl;
		}
	}

	for(auto it=valid_interger_str.begin();it!=valid_interger_str.end();it++)
	{
		my_int a(*it);
		assert(a==*it);
	}

	for(auto i=-200;i<=200;i++)
	{
		for(auto j=-200;j<=200;j++)
		{
			assert((i<j)==(my_int(i)<my_int(j)));
			assert((i==j)==(my_int(i)==my_int(j)));
			assert((i>j)==(my_int(i)>my_int(j)));
	//		cout<<"i "<<i<<"j "<<j<<"="<<(my_int(i)+j)<<endl;
			assert(i+j==(my_int(i)+j));

			assert(i+j==(my_int(i)+my_int(j)));
			assert(i-j==(my_int(i)-my_int(j)));
			assert(i*j==(my_int(i)*my_int(j)));
			if(j==0)
				continue;
			assert(i/j==(my_int(i)/j));
			assert(i/j==(my_int(i)/my_int(j)));
			assert(i%j==(my_int(i)%j));
			assert(i%j==(my_int(i)%my_int(j)));
		}
	}
	assert(my_int(INT64_MAX)+UINT64_MAX==string("27670116110564327422"));

	my_int a;
	a=0;
	assert(++a==1);
	assert(--a==0);
	assert((a++)==0 && a==1);
	assert((a--)==1 && a==0);

	a=2;
	assert(a*UINT64_MAX==string("36893488147419103230"));
	assert(a*INT64_MIN==string("-18446744073709551616"));

	return 0;
}
