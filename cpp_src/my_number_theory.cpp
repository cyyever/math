/*
 *	程序名：my_number_theory.cpp
 *	作者：陈源源
 *	日期：2016-04-04
 *	功能：包含数论相关函数
 */

#include "my_number_theory.h"

#include <vector>
#include <iostream>

using std::vector;

const uint64_t cached_prime_upper_limit=10000000;
static vector<uint64_t> primes(cached_prime_upper_limit+1,0);

namespace my_math
{
/*
 *	功能：缓存质数
 *	参数：
 *		无
 *	返回值：
 *		无
 */
static void cache_primes(void)
{
	uint64_t i,j;

	if(primes.back()==0)
	{
		//标识质数
		for(i=2;i<=cached_prime_upper_limit;i++)
			primes[i]=1;
		for(i=2;i<=cached_prime_upper_limit;i++)
		{
			if(primes[i])
				for(j=i*2;j<=cached_prime_upper_limit;j+=i)
					primes[j]=0;
		}
		//重新定位
		for(i=2,j=0;i<=cached_prime_upper_limit;i++)
		{
			if(primes[i])
				primes[j++]=i;
		}
		primes.resize(j);
	}
	return;
}

/*
 *	功能：判定质数
 *	参数：
 *		num：要判定的数
 *	返回值：
 *		true：是质数
 *		false：不是质数
 */
bool is_prime(uint64_t num)
{
	decltype(primes.size())	i,j,k;
	uint64_t square,prime;

	cache_primes();

	if(num==UINT64_MAX)
		return false;
	if(num<=1)
		return false;
	if((num&1)==0 && num!=2)
		return false;
	if(num<=primes.back())
	{
		i=0;
		j=primes.size()-1;

		while(i<=j)
		{
			k=(i+j)/2;

			if(num==primes[k])
				return true;
			else if(num<primes[k])
				j=k-1;
			else
				i=k+1;
		}
		return false;
	}

	for(i=1;i<100;i++)
	{
		if((num%primes[i])==0)
			return false;
	}

	i=1;
	if(num>UINT32_MAX)
		j=(uint64_t)(UINT32_MAX)+1;
	else
		j=num;
	while(i<=j)
	{
		k=(i+j)/2;
		square=k*k;
		if(num==square)
			return false;
		else if(num<square)
			j=k-1;
		else
			i=k+1;
	}
	for(i=100;i<primes.size();i++)
	{
		if(primes[i]>j)
			return true;
		if((num%primes[i])==0)
			return false;
	}

	for(prime=cached_prime_upper_limit;prime<=j;prime+=2)
	{
		if((num%prime)==0)
			return false;
	}
	return true;
}

/*
 *	功能：生成质数
 *	参数：
 *		upper_limit：质数上限
 *	返回值：
 *		NULL：失败
 *		非NULL：质数数组，以零结尾
uint64_t *my_primes(uint64_t upper_limit)
{
	uint64_t *local_primes;
	uint64_t prime;
	size_t i;

	cache_primes();

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

	for(prime=10000001;prime<=upper_limit;prime+=2)
	{
		if(is_prime(prime))
		{
			local_primes[i]=prime;
			i++;
		}
	}
	local_primes[i]=0;
	return local_primes;
}
 */

}
