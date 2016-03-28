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
		cout<<a<<endl;
	}
	 
	my_int a=10;
	a+=10;
	cout<<a<<endl;
	return 0;
}
