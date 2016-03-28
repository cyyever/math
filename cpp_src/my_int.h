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

//整数表示成 (sign) (my_digit ... my_digit)*(10^4)^power，10^4进制
class my_int
{
	public:
		my_int();
		my_int(const string &int_str);
		~my_int() =default;
		operator string() const;
	private:
		list<int32_t> my_digit_list;
		ssize_t power;
		uint8_t sign;	//1-正数 0-负数
		bool is_valid_int_str(const string &str);
};

/*
 *	功能：重载ostream的<<给my_int
 * 	参数：
 *		os：输出流
 *		a：整数
 * 	返回值：
 * 		输出流
 */
ostream &operator <<(ostream &os,const my_int &a);
#endif
