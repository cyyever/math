/*
 *	程序名：my_int.cpp
 *	作者：陈源源
 *	日期：2016-03-28
 *	功能：整数相关类和函数
 */

#include <cstdlib>
#include <regex>
#include <iostream>

#include "my_int.h"
#include "my_log.h"

my_int::my_int():power(0),sign(1)
{
	my_digit_list.push_back(0);
	return;
}

my_int::my_int(const string &int_str):power(0),sign(1)
{
	if(!is_valid_int_str(int_str))
	{
		my_log("invalid interger");
		throw std::invalid_argument(int_str);
	}

	auto i=int_str.size()%my_digit_num;
	if(int_str[0]=='-')
	{
		sign=0;
		if(i==0)
			i=my_digit_num-1;
		else
			i--;
	}
	if(i!=0)
	{
		if(int_str[0]=='-')
			my_digit_list.push_back(stoull(int_str.substr(1,i)));
		else
			my_digit_list.push_back(stoull(int_str.substr(0,i)));
	}
	if(int_str[0]=='-')
		i++;

	while(i<int_str.size())
	{
		my_digit_list.push_back(stoull(int_str.substr(i,my_digit_num)));
		i+=my_digit_num;
	}
	return;
}

my_int::my_int(uint64_t num):power(0),sign(1)
{
	my_digit_list=get_my_digit_list(num);
	return;
}

my_int::operator string() const
{
	string int_str;
	if(sign==0)
		int_str.push_back('-');

	for(auto it=my_digit_list.begin();it!=my_digit_list.end();it++)
		int_str.append(std::to_string(*it));
	return int_str;
}

my_int& my_int::operator +=(uint64_t num)
{
	list<uint64_t> digit_list=get_my_digit_list(num);
	uint64_t carry;

	if(num==0)
		return *this;

	if(my_digit_list.size()<digit_list.size())
		my_digit_list.push_front(0);
	auto it=--my_digit_list.end();
	auto it2=--digit_list.end();
			
	while(1)
	{
		(*it)+=(*it2);
		if(it2==digit_list.begin())
			break;
		it--;
		it2--;
	}
		
	carry=0;
	it=--my_digit_list.end();

	for(;;it--)	
	{
		*it+=carry;
		if(*it>my_base)
		{
			*it-=my_base;
			carry=1;
		}
		else
			break;

		if(it==my_digit_list.begin())
		{
			my_digit_list.push_front(1);
			break;
		}
	}
	return *this;
}

ostream &operator <<(ostream &os,const my_int &a)
{
	os<<(string)a;
	return os;
}

/*
 *	功能：检查传入的字符串是否整数，即是否匹配 ^[+-]?[1-9][0-9]*$
 * 	参数：
 *		str：要检查的字符串
 * 	返回值：
 *		true：是
 *		false：不是
 */
bool my_int::is_valid_int_str(const string &str)
{
	std::regex int_regex("^[+-]?(0|[1-9][0-9]*)$");
	std::smatch m;
	if(std::regex_search(str,m,int_regex))
		return true;
	else
		return false;
}

/*
 *	功能：把传入的uint64_t数字转成我们需要的digit链表
 * 	参数：
 *		num：要处理的数字
 * 	返回值：
 * 		digit链表
 */
list<uint64_t> inline my_int::get_my_digit_list(uint64_t num)
{
	if(num<my_base)
		return {num%my_base};
	else
		return {num/my_base,num%my_base};
}
