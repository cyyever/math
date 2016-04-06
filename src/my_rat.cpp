/*
 *	��������my_rat.cpp
 *	���ߣ���ԴԴ
 *	���ڣ�2016-04-06
 *	���ܣ������������ͺ���
 */

#include <cstdlib>
#include <iostream>
#include <algorithm>

#include "my_rat.h"

using std::cout;
using std::endl;

namespace my_math
{

my_rat::my_rat():p(0),q(1)
{
}

/*
 *	���ܣ�ͨ���ַ����������������ַ��������Ƿ�����ʽ
 * 	������
 *		rat_str���ַ���
 * 	����ֵ��
 * 		��
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

	if(q<0)
	{
		(this->p)*=-1;
		(this->q)*=-1;
	}
}

my_rat::my_rat(my_int &&p,my_int &&q):p(p),q(q)
{
	if(q.is_zero())
		throw std::invalid_argument("q is zero");
	if(q<0)
	{
		(this->p)*=-1;
		(this->q)*=-1;
	}
}

my_rat::operator string() const
{
	string tmp;
	
	if(p<0)
		tmp.push_back('-');
	tmp.append(p);
	tmp.push_back('/');
	tmp.append(q);
	return tmp;
}

/*
 *	���ܣ��ȽϺ���һ���������Ĵ�С
 * 	������
 *		rhs����һ��������
 * 	����ֵ��
 * 		>0��������һ��������
 * 		0���������������
 * 		<0��С����һ��������
 */
int my_rat::compare(const my_rat &rhs) const
{
	if(p.sign<rhs.p.sign)
		return -1;
	else if(p.sign>rhs.p.sign)
		return 1;
	
	return (p*rhs.q).compare(q*rhs.p);
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
		b.p*=-1;
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
	if(p<0)	//���Ų�ͬ��ת���ɼ���
	{
		p*=-1;
		operator-=(rhs);
		if(!is_zero())
			p*=-1;
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

	if(p.sign!=rhs.p.sign)	//���Ų�ͬ��ת���ɼ���
	{
		p*=-1;
		operator-=(rhs);
		if(!is_zero())
			p*=-1;
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
	if(p<0)	//���Ų�ͬ��ת���ɼӷ�
	{
		p*=-1;
		operator+=(rhs);
		if(!is_zero())
			p*=-1;
		return *this;
	}

	p-=q*rhs;
	if(p.is_zero())
		q=1;
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
		p=1;
		q=0;
		return *this;
	}

	if(p.sign!=rhs.p.sign)	//���Ų�ͬ��ת���ɼӷ�
	{
		p*=-1;
		operator+=(rhs);
		if(!is_zero())
			p*=-1;
		return *this;
	}

	my_int tmp_p=p*rhs.q-q*rhs.p;
	q*=rhs.q;
	p=tmp_p;
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

my_rat& my_rat::operator *=(uint64_t rhs)
{
	p*=rhs;
	if(p.is_zero())
		q=1;
	return *this;
}

my_rat& my_rat::operator *=(int64_t rhs)
{
	operator*=(abs(rhs));
	if(rhs<0 && !this->is_zero())
		p*=-1;
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
		return *this;
	}

	if(this->is_zero())
		return *this;
	if(rhs.is_abs_one())
	{
		if(rhs.p<0)
			p*=-1;
		return *this;
	}
	if(rhs.is_zero())
	{
		*this=rhs;
		return *this;
	}

	p*=rhs.p;
	if(p.is_zero())
		q=1;
	else
		q*=rhs.q;
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
	if(rhs==0)
		throw std::invalid_argument("divided by zero");

	if(this->is_zero() || rhs==1)
		return *this;

	q*=rhs;
	return *this;
}

my_rat& my_rat::operator /=(int64_t rhs)
{
	operator/=(abs(rhs));
	if(rhs<0 && !this->is_zero())
		p*=-1;
	return *this;
}

my_rat& my_rat::operator /=(int rhs)
{
	return operator/=((int64_t)rhs);
}

my_rat& my_rat::operator /=(const my_rat &rhs)
{
	if(rhs.is_zero())
		throw std::invalid_argument("divided by zero");

	if(this==&rhs)
	{
		p=1;
		q=1;
		return *this;
	}

	if(this->is_zero())
		return *this;

	if(rhs.is_abs_one())
	{
		if(rhs.p<0)
			p*=-1;
		return *this;
	}

	p*=rhs.q;
	q*=rhs.p;
	if(q<0)
	{
		p*=-1;
		q*=-1;
	}
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

my_rat operator /(uint64_t a,const my_rat &b)
{
	if(b.is_zero())
		throw std::invalid_argument("divided by zero");
	if(a==0)
		return my_rat();

	my_rat c(b.q,b.p);
	if(a==1)
		return c;
	c.p*=a;
	return c;
}
my_rat operator /(int64_t a,const my_rat &b)
{
	my_rat c=operator/(abs(a),b);
	if(a<0 && !c.is_zero())
		c.p*=-1;
	return c;
}
my_rat operator /(int a,const my_rat &b)
{
	return operator/((int64_t)a,b);
}

ostream &operator <<(ostream &os,const my_rat &a)
{
	os<<(string)a;
	return os;
}

}
