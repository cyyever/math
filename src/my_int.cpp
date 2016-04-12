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
	uint8_t flag;
	char tmp[my_digit_num+1];
	if(sign==0)
		int_str.push_back('-');

	flag=1;
	for(auto it=--my_digit_list.end();;it--)
	{
		if(flag)
		{
			sprintf(tmp,"%" PRId64,*it);
			flag=0;
		}
		else
			sprintf(tmp,"%0*" PRId64,(int)my_digit_num,*it);
		int_str.append(tmp);
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
		for(;;it--,it2--)
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

/*
 *	功能：获取整数的位数
 * 	参数：
 * 		无
 * 	返回值：
 * 		位数
 */
uint64_t my_int::digit_num() const
{
	return my_digit_num*(my_digit_list.size()-1)+std::to_string(my_digit_list.back()).size();
}
uint64_t abs(int64_t num)
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

my_int operator -(const my_int &a)
{
	my_int b=a;
	if(!b.is_zero())
		b.sign=1-b.sign;
	return b;
}

my_int& my_int::operator +=(int64_t rhs)
{
	if(rhs>=0)
		return operator+=((uint64_t)rhs);
	else
		return operator-=(abs(rhs));
}

my_int& my_int::operator +=(int rhs)
{
	return operator+=((int64_t)rhs);
}

my_int& my_int::operator +=(uint64_t rhs)
{
	if(sign==0)	//符号不同，转换成减法
	{
		sign=1-sign;
		operator-=(rhs);
		if(!is_zero())
			sign=1-sign;
		return *this;
	}
	unsigned __int128 tmp,carry;
	auto it=my_digit_list.begin();

	tmp=(unsigned __int128)(*it)+rhs;

	if(tmp>=my_base)
	{
		carry=1;
		tmp-=my_base;
		if(tmp>=my_base)
		{
			carry+=(tmp/my_base);
			*it=tmp%my_base;
		}
		else
			*it=tmp;

		it++;

		while(it!=my_digit_list.end())
		{
			tmp=*it+carry;
			if(tmp>=my_base)
			{
				carry=1;
				tmp-=my_base;
				if(tmp>=my_base)
				{
					carry+=(tmp/my_base);
					*it=tmp%my_base;
				}
				else
					*it=tmp;
			}
			else
			{
				*it=tmp;
				carry=0;
				break;
			}
			it++;
		}

		if(carry>0)
		{
			if(carry<my_base)
				my_digit_list.push_back(carry);
			else
			{
				while(carry)
				{
					my_digit_list.push_back(carry%my_base);
					carry/=my_base;
				}
			}
		}
	}
	else
		*it=tmp;
	return *this;
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
		if(!is_zero())
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
	if(it2!=rhs.my_digit_list.cend())
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
	else if(carry==1 && it!=my_digit_list.end())
	{
		do
		{
			*it+=1;
			if((*it)>=my_base)
				*it-=my_base;
			else
			{
				carry=0;
				break;
			}
			it++;
		}
		while(it!=my_digit_list.end());
	}

	if(carry==1)
		my_digit_list.push_back(1);
	return *this;
}

my_int operator +(const my_int &a,uint64_t b)
{
	return my_int(a)+=b;
}

my_int operator +(const my_int &a,int64_t b)
{
	return my_int(a)+=b;
}

my_int operator +(const my_int &a,int b)
{
	return my_int(a)+=b;
}

my_int operator +(const my_int &a,const my_int &b)
{
	return my_int(a)+=b;
}

my_int& my_int::operator -=(uint64_t rhs)
{
	if(sign==0)	//符号不同，转换成加法
	{
		sign=1-sign;
		operator+=(rhs);
		if(!is_zero())
			sign=1-sign;
		return *this;
	}

	__int128 tmp,carry;
	auto it=my_digit_list.begin();

	tmp=(__int128)(*it)-rhs;

	if(tmp<0)
	{
		carry=1;
		tmp+=my_base;
		if(tmp<0)
		{
			carry+=((-tmp)/my_base);
			tmp+=(carry-1)*my_base;
			if(tmp<0)
			{
				carry++;
				tmp+=my_base;
			}
		}
		*it=tmp;

		it++;

		while(it!=my_digit_list.end())
		{
			tmp=*it-carry;
			if(tmp<0)
			{
				carry=1;
				tmp+=my_base;
				if(tmp<0)
				{
					carry+=((-tmp)/my_base);
					tmp+=(carry-1)*my_base;
					if(tmp<0)
					{
						carry++;
						tmp+=my_base;
					}
				}
				*it=tmp;
			}
			else
			{
				*it=tmp;
				carry=0;
				break;
			}
			it++;
		}

		if(carry>0)
		{
			//这样的话前面几个my_digit是负数，我们重新翻转正负号并补位
			for(auto it=my_digit_list.begin();it!=my_digit_list.end();it++)
				*it=-(*it);

			if(carry<my_base)
				my_digit_list.push_back(carry);
			else
			{
				while(carry)
				{
					my_digit_list.push_back(carry%my_base);
					carry/=my_base;
				}
			}

			carry=0;
			for(auto it=my_digit_list.begin();it!=my_digit_list.end();it++)
			{
				*it-=carry;
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
	}
	else
		*it=tmp;

	//去除前面的0
	while(my_digit_list.back()==0 && my_digit_list.size()>1)
		my_digit_list.pop_back();

	//如果两个相等的负数相减，这边我们要调整符号为正
	if(is_zero())
		sign=1;
	return *this;
}

my_int& my_int::operator -=(int64_t rhs)
{
	if(rhs>=0)
		return operator-=((uint64_t)rhs);
	else
		return operator+=(abs(rhs));
}

my_int& my_int::operator -=(int rhs)
{
	return operator-=((int64_t)rhs);
}

my_int& my_int::operator -=(const my_int &rhs)
{
	if(this==&rhs)
	{
		*this=0;
		return *this;
	}

	if(sign!=rhs.sign)	//符号不同，转换成加法
	{
		sign=1-sign;
		operator+=(rhs);
		if(!is_zero())
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
	if(is_zero())
		sign=1;
	return *this;
}

my_int operator -(const my_int &a,uint64_t b)
{
	return my_int(a)-=b;
}

my_int operator -(const my_int &a,int64_t b)
{
	return my_int(a)-=b;
}

my_int operator -(const my_int &a,int b)
{
	return my_int(a)-=b;
}

my_int operator -(const my_int &a,const my_int &b)
{
	return my_int(a)-=b;
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

my_int& my_int::operator *=(uint64_t rhs)
{
	int64_t carry;
	unsigned __int128 tmp_my_digit;

	if(this->is_zero() || rhs==1)
		return *this;

	if(rhs==0)
	{
		*this=0;
		return *this;
	}

	carry=0;
	for(auto it=my_digit_list.begin();it!=my_digit_list.end();it++)
	{
		tmp_my_digit=(unsigned __int128)(*it)*rhs+carry;
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
			my_digit_list.push_back(carry%my_int::my_base);
			carry/=my_int::my_base;
		}
		else
		{
			my_digit_list.push_back(carry);
			carry=0;
		}
	}
	return *this;
}

my_int& my_int::operator *=(int64_t rhs)
{
	if(rhs==-1)	//这个常用，为了提高性能，特殊判定
	{
		if(!this->is_zero())
			sign=1-sign;
	}
	else
	{
		operator*=(abs(rhs));
		if(rhs<0 && !this->is_zero())
			sign=1-sign;
	}
	return *this;
}

my_int& my_int::operator *=(int rhs)
{
	return operator*=((int64_t)rhs);
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
	if(rhs.is_abs_one())
	{
		if(rhs.sign==0)
			sign=1-sign;
		return *this;
	}
	if(rhs.is_zero())
	{
		*this=rhs;
		return *this;
	}
	decltype(rhs.my_digit_list.size()) cnt=0,i;
	uint8_t new_sign=!(sign^rhs.sign);
	my_int tmp_this=std::move(*this);

	*this=0;
	for(auto it=rhs.my_digit_list.cbegin();it!=rhs.my_digit_list.cend();it++,cnt++)
	{
		my_int tmp=tmp_this*(*it);
		for(i=0;i<cnt;i++)
			tmp.my_digit_list.push_front(0);
		operator+=(tmp);
	}
	sign=new_sign;
	return *this;
}

my_int operator *(const my_int &a,uint64_t b)
{
	return my_int(a)*=b;
}

my_int operator *(const my_int &a,int64_t b)
{
	return my_int(a)*=b;
}

my_int operator *(const my_int &a,int b)
{
	return my_int(a)*=b;
}

my_int operator *(const my_int &a,const my_int &b)
{
	return my_int(a)*=b;
}


my_int& my_int::operator /=(uint64_t rhs)
{
	unsigned __int128 tmp,carry;
	if(rhs==0)
		throw std::invalid_argument("divided by zero");

	if(this->is_zero() || rhs==1)
		return *this;

	carry=0;
	for(auto it=--my_digit_list.end();;it--)
	{
		tmp=*it;
		if(carry)
			tmp+=carry*my_base;

		if(tmp>=rhs)
		{
			*it=tmp/rhs;
			carry=tmp%rhs;
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

	if(is_zero())
		sign=1;
	return *this;
}

my_int& my_int::operator /=(int64_t rhs)
{
	operator/=(abs(rhs));
	if(rhs<0 && !this->is_zero())
		sign=1-sign;
	return *this;
}

my_int& my_int::operator /=(int rhs)
{
	return operator/=((int64_t)rhs);
}

my_int& my_int::operator /=(const my_int &rhs)
{
	my_int quotient,tmp,low_bound,high_bound;
	uint8_t org_sign=sign;
	int compare_res=0;
	if(rhs.is_zero())
		throw std::invalid_argument("divided by zero");

	if(this==&rhs)
	{
		*this=1;
		return *this;
	}

	if(this->is_zero())
		return *this;
	if(rhs.is_abs_one())
	{
		if(rhs.sign==0)
			sign=1-sign;
		return *this;
	}


	//通过二分法找出来，注意这边我们转成正数
	sign=1;
	high_bound=*this;

	while(low_bound<=high_bound)
	{
		my_int res=(high_bound+low_bound)/2;
		tmp=res*rhs;
		if(rhs.sign==0)
			tmp.sign=1-tmp.sign;

		compare_res=compare(tmp);
		if(compare_res>=0)
			quotient=std::move(res);
		if(compare_res==0)
			break;
		else if(compare_res>0)
			low_bound=res+1;
		else
			high_bound=res-1;
	}

	*this=std::move(quotient);

	if(is_zero())
		sign=1;
	else
		sign=!(org_sign^rhs.sign);
	return *this;
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

my_int operator /(const my_int &a,const my_int &b)
{
	return my_int(a)/=b;
}

my_int& my_int::operator %=(uint64_t rhs)
{
	*this=(*this)-((*this)/rhs)*rhs;
	return *this;
}

my_int& my_int::operator %=(int64_t rhs)
{
	*this=(*this)-((*this)/rhs)*rhs;
	return *this;
}

my_int& my_int::operator %=(int rhs)
{
	return operator%=((int64_t)rhs);
}

my_int& my_int::operator %=(const my_int &rhs)
{
	*this=(*this)-((*this)/rhs)*rhs;
	return *this;
}

my_int operator %(const my_int &a,uint64_t b)
{
	return my_int(a)%=b;
}

my_int operator %(const my_int &a,int64_t b)
{
	return my_int(a)%=b;
}

my_int operator %(const my_int &a,int b)
{
	return my_int(a)%=b;
}

my_int operator %(const my_int &a,const my_int &b)
{
	return my_int(a)%=b;
}

/*
 *	功能：计算base的power次的乘方
 * 	参数：
 *		base：底数
 *		power：指数
 * 	返回值：
 * 		乘方
 */
my_int power(const my_int &base,uint64_t power)
{
	my_int res=1;
	my_int tmp_base=base;

	while(power)
	{
		if(power&1)
			res*=tmp_base;
		power>>=1;
		tmp_base*=tmp_base;
	}       
	return res;
}

/*
 *	功能：计算n的阶乘
 * 	参数：
 *		n：参数
 * 	返回值：
 * 		阶乘
 */
my_int factorial(uint64_t n)
{
	my_int res=1;
	while(n>=2)
	{
		res*=n;
		n--;
	}
	return res;
}

ostream &operator <<(ostream &os,const my_int &a)
{
	os<<static_cast<string>(a);
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

}
