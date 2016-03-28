/*
 *	程序名：my_int.cpp
 *	作者：陈源源
 *	日期：2016-03-28
 *	功能：整数相关类和函数
 */

#include <cstdlib>
#include <regex>
#include <iostream>
#include <algorithm>

#include "my_int.h"
#include "my_log.h"

my_int::my_int():power(0),sign(1)
{
	my_digit_list.push_back(0);
	return;
}

my_int::my_int(const string &int_str):power(0),sign(1)
{
	decltype(int_str.size()) i,first_digit_index;
	if(!is_valid_int_str(int_str))
	{
		my_log("invalid interger");
		throw std::invalid_argument(int_str);
	}

	i=int_str.size();
	if(int_str[0]=='-')
	{
		sign=0;
		first_digit_index=1;
	}
	else
		first_digit_index=0;

	i=int_str.size();
	while(i-first_digit_index>=my_digit_num)
	{
		my_digit_list.push_back(stoll(int_str.substr(i-my_digit_num,my_digit_num)));
		i-=my_digit_num;
	}
	if(i-first_digit_index>0)
		my_digit_list.push_back(stoll(int_str.substr(first_digit_index,i-first_digit_index)));
	return;
}

my_int::my_int(int64_t num):power(0),sign(num<0?0:1)
{
	uint64_t tmp;

	if(num<0)
	{
		if(num==INT64_MIN)
		{
			tmp=INT64_MAX;
			tmp++;
		}
		else
			tmp=-num;
	}
	else
		tmp=num;

	if(tmp==0)
		my_digit_list.push_back(0);
	else
	{
		while(tmp)
		{
			my_digit_list.push_back(tmp%my_base);
			tmp/=my_base;
		}
	}
	return;
}

my_int::operator string() const
{
	string int_str;
	if(sign==0)
		int_str.push_back('-');

	for(auto it=--my_digit_list.end();;it--)
	{
		int_str.append(std::to_string(*it));
		if(it==my_digit_list.begin())
			break;
	}
	return int_str;
}

bool operator ==(const my_int &a,const my_int &b)
{
	if(a.sign!=b.sign)
		return false;
	if(a.my_digit_list.size()!=b.my_digit_list.size())
		return false;
	if(!std::equal(a.my_digit_list.begin(),a.my_digit_list.end(),b.my_digit_list.begin()))
		return false;
	return true;
}

bool operator !=(const my_int &a,const my_int &b)
{
	return !(a==b);
}

bool operator <(const my_int &a,const my_int &b)
{
	bool res;
	if(a.sign<b.sign)
		return true;
	else if(a.sign>b.sign)
		return false;
		
	if(a.my_digit_list.size()<b.my_digit_list.size())
		res=true;
	else if(a.my_digit_list.size()>b.my_digit_list.size())
		res=false;
	else
	{
		res=false;
		for(auto it=--a.my_digit_list.end(),it2=--b.my_digit_list.end();;it++,it2++)
		{
			if(*it<*it2)
			{
				res=true;
				break;
			}
			if(it==a.my_digit_list.begin())
				break;
		}
	}
	if(a.sign==0)	//负数
		res=!res;
	return res;
}

bool operator <=(const my_int &a,const my_int &b)
{
	return (a<b)||(a==b);
}

bool operator >(const my_int &a,const my_int &b)
{
	return !(a<=b);
}

bool operator >=(const my_int &a,const my_int &b)
{
	return !(a<b);
}

my_int& my_int::operator +=(const my_int &rhs)
{
	if(sign==rhs.sign)
	{
		uint8_t carry=0;

		auto it=my_digit_list.begin();
		auto it2=rhs.my_digit_list.cbegin();
		for(;it!=my_digit_list.end()&&it2!=rhs.my_digit_list.cend();it++,it2++)
		{
			*it+=*it2+carry;
			if((*it)>=my_base)
			{
				carry=1;
				*it-=my_base;
			}
			else
				carry=0;
		}
		if(it!=my_digit_list.end() && carry==1)
		{
			do
			{
				*it+=carry;
				if((*it)>=my_base)
				{
					carry=1;
					*it-=my_base;
				}
				else
				{
					carry=0;
					break;
				}
				it++;
			}
			while(it!=my_digit_list.end());
		}
		else if(it2!=rhs.my_digit_list.cend())
		{
			do
			{
				int64_t my_digit=*it2+carry;

				if(my_digit>=my_base)
				{
					carry=1;
					my_digit-=my_base;
				}
				else
					carry=0;
				my_digit_list.push_back(my_digit);
				it2++;
			}
			while(it2!=rhs.my_digit_list.cend());
		}
		if(carry==1)
			my_digit_list.push_back(1);
	}
	else 
		throw std::invalid_argument("no the same sign");




	/*

	//	if(my_digit_list.size()<digit_list.size())
	//		my_digit_list.push_front(0);
	auto it=--my_digit_list.end();
	if(
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
	*/
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
