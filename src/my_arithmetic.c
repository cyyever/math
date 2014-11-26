/*
 *	程序名：my_arithmetic.c
 *	作者：陈源源
 *	日期：2013-02-10
 *	功能：包含算术运算操作
 */

#include <string.h>
#include <errno.h>
#include "my_arithmetic.h"
#include "my_log.h"

static unsigned int power10[]={1,10,100,1000};

/*
 *	功能：比较有理数的大小
 *	参数：
 *		a，b：要比较的有理数
 *		cmp_res：比较结果：
 *			>0：a>b
 *			0：a=b
 *			<0：a<b
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
int my_rats_cmp(my_rat *a,my_rat *b,int32_t *cmp_res)
{
	size_t a_digit_num,b_digit_num;
	my_node *p,*q;

	//检查参数
	if(!a)
	{
		my_log("a is NULL");
		return MY_ERROR;	
	}
	if(!MY_RAT_HAS_INITED(a))
	{
		my_log("a is uninitialized");
		return MY_ERROR;	
	}
	if(!b)
	{
		my_log("b is NULL");
		return MY_ERROR;	
	}
	if(!MY_RAT_HAS_INITED(b))
	{
		my_log("b is uninitialized");
		return MY_ERROR;	
	}

	if(!cmp_res)
	{
		my_log("cmp_res is NULL");
		return MY_ERROR;	
	}
	my_rat_strip_zero_end_nodes(a);
	my_rat_strip_zero_end_nodes(b);

	if(a->msn->data==0) //0
		a->sign=1;
	if(b->msn->data==0) //0
		b->sign=1;
	//符号不等
	if(a->sign!=b->sign)
	{
		*cmp_res=a->sign-b->sign;
		return MY_SUCC;
	}

	p=a->msn;
	q=b->msn;
	//FIXME:可能溢出
	a_digit_num=MY_RAT_DIGIT_NUM(p)+((a->used_node_num-1)<<2)+a->power;
	b_digit_num=MY_RAT_DIGIT_NUM(q)+((b->used_node_num-1)<<2)+b->power;

	if(a_digit_num>b_digit_num)
		*cmp_res=1;
	else if(a_digit_num<b_digit_num)
		*cmp_res=-1;
	else
	{
		*cmp_res=0;
		size_t min_node_num=MY_MIN(a->used_node_num,b->used_node_num);
		while(min_node_num)
		{
			if(p->data!= q->data)
			{
				*cmp_res=p->data-q->data;
				break;
			}
			p=p->prev;
			q=q->prev;
			min_node_num--;
		}

		if(*cmp_res==0)
		{
			if(a->used_node_num>b->used_node_num)
				*cmp_res=1;
			else if(a->used_node_num<b->used_node_num)
				*cmp_res=-1;
		}
	}
	if(a->sign==-1) //负数
		*cmp_res=-(*cmp_res);
	return MY_SUCC;
}

/*
 *	功能：比较有理数的绝对值大小
 *	参数：
 *		a，b：要比较的有理数
 *		cmp_res：比较结果：
 *			>0：|a|>|b|
 *			0：|a|=|b|
 *			<0：|a|<|b|
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
int my_rats_cmp_abs(my_rat *a,my_rat *b,int32_t *cmp_res)
{
	int sign_a,sign_b;
	int res;

	//检查参数
	if(!a)
	{
		my_log("a is NULL");
		return MY_ERROR;	
	}
	if(!MY_RAT_HAS_INITED(a))
	{
		my_log("a is uninitialized");
		return MY_ERROR;	
	}
	if(!b)
	{
		my_log("b is NULL");
		return MY_ERROR;	
	}
	if(!MY_RAT_HAS_INITED(b))
	{
		my_log("b is uninitialized");
		return MY_ERROR;	
	}

	if(!cmp_res)
	{
		my_log("cmp_res is NULL");
		return MY_ERROR;	
	}
		
	//保存正负号
	sign_a=a->sign;
	sign_b=b->sign;
	//设置为正数
	a->sign=1;
	b->sign=1;
	res=my_rats_cmp(a,b,cmp_res);
	//恢复
	a->sign=sign_a;
	b->sign=sign_b;
	return res;
}

/*
 *	功能：减少有理数指数部分，作为0增加到整数部分
 *	参数：
 *		n：要处理的有理数
 *		delta:指数减少量
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
static int my_rat_reduce_power(my_rat *n,ssize_t delta)
{
	my_node *p;
	unsigned int rest_power,carry;
	int32_t tmp;
	size_t i;

	//检查参数
	if(!n)
	{
		my_log("n is NULL");
		return MY_ERROR;
	}

	if(!MY_RAT_HAS_INITED(n))
	{
		my_log("n is uninitialized");
		return MY_ERROR;
	}

	if(delta==0) //指数不变
		return MY_SUCC;
	//溢出
	if(n->power-delta > n->power);  
	{
		my_log("power will overflow");
		return MY_ERROR;
	}

	//预先分配多余的节点来容纳
	if(my_rat_add_node(n,delta/4+1)!=MY_SUCC)
	{
		my_log("my_rat_add_node failed");
		return MY_ERROR;
	}

	//设置新的指数
	n->power-=delta;  
	rest_power=power10[delta%4];
	delta=delta-(delta%4);

	//先乘上剩余的指数
	if(rest_power!=1)
	{
		for(i=0,p=n->lsn,carry=0;i<n->used_node_num;i++,p=p->next)
		{
			tmp=p->data*rest_power;
			p->data=tmp%10000+carry;
			carry=tmp/10000;
		}
		if(carry)
		{
			n->msn=p;
			p->data=carry;
			n->used_node_num++;
		}
		else
			n->msn=p->prev;
	}
	n->used_node_num+=delta/4;
	while(delta)
	{
		n->lsn=n->lsn->prev;
		n->lsn->data=0;		//由于新节点是在最高节点后初始化的，从最低节点下去可能没初始化到
		delta-=4;
	}
	return MY_SUCC;
}

/*
 *	功能：有理数加法
 *	参数：
 *		a,b：要相加的有理数
 *		saving_type：结果存放方式，取值以下：
 *			MY_NEW_RES：和作为新的有理数返回
 *			MY_ARG_RES：和放在a
 *	返回值：
 *		非NULL：和
 *		NULL：出错
 */
