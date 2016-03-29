/*
 *	程序名：my_int_test.cpp
 *	作者：陈源源
 *	日期：2016-03-28
 *	功能：测试my_int类
 */
#include <iostream>
#include <stdexcept>
#include <vector>
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
		if(a!=*it)
			throw std::logic_error((*it)+" != "+a);
	}
	
	my_int a=10,b=10;

	if(a!=b)
		throw std::logic_error("10 != 10");
	b=20;
	if(a==b)
		throw std::logic_error("10 == 20");
	b=-10;
	if(a<b)
		throw std::logic_error("10 < -20");
	a=-11;
	if(a>=b)
		throw std::logic_error("-11 >= -10");
	a=111;
	b=99;
	a+=b;
	if(a!=210)
		throw std::logic_error("111+99 != 210");
	b=-99;
	a=-111;
	a+=b;
	if(a!=-210)
		throw std::logic_error("-111+(-99) != -210");

	a=99;
	b=99;
	a+=b;
	if(a!=198)
		throw std::logic_error("99+99 != 198");

	a=11;
	b=10000;
	a-=b;
	if(a!=-9989)
		throw std::logic_error("11-10000 != -9989");

	a=111111;
	b=1;
	a-=b;
	if(a!=111110)
		throw std::logic_error("111111-1 != 111110");

	a=111111;
	b=999999;
	a-=b;
	if(a!=-888888)
		throw std::logic_error("111111-999999 != -888888");
	a=-111;
	b=-111;
	a-=b;
	if(a!=0)
		throw std::logic_error("-111-(-111) != 0");

	a=-111;
	b=111;
	a-=b;
	if(a!=-222)
		throw std::logic_error("-111-111 != -222");

	a=1;
	b=-2;
	a+=b;
	if(a!=-1)
		throw std::logic_error("1+(-2) != -1");
	a=100;
	if(-a!=-100)
		throw std::logic_error("-100 != -100");
	a=2;
	b=1;
	if(a-b!=1)
		throw std::logic_error("2-1 != 1");

	a=1;
	b=1;
	if(a+b!=2)
		throw std::logic_error("1+1 != 2");

	a=0;
	if(++a!=1)
		throw std::logic_error("prefix ++ failed");

	if(--a!=0)
		throw std::logic_error("prefix -- failed");

	if(a++!=0 || a!=1)
		throw std::logic_error("postfix ++ failed");

	if(a--!=1 || a!=0)
		throw std::logic_error("postfix -- failed");
	return 0;
}
