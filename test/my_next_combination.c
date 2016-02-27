/*
 *	程序名：my_next_combination.c
 *	作者：陈源源
 *	日期：2016-02-27
 *	功能：包含my_next_combination函数的测试
 */
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <my_number_theory.h>
int main(int argc, char** argv)
{
	size_t i;
	uint8_t *combination;

	combination=NULL;
	while(my_next_combination(5,2,&combination))
	{
		for(i=0;i<5;i++)
			putchar('0'+combination[i]);
		putchar('\n');
	}
	free(combination);
	return 0;
}
