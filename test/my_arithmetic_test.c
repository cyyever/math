/*
 *	程序名：ln_helper_test.c
 *	作者：陈源源
 *	日期：2013-02-06
 *	功能：包含my_arithmetic相关测试函数
 */
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <my_arithmetic.h>
int main(int argc, char** argv)
{
	my_rat *rat[3];
	int i,j,cmp_res;
	int64_t num[3]={INT64_MIN+1,0,INT64_MAX};
	for(i=0;i<3;i++)
	{
		rat[i]=my_rat_from_int64(NULL,num[i]);
		if(!rat[i])
		{
			printf("++测试失败：%d\n",__LINE__);
			return -1;
		}
	}

	puts("开始测试my_rats_cmp");
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			printf("++测试my_rats_cmp %zd %zd\n",num[i],num[j]);
			if(my_rats_cmp(rat[i],rat[j],&cmp_res)!=MY_SUCC)
			{
				printf("++测试失败：%d\n",__LINE__);
				return -1;
			}

			if((num[i]>num[j] && cmp_res !=1)
			 || (num[i]==num[j] && cmp_res !=0)
			 || (num[i]<num[j] && cmp_res !=-1))
			{
				printf("++测试失败：%d\n",__LINE__);
				return -1;
			}
		}
	}
	puts("结束测试my_rats_cmp");


	puts("开始测试my_rats_cmp_abs");
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			printf("++测试my_rats_cmp_abs %zd %zd\n",num[i],num[j]);
			if(my_rats_cmp_abs(rat[i],rat[j],&cmp_res)!=MY_SUCC)
			{
				printf("++测试失败：%d\n",__LINE__);
				return -1;
			}

			if((imaxabs(num[i])>imaxabs(num[j]) && cmp_res !=1)
			 || (imaxabs(num[i])==imaxabs(num[j]) && cmp_res !=0)
			 || (imaxabs(num[i])<imaxabs(num[j]) && cmp_res !=-1))
			{
				printf("++测试失败：%d\n",__LINE__);
				return -1;
			}
		}
	}
	puts("结束测试my_rats_cmp_abs");

	for(i=0;i<3;i++)
		my_rat_free(rat[i]);
	
	puts("开始测试my_rat_multiply_small_int");
	my_rat *a;
	char* str;
	a=my_rat_from_int64(NULL,1);
	if(!a)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}
	
	for(i=0;i<10;i++)
	{
		if(my_rat_multiply_small_int(a,999,MY_ARG_RES)==NULL)
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


	puts("开始测试my_rats_add");
	my_rat *b;
	a=my_rat_from_int64(NULL,INT64_MAX);
	if(!a)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}
	
	b=my_rat_from_int64(NULL,INT64_MIN);
	if(!b)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}

	if(my_rats_add(a,b,MY_ARG_RES)==NULL)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}

	str=my_rat_to_str(a);
	if(!str)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}
	puts(str);
	free(str);

	if(my_rats_add(a,b,MY_ARG_RES)==NULL)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}

	str=my_rat_to_str(a);
	if(!str)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}
	puts(str);
	free(str);

	my_rat_free(a);
	my_rat_free(b);

	puts("开始测试my_factorial");
	a=my_factorial(10);
	if(!a)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}
	
	str=my_rat_to_str(a);
	if(!str)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}
	puts(str);
	free(str);
	my_rat_free(a);

	return 0;
}
