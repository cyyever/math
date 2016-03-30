/*
 *	程序名：my_int_test.cpp
 *	作者：陈源源
 *	日期：2016-03-28
 *	功能：测试my_int类
 */
#include <iostream>
#include <stdexcept>
#include <vector>
#include <assert.h>
#include <my_int.h>

using namespace std;

int main(int argc, char** argv)
{
	vector<string> invalid_interger_str={"aaa","0123"};
	vector<string> valid_interger_str={"1","-1","0","-0","123456789012345678901","-12345678901234567890"};

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

	a=2;
	b=3;
	assert(a*b==6);
	return 0;
}
