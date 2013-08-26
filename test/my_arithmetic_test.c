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
		assert(rat[i]);
	}

	puts("开始测试my_rats_cmp");
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			printf("++测试my_rats_cmp %zd %zd\n",num[i],num[j]);
			if(my_rats_cmp(rat[i],rat[j],&cmp_res)!=MY_SUCC)
			{
				printf("++测试my_rats_cmp %zd %zd 失败：my_rats_cmp\n",num[i],num[j]);
				return -1;
			}

			if((num[i]>num[j] && cmp_res !=1)
			 || (num[i]==num[j] && cmp_res !=0)
			 || (num[i]<num[j] && cmp_res !=-1))
			{
				printf("++测试my_rats_cmp %zd %zd 失败：%d\n",num[i],num[j],cmp_res);
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
				printf("++测试my_rats_cmp_abs %zd %zd 失败：my_rats_cmp_abs\n",num[i],num[j]);
				return -1;
			}

			if((imaxabs(num[i])>imaxabs(num[j]) && cmp_res !=1)
			 || (imaxabs(num[i])==imaxabs(num[j]) && cmp_res !=0)
			 || (imaxabs(num[i])<imaxabs(num[j]) && cmp_res !=-1))
			{
				printf("++测试my_rats_cmp_abs %zd %zd 失败：%d\n",num[i],num[j],cmp_res);
				return -1;
			}
		}
	}
	puts("结束测试my_rats_cmp_abs");

	return 0;
}