my_rat *my_rats_add(my_rat *a,my_rat *b,my_result_saving_type saving_type)
{
	my_rat *c;
	int8_t carry;
	my_node *y,*z;
	size_t i;

	//验证参数
	if(!a)
	{
		my_log("a is NULL");
		return NULL;	
	}
	if(!MY_RAT_HAS_INITED(a))
	{
		my_log("a is uninitialized");
		return NULL;	
	}
	if(!b)
	{
		my_log("b is NULL");
		return NULL;	
	}
	if(!MY_RAT_HAS_INITED(b))
	{
		my_log("b is uninitialized");
		return NULL;	
	}

	//整理
	my_rat_strip_zero_end_nodes(a);
	my_rat_strip_zero_end_nodes(b);

	//把指数调整为一致
	if(a->power>b->power)
		my_rat_reduce_power(a,a->power-b->power);
	if(b->power>a->power)
		my_rat_reduce_power(b,b->power-a->power);

	//根据存放方式设置c
	if(saving_type==MY_ARG_RES) 
		c=a;
	else
	{
		c=my_rat_copy(NULL,a);
		if(!c)
		{
			my_log("my_rat_copy failed");
			return NULL;	
		}
	}

	//预先增加需要的节点，同时考虑加法进位时需要多一个节点
	if(my_rat_add_node(c,MY_MAX(c->used_node_num,b->used_node_num)+1-c->used_node_num)!=MY_SUCC)
	{
		my_log("my_rat_add_node failed");
		if(c!=a)
			my_rat_free(c);
		return NULL;	
	}

	z=c->lsn;
	y=b->lsn;
	//c,b符号相同,加法
	if(c->sign==b->sign)	
	{
		for(i=0;i<b->used_node_num;i++,y=y->next,z=z->next)
			z->data+=y->data;
	}
	else	//c,b符号不同，减法
	{
		if(c->sign==1)	//c正b负
		{
			for(i=0;i<b->used_node_num;i++,y=y->next,z=z->next)
				z->data-=y->data;
		}
		else	//c负b正
		{
			for(i=0;i<b->used_node_num;i++,y=y->next,z=z->next)
				z->data=y->data-z->data;
		}
		//如果c是负数，它的负号已经融合进节点了，所以我们先设置为正号
		c->sign=1;
	}
	if(c->used_node_num<b->used_node_num)
	{
		c->msn=z->prev;
		c->used_node_num=b->used_node_num;
	}

	//接下来我们要处理加法进位和减法借位
	//根据最高节点决定结果是正数还是负数
	z=c->msn;
	while(z->data==0 && z!=c->lsn)
	{
		z=z->prev;
		c->used_node_num--;
	}

	c->msn=z;
	if(c->msn->data==0)
	{
		c->sign=0;
		c->power=0;
		return c;
	}
	else if(c->msn->data>0)	//按照正数来处理
	{
		z=c->lsn;
		carry=0;
		for(i=0;i<c->used_node_num;i++,z=z->next)
		{
			z->data+=carry;
			if(z->data>=10000)	//加法导致
			{
				z->data-=10000;
				carry=1;
			}
			else if(z->data<0)	//减法导致
			{
				z->data+=10000;
				carry=-1;
			}
			else
				carry=0;
		}
		if(carry==1)
		{
			c->msn=z;
			c->used_node_num++;
		}
	}
	else	//按照负数来处理
	{
		c->sign=-1;
		z=c->lsn;
		carry=0;
		for(i=0;i<c->used_node_num;i++,z=z->next)
		{
			z->data+=carry;
			if(z->data>0)
			{
				z->data=10000-z->data;
				carry=1;
			}
			else
			{
				z->data=-z->data;
				carry=0;
			}
		}
	}

	//整理
	my_rat_strip_zero_end_nodes(c);
	return c;
}

