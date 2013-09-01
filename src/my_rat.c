/*
 *	程序名：my_rat.c
 *	作者：陈源源
 *	日期：2013-02-09
 *	功能：有理数相关定义和函数
 */

#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>

#include "my_rat.h"
#include "my_arithmetic.h"
#include "my_log.h"

static unsigned int power10[]={1,10,100,1000};

/*
 *	功能：增加有理数的节点
 *	参数：
 *		n：要处理的有理数
 *		num：增加的节点数
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
int my_rat_add_node(my_rat* n,size_t num)
{
	my_node *p,*q;
	
	//开始增加节点
	while(num)
	{
		p=(my_node*)calloc(1,sizeof(*p));
		if(!p)
		{
			my_log("calloc failed:%s",strerror(errno));
			return MY_ERROR;
		}

		if(!n->lsn)
		{
			n->lsn=p;
			n->msn=p;
			p->next=p;
			p->prev=p;
		}
		else
		{
			q=n->lsn->prev;
			p->next=n->lsn;
			n->lsn->prev=p;
			p->prev=q;
			q->next=p;
		}
		n->total_node_num++;
		num--;
	}
	return MY_SUCC;
}


/*
 *	功能：检查传入的字符串是否是合法有理数，即是否匹配 ^[-]?\d+(.\d+)?$
 * 	参数：
 *		str：要检查的字符串
 *		point：
 *			非NULL：如果存在小数点，指向小数点，否则设置为NULL
 * 	返回值：
 *		1：合法
 *		0：非法
 */
static int my_rat_str_is_valid(const char* str,const char** point)
{
	int needdigit;	//0-不需要数字 1-需要数字
	const char* point2; 
	const char* p;

	if(!str || !*str)
	{
		my_log("str is NULL or empty");
		return 0;
	}

	point2=NULL;
	needdigit=1;
	p=str;
	while(*p)
	{
		if(*p=='-')
		{
		       	if(p!=str)
			{
				my_log("invalid char - at index %zd of str %s",p-str,str);
				return 0;
			}
		}
		else if(*p == '.')
		{
			if(p==str)
			{
				my_log("invalid char . at index 0 of str %s",str);
				return 0;
			}
			else if(!point2)
			{
				point2=p;
				needdigit=1;
			}
			else //已经有了
			{
				my_log("invalid char . at %zd of str %s",p-str,str);
				return 0;
			}
		}
		else if(!isdigit(*p))
		{
			my_log("invalid char %c at %zd of str %s",*p,p-str,str);
			return 0;
		}
		else
			needdigit=0;
		p++;
	}

	if(needdigit)
	{
		my_log("lack of [0-9] of str %s",str);
		return 0;
	}
	if(point)
		*point=point2;
	return 1;
}

/*
 *	功能：把有理数的前置0去掉
 *	参数：
 *		n：要处理的有理数
 *	返回值：
 *		无
 */
void my_rat_strip_leading_zero(my_rat* n)
{
	my_node *p;
	p=n->msn;
	while(p->data==0 && p != n->lsn)
	{
		p=p->prev;
		n->used_node_num--;
	}
	n->msn=p;
	return;
}

/*
 *	功能：把有理数的后置0去掉
 *	参数：
 *		n：要处理的有理数
 *	返回值：
 *		无
 */
void my_rat_strip_ending_zero(my_rat* n)
{
	my_node *p;
	p=n->lsn;
	while(p->data==0 && p != n->msn)
	{
		p=p->next;
		n->power+=4;
		n->used_node_num--;
	}
	n->lsn=p;
	return;
}


/*
 *	功能：释放有理数
 *	参数：
 *		n：要释放的有理数
 *	返回值：
 * 		无
 */
void my_rat_free(my_rat* n)
{
	my_node *p;
	if(!n)
		return;	

	//释放节点
	p=n->lsn->next;
	while(p!=n->lsn)
	{
		p=p->next;
		free(p->prev);
	}
	free(p);
	free(n);
	return;
}

