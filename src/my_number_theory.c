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
		printf("malloc failed:%m\n");
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
		printf("realloc failed:%m");
		free(primes);
		return NULL;
	}
	primes=tmp;
	return primes;
}
