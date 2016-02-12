/*
 *	程序名：my_arithmetic_test.c
 *	作者：陈源源
 *	日期：2013-02-06
 *	功能：包含my_arithmetic相关测试函数
 */
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <my_arithmetic.h>
int main(int argc, char** argv)
{
	my_rat *rat[3];
	my_rat *a;
	my_rat *b;
	int i,j,cmp_res;
	int64_t num[3]={INT64_MIN+1,0,INT64_MAX};
	char *str;
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

			if((num[i]>num[j] && cmp_res <=0)
			 || (num[i]==num[j] && cmp_res !=0)
			 || (num[i]<num[j] && cmp_res >=0))
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
	
	puts("开始测试my_rats_add");
	int64_t a_nums[]={88888,8};
	int64_t b_nums[]={-99999,-9};
	for(i=0;i<2;i++)
	{
		for(j=0;j<2;j++)
		{
			a=my_rat_from_int64(NULL,a_nums[i]);
			if(!a)
			{
				printf("++测试失败：%d\n",__LINE__);
				return -1;
			}

			b=my_rat_from_int64(NULL,b_nums[j]);
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
			printf("(%"PRId64")+(%"PRId64")=%s\n",a_nums[i],b_nums[j],str);
			free(str);
			my_rat_free(a);
			my_rat_free(b);
		}
	}
	puts("测试my_rats_add");

	puts("开始测试my_rats_multliply");
	for(i=0;i<2;i++)
	{
		for(j=0;j<2;j++)
		{
			a=my_rat_from_int64(NULL,a_nums[i]);
			if(!a)
			{
				printf("++测试失败：%d\n",__LINE__);
				return -1;
			}

			b=my_rat_from_int64(NULL,b_nums[j]);
			if(!b)
			{
				printf("++测试失败：%d\n",__LINE__);
				return -1;
			}

			if(my_rats_multiply(a,b,MY_ARG_RES)==NULL)
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
			printf("(%"PRId64")*(%"PRId64")=%s\n",a_nums[i],b_nums[j],str);
			free(str);
			my_rat_free(a);
			my_rat_free(b);
		}
	}
	puts("结束测试my_rats_multliply");

	puts("开始测试my_factorial");
	for(i=0;i<=10;i++)
	{
		a=my_factorial(i);
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
		printf("%d!=%s\n",i,str);
		free(str);
		my_rat_free(a);
	}
	puts("结束测试my_factorial");

	puts("开始测试my_rat_sum_digits和my_rat_digit_num");
	uint64_t digit_sum,digit_num;
	a=my_rat_from_str(NULL,"12341");
	if(!a)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}
	a->lsn->data=2340;
	a->power=-1;
	if(my_rat_sum_digits(a,&digit_sum)!=MY_SUCC)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}
	printf("digit_sum=%"PRIu64"\n",digit_sum);
	if(my_rat_digit_num(a,&digit_num)!=MY_SUCC)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}
	printf("digit_num=%"PRIu64"\n",digit_num);
	my_rat_free(a);
	puts("结束测试my_rat_sum_digits和my_rat_digit_num");

	puts("开始测试my_mod_uint32");
	uint32_t remainder;
	a=my_rat_from_str(NULL,"10000000000");
	if(!a)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}
	if(my_mod_uint32(a,7,&remainder)!=MY_SUCC)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}
	printf("remainder=%"PRIu32"\n",remainder);
	my_rat_free(a);
	puts("结束测试my_mod_uint32");

	return 0;
}
