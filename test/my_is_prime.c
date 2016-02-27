/*
 *	程序名：my_is_prime.c
 *	作者：陈源源
 *	日期：2016-02-27
 *	功能：包含my_is_prime函数的测试
 */
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <my_number_theory.h>
int main(int argc, char** argv)
{
	if(!my_is_prime(252097800623))
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}
	return 0;
}
