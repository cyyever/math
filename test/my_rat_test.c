/*
 *	程序名：ln_helper_test.c
 *	作者：陈源源
 *	日期：2013-02-06
 *	功能：包含my_rat相关测试函数
 */
#include <assert.h>
#include <stdio.h>
#include <my_rat.h>
int main(int argc, char** argv)
{
	my_rat *a;
	puts("开始测试my_rat_from_str");
	puts("++测试my_rat_from_str 00");
	a=my_rat_from_str(NULL,"00");
	assert(a);
	puts("++测试my_rat_from_str NULL");
	assert(!my_rat_from_str(a,NULL));
	puts("++测试my_rat_from_str 空串");
	assert(!my_rat_from_str(a,""));
	puts("++测试my_rat_from_str -");
	assert(!my_rat_from_str(a,"-"));
	puts("++测试my_rat_from_str .");
	assert(!my_rat_from_str(a,"."));
	puts("++测试my_rat_from_str -0");
	assert(my_rat_from_str(a,"-0"));
	puts("++测试my_rat_from_str -0.");
	assert(!my_rat_from_str(a,"-0."));
	puts("++测试my_rat_from_str -0.0");
	assert(my_rat_from_str(a,"-0.0"));
	puts("++测试my_rat_from_str -0.0.0");
	assert(!my_rat_from_str(a,"-0.0.0"));
	my_rat_free(a);
	puts("结束测试my_rat_from_str");

	char* str;
	puts("开始测试my_rat_to_str");
	puts("++测试my_rat_to_str -00000");
	a=my_rat_from_str(NULL,"-00000");
	assert(a);
	str=my_rat_to_str(a);
	assert(str);
	puts(str);
	free(str);

	puts("++测试my_rat_to_str -12300000");
	a=my_rat_from_str(a,"-12300000");
	assert(a);
	str=my_rat_to_str(a);
	assert(str);
	puts(str);
	free(str);

	puts("++测试my_rat_to_str 123");
	a=my_rat_from_str(a,"123");
	assert(a);
	str=my_rat_to_str(a);
	assert(str);
	puts(str);
	free(str);

	puts("++测试my_rat_to_str 12345");
	a=my_rat_from_str(a,"12345");
	assert(a);
	str=my_rat_to_str(a);
	assert(str);
	puts(str);
	free(str);

	puts("++测试my_rat_to_str -123.4");
	a=my_rat_from_str(a,"-123.4");
	assert(a);
	str=my_rat_to_str(a);
	assert(str);
	puts(str);
	free(str);

	puts("++测试my_rat_to_str -0.0001234");
	a=my_rat_from_str(a,"-0.0001234");
	assert(a);
	str=my_rat_to_str(a);
	assert(str);
	puts(str);
	free(str);

	puts("++测试my_rat_to_str -0.00001234");
	a=my_rat_from_str(a,"-0.00001234");
	assert(a);
	str=my_rat_to_str(a);
	assert(str);
	puts(str);
	free(str);

	puts("++测试my_rat_to_str -1000.00001234");
	a=my_rat_from_str(a,"-1000.00001234");
	assert(a);
	str=my_rat_to_str(a);
	assert(str);
	puts(str);
	free(str);
	my_rat_free(a);
	puts("结束测试my_rat_to_str");

	/*
	puts("开始测试ln_init");
	ln a,b,c;
	puts("++测试ln_init 1");
	a=ln_init(1);
	assert(a);
	puts("++测试ln_init 0");
	b=ln_init(0);
	assert(b);
	puts("++测试ln_init -1");
	c=ln_init(-1);
	assert(c);
	puts("结束测试ln_init");

	puts("开始测试ln_cmp");
	puts("++测试ln_cmp 1 1");
	assert(ln_cmp(a,a)==0);
	puts("++测试ln_cmp 1 0");
	assert(ln_cmp(a,b)==1);
	puts("++测试ln_cmp 1 -1");
	assert(ln_cmp(a,c)==1);
	puts("++测试ln_cmp 0 1");
	assert(ln_cmp(b,a)==-1);
	puts("++测试ln_cmp 0 0");
	assert(ln_cmp(b,b)==0);
	puts("++测试ln_cmp 0 -1");
	assert(ln_cmp(b,c)==1);
	puts("++测试ln_cmp -1 1");
	assert(ln_cmp(c,a)==-1);
	puts("++测试ln_cmp -1 0");
	assert(ln_cmp(c,b)==-1);
	puts("++测试ln_cmp -1 -1");
	assert(ln_cmp(c,c)==0);
	puts("结束测试ln_cmp");

	puts("a");
	ln_free(a);
	puts("b");
	ln_free(b);
	puts("c");
	ln_free(c);
	*/
	return 0;
}
