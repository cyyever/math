/*
 *	程序名：my_rat.cpp
 *	作者：陈源源
 *	日期：2016-04-06
 *	功能：有理数相关类和函数
 */

#include <cstdlib>
#include <iostream>
#include <algorithm>

#include "my_rat.h"

using std::cout;
using std::endl;

namespace my_math
{

/*
 *	功能：通过字符串构造有理数，字符串必须是分数形式
 * 	参数：
 *		rat_str：字符串
 * 	返回值：
 * 		无
 */
my_rat::my_rat(const string &rat_str)
{
	auto i=rat_str.find('/');
	if(i==string::npos)
		throw std::invalid_argument(rat_str);
	*this=my_rat(my_int(rat_str.substr(0,i)),my_int(rat_str.substr(i+1)));
	return;
}

my_rat::my_rat(const my_int &p,const my_int &q):p(p),q(q)
{
	if(q.is_zero())
		throw std::invalid_argument("q is zero");

	if(p.sign!=q.sign)
		sign=0;
	else
		sign=1;
	this->p.sign=1;
	this->q.sign=1;
}

my_rat::my_rat(my_int &&p,my_int &&q):p(p),q(q)
{
	if(q.is_zero())
		throw std::invalid_argument("q is zero");

	if(p.sign!=q.sign)
		sign=0;
	else
		sign=1;
	this->p.sign=1;
	this->q.sign=1;
}

my_rat::operator string() const
{
	string tmp;
	
	if(sign==0)
		tmp.push_back('-');
	tmp.append(p);
	tmp.push_back('/');
	tmp.append(q);
	return tmp;
}

/*
 *	功能：比较和另一个有理数的大小
 * 	参数：
 *		rhs：另一个有理数
 * 	返回值：
 * 		>0：大于另一个有理数
 * 		0：两个有理数相等
 * 		<0：小于另一个有理数
 */
int my_rat::compare(const my_rat &rhs) const
{
	int res;
	if(sign<rhs.sign)
		return -1;
	else if(sign>rhs.sign)
		return 1;

	res=(p*rhs.q).compare(q*rhs.p);
	if(sign==0)	//负数
		res=-res;
	return res;
}


bool operator ==(const my_rat &a,const my_rat &b)
{
	if(a.compare(b)==0)
		return true;
	return false;
}

bool operator !=(const my_rat &a,const my_rat &b)
{
	return !(a==b);
}

bool operator <(const my_rat &a,const my_rat &b)
{
	if(a.compare(b)<0)
		return true;
	return false;
}

bool operator <=(const my_rat &a,const my_rat &b)
{
	if(a.compare(b)<=0)
		return true;
	return false;
}

bool operator >(const my_rat &a,const my_rat &b)
{
	return !(a<=b);
}

bool operator >=(const my_rat &a,const my_rat &b)
{
	return !(a<b);
}

my_rat operator -(const my_rat &a)
{
	my_rat b=a;
	if(!b.is_zero())
		b.sign=1-b.sign;
	return b;
}
#ifdef cyy

my_rat& my_rat::operator +=(int64_t rhs)
{
	if(rhs>=0)
		return operator+=((uint64_t)rhs);
	else
		return operator-=(abs(rhs));
}

my_rat& my_rat::operator +=(int rhs)
{
	return operator+=((int64_t)rhs);
}

my_rat& my_rat::operator +=(uint64_t rhs)
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

my_rat& my_rat::operator +=(const my_rat &rhs)
{
	if(this==&rhs)
	{
		my_rat tmp_this=*this;
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

my_rat operator +(const my_rat &a,uint64_t b)
{
	return my_rat(a)+=b;
}

my_rat operator +(const my_rat &a,int64_t b)
{
	return my_rat(a)+=b;
}

my_rat operator +(const my_rat &a,int b)
{
	return my_rat(a)+=b;
}

my_rat operator +(const my_rat &a,const my_rat &b)
{
	return my_rat(a)+=b;
}

my_rat& my_rat::operator -=(uint64_t rhs)
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
	if(is_abs_zero())
		sign=1;
	return *this;
}

my_rat& my_rat::operator -=(int64_t rhs)
{
	if(rhs>=0)
		return operator-=((uint64_t)rhs);
	else
		return operator+=(abs(rhs));
}

my_rat& my_rat::operator -=(int rhs)
{
	return operator-=((int64_t)rhs);
}

my_rat& my_rat::operator -=(const my_rat &rhs)
{
	if(this==&rhs)
	{
		my_rat tmp_this=*this;
		return operator-=(tmp_this);
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
	if(is_abs_zero())
		sign=1;
	return *this;
}

my_rat operator -(const my_rat &a,uint64_t b)
{
	return my_rat(a)-=b;
}

my_rat operator -(const my_rat &a,int64_t b)
{
	return my_rat(a)-=b;
}

my_rat operator -(const my_rat &a,int b)
{
	return my_rat(a)-=b;
}

my_rat operator -(const my_rat &a,const my_rat &b)
{
	return my_rat(a)-=b;
}

my_rat& my_rat::operator ++()
{
	operator+=(1);
	return *this;
}

my_rat& my_rat::operator --()
{
	operator-=(1);
	return *this;
}

my_rat my_rat::operator ++(int)
{
	my_rat tmp=*this;
	operator+=(1);
	return tmp;
}

my_rat my_rat::operator --(int)
{
	my_rat tmp=*this;
	operator-=(1);
	return tmp;
}

my_rat& my_rat::operator *=(uint64_t rhs)
{
	int64_t carry;
	unsigned __int128 tmp_my_digit;

	if(this->is_zero() || rhs==1)
	{
		return *this;
	}
	if(rhs==0)
	{
		*this=0;
		return *this;
	}

	carry=0;
	for(auto it=my_digit_list.begin();it!=my_digit_list.end();it++)
	{
		tmp_my_digit=(unsigned __int128)(*it)*rhs+carry;
		if(tmp_my_digit>=my_rat::my_base)
		{
			*it=tmp_my_digit%my_rat::my_base;
			carry=tmp_my_digit/my_rat::my_base;
		}
		else
		{
			*it=tmp_my_digit;
			carry=0;
		}
	}

	while(carry)
	{
		if(carry>=my_rat::my_base)
		{
			my_digit_list.push_back(carry%my_rat::my_base);
			carry/=my_rat::my_base;
		}
		else
		{
			my_digit_list.push_back(carry);
			carry=0;
		}
	}
	return *this;
}

my_rat& my_rat::operator *=(int64_t rhs)
{
	operator*=(abs(rhs));
	if(rhs<0 && !this->is_zero())
		sign=1-sign;
	return *this;
}

my_rat& my_rat::operator *=(int rhs)
{
	return operator*=((int64_t)rhs);
}

my_rat& my_rat::operator *=(const my_rat &rhs)
{
	if(this==&rhs)
	{
		my_rat tmp_this=*this;
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
	my_rat tmp_this=std::move(*this);

	*this=0;
	for(auto it=rhs.my_digit_list.cbegin();it!=rhs.my_digit_list.cend();it++,cnt++)
	{
		my_rat tmp=tmp_this*(*it);
		for(i=0;i<cnt;i++)
			tmp.my_digit_list.push_front(0);
		operator+=(tmp);
	}
	sign=new_sign;
	return *this;
}

my_rat operator *(const my_rat &a,uint64_t b)
{
	return my_rat(a)*=b;
}

my_rat operator *(const my_rat &a,int64_t b)
{
	return my_rat(a)*=b;
}

my_rat operator *(const my_rat &a,int b)
{
	return my_rat(a)*=b;
}

my_rat operator *(const my_rat &a,const my_rat &b)
{
	return my_rat(a)*=b;
}


my_rat& my_rat::operator /=(uint64_t rhs)
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

	if(is_abs_zero())
		sign=1;
	return *this;
}

my_rat& my_rat::operator /=(int64_t rhs)
{
	operator/=(abs(rhs));
	if(rhs<0 && !this->is_zero())
		sign=1-sign;
	return *this;
}

my_rat& my_rat::operator /=(int rhs)
{
	return operator/=((int64_t)rhs);
}

my_rat& my_rat::operator /=(const my_rat &rhs)
{
	my_rat quotient,tmp,low_bound,high_bound;
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
		my_rat res=(high_bound+low_bound)/2;
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

	if(is_abs_zero())
		sign=1;
	else
		sign=!(org_sign^rhs.sign);
	return *this;
}

my_rat operator /(const my_rat &a,uint64_t b)
{
	return my_rat(a)/=b;
}

my_rat operator /(const my_rat &a,int64_t b)
{
	return my_rat(a)/=b;
}

my_rat operator /(const my_rat &a,int b)
{
	return my_rat(a)/=b;
}

my_rat operator /(const my_rat &a,const my_rat &b)
{
	return my_rat(a)/=b;
}

my_rat& my_rat::operator %=(uint64_t rhs)
{
	*this=(*this)-((*this)/rhs)*rhs;
	return *this;
}

my_rat& my_rat::operator %=(int64_t rhs)
{
	*this=(*this)-((*this)/rhs)*rhs;
	return *this;
}

my_rat& my_rat::operator %=(int rhs)
{
	return operator%=((int64_t)rhs);
}

my_rat& my_rat::operator %=(const my_rat &rhs)
{
	*this=(*this)-((*this)/rhs)*rhs;
	return *this;
}

my_rat operator %(const my_rat &a,uint64_t b)
{
	return my_rat(a)%=b;
}

my_rat operator %(const my_rat &a,int64_t b)
{
	return my_rat(a)%=b;
}

my_rat operator %(const my_rat &a,int b)
{
	return my_rat(a)%=b;
}

my_rat operator %(const my_rat &a,const my_rat &b)
{
	return my_rat(a)%=b;
}

ostream &operator <<(ostream &os,const my_rat &a)
{
	os<<(string)a;
	return os;
}

inline bool my_rat::is_zero() const
{
	return my_digit_list.back()==0 && sign==1 && my_digit_list.size()==1;
}

inline bool my_rat::is_abs_zero() const
{
	return my_digit_list.back()==0 && my_digit_list.size()==1;
}

inline bool my_rat::is_abs_one() const
{
	return my_digit_list.back()==1 && my_digit_list.size()==1;
}

/*
 *	功能：检查传入的字符串是否有理数，即是否匹配 ^[+-]?[1-9][0-9]*$
 * 	参数：
 *		str：要检查的字符串
 * 	返回值：
 *		true：是
 *		false：不是
 */
bool my_rat::is_valid_int_str(const string &str)
{
	std::regex int_regex("^[+-]?(0|[1-9][0-9]*)$");
	std::smatch m;
	if(std::regex_search(str,m,int_regex))
		return true;
	else
		return false;
}
#endif
}