/*
 * 	功能：把字符串转换为my_rat
 * 	参数：
 *		n：	
 *			NULL：新建有理数并返回
 *			非NULL：保存有理数于此
 *		str：要转换的字符串
 *	返回值：
 * 		非NULL：返回生成的有理数
 * 		NULL：出错
 */
my_rat* my_rat_from_str(my_rat* n,const char* str)
{
	const char *lastdigit,*point;
	size_t len;
	int bases[]={1,10,100,1000};
	int i;
	my_rat* m;
	my_node *p;
		
	//str格式验证
	if(!my_rat_str_is_valid(str,&point))
	{
		my_log("invalid str %s",str);
		return NULL;
	}

	if(n)
	{
		MY_RAT_INIT(n);
		m=n;
	}
	else
	{
		//分配空间
		m=(my_rat *)calloc(1,sizeof(*m));
		if(!m)
		{
			my_log("calloc failed:%s",strerror(errno));
			return NULL;			
		}
	}

	//设置符号
	if(*str=='-')
	{
		m->sign=-1;
		str++;
	}
	else
		m->sign=1;

	len=strlen(str);
	lastdigit=str+len-1;	

	//清除起始的0
	while(*str=='0'&& (!point || str<point-1) && str < lastdigit)
		str++;
	
	//清除小数点后终结的0
	if(point)
	{
		while(*lastdigit=='0')
			lastdigit--;
		if(lastdigit==point) //小数点后全是0 其实是整数
		{
			lastdigit--;
			point=NULL;
		}
	}
	else //后置0并入指数
	{
		while(*lastdigit=='0' && lastdigit > str)
		{
			m->power++;
			lastdigit--;
		}
	}


	//处理-0
	if(m->sign==-1 && lastdigit==str && *str=='0')
		m->sign=1;
	//增加节点
	size_t node_num=((lastdigit-str+1)>>2)+1;
	if(MY_RAT_FREE_NODE_NUM(m) < node_num)
	{
		if(my_rat_add_node(m,node_num-MY_RAT_FREE_NODE_NUM(m)) !=MY_SUCC)
		{
			my_log("my_rat_add_node failed");
			if(!n)
				my_rat_free(m);
			return NULL;
		}
	}

	//nnnn.mmmm
	if(point)
		m->power=point-lastdigit;

	p=m->lsn;
	p->data=0;
	i=0;
	m->used_node_num=1;
	while(lastdigit>=str) //从最低位向上构造digit部分
	{
		if(lastdigit!=point)
		{
			p->data+=(*lastdigit-'0')*bases[i];
			i++;
			if(i==4)
			{
				i=0;
				if(lastdigit > str)
				{
					p=p->next;
					p->data=0;
					m->used_node_num++;
				}
			}
		}
		lastdigit--;
	}
	m->msn=p;

	return m;
}

/*
 * 	功能：把int64_t转换为有理数
 * 	参数：
 *		n：	
 *			NULL：新建有理数并返回
 *			非NULL：保存有理数于此
 *		num：要处理的数字
 *	返回值：
 * 		非NULL：返回生成的有理数
 * 		NULL：出错
 */
