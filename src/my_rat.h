/*
 *	程序名：my_rat.h
 *	作者：陈源源
 *	日期：2016-04-06
 *	功能：有理数相关类和函数
 */
#ifndef MY_RAT_H
#define MY_RAT_H

#include <string>
#include <cinttypes>
#include "my_int.h"

using std::string;
using std::ostream;

namespace my_math
{

class my_rat
{
	public:
		my_rat();
		my_rat(const string &rat_str);
		my_rat(const my_int &p,const my_int &q);
		my_rat(my_int &&p,my_int &&q);
		~my_rat() =default;
		operator string() const;
		const my_int &numerator() const	//获取有理数的分子
		{
			return p;
		}
		const my_int &denominator() const	//获取有理数的分母
		{
			return q;
		}
		my_rat& operator +=(uint64_t rhs);
		my_rat& operator +=(int64_t rhs);
		my_rat& operator +=(int rhs);
		my_rat& operator +=(const my_rat &rhs);
		my_rat& operator -=(uint64_t rhs);
		my_rat& operator -=(int64_t rhs);
		my_rat& operator -=(int rhs);
		my_rat& operator -=(const my_rat &rhs);
		my_rat& operator *=(uint64_t rhs);
		my_rat& operator *=(int64_t rhs);
		my_rat& operator *=(int rhs);
		my_rat& operator *=(const my_rat &rhs);
		my_rat& operator /=(uint64_t rhs);
		my_rat& operator /=(int64_t rhs);
		my_rat& operator /=(int rhs);
		my_rat& operator /=(const my_rat &rhs);
		my_rat& operator %=(uint64_t rhs);
		my_rat& operator %=(int64_t rhs);
		my_rat& operator %=(int rhs);
		my_rat& operator %=(const my_rat &rhs);
		my_rat &simplify();
	private:
		my_int p;			//分子，有理数的符号由它体现
		my_int q;			//分母，恒为正数

		//对常用的比较我们另外写函数，避免构造对象的开销
		bool is_zero() const
		{
			return p==0;
		}
		bool is_abs_one() const
		{
			return p==q;
		}
		int compare(const my_rat &rhs) const;

	friend bool operator ==(const my_rat &a,const my_rat &b);
	friend bool operator !=(const my_rat &a,const my_rat &b);
	friend bool operator <(const my_rat &a,const my_rat &b);
	friend bool operator <=(const my_rat &a,const my_rat &b);
	friend bool operator >(const my_rat &a,const my_rat &b);
	friend bool operator >=(const my_rat &a,const my_rat &b);
	friend my_rat operator -(const my_rat &a);
	friend my_rat operator +(const my_rat &a,uint64_t b);
	friend my_rat operator +(const my_rat &a,int64_t b);
	friend my_rat operator +(const my_rat &a,int b);
	friend my_rat operator +(const my_rat &a,const my_rat &b);
	friend my_rat operator -(const my_rat &a,uint64_t b);
	friend my_rat operator -(const my_rat &a,int64_t b);
	friend my_rat operator -(const my_rat &a,int b);
	friend my_rat operator -(const my_rat &a,const my_rat &b);
	friend my_rat operator *(const my_rat &a,uint64_t b);
	friend my_rat operator *(const my_rat &a,int64_t b);
	friend my_rat operator *(const my_rat &a,int b);
	friend my_rat operator *(const my_rat &a,const my_rat &b);
	friend my_rat operator /(const my_rat &a,uint64_t b);
	friend my_rat operator /(const my_rat &a,int64_t b);
	friend my_rat operator /(const my_rat &a,int b);
	friend my_rat operator /(const my_rat &a,const my_rat &b);
	friend my_rat operator /(uint64_t a,const my_rat &b);
	friend my_rat operator /(int64_t a,const my_rat &b);
	friend my_rat operator /(int a,const my_rat &b);
};

bool operator ==(const my_rat &a,const my_rat &b);
bool operator !=(const my_rat &a,const my_rat &b);
bool operator <(const my_rat &a,const my_rat &b);
bool operator <=(const my_rat &a,const my_rat &b);
bool operator >(const my_rat &a,const my_rat &b);
bool operator >=(const my_rat &a,const my_rat &b);
my_rat operator -(const my_rat &a);
my_rat operator +(const my_rat &a,uint64_t b);
my_rat operator +(const my_rat &a,int64_t b);
my_rat operator +(const my_rat &a,int b);
my_rat operator +(const my_rat &a,const my_rat &b);
my_rat operator -(const my_rat &a,uint64_t b);
my_rat operator -(const my_rat &a,int64_t b);
my_rat operator -(const my_rat &a,int b);
my_rat operator -(const my_rat &a,const my_rat &b);
my_rat operator *(const my_rat &a,uint64_t b);
my_rat operator *(const my_rat &a,int64_t b);
my_rat operator *(const my_rat &a,int b);
my_rat operator *(const my_rat &a,const my_rat &b);
my_rat operator /(const my_rat &a,uint64_t b);
my_rat operator /(const my_rat &a,int64_t b);
my_rat operator /(const my_rat &a,int b);
my_rat operator /(const my_rat &a,const my_rat &b);
my_rat operator /(uint64_t a,const my_rat &b);
my_rat operator /(int64_t a,const my_rat &b);
my_rat operator /(int a,const my_rat &b);

ostream &operator <<(ostream &os,const my_rat &a);
}
#endif
