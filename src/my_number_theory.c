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

static uint64_t *primes;
static size_t prime_num;

/*
 *	功能：缓存质数
 *	参数：
 *		无
 *	返回值：
 *		无
 */
static void my_cache_primes(void)
{
	uint64_t *tmp;
	size_t i,j;

	if(!primes)
	{
		primes=malloc(sizeof(*primes)*(100000+1));
		if(!primes)
		{
			my_log("malloc failed:%m");
			exit(EXIT_FAILURE);
		}

		//标识质数
		for(i=2;i<=100000;i++)
			primes[i]=1;
		for(i=2;i<=100000;i++)
		{
			if(primes[i])
				for(j=i*2;j<=100000;j+=i)
					primes[j]=0;
		}
		//重新定位
		for(i=2,j=0;i<=100000;i++)
		{
			if(primes[i])
				primes[j++]=i;
		}
		prime_num=j;
		tmp=realloc(primes,j*sizeof(*tmp));
		if(!tmp)
		{
			my_log("realloc failed:%m");
			free(primes);
			primes=NULL;
			exit(EXIT_FAILURE);
		}
		primes=tmp;
	}
	return;
}

/*
 *	功能：判定质数
 *	参数：
 *		num：要判定的数
 *	返回值：
 *		1：是质数
 *		0：不是质数
 */
int my_is_prime(uint64_t num)
{
	uint64_t square;
	size_t i,j,k,prime;

	my_cache_primes();

	if(num==UINT64_MAX)
		return 0;
	if(num<=1)
		return 0;
	if((num&1)==0)
		return 0;
	if(num<=primes[prime_num-1])
	{
		i=0;
		j=prime_num-1;

		while(i<=j)
		{
			k=(i+j)/2;

			if(num==primes[k])
				return 1;
			else if(num<primes[k])
				j=k-1;
			else
				i=k+1;
		}
		return 0;
	}


	i=1;
	if(num>UINT32_MAX)
		j=UINT32_MAX+1;
	else
		j=num;
	while(i<=j)
	{
		k=(i+j)/2;
		square=k*k;

		if(num==square)
			return 0;
		else if(num<square)
			j=k-1;
		else
			i=k+1;
	}
	j++;

	for(i=1;i<prime_num;i++)
	{
		if(primes[i]>j)
			return 1;
		if((num%primes[i])==0)
			return 0;
	}

	for(prime=100001;prime<=j;prime+=2)
	{
		if(my_is_prime(prime))
		{
			if((num%prime)==0)
				return 0;
		}
	}
	return 1;
}

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
	uint64_t *local_primes;
	uint64_t prime;
	size_t i;

	my_cache_primes();

	local_primes=malloc(sizeof(*local_primes)*(upper_limit/2+1));
	if(!local_primes)
	{
		my_log("malloc failed:%m");
		return NULL;
	}

	for(i=0;i<prime_num;i++)
	{
		if(primes[i]>upper_limit)
		{
			local_primes[i]=0;
			break;
		}
		local_primes[i]=primes[i];
	}

	if(i<prime_num)
		return local_primes;

	for(prime=100001;prime<=upper_limit;prime+=2)
	{
		if(my_is_prime(prime))
		{
			local_primes[i]=prime;
			i++;
		}
	}
	local_primes[i]=0;
	return local_primes;
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
		my_log("k is out of range");
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

/*
 *	功能：生成组合
 *	参数：
 *		n,k：在n个数中选k个数的组合
 *		prev_combination：如果*prev_combination非NULL，把第一个组合写入此，否则包括上一次调用此函数生成的组合，此函数把下一个组合写入此
 *	返回值：
 *		非NULL：返回*prev_combination
 *		NULL：组合已经全部生成完毕
 */
uint8_t *my_next_combination(uint64_t n,uint64_t k,uint8_t **prev_combination)
{
	uint64_t i,cnt;
	uint8_t find_zero;

	if(k>n)
	{
		my_log("k is out of range");
		exit(EXIT_FAILURE);
	}
	if(!prev_combination)
	{
		my_log("prev_combination is NULL");
		exit(EXIT_FAILURE);
	}

	if(!*prev_combination)
	{
		*prev_combination=calloc(n,1);
		if(!*prev_combination)
		{
			my_log("calloc failed:%m");
			exit(EXIT_FAILURE);
		}
		memset(*prev_combination,1,k);
		return *prev_combination;
	}

	cnt=0;
	find_zero=0;
	i=n-1;
	while(1)
	{
		if((*prev_combination)[i]==1)
		{
			cnt++;
			(*prev_combination)[i]=0;
			if(find_zero)
			{
				i++;
				memset((*prev_combination)+i,1,cnt);
				return *prev_combination;
			}
		}
		else
			find_zero=1;
		if(i==0)
			break;
		i--;
	}
	return NULL;
}