my_rat* my_rat_from_int64(my_rat* n,int64_t num)
{
	my_rat* m;
	my_node *p;
		
	if(n)
	{
		MY_RAT_INIT(n);
		m=n;
	}
	else
	{
		//分配空间
		m=(my_rat *)calloc(1,sizeof(*m));
		if(!m)
		{
			my_log("calloc failed:%s",strerror(errno));
			return NULL;			
		}
	}

	size_t node_num=MY_INT64_MIN_STR_LEN>>2;
	if(MY_RAT_FREE_NODE_NUM(m) < node_num)
	{
		if(my_rat_add_node(m,node_num-MY_RAT_FREE_NODE_NUM(m)) !=MY_SUCC)
		{
			my_log("my_rat_add_node failed");
			if(!n)
				my_rat_free(m);
			return NULL;
		}
	}
	
	p=m->lsn;
	m->used_node_num=1;

	if(num>=0)
	{
		m->sign=1;
		p->data=0;
	}
	else
	{
		m->sign=-1;
		if(num==INT64_MIN)
		{
			p->data=1;
			num=INT64_MAX;
		}
		else
		{
			p->data=0;
			num=-num;
		}
			
	}

	while(num)
	{
		p->data+=num%10000;
		num/=10000;
		if(num)
		{
			p=p->next;
			p->data=0;
			m->used_node_num++;
		}
	}
	m->msn=p;

	return m;

}


/*
 *	功能：把my_rat转换为字符串
 *	参数：
 *		n：要处理的有理数
 *	返回值：
 * 		非NULL：字符串
 * 		NULL：出错
 */
char* my_rat_to_str(my_rat* n)
{
	int bases[]={1000,100,10,1};
	int i;
	size_t j;
	char *p,*str;
	size_t len,node_num;
	my_node *node;
		
	//检查参数
	if(!n)
	{
		my_log("n is NULL");
		return NULL;	
	}
	if(!MY_RAT_HAS_INITED(n))
	{
		my_log("n is uninitialized");
		return NULL;	
	}

	//剔除0
	my_rat_strip_leading_zero(n);
	my_rat_strip_ending_zero(n);

	node_num=MY_RAT_USED_NODE_NUM(n);
	if(n->power>=0) //整数的显示
	{
		len=(node_num<<2)+n->power+2;
		//分配空间
		str=(char*)malloc(len);
		if(str==NULL)
		{
			my_log("malloc failed:%s",strerror(errno));
			return NULL;			
		}

		p=str;
		//打印符号
		if(n->sign==-1)
			*p++='-';

		//打印有效数字
		for(node=n->msn,j=0;j<node_num;j++)
		{
			i=0;
			if(j==0)
			{
				while(node->data<bases[i] && i<3)
					i++;
			}
			while(i<4)
			{
				*p++=(node->data/bases[i])%10+'0';
				i++;
			}
			node=node->prev;
		}

		if(n->power>0)
		{
			memset(p,'0',n->power);
			p[n->power]='\0';
		}
		else
			*p='\0';
	}
	else //小数的显示
	{
		ssize_t power;
		char *lastdigit;

		power=-n->power;

		//先计算需要的空间
		len=MY_MAX(power,MY_RAT_USED_NODE_NUM(n)<<2)+5;

		//分配空间
		str=(char*)malloc(len);
		if(!str)
		{
			my_log("malloc failed:%s",strerror(errno));
			return NULL;			
		}

		p=str+len-2;  //p指向字符串尾部
		lastdigit=p;
		//打印有效数字
		for(node=n->lsn,j=0;j<node_num;j++)
		{
			i=3;
			while(i>=0)
			{
				if(j==node_num-1 && bases[i]>node->data)
					break;
				*p--=(node->data/bases[i])%10+'0';
				i--;
				power--;
				if(power==0)
					*p--='.';
			}
			node=node->next;
		}

		//剩下的指数用0填充
		if(power>0)
		{
			p-=power;
			memset(p+1,'0',power);
			*p--='.';
		}

		if(*(p+1)=='.')
			*p--='0';
		if(n->sign==-1)
			*p--='-';

		//整理
		while(*lastdigit=='0')
			lastdigit--;
		*(lastdigit+1)='\0';
		if(str!=p+1)
		{
			len=lastdigit-p;
			memmove(str,p+1,len);
			str[len]='\0';
		}
	}
	return str;
}

/*
 *	功能：复制有理数b
 *	参数：
 *		src：源有理数
 *		dest：目的有理数，取以下值：
 *			NULL：返回新建的副本
 *			非NULL：副本保存于此
 *	返回值：
 *		非NULL：返回副本
 *		NULL：出错
 */
