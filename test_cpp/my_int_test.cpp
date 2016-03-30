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

	my_int a=10,b=10;

	assert(a==b);
	b=20;
	assert(a!=b);
	b=-10;
	assert(a>=b);
	a=-11;
	assert(a<b);
	a=111;
	b=99;
	a+=b;
	assert(a==210);
	b=-99;
	a=-111;
	a+=b;
	assert(a==-210);
	a=99;
	b=99;
	a+=b;
	assert(a==198);
	a+=a;
	assert(a==396);
	a=11;
	b=10000;
	a-=b;
	assert(a==-9989);

	a=111111;
	b=1;
	a-=b;
	assert(a==111110);
	a=111111;
	b=999999;
	a-=b;
	assert(a==-888888);
	a=-111;
	b=-111;
	a-=b;
	assert(a==0);

	a=-111;
	b=111;
	a-=b;
	assert(a==-222);
	a-=a;
	assert(a==0);
	a=1;
	b=-2;
	a+=b;
	assert(a==-1);
	a=100;
	assert(-a==-100);
	a=0;
	assert(-a==0);

	a=2;
	b=1;
	assert(a-b==1);

	a=1;
	b=1;
	assert(a+b==2);

	a=0;
	assert(++a==1);
	assert(--a==0);
	assert((a++)==0 && a==1);
	assert((a--)==1 && a==0);

	a=99;
	a*=a;
	assert(a==9801);

	a=-1;
	b=0;
	a*=b;
	assert(a==0);

	a=0;
	b=-1;
	a*=b;
	assert(a==0);

	a=2;
	b=3;
	assert(a*b==6);
	
	a=2;
	assert(a*UINT64_MAX==string("36893488147419103230"));
	assert(a*INT64_MIN==string("-18446744073709551616"));

	a=99;
	a/=9;
	assert(a==11);
	a=77;
	a/=9;
	assert(a==8);
	a=0;
	a/=-1;
	assert(a==0);

	a=-77;
	a/=9;
	assert(a==(-77)/9);

	a=77;
	a/=-9;
	assert(a==77/(-9));
	return 0;
}
