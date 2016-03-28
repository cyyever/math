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
#include <cstdint>

using std::string;
using std::list;
using std::ostream;

//整数表示成 (sign) (my_digit ... my_digit)*10^power，10^10进制
class my_int
{
	public:
		my_int();
		my_int(const string &int_str);
		my_int(uint64_t num);
		~my_int() =default;
		operator string() const;
		my_int& operator +=(uint64_t num);
	private:
		static const size_t my_digit_num=19;			//单个my_digit所包含的位数
		static const size_t my_base=10000000000000000000ULL;	//单个my_digit的base
		list<uint64_t> my_digit_list;
		ssize_t power;
		uint8_t sign;	//1-正数 0-负数
		bool is_valid_int_str(const string &str);
		list<uint64_t> inline get_my_digit_list(uint64_t num);
};

ostream &operator <<(ostream &os,const my_int &a);
#endif