/*
 *	功能：有理数乘法
 *	参数：
 *		a,b：要相乘的有理数
 *		saving_type：结果存放方式，取值以下：
 *			MY_NEW_RES：积作为新的有理数返回
 *			MY_ARG_RES：积放在a
 *	返回值：
 *		非NULL：和
 *		NULL：出错
 */
my_rat *my_rats_multiply(my_rat *a,my_rat *b,my_result_saving_type saving_type)
{
	my_rat *c;
	my_node *x,*y,*z,*x1,*y1,*z1;
	int32_t carry;

	//验证参数
	if(!a)
	{
		my_log("a is NULL");
		return NULL;	
	}
	if(!MY_RAT_HAS_INITED(a))
	{
		my_log("a is uninitialized");
		return NULL;	
	}
	if(!b)
	{
		my_log("b is NULL");
		return NULL;	
	}
	if(!MY_RAT_HAS_INITED(b))
	{
		my_log("b is uninitialized");
		return NULL;	
	}

	//整理
	my_rat_strip_zero_end_nodes(a);
	my_rat_strip_zero_end_nodes(b);

	//预先增加需要的节点
	c=(my_rat *)calloc(1,sizeof(*tmp));
	if(!c)
	{
		my_log("calloc failed:%s",strerror(errno));
		return NULL;			
	}

	if(my_rat_add_node(c,a->used_node_num+b->used_node_num)!=MY_SUCC)
	{
		my_log("my_rat_add_node failed");
		my_rat_free(c);
		return NULL;	
	}

	//确定正负号
	if(a->sign==b->sign)
		c->sign=1;
	else
		c->sign=-1;

	//确定指数
	c->power=a->power+b->power;

	y=b->lsn;
	z=c->lsn;
	for(i=0;i<b->used_node_num;i++)
	{
		z1=z;
		x=a->lsn;
		carry=0;
		for(j=0;j<a->used_node_num;j++)
		{
			z1->data+=x->data+y->data+carry;
			if(z1->data>=10000)
			{
				carry=z1->data/10000;
				z1->data%=10000;
			}
			else
				carry=0;
			x=x->next;
			z1=z1->next;
		}
		if(carry)
			z1->data+=carry;

		if(i==b-used_node_num-1)
		{
			if(carry)
				c->msn=z1;
			else
				c->msn=z1->prev;
		}
		z=z->next;
		y=y->next;
	}

	//计算已用节点数
	z=c->lsn;
	c->used_node_num=1;
	while(z!=c->msn)
	{
		z=z->next;
		c->used_node_num++;
	}

	//整理
	my_rat_strip_zero_end_nodes(c);
	return c;
}








