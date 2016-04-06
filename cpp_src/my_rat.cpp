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
		if(!is_abs_zero())
			sign=1-sign;
		return *this;
	}

	p+=q*rhs;
	return *this;
}

my_rat& my_rat::operator +=(const my_rat &rhs)
{
	if(this==&rhs)
	{
		p*=2;
		return *this;
	}

	if(sign!=rhs.sign)	//符号不同，转换成减法
	{
		sign=1-sign;
		operator-=(rhs);
		if(!is_abs_zero())
			sign=1-sign;
		return *this;
	}

	my_int tmp_p=p*rhs.q+q*rhs.p;
	q*=rhs.q;
	p=tmp_p;
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
		if(!is_abs_zero())
			sign=1-sign;
		return *this;
	}

	p-=q*rhs;
	if(p.sign==0)
	{
		p.sign=1;
		sign=1-sign;
	}
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
		p=0;
		q=1;
		sign=1;
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

	my_int tmp_p=p*rhs.q-q*rhs.p;
	q*=rhs.q;
	p=tmp_p;
	if(p.sign==0)
	{
		p.sign=1;
		sign=1-sign;
	}
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
	p*=rhs;
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
		p*=p;
		q*=q;
		sign=1;
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
	if(rhs.is_zero())
	{
		*this=rhs;
		return *this;
	}

	p*=rhs.p;
	q*=rhs.q;
	sign=!(sign^rhs.sign);
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

#ifdef cyy
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

	/*
	if(rhs.is_abs_one())
	{
		if(rhs.sign==0)
			sign=1-sign;
		return *this;
	}
	*/


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
