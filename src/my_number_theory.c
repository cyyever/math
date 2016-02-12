/*
 *	程序名：my_number_theory.c
 *	作者：陈源源
 *	日期：2016-02-11
 *	功能：包含数论相关函数
 */

#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "my_arithmetic.h"
#include "my_log.h"

/*
 *	功能：生成质数
 *	参数：
 *		upper_limit：质数上限
 *	返回值：
 *		NULL：失败
 *		非NULL：质数数组，以零结尾
 */
uint64_t *my_primes(uint64_t upper_limit)
{
	uint64_t *primes,*tmp;
	size_t i,j;

	primes=malloc(8*(upper_limit+1));
	if(!primes)
	{
		my_log("malloc failed:%m");
		return NULL;
	}

	//标识质数
	for(i=2;i<=upper_limit;i++)
		primes[i]=1;
	for(i=2;i<=upper_limit;i++)
	{
		if(primes[i])
			for(j=i*2;j<=upper_limit;j+=i)
				primes[j]=0;
	}
	//重新定位
	for(i=2,j=0;i<=upper_limit;i++)
	{
		if(primes[i])
			primes[j++]=i;
	}
	primes[j]=0;
	tmp=realloc(primes,(j+1)*8);
	if(!tmp)
	{
		my_log("realloc failed:%m");
		free(primes);
		return NULL;
	}
	primes=tmp;
	return primes;
}

/*
 *	功能：生成伯努利系数C(n,k)
 *	参数：
 *		n,k：参数
 *	返回值：
 *		非NULL：C(n,k)
 *		NULL：失败
 */
my_rat *my_binomial(int64_t n,uint32_t k)
{
	my_rat *numerator;
	uint64_t tmp;
	uint32_t denominator,i;

	if(k>n)
	{
		my_log("k is out of range:%m");
		return NULL;
	}
	if(n-k<k)
		k=n-k;
	if(k==0)
		numerator=my_rat_from_int64(NULL,1);
	else
		numerator=my_rat_from_int64(NULL,n);
	if(!numerator)
	{
		my_log("my_rat_from_int64 failed");
		return NULL;
	}
	denominator=1;

	for(i=2;i<=k;i++)
	{
		if(my_rat_multiply_int64(numerator,n-i+1,MY_ARG_RES)==NULL)
		{
			my_log("my_rat_multiply_int64 failed");
			my_rat_free(numerator);
			return NULL;
		}
		tmp=(uint64_t)denominator*i;
		my_log("denominator=%llu",(unsigned long long)denominator);
		if(tmp<UINT32_MAX)
			denominator=tmp;
		else
		{
			if(my_divide_uint32(numerator,denominator,0,MY_TRUNC,MY_ARG_RES)==NULL)
			{
				my_log("my_rat_multiply_int64 failed");
				my_rat_free(numerator);
				return NULL;
			}
			denominator=i;
		}
	}

	if(denominator!=1)
	{
		if(my_divide_uint32(numerator,denominator,0,MY_TRUNC,MY_ARG_RES)==NULL)
		{
			my_log("my_divide_uint32 failed");
			my_rat_free(numerator);
			return NULL;
		}
	}
	return numerator;
}