my_rat* my_rat_copy(my_rat* src,my_rat* dest)
{
	my_rat* tmp;
	my_node *p,*q;
	size_t i;

	//验证参数
	if(!src)
	{
		my_log("src is NULL");
		return NULL;	
	}

	if(!MY_RAT_HAS_INITED(src))
	{
		my_log("src is uninitialized");
		return NULL;
	}

	if(dest)
	{
		MY_RAT_INIT(dest);
		tmp=dest;
	}
	else
	{
		//分配空间
		tmp=(my_rat *)calloc(1,sizeof(*tmp));
		if(!tmp)
		{
			my_log("calloc failed:%s",strerror(errno));
			return NULL;			
		}
	}

	if(MY_RAT_FREE_NODE_NUM(tmp) < MY_RAT_USED_NODE_NUM(src))
	{
		if(my_rat_add_node(tmp,MY_RAT_USED_NODE_NUM(src)-MY_RAT_FREE_NODE_NUM(tmp)) !=MY_SUCC)
		{
			my_log("my_rat_add_node failed");
			if(!dest)
				my_rat_free(tmp);
			return NULL;
		}
	}

	tmp->sign=src->sign;
	tmp->power=src->power;
	p=tmp->lsn;
	q=src->lsn;
	for(i=0;i<MY_RAT_USED_NODE_NUM(src);i++)
	{
		p->data=q->data;
		p=p->next;
		q=q->next;
	}
	tmp->msn=p->prev;
	tmp->used_node_num=MY_RAT_USED_NODE_NUM(src);
	return tmp;
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
int my_rat_reduce_power(my_rat *n,size_t delta)
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

	if(delta==0) //指数不变
		return MY_SUCC;
	//溢出
	if(n->power-delta > n->power);  
	{
		my_log("power will overflow");
		return MY_ERROR;
	}


	//设置新的指数
	n->power-=delta;  

	//先乘上剩余部分的0
	if(my_rat_multiply_small_int(n,power10[delta%4],MY_ARG_RES)==NULL)
	{
		my_log("my_rat_multiply_small_int failed");
		return MY_ERROR;
	}

	while(delta>=4)
	{
		if(n->used_node_num!=n->total_node_num) //还有多余的节点，利用它
		{
			n->lsn=n->lsn->prev;
			n->lsn->data=0;
			n->used_node_num++;
			delta-=4;
		}
		else //一次性分配剩下的节点
		{
			if(my_rat_add_node(n,delta/4)!=MY_SUCC)
			{
				my_log("my_rat_add_node failed");
				return MY_ERROR;
			}
			n->lsn=n->msn->next;
			break;
		}
	}
	return MY_SUCC;
}








#ifdef cyy







/*
 * 功能：获取ln整数部分的十进制位数
 * 副功能：使用ln_stripleadingzero()把ln整数部分前置0去掉
 * 参数：
 *	n:要计算的ln
 *	q:截止节点
 *	返回值：
 * 	成功:返回十进制位数
 * 	失败:返回-1
 */
int ln_digitnum(ln n)
{
	int i,j;
	cell p;

	//检查参数
	if(ln_checknull(n) !=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}

	//去掉前置0,避免计算出错
	ln_stripleadingzero(n);

	//计算有效节点总数
	i=ln_cell_num(n);
	if(i==-1)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_cell_num fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}
	i=(i-1)*4;

	//计算最高节点的位数
	p=n->msn;
	j=1;
	while(j<=p->data)
	{
		i++;
		j*=10;
	}
	return i;
}


/*
 * 参数要求:n必须去掉前置0
 * 功能：获取节点的小数点位数,如果是在小数点前面,则取负值
 * 参数：
 *	n:要计算的ln
 *	q:节点
 *	返回值：返回小数点位数
 */
