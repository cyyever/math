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

using std::cout;
using std::endl;

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

/*
 *	功能：比较和另一个整数的大小
 * 	参数：
 *		rhs：另一个整数
 * 	返回值：
 * 		>0：大于另一个整数
 * 		0：两个整数相等
 * 		<0：小于另一个整数
 */
int my_int::cmp(const my_int &rhs) const
{
	int res;
	if(sign<rhs.sign)
		return -1;
	else if(sign>rhs.sign)
		return 1;
		
	if(my_digit_list.size()<rhs.my_digit_list.size())
		res=-1;
	else if(my_digit_list.size()>rhs.my_digit_list.size())
		res=1;
	else
	{
		res=0;
		auto it=--my_digit_list.end();
		auto it2=--rhs.my_digit_list.cend();
		for(;;it++,it2++)
		{
			if(*it<*it2)
			{
				res=-1;
				break;
			}
			else if(*it>*it2)
			{
				res=1;
				break;
			}
			if(it==my_digit_list.begin())
				break;
		}
	}
	if(sign==0)	//负数
		res=-res;
	return res;
}
bool operator ==(const my_int &a,const my_int &b)
{
	if(a.cmp(b)==0)
		return true;
	return false;
}

bool operator !=(const my_int &a,const my_int &b)
{
	return !(a==b);
}

bool operator <(const my_int &a,const my_int &b)
{
	if(a.cmp(b)<0)
		return true;
	return false;
}

bool operator <=(const my_int &a,const my_int &b)
{
	if(a.cmp(b)<=0)
		return true;
	return false;
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
	if(sign!=rhs.sign)	//符号不同，转换成减法
	{
		sign=1-sign;
		operator-=(rhs);
		sign=1-sign;
		return *this;
	}
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
	while(it2!=rhs.my_digit_list.cend())
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
	if(carry==1)
	{
		if(it!=my_digit_list.end())
		{
			do
			{
				*it+=1;
				if((*it)>=my_base)
					*it-=my_base;
				else
					break;
				it++;
			}
			while(it!=my_digit_list.end());
		}
		else
			my_digit_list.push_back(1);
	}
	return *this;
}

my_int& my_int::operator -=(const my_int &rhs)
{
	if(sign!=rhs.sign)	//符号不同，转换成加法
	{
		sign=1-sign;
		operator+=(rhs);
		sign=1-sign;
		return *this;
	}

	uint8_t carry=0;

	auto it=my_digit_list.begin();
	auto it2=rhs.my_digit_list.cbegin();
	for(;it!=my_digit_list.end()&&it2!=rhs.my_digit_list.cend();it++,it2++)
	{
		*it-=(*it2+carry);
		if((*it)<0)
		{
			*it+=my_base;
			carry=1;
		}
		else
			carry=0;
	}
	if(it2!=rhs.my_digit_list.cend())
	{
		do{
			int64_t my_digit=-(*it2+carry);

			if(my_digit<0)
			{
				my_digit+=my_base;
				carry=1;
			}
			else
				carry=0;
			my_digit_list.push_back(my_digit);
			it2++;
		}
		while(it2!=rhs.my_digit_list.cend());
	}
	else if(carry==1)
	{
		for(;it!=my_digit_list.end();it++)
		{
			*it-=1;
			if((*it)<0)
				*it+=my_base;
			else
			{
				carry=0;
				break;
			}
		}
	}

	if(carry==1)
	{
		my_digit_list.push_back(-1);
		//这样的话前面几个my_digit是负数，我们重新翻转正负号并补位
		carry=0;
		for(auto it=my_digit_list.begin();it!=my_digit_list.end();it++)
		{
			*it=-(*it+carry);
			if(*it<0)
			{
				*it+=my_base;
				carry=1;
			}
			else
				carry=0;
		}
		sign=1-sign;
	}

	//去除前面的0
	while(my_digit_list.back()==0 && my_digit_list.size()>1)
		my_digit_list.pop_back();

	//如果两个相等的负数相减，这边我们要调整符号为正
	if(my_digit_list.back()==0)
		sign=1;
	return *this;
}

my_int operator -(const my_int &a)
{
	my_int b=a;
	b.sign=1-b.sign;
	return b;
}

my_int operator -(const my_int &a,const my_int &b)
{
	my_int c=a;
	c-=b;
	return c;
}

my_int operator +(const my_int &a,const my_int &b)
{
	my_int c=a;
	c+=b;
	return c;
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