/*
 *	功能：有理数乘以小整数
 *	参数:
 *		a：有理数
 *		b：乘数，必须在[-9999,9999]区间
 *		saving_type：结果存放方式，取值以下：
 *			MY_NEW_RES：积作为新的有理数返回
 *			MY_ARG_RES：积放在a
 *	返回值：
 *		非NULL：积
 *		NULL：出错
 */
my_rat *my_rat_multiply_small_int(my_rat *a,int32_t b,my_result_saving_type saving_type)
{
	my_rat *c;
	my_node *p,*q;
	size_t node_num;
	int32_t tmp,carry;

	//验证参数
	if(!a)
	{
		my_log("a is NULL");
		return NULL;	
	}
	if(!MY_RAT_HAS_INITED(a))
	{
		my_log("a is uninitialized");
		return NULL;	
	}

	if(b>9999 || b<-9999)
	{
		my_log("b is out of [-9999,9999]");
		return NULL;	
	}

	//整理
	my_rat_strip_zero_end_nodes(a);

	if(saving_type==MY_ARG_RES)
		c=a;
	else
	{
		//分配空间
		c=my_rat_copy(a,NULL);
		if(!c)
		{
			my_log("my_rat_copy failed");
			return NULL;			
		}
	}

	//乘法可能导致溢出
	if(MY_RAT_FREE_NODE_NUM(c)==0)
	{
		if(my_rat_add_node(c,1)!=MY_SUCC)
		{
			my_log("my_rat_add_node failed");
			if(c!=a)
				my_rat_free(c);
			return NULL;
		}
	}

	if(b<0)
	{
		b=-b;
		c->sign=-(c->sign);
	}

	carry=0;
	p=a->lsn;
	q=c->lsn;
	node_num=a->used_node_num;
	while(node_num)
	{
		tmp=p->data*b;
		q->data=tmp%10000+carry;
		carry=tmp/10000;
		p=p->next;
		q=q->next;
		node_num--;
	}
	if(carry)
	{
		c->msn=q;
		q->data=carry;
		c->used_node_num++;
	}
	else
		c->msn=q->prev;
	return c;
}

#ifdef cyy
/*
 *	功能：有理数除以小整数
 *	参数:
 *		a:有理数
 *		b:除数
 *		fraction：保留的小数位数
 *		round_mode：舍入模式
 *		restype:结果存放方式
 *	返回值:
 *		成功:商
 *		失败:NULL
 */
my_rat *my_divide_small_int(my_rat *a,int b,ssize_t fraction,my_round_mode round_mode,my_result_saving_type saving_type)
{
	int res=0;
	int carry=0;
	int inc_prec=0; //累积精度
	ln c,d;
	cell x,z;

	//检查参数
	if(!a)
	{
		my_log("a is NULL");
		return NULL;	
	}
	if(!MY_RAT_HAS_INITED(a))
	{
		my_log("a is uninitialized");
		return NULL;	
	}

	if(fraction<0)
	{
		my_log("invalid fraction:%zd",fraction);
		return NULL;	
	}

	//除数不能为0
	if(b==0)
	{
		my_log("b is 0:%zd",fraction);
		return NULL;
	}

	//除数必须在-200000-200000之间否则下面计算有可能溢出,因此必须把b转化成ln再操作
	//这个范围是根据INT_MAX/UNIT即2147483647/10000得出
	if(b>200000 || b<-200000)
	{
		d=ln_init(b);
		if(d==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_init fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}
		c=ln_divide(a,d,precision,mode,restype);
		if(c==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_divide fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}
		return c;
	}
	

	//去除前置0
	ln_stripleadingzero(a);

	//没指定商的精度 那就使用默认精度
	if(precision<0)
		precision=DIV_PREC;

	if(restype==newln)
	{
		c=ln_creat(ln_cellnum(a));
		if(c==NULL)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_creat fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}
		c->msd=c->lsd->lcell;
	}
	else
		c=a;

	//确定符号
	if(b>0)
		c->sign=a->sign;
	else
	{
		c->sign=-a->sign;
		b=-b;
	}

	//算出初始精度
	inc_prec=ln_pointnum(a,a->msd)-DIGIT_NUM;

	//开始计算
	carry=0;
	x=a->msd;
	z=c->msd;
	res=x->num;
	while(1)
	{
		res+=carry*UNIT;
		z->num=res/b;
		carry=res%b;
		//开始计算当前精度
		inc_prec+=DIGIT_NUM;
		if(inc_prec>precision) //已经达到需要的精度
		{
			c->lsd=z;
			//确定指数
			c->power=-inc_prec;
			break;
		}
		//必须这样标识，否则如果a和c一样，则下面会修改lsd
		if(x==a->lsd)
			x=NULL;
		if(x==NULL)
		{
			if(carry==0) //除得尽
			{
				c->lsd=z;
				//确定指数
				c->power=-inc_prec;
				break;
			}
			res=0;
		}
		else
		{
			x=x->lcell;
			res=x->num;
		}

		if(z==c->lsd) //增加节点
		{
			if(ln_addcell(c,INIT_SIZE) ==NULL)
			{
				fprintf(stderr,"[%s %d] %s error,reason: ln_addcell fail\n",__FILE__,__LINE__,__FUNCTION__);
				return NULL;
			}
			c->lsd=c->msd->hcell;
		}
		z=z->lcell;
	}

	//获取精度
	ln_fix(c,precision,mode);
	return c;
}
#endif

