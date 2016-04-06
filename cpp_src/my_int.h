/*
 *	程序名：my_int.h
 *	作者：陈源源
 *	日期：2016-03-28
 *	功能：整数相关类和函数
 */
#ifndef MY_INT_H
#define MY_INT_H

#include <string>
#include <list>
#include <cinttypes>

using std::string;
using std::list;
using std::ostream;

namespace my_math
{

class my_rat;

//整数表示成 (sign) (my_digit ... my_digit)*10^power，10^10进制
class my_int
{
	public:
		my_int();
		my_int(const string &int_str);
		my_int(uint64_t num);
		my_int(int64_t num);
		my_int(int num);
		~my_int() =default;
		operator string() const;
		my_int& operator +=(uint64_t rhs);
		my_int& operator +=(int64_t rhs);
		my_int& operator +=(int rhs);
		my_int& operator +=(const my_int &rhs);
		my_int& operator -=(uint64_t rhs);
		my_int& operator -=(int64_t rhs);
		my_int& operator -=(int rhs);
		my_int& operator -=(const my_int &rhs);
		my_int& operator ++();	//前缀
		my_int& operator --();
		my_int operator ++(int);	//后缀
		my_int operator --(int);
		my_int& operator *=(uint64_t rhs);
		my_int& operator *=(int64_t rhs);
		my_int& operator *=(int rhs);
		my_int& operator *=(const my_int &rhs);
		my_int& operator /=(uint64_t rhs);
		my_int& operator /=(int64_t rhs);
		my_int& operator /=(int rhs);
		my_int& operator /=(const my_int &rhs);
		my_int& operator %=(uint64_t rhs);
		my_int& operator %=(int64_t rhs);
		my_int& operator %=(int rhs);
		my_int& operator %=(const my_int &rhs);
	private:
#ifdef NDEBUG
		static const size_t my_digit_num=18;			//单个my_digit所包含的位数
		static const int64_t my_base=1000000000000000000LL;	//单个my_digit的base
#else
		static const size_t my_digit_num=1;			//单个my_digit所包含的位数
		static const int64_t my_base=10;			//单个my_digit的base
#endif
		static bool is_valid_int_str(const string &str);


		list<int64_t> my_digit_list;
		uint8_t sign;	//1-正数 0-负数

		//对常用的比较我们另外写函数，避免构造对象的开销
		bool is_zero() const
		{
			return my_digit_list.back()==0 && sign==1 && my_digit_list.size()==1;
		}
		bool is_abs_zero() const
		{
			return my_digit_list.back()==0 && my_digit_list.size()==1;
		}
		bool is_abs_one() const
		{
			return my_digit_list.back()==1 && my_digit_list.size()==1;
		}

		int compare(const my_int &rhs) const;



	friend bool operator ==(const my_int &a,const my_int &b);
	friend bool operator !=(const my_int &a,const my_int &b);
	friend bool operator <(const my_int &a,const my_int &b);
	friend bool operator <=(const my_int &a,const my_int &b);
	friend bool operator >(const my_int &a,const my_int &b);
	friend bool operator >=(const my_int &a,const my_int &b);
	friend my_int operator -(const my_int &a);
	friend my_int operator +(const my_int &a,uint64_t b);
	friend my_int operator +(const my_int &a,int64_t b);
	friend my_int operator +(const my_int &a,int b);
	friend my_int operator +(const my_int &a,const my_int &b);
	friend my_int operator -(const my_int &a,uint64_t b);
	friend my_int operator -(const my_int &a,int64_t b);
	friend my_int operator -(const my_int &a,int b);
	friend my_int operator -(const my_int &a,const my_int &b);
	friend my_int operator *(const my_int &a,uint64_t b);
	friend my_int operator *(const my_int &a,int64_t b);
	friend my_int operator *(const my_int &a,int b);
	friend my_int operator *(const my_int &a,const my_int &b);
	friend my_int operator /(const my_int &a,uint64_t b);
	friend my_int operator /(const my_int &a,int64_t b);
	friend my_int operator /(const my_int &a,int b);
	friend my_int operator /(const my_int &a,const my_int &b);
	friend my_int operator %(const my_int &a,uint64_t b);
	friend my_int operator %(const my_int &a,int64_t b);
	friend my_int operator %(const my_int &a,int b);
	friend my_int operator %(const my_int &a,const my_int &b);
	friend class my_rat;
};

uint64_t inline abs(int64_t num);

bool operator ==(const my_int &a,const my_int &b);
bool operator !=(const my_int &a,const my_int &b);
bool operator <(const my_int &a,const my_int &b);
bool operator <=(const my_int &a,const my_int &b);
bool operator >(const my_int &a,const my_int &b);
bool operator >=(const my_int &a,const my_int &b);
my_int operator -(const my_int &a);
my_int operator +(const my_int &a,uint64_t b);
my_int operator +(const my_int &a,int64_t b);
my_int operator +(const my_int &a,int b);
my_int operator +(const my_int &a,const my_int &b);
my_int operator -(const my_int &a,uint64_t b);
my_int operator -(const my_int &a,int64_t b);
my_int operator -(const my_int &a,int b);
my_int operator -(const my_int &a,const my_int &b);
my_int operator *(const my_int &a,uint64_t b);
my_int operator *(const my_int &a,int64_t b);
my_int operator *(const my_int &a,int b);
my_int operator *(const my_int &a,const my_int &b);
my_int operator /(const my_int &a,uint64_t b);
my_int operator /(const my_int &a,int64_t b);
my_int operator /(const my_int &a,int b);
my_int operator /(const my_int &a,const my_int &b);
my_int operator %(const my_int &a,uint64_t b);
my_int operator %(const my_int &a,int64_t b);
my_int operator %(const my_int &a,int b);
my_int operator %(const my_int &a,const my_int &b);

ostream &operator <<(ostream &os,const my_int &a);
}
#endif
