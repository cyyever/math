/*
 *	程序名：my_int.cpp
 *	作者：陈源源
 *	日期：2016-03-28
 *	功能：整数相关类和函数
 */

#include <cstdlib>
#include <regex>

#include "my_int.h"
#include "my_log.h"

my_int::my_int():power(0),sign(1)
{
	my_digit_list.push_back(0);
	return;
}

my_int::my_int(const string &int_str):power(0),sign(1)
{
	string tmp_str;
	if(!is_valid_int_str(int_str))
	{
		my_log("invalid interger");
		throw std::invalid_argument(int_str);
	}

	if(int_str[0]=='-')
	{
		sign=0;
		tmp_str=int_str.substr(1);
	}
	else
		tmp_str=int_str;
	auto i=tmp_str.size();
	while(i>=4)
	{
		my_digit_list.push_back(stoi(tmp_str.substr(i-4,4)));
		i-=4;
	}
	if(i>0)
		my_digit_list.push_back(stoi(tmp_str.substr(0,i)));
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

/*
 *	功能：重载ostream的<<给my_int
 * 	参数：
 *		os：输出流
 *		a：整数
 * 	返回值：
 * 		输出流
 */
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
