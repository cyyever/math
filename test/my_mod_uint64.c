/*
 *	程序名：my_mod_uint64.c
 *	作者：陈源源
 *	日期：2016-02-29
 *	功能：包含my_mod_uint64函数的测试用例
 */
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <my_arithmetic.h>

int main(int argc, char** argv)
{
	my_rat *a;
	uint64_t remainder;

	a=my_rat_from_str(NULL,"123456789987654321");
	if(!a)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}

	if(my_mod_uint64(a,333667,&remainder)!=MY_SUCC)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}

	printf("remainder=%"PRIu64"\n",remainder);
	my_rat_free(a);
	puts("结束测试my_mod_uint64");

	return 0;
}
