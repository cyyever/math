/*
 *	程序名：ln_helper_test.c
 *	作者：陈源源
 *	日期：2013-02-06
 *	功能：包含my_rat相关测试函数
 */
#include <stdio.h>
#include <inttypes.h>
#include <my_rat.h>

int main(int argc, char **argv)
{
	my_rat *a;
	char *str;
	char *err_str[]={NULL,"","-",".","-0.","-0.0.0"};
	char *num_str[]={"00","-0","-0.0","-000000","12300000","123","12345","-123.4","-0.0001234","-0.00001234","-1000.00001234"};
	int64_t int64_arr[]={INT64_MIN,INT64_MAX};
	int i;

	puts("开始测试my_rat_from_str与my_rat_to_str");

	for(i=0;i<sizeof(err_str)/sizeof(char*);i++)
	{
		printf("++测试my_rat_from_str %s\n",err_str[i]?(err_str[i][0]?err_str[i]:"空串"):"NULL");
		a=my_rat_from_str(NULL,err_str[i]);
		if(a)
		{
			printf("++测试失败：%d\n",__LINE__);
			my_rat_free(a);
			return -1;
		}

	}

	for(i=0;i<sizeof(num_str)/sizeof(char*);i++)
	{
		printf("++测试my_rat_from_str %s\n",num_str[i]);
		a=my_rat_from_str(NULL,num_str[i]);
		if(!a)
		{
			printf("++测试失败：%d\n",__LINE__);
			return -1;
		}
		str=my_rat_to_str(a);
		if(!str)
		{
			printf("++测试失败：%d\n",__LINE__);
			my_rat_free(a);
			return -1;
		}
		puts(str);
		my_rat_free(a);
		free(str);
	}

	puts("结束测试my_rat_from_str与my_rat_to_str");

	puts("开始测试my_rat_from_int64");

	for(i=0;i<sizeof(int64_arr)/sizeof(int64_t);i++)
	{
		printf("++测试my_rat_from_int64 %"PRId64"\n",int64_arr[i]);
		a=my_rat_from_int64(NULL,int64_arr[i]);
		if(!a)
		{
			printf("++测试失败：%d\n",__LINE__);
			my_rat_free(a);
			return -1;
		}

		str=my_rat_to_str(a);
		if(!str)
		{
			printf("++测试失败：%d\n",__LINE__);
			my_rat_free(a);
			return -1;
		}
		puts(str);
		my_rat_free(a);
		free(str);
	}

	puts("结束测试my_rat_from_int64");



	puts("开始测试my_rat_round");

	a=my_rat_from_str(NULL,"-0.123456789");
	if(!a)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}

	for(i=9;i>=0;i--)
	{
		if(my_rat_round(a,i,MY_TRUNC)!=MY_SUCC)
		{
			printf("++测试失败：%d\n",__LINE__);
			return -1;
		}
		str=my_rat_to_str(a);
		if(!str)
		{
			printf("++测试失败：%d\n",__LINE__);
			my_rat_free(a);
			return -1;
		}
		puts(str);
		free(str);
	}
	my_rat_free(a);
	puts("结束测试my_rat_round");
	return 0;
}