int ln_pointnum(ln n,cell q)
{
	int pointnum;
	cell p;
	//检查参数
	if(ln_checknull(n) !=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -1;	
	}
	//检查n的格式
	if(n->msn->data==0)
	{
		if(n->msn==n->lsn)  //这个ln是0
			return 0;
		else //前置0没有去掉,报错
		{
			fprintf(stderr,"[%s %d] %s error,reason: addr num %d\n",__FILE__,__LINE__,__FUNCTION__,(int)&(n->msn->data));
			fprintf(stderr,"[%s %d] %s error,reason: n format error,n has leading zeros\n",__FILE__,__LINE__,__FUNCTION__);
			return -1;	
		}
	}
	pointnum=-n->power;
	p=n->lsn;
	while(p!=q)
	{
		p=p->hcell;
		pointnum-=DIGIT_NUM;
	}
	return pointnum;
}




/*
 * 功能：比较ln a和ln b的绝对值大小
 * 副功能：使用ln_stripleadingzero()把a,b整数部分前置0去掉
 * 参数：
 * 	a,b:待比较的ln
 *	返回值：
 * 	0: |a|=|b|
 * 	1: |a|>|b|
 * 	-1: |a|<|b|
 * 	-2: 出错
 */
int ln_cmp_abs(ln a,ln b)
{
	int res;
	int sign_a,sign_b;

	//检查参数
	if(ln_checknull(a)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -2;	
	}
	if(ln_checknull(b)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -2;	
	}

	//保存正负号
	sign_a=a->sign;
	sign_b=b->sign;
	//设置为正数
	a->sign=1;
	b->sign=1;
	res=ln_cmp(a,b);
	//恢复
	a->sign=sign_a;
	b->sign=sign_b;
	return res;
}

/*
 * 功能：比较ln a和int b的大小
 * 副功能：使用ln_stripleadingzero()把a整数部分前置0去掉
 * 参数：
 * 	a:待比较的ln
 * 	b:待比较的int
 *	返回值：
 * 	0: a=b
 * 	1: a>b
 * 	-1: a<b
 * 	-2: 出错
 */
int ln_cmp_int(ln a,int b)
{
	int res;
	ln c;
	//检查参数
	if(ln_checknull(a)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -2;	
	}
	c=ln_init(b);
	if(c==NULL)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_init fail\n",__FILE__,__LINE__,__FUNCTION__);
		return -2;	
	}
	res=ln_cmp(a,c);
	ln_free(c);
	return res;
}

/*
 * 功能：检查str的格式是否为[-]?\d+(.\d+)? 
 * 参数：
 *	str:要检查的字符串
 *	返回值：
 * 	成功:0
 * 	失败:-1
 */
int ln_checkstr(const char* str)
{
	int haspoint;	//0-无小数点 1-有小数点
	int needdigit;	//0-不需要数字 1-需要数字
	const char* p;
	if(!str || !*str)
	{
		my_log("str is NULL or empty");
		return MY_ERROR;
	}

	p=str;
	haspoint=0;
	needdigit=1;
	while(*p)
	{
		if(*p=='-')
		{
			if(p!=str)
			{
				my_log("invalid char - at index %zd of str %s",p-str,str);
				return MY_ERROR;
			}
		}
		else if(*p == '.')
		{
			if(p==str)
			{
				my_log("invalid char . at index 0 of str %s",str);
				return MY_ERROR;
			}
			else if(haspoint==0)
			{
				haspoint=1;
				needdigit=1;
			}
			else //已经有了
			{
				my_log("invalid char . at %zd of str %s",p-str,str);
				return MY_ERROR;
			}
		}
		else if(!isdigit(*p))
		{
			my_log("invalid char %c at %zd of str %s",*p,p-str,str);
			return MY_ERROR;
		}
		else
			needdigit=0;
		p++;
	}

	if(needdigit)
	{
		my_log("lack of digit of str %s",str);
		return MY_ERROR;
	}
	return LN_SUCC;
}