/*
 *	功能：计算传入整数的阶乘
 *	参数：
 *		n：计算n!
 *	返回值：
 *		非NULL：对应的阶乘
 *		NULL：出错
 */
my_rat *my_factorial(uint64_t n)
{
	uint64_t i;
	my_rat *f;

	f=my_rat_from_int64(NULL,1);
	if(!f)
	{
		my_log("my_rat_from_int64 failed");
		return NULL;
	}

	//0!=1
	if(n==0)
		return f;

	for(i=2;i<=n;i++)
	{
		if(my_rat_multiply_small_int(f,i,MY_ARG_RES)==NULL)
		{
			my_log("my_rat_multiply_small_int failed");
			my_rat_free(f);
			return NULL;
		}
	}
	return f;
}

/*
 *	功能：如果有理数是一个正整数，获取其每位上的数的和
 *	参数：
 *		n：要处理的有理数
 *		digit_sum：保存和
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
int my_rat_sum_digits(my_rat *n,uint64_t *digit_sum)
{
	size_t i,j;
	my_node *p;
	int32_t data;
	//检查参数
	if(!n)
	{
		my_log("n is NULL");
		return MY_ERROR;	
	}

	if(!MY_RAT_HAS_INITED(n))
	{
		my_log("n is uninitialized");
		return MY_ERROR;	
	}

	if(!digit_sum)
	{
		my_log("digit_sum is NULL");
		return MY_ERROR;	
	}

	if(n->sign!=1)
	{
		my_log("n isn't postive");
		return MY_ERROR;	
	}

	//剔除0
	my_rat_strip_zero_end_nodes(n);

	if(n->power<0)	//非整数
	{
		my_log("n is not interger");
		return MY_ERROR;	
	}

	*digit_sum=0;
	p=n->lsn;
	for(i=0;i<n->used_node_num;i++)
	{
		data=p->data;

		for(j=0;j<4;j++)
		{
			(*digit_sum)+=(data%10);
			data/=10;
		}
		p=p->next;
	}
	return MY_SUCC;
}

/*
 *	功能：如果有理数是一个正整数，获取其数位的数量
 *	参数：
 *		n：要处理的有理数
 *		digit_num：保存数量
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
int my_rat_digit_num(my_rat *n,uint64_t *digit_num)
{
	//检查参数
	if(!n)
	{
		my_log("n is NULL");
		return MY_ERROR;	
	}

	if(!MY_RAT_HAS_INITED(n))
	{
		my_log("n is uninitialized");
		return MY_ERROR;	
	}

	if(!digit_num)
	{
		my_log("digit_num is NULL");
		return MY_ERROR;	
	}
	
	if(n->sign!=1)
	{
		my_log("n isn't postive");
		return MY_ERROR;	
	}

	//剔除0
	my_rat_strip_zero_end_nodes(n);

	if(n->power<0)	//非整数
	{
		my_log("n is not interger");
		return MY_ERROR;	
	}

	*digit_num=(n->used_node_num-1)*4+n->power+MY_RAT_DIGIT_NUM(n->msn);
	return MY_SUCC;
}
