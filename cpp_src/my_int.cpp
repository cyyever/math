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

using std::cout;
using std::endl;

namespace my_math
{

my_int::my_int():sign(1)
{
	my_digit_list.push_back(0);
	return;
}

my_int::my_int(const string &int_str):sign(1)
{
	decltype(int_str.size()) i,first_digit_index;
	if(!is_valid_int_str(int_str))
		throw std::invalid_argument(int_str);

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

my_int::my_int(uint64_t num):sign(1)
{
	if(num<my_base)
		my_digit_list.push_back(num);
	else
	{
		while(num)
		{
			my_digit_list.push_back(num%my_base);
			num/=my_base;
		}
	}
	return;
}

my_int::my_int(int64_t num)
{
	*this=my_int(abs(num));
	if(num<0)
		sign=0;
	return;
}

my_int::my_int(int num):my_int((int64_t)num)
{
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
int my_int::compare(const my_int &rhs) const
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

uint64_t inline abs(int64_t num)
{
	uint64_t tmp;
	if(num>=0)
		return (uint64_t)num;

	if(num==INT64_MIN)
	{
		tmp=INT64_MAX;
		tmp++;
	}
	else
		tmp=-num;
	return tmp;
}

bool operator ==(const my_int &a,const my_int &b)
{
	if(a.compare(b)==0)
		return true;
	return false;
}

bool operator !=(const my_int &a,const my_int &b)
{
	return !(a==b);
}

bool operator <(const my_int &a,const my_int &b)
{
	if(a.compare(b)<0)
		return true;
	return false;
}

bool operator <=(const my_int &a,const my_int &b)
{
	if(a.compare(b)<=0)
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
	if(this==&rhs)
	{
		my_int tmp_this=*this;
		return operator+=(tmp_this);
	}

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
	if(this==&rhs)
	{
		my_int tmp_this=*this;
		return operator-=(tmp_this);
	}

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

my_int& my_int::operator ++()
{
	operator+=(1);
	return *this;
}

my_int& my_int::operator --()
{
	operator-=(1);
	return *this;
}

my_int my_int::operator ++(int)
{
	my_int tmp=*this;
	operator+=(1);
	return tmp;
}

my_int my_int::operator --(int)
{
	my_int tmp=*this;
	operator-=(1);
	return tmp;
}

my_int operator -(const my_int &a)
{
	my_int b=a;
	if(b.my_digit_list.back()!=0)
		b.sign=1-b.sign;
	return b;
}

my_int operator -(const my_int &a,const my_int &b)
{
	return my_int(a)-=b;
}

my_int operator +(const my_int &a,const my_int &b)
{
	return my_int(a)+=b;
}

my_int& my_int::operator *=(const my_int &rhs)
{
	if(this==&rhs)
	{
		my_int tmp_this=*this;
		return operator*=(tmp_this);
	}

	if(this->is_zero())
		return *this;
	if(rhs.is_zero() || rhs.is_one())
	{
		*this=rhs;
		return *this;
	}

	decltype(rhs.my_digit_list.size()) cnt=0,i;
	my_int tmp_this=std::move(*this);

	my_digit_list={0};
	if(sign!=rhs.sign)	
		sign=0;
	else
		sign=1;
	for(auto it=rhs.my_digit_list.cbegin();it!=rhs.my_digit_list.cend();it++,cnt++)
	{
		my_int tmp=tmp_this*(*it);
		for(i=0;i<cnt;i++)
			tmp.my_digit_list.push_front(0);
		operator+=(tmp);
	}
	return *this;
}

my_int operator *(const my_int &a,int64_t b)
{
	my_int c=operator *(a,abs(b));

	if(b<0 && c!=0)
		c.sign=0;
	return c;
}

my_int operator *(const my_int &a,uint64_t b)
{
	my_int c;
	int64_t carry;
	unsigned __int128 tmp_my_digit,tmp_b;

	if(b==0 || a==0)
		return 0;
	c=a;
	if(b==1)
		return c;

	tmp_b=b;
	carry=0;
	for(auto it=c.my_digit_list.begin();it!=c.my_digit_list.end();it++)
	{
		tmp_my_digit=(*it)*tmp_b+carry;
		if(tmp_my_digit>=my_int::my_base)
		{
			*it=tmp_my_digit%my_int::my_base;
			carry=tmp_my_digit/my_int::my_base;
		}
		else
		{
			*it=tmp_my_digit;
			carry=0;
		}
	}

	while(carry)
	{
		if(carry>=my_int::my_base)
		{
			c.my_digit_list.push_back(carry%my_int::my_base);
			carry/=my_int::my_base;
		}
		else
		{
			c.my_digit_list.push_back(carry);
			carry=0;
		}
	}
	return c;
}

my_int operator *(const my_int &a,const my_int &b)
{
	return my_int(a)*=b;
}


my_int& my_int::operator /=(uint64_t num)
{
	unsigned __int128 tmp;
	if(num==0)
		throw std::invalid_argument("divided by zero");

	if(this->is_zero() || num==1)
		return *this;

	int64_t carry=0;
	for(auto it=--my_digit_list.end();;it--)
	{
		tmp=*it;
		if(carry!=0)
			tmp+=carry*my_int::my_base;
		if(tmp>=num)
		{
			*it=tmp/num;
			carry=tmp%num;
		}
		else
		{
			*it=0;
			carry=tmp;
		}
		if(it==my_digit_list.begin())
			break;
	}

	//去除前面的0
	while(my_digit_list.back()==0 && my_digit_list.size()>1)
		my_digit_list.pop_back();
	return *this;
}

my_int& my_int::operator /=(int64_t num)
{
	operator/=(abs(num));
	if(num<0 && !this->is_zero())
		sign=1-sign;
	return *this;
}

my_int& my_int::operator /=(int num)
{
	return operator/=((int64_t)num);
}

my_int operator /(const my_int &a,uint64_t b)
{
	return my_int(a)/=b;
}

my_int operator /(const my_int &a,int64_t b)
{
	return my_int(a)/=b;
}

my_int operator /(const my_int &a,int b)
{
	return my_int(a)/=b;
}

ostream &operator <<(ostream &os,const my_int &a)
{
	os<<(string)a;
	return os;
}

inline bool my_int::is_zero() const
{
	return my_digit_list.back()==0 && sign==1 && my_digit_list.size()==1;
}

inline bool my_int::is_one() const
{
	return my_digit_list.back()==1 && sign==1 && my_digit_list.size()==1;
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

}
