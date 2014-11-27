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
#include <stdio.h>

#include "my_rat.h"
#include "my_arithmetic.h"
#include "my_log.h"

static unsigned int power10[]={1,10,100,1000};

/*
 *	功能：增加指定数量的未使用节点到有理数的最高节点，并且节点初始化为0
 *	参数：
 *		n：要处理的有理数
 *		num：增加的节点数
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 *	注意：
 *		如果有理数已经存在一些未使用节点，优先初始化它们
 */
int my_rat_add_node(my_rat *n,size_t num)
{
	size_t i;
	my_node *p,*q;
	
	if(!n)
	{
		my_log("n is NULL");
		return MY_ERROR;	
	}

	if(n->used_node_num==0)
		p=n->msn;
	else
		p=n->msn->next;;
	for(i=0;i<MY_MIN(num,MY_RAT_FREE_NODE_NUM(n));i++)
	{
		p->data=0;
		p=p->next;
	}
	num-=MY_MIN(num,MY_RAT_FREE_NODE_NUM(n));
	if(num==0)
		return MY_SUCC;

	//那么我们还必须要新增节点
	if(n->total_node_num+num<n->total_node_num)
	{
		my_log("total node number will overflow");
		return MY_ERROR;	
	}

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
			q=n->msn->next;
			p->prev=n->msn;
			n->msn->next=p;
			p->next=q;
			q->prev=p;
		}
		n->total_node_num++;
		num--;
	}
	return MY_SUCC;
}

/*
 *	功能：去除有理数的两端的0数据节点
 *	参数：
 *		n：要处理的有理数
 *	返回值：
 *		无
 */
