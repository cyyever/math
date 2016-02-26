/*
 *	程序名：my_number_theory_test.c
 *	作者：陈源源
 *	日期：2016-02-12
 *	功能：包含my_number_theory函数的测试
 */
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <my_number_theory.h>
int main(int argc, char** argv)
{
	size_t i;
	uint64_t *primes;
	my_rat *num;
	char *str;

	puts("开始测试my_primes");
	primes=my_primes(100);
	if(!primes)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}
	for(i=0;primes[i];i++)
		printf("%"PRIu64"\n",primes[i]);
	puts("结束测试my_primes");
	free(primes);

	puts("开始测试my_is_prime");
	if(!my_is_prime(252097800623))
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}
	puts("结束测试my_is_prime");

	puts("开始测试my_binomial");
	num=my_binomial(100,50);
	if(!num)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}
	str=my_rat_to_str(num);
	if(!str)
	{
		printf("++测试失败：%d\n",__LINE__);
		return -1;
	}
	printf("C(100,50)=%s\n",str);
	free(str);
	my_rat_free(num);
	puts("结束测试my_binomial");

	return 0;
}
