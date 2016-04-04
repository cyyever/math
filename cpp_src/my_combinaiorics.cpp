/*
 *	程序名：my_number_theory.cpp
 *	作者：陈源源
 *	日期：2016-04-04
 *	功能：包含组合学相关函数
 */

#include "my_combinaiorics.h"

#include <iostream>

namespace my_math
{

/*
 *	功能：生成伯努利系数C(n,k)
 *	参数：
 *		n,k：参数
 *	返回值：
 *		C(n,k)
 */
my_int binomial_coefficient(uint64_t n,uint32_t k)
{
	my_int numerator;
	uint64_t tmp;
	uint32_t denominator,i;

	if(k>n)
		throw std::out_of_range("k>n");

	if(n<k*2)
		k=n-k;
	if(k==0)
		return 1;
	numerator=n;
	denominator=1;

	for(i=2;i<=k;i++)
	{
		numerator*=(n-i+1);
		tmp=(uint64_t)denominator*i;
		if(tmp<UINT32_MAX)
			denominator=tmp;
		else
		{
			numerator/=(uint64_t)denominator;
			denominator=i;
		}
	}

	if(denominator!=1)
		numerator/=(uint64_t)denominator;
	return numerator;
}

#ifdef cyy
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
#endif
}