/*
 * 功能：把ln的前置0去掉,不影响ln的数值,只是方便一些操作
 * 参数：
 *	n:要处理的ln
 *	返回值：
 * 	无
 */
void ln_stripleadingzero(ln n)
{
	cell p;
	if(ln_checknull(n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;
	}
	p=n->msn;
	while(p->data==0 && p != n->lsn)
		p=p->lcell;
	n->msn=p;
	return;
}

/*
 * 功能：把ln的后置0节点去掉,增加指数
 * 参数：
 *	n:要处理的ln
 *	返回值：
 * 	无
 */
void ln_stripendingzero(ln n)
{
	cell p;
	if(ln_checknull(n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;
	}
	p=n->lsn;
	while(p->data==0 && p != n->msn)
	{
		p=p->hcell;
		n->power+=DIGIT_NUM;
	}
	n->lsn=p;
	return;
}

/*
 * 功能：获取ln的整数部分的结尾0的个数
 * 副功能：使用ln_stripleadingzero()把ln整数部分前置0去掉
 * 参数：
 *	n:要处理的ln
 *	返回值：
 * 	成功:返回个数
 * 	失败:-1
 */
int ln_endingzeronum(ln n)
{
	cell p;
	int i,j;

	//去除前置0
	ln_stripleadingzero(n);
	if(n->msn->data==0)  //整数部分是0
		return 0;

	i=0;
	p=n->lsn;
	while(1)
	{
		if(p->data==0)
			i+=DIGIT_NUM;
		else
		{
			j=10;
			while(p->data%j==0)
			{
				i++;
				j*=10;
			}
			return i;
		}
		p=p->hcell;
	}
	return i;
}

/*
 * 功能：把ln取精度
 * 副功能：把ln前置0和后置0去掉
 * 参数：
 *	n:要处理的ln
 *	prevcision:所需精度(保留的小数位数)
 *	mode:指定截断或者四舍五入
 *	返回值：
 * 	成功:返回ln
 * 	失败:NULL
 */
ln ln_fix(ln n,int prevcision,divide_mode mode)
{
	int pointnum;
	int power;
	cell p;
	//验证参数
	if(ln_checknull(n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}
	//精度参数有误
	if(prevcision<0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: prevcision error\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}
	//去除前置0
	ln_stripleadingzero(n);
	//去除后置0
	ln_adjustpower(n,ln_endingzeronum(n));
	//由于上一个函数可能导致前置0,再次去除
	ln_stripleadingzero(n);

	//获取最低节点的小数点位数
	pointnum=ln_pointnum(n,n->lsn);
	if(pointnum==-1)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_pointnum fail\n",__FILE__,__LINE__,__FUNCTION__);
		return NULL;
	}

	//大于pointnum,直接返回
	if(prevcision >=pointnum)
		return n;

	//只存在一个节点
	if(n->lsn==n->msn)
	{
		p=n->lsn;
		power=power10(pointnum-prevcision);
		if(mode==trunc_res) //截断
			p->data-=p->data%power;
		else
		{
			if(p->data%power>=5*power/10)
				p->data=(p->data/power+1)*power;
			else
				p->data-=p->data%power;
		}

	}
	else
	{
		p=n->lsn;
		while(pointnum>prevcision)
		{
			p=p->hcell;
			pointnum-=DIGIT_NUM;
		}
		if(pointnum==prevcision) //现在p所指的节点精度就是prevcision
		{
			if(mode==round_res && p->lcell->data>=10000/2) //四舍五入
				p->data++;
		}
		else
		{
			p=p->lcell;
			power=power10(DIGIT_NUM+pointnum-prevcision);
			if(mode==trunc_res) //截断
			{
				p->data-=p->data%power;
			}
			else
			{
				if(p->data%power>=5*power/10)
					p->data=(p->data/power+1)*power;
				else
					p->data-=p->data%power;
			}
		}
		//p后面有效节点被舍去,因此重置为0
		if(p!=n->lsn)	
		{
			do	
			{
				p=p->lcell;
				p->data=0;
			}
			while(p!=n->lsn);
		}
	}
	//有可能四舍五入导致溢出,需要做调整
	p=n->lsn;
	while(1)
	{
		if(p->data>=10000)
		{
			p->data-=10000;
			if(p==n->msn)
			{
				if(ln_addcell(n,INIT_SIZE) !=LN_SUCC)
				{
					fprintf(stderr,"[%s %d] %s error,reason: ln_addcell fail\n",__FILE__,__LINE__,__FUNCTION__);
					return NULL;
				}
				n->msn=p->hcell;
				n->msn->data=1;
				break;
			}
			else
			{
				p=p->hcell;
				p->data+=1;
			}

		}
		else
			break;
	}
	return n;
}


/*
 * 功能：把字符串转换为ln
 * 参数：
 *	n:要赋值的ln,如果为NULL则构造一个新ln
 *	str:要转换的字符串
 *	返回值：
 * 	成功:返回ln
 * 	失败:NULL
 */
ln str2ln(ln n,const char* str)
{
	int i;
	const char *lastdigit,*point;
	cell p;

	//创建一个ln
	if(n==NULL)
	{
		n=ln_creat(INIT_SIZE);
		if(n==NULL)
		{
			my_log("ln_creat failed");
			return NULL;
		}
	}
	else
	{
		if(ln_checknull(n) !=0)
		{
			my_log("ln_checknull failed");
			return NULL;
		}
	}

	//str格式验证
	if(ln_checkstr(str) !=0)
	{
		my_log("ln_checkstr failed");
		return NULL;
	}

	//设置符号
	if(*str=='-')
		n->sign=-1;
	else
		n->sign=1;

	//设置变量
	lastdigit=str+strlen(str)-1;	
	point=strstr(str,".");

	//清除符号
	if(*str=='+' || *str=='-')
		str++;
	//清除起始的0
	while(str != lastdigit && *str=='0')
		str++;
	//碰到0.XXX的情况,回退
	if(*str=='.')
		str--;

	//清除小数点后终结的0
	if(point !=NULL) //存在小数点	
	{
		while(*lastdigit=='0')
			lastdigit--;
		if(*lastdigit=='.') //小数点后全是0 其实是整数
		{
			lastdigit--;
			point=NULL;
		}
	}

	//求出指数
	n->power=0;
	if(point !=NULL) //存在小数点	
		n->power=point-lastdigit;
	else if(*lastdigit=='0' && lastdigit !=str) //以0结尾的整数 (不是0本身)
	{
		while(*lastdigit=='0')  
		{
			lastdigit--;
			n->power++;
		}
	}

	i=1;
	p=n->lsn;
	p->data=0;

	if(point !=NULL) //存在小数点	
	{
		//跳过初始0,直奔有效数字
		if(*str=='0' && *(str+1)=='.')
			str+=2;
		while(*str =='0')
			str++;
	}

	while(lastdigit>=str) //从最低位向上构造digit部分
	{
		if(*lastdigit!='.')
		{
			p->data+=(*lastdigit-'0')*i;
			lastdigit--;
			if(i*10==10000)
			{
				if(lastdigit>=str)
				{
					i=1;
					if(p->hcell==n->lsn) //节点已经用完,分配新节点
					{
						if(ln_addcell(n,(lastdigit-str+1)/DIGIT_NUM+1) !=LN_SUCC)
						{
							fprintf(stderr,"[%s %d] %s error,reason: ln_addcell fail\n",__FILE__,__LINE__,__FUNCTION__);
							return NULL;
						}
					}
					p=p->hcell;
					p->data=0;
				}
				else
					break;
			}
			else 
				i*=10;
		}
		else
			lastdigit--;
	}
	n->msn=p;
	return n;
}


/*
 * 功能：打印ln的值
 * 参数：
 *	n:要输出的ln
 *	返回值：
 * 	无
 */
void ln_output(ln n)
{
	char* p;
	//检测参数
	if(ln_checknull(n)!=0)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln_checknull fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;	
	}
	p=ln2str(n);
	if(!p)
	{
		fprintf(stderr,"[%s %d] %s error,reason: ln2str fail\n",__FILE__,__LINE__,__FUNCTION__);
		return;			
	}
	puts(p);
	free(p);
	return;			
}

/*
 *	功能：调整指数部分
 *	参数：
 *		n：要处理的有理数
 *		inc_power:指数增量 >0 增加指数,把整数部分结尾的0去掉 <0 减少指数,增加整数部分结尾的0
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
int my_rat_change_power(my_rat *n,ssize_t inc_power)
{
	int a;
	int res,carry;
	int zeronum;
	my_node *p;

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

	if(inc_power==SSIZE_T_MIN)
	{
		my_log("inc_power is SSIZE_T_MIN");
		return MY_ERROR;
	}

	if(inc_power==0) //指数不变
		return MY_SUCC;

	if(inc_power<0) //减少指数
	{
		//设置新的指数
		n->power+=inc_power;  
		//转正
		inc_power=-inc_power;  

		//先乘上剩余部分的0
		a=power10[inc_power%4];
		p=n->lsn;
		res=0;
		carry=0;
		while(1)
		{
			res=a*p->data+carry;
			p->data=res%10000;
			carry=res/10000;
			if(p==n->msn)
			{
				if(carry>0) //存在进位
				{
					if(n->used_node_num==n->total_node_num) //节点不够
					{
						if(my_rat_add_node(n,1) !=MY_SUCC) //分配失败
						{
							my_log("my_rat_add_node failed");
							return MY_ERROR;
						}
					}
					p->next->data=carry;
					n->msn=p->next;
				}
				break;
			}
			p=p->next;
		}

		inc_power-=inc_power%DIGIT_NUM;
		while(inc_power>0)
		{
			if(n->used_node_num!=n->total_node_num) //还有多余的节点，利用它
			{
				n->lsn=n->lsn->prev;
				n->lsn->data=0;
				n->used_node_num++;
				inc_power-=DIGIT_NUM;
			}
			else //一次性分配剩下的节点
			{
				if(my_rat_add_node(n,inc_power/4)==LN_SUCC)
				{
					my_log("my_rat_add_node failed");
					return MY_ERROR;
				}
				n->lsn=n->msn->next;
				break;
			}
		}
	}
	else
	{
		//获取结尾0的个数
		zeronum=ln_endingzeronum(n);
		if(zeronum==-1)
		{
			fprintf(stderr,"[%s %d] %s error,reason: ln_endingzeronum fail\n",__FILE__,__LINE__,__FUNCTION__);
			return NULL;
		}
		//不能去掉这么多0
		if(inc_power>zeronum)
		{
			fprintf(stderr,"[%s %d] %s error,reason: inc_power too large (%d)\n",__FILE__,__LINE__,__FUNCTION__,inc_power);
			return NULL;
		}
		//设置新的指数
		n->power+=inc_power;  

		//先处理结尾的0节点
		p=n->lsn;
		while(p->data==0 && inc_power>=DIGIT_NUM)
		{
			inc_power-=DIGIT_NUM;
			p=p->hcell;
		}
		n->lsn=p;

		//除以剩下的0
		if(inc_power==0)
			return n;

		a=power10(inc_power);
		p=n->msn;
		res=0;
		carry=0;
		while(1)
		{
			res=p->data+carry*10000;
			p->data=res/a;
			carry=res%a;
			if(p==n->lsn)
				break;
			p=p->lcell;
		}
	}
	return n;
}
#endif