void my_rat_strip_zero_end_nodes(my_rat *n)
{
	my_node *p;
	if(!n)
	{
		my_log("n is NULL");
		return;	
	}
	if(!MY_RAT_HAS_INITED(n))
	{
		my_log("n is uninitialized");
		return;	
	}

	p=n->msn;
	while(p->data==0 && p!=n->lsn)
	{
		p=p->prev;
		n->used_node_num--;
	}
	n->msn=p;

	p=n->lsn;
	while(p->data==0 && p!=n->msn)
	{
		p=p->next;
		//FIXME:可能溢出
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
void my_rat_free(my_rat *n)
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
 *	功能：检查传入的字符串是否是合法有理数，即是否匹配 ^[-]?\d+(.\d+)?$
 * 	参数：
 *		str：要检查的字符串
 *		point：
 *			非NULL：如果存在小数点，指向小数点，否则设置为NULL
 * 	返回值：
 *		1：合法
 *		0：非法
 */
static int my_rat_str_is_valid(const char *str,const char **point)
{
	int needdigit;	//0-不需要数字 1-需要数字
	const char *_point; 
	const char *p;

	if(!str || !*str)
	{
		my_log("str is NULL or empty");
		return 0;
	}

	_point=NULL;
	needdigit=1;
	p=str;
	if(*p=='-')
		p++;
	while(*p)
	{
		if(*p=='.')
		{
			if(p==str)
			{
				my_log("invalid char . at index 0 of str %s",str);
				return 0;
			}
			else if(!_point)
			{
				_point=p;
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
		*point=_point;
	return 1;
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
my_rat *my_rat_from_str(my_rat *n,const char *str)
{
	const char *lastdigit,*point;
	size_t len;
	int i;
	my_rat *m;
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

	//清除小数点后结尾的0
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
	else //整数结尾的0并入指数
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
	if(my_rat_add_node(m,((lastdigit-str+1)/4)+1)!=MY_SUCC)
	{
		my_log("my_rat_add_node failed");
		if(!n)
			my_rat_free(m);
		return NULL;
	}

	//nnnn.mmmm
	if(point)
		m->power=point-lastdigit;

	p=m->lsn;
	i=0;
	m->used_node_num=1;
	while(lastdigit>=str) //从最低位向上构造digit部分
	{
		if(lastdigit!=point)	//跳过小数点
		{
			p->data+=(*lastdigit-'0')*power10[i];
			i++;
			if(i==4)
			{
				i=0;
				if(lastdigit>str)
				{
					p=p->next;
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
my_rat *my_rat_from_int64(my_rat *n,int64_t num)
{
	my_rat *m;
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

	if(my_rat_add_node(m,MY_INT64_MIN_STR_LEN/4+1)!=MY_SUCC)
	{
		my_log("my_rat_add_node failed");
		if(!n)
			my_rat_free(m);
		return NULL;
	}

	p=m->lsn;

	if(num>=0)
		m->sign=1;
	else
	{
		m->sign=-1;
		if(num==INT64_MIN)	//这个要特殊对待
		{
			p->data=1;
			num=INT64_MAX;
		}
		else
			num=-num;
	}

	//处理lsn，由于对INT64_MIN进行特殊处理，这边要+=
	p->data+=num%10000;
	num/=10000;
	m->used_node_num++;

	while(num)
	{
		p=p->next;
		p->data=num%10000;
		num/=10000;
		m->used_node_num++;
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
char *my_rat_to_str(my_rat *n)
{
	int i;
	size_t j;
	char *p,*str;
	size_t len;
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
	my_rat_strip_zero_end_nodes(n);

	if(n->power>=0) //整数的显示
	{
		len=(n->used_node_num<<2)+n->power+2;
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
		for(node=n->msn,j=0;j<n->used_node_num;j++)
		{

			i=3;
			if(j==0) //如果是第一个节点，就去掉前置0
			{
				while(node->data<power10[i])
					i--;
			}
			while(i>=0)
			{
				*p++=(node->data/power10[i])%10+'0';
				i--;
			}
			node=node->prev;
		}

		memset(p,'0',n->power);
		p[n->power]='\0';
	}
	else //小数的显示
	{
		ssize_t power;
		char *lastdigit;

		power=-n->power;

		//先计算需要的空间
		len=MY_MAX(power,(n->used_node_num)<<2)+4;

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
		for(node=n->lsn,j=0;j<n->used_node_num;j++)
		{
			i=0;
			while(i<4)
			{
				if(j==n->used_node_num-1 && power10[i]>node->data)
					break;
				*p--=(node->data/power10[i])%10+'0';
				i++;
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

		//剔除结尾0
		while((*lastdigit=='0' || *lastdigit=='.') && lastdigit!=p+1)
			lastdigit--;
		*(lastdigit+1)='\0';

		//剔除结尾0后可能导致0
		if(strcmp(p+1,"0")!=0)
		{
			if(n->sign==-1)
				*p--='-';
		}

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
 *		dest：目的有理数，取以下值：
 *			NULL：返回新建的副本
 *			非NULL：副本保存于此
 *		src：源有理数
 *	返回值：
 *		非NULL：返回副本
 *		NULL：出错
 */
my_rat *my_rat_copy(my_rat *dest,my_rat *src)
{
	my_rat *tmp;
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

	//整理
	my_rat_strip_zero_end_nodes(src);

	//预先增加足够节点
	if(my_rat_add_node(tmp,src->used_node_num)!=MY_SUCC)
	{
		my_log("my_rat_add_node failed");
		if(!dest)
			my_rat_free(tmp);
		return NULL;
	}

	tmp->sign=src->sign;
	tmp->power=src->power;
	p=tmp->lsn;
	q=src->lsn;
	for(i=0;i<src->used_node_num;i++)
	{
		p->data=q->data;
		p=p->next;
		q=q->next;
	}
	tmp->msn=p->prev;
	tmp->used_node_num=src->used_node_num;
	return tmp;
}


/*
 *	功能：舍入有理数到指定小数位数
 *	参数：
 *		n：要处理的有理数
 *		fraction：小数位数
 *		round_mode：舍入模式
 *	返回值：
 *		MY_SUCC：成功
 *		MY_ERROR：出错
 */
int my_rat_round(my_rat *n,ssize_t fraction,my_round_mode round_mode)
{
	my_node *p;
	size_t i;
	ssize_t cur_fraction;

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

	if(fraction<0)
	{
		my_log("invalid fraction:%zd",fraction);
		return MY_ERROR;
	}

	if(round_mode!=MY_TRUNC)	//目前仅支持截断
	{
		my_log("invalid round mode:%d",(int)round_mode);
		return MY_ERROR;
	}

	//整理
	my_rat_strip_zero_end_nodes(n);

	if(n->power>=0)		//非小数
		return MY_SUCC;

	cur_fraction=-(n->power);
	p=n->lsn;
	for(i=0;i<n->used_node_num;i++)
	{
		if(cur_fraction-3<=fraction)
			break;
		cur_fraction-=4;
		p=p->next;
	}

	if(cur_fraction-3>fraction)	//这意味着当前的第一个有效数字的小数位数大于要舍入的小数位数，那只能设置为0
	{
		if(my_rat_from_int64(n,0)==NULL)
		{
			my_log("my_rat_from_int64 failed");
			return MY_ERROR;
		}
		return MY_SUCC;
	}

	if(cur_fraction>fraction)	
		p->data-=(p->data%power10[cur_fraction-fraction]);

	if(p!=n->lsn)
	{
		do
		{
			p=p->prev;
			p->data=0;
		}
		while(p!=n->lsn);
	}
	//整理
	my_rat_strip_zero_end_nodes(n);
	return MY_SUCC;
}

#ifdef cyy






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
	if(ln_checknull(n)!=0)
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
						if(my_rat_add_node(n,1)!=MY_SUCC) //分配失败
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
