#include <stdio.h>
#include "plat_include/ring.h"
#include "ucos_ii.h"
/*
@brief 环形缓冲区类型声明
*/
struct RingBuffer{
unsigned char* Ring;
int len;
unsigned char* Ring_Head;
unsigned char* Ring_Tail;
int count;
};
/*
@brief 环形缓冲区实例列表
*/
static struct RingBuffer RingBufferList[RING_COUNT];
/*
@brief 下一个可用的环形缓冲区实例
*/
static int index = 0;

/*
@brief 初始化环形缓冲区
@param buf 预分配的缓冲区空间
@param len 缓冲区长度
@return 成功缓冲区指针, 否则NULL(可调整RING_COUNT)
*/
RingBuffer* RingInit(unsigned char* buf,int len)
{
	struct RingBuffer* r = NULL;
	if(index < RING_COUNT)
	{
		r = RingBufferList + index;
		r->Ring = buf;
		r->len = len;
		r->Ring_Head = r->Ring_Tail = r->Ring;
		r->count = 0;
		index ++;
	}
	return r;
}
/*
@brief 清缓存
*/
void RingClear(RingBuffer* Ring)
{
	Ring->Ring_Head = Ring->Ring_Tail = Ring->Ring;
	Ring->count = 0;
}

/*
@brief 放入数据
@param Ring 缓冲区指针
@param data 要放入的数据
*/
void RingSet(RingBuffer* Ring,unsigned char data)
{
	if(Ring->count >= Ring->len)
	{
		Ring->count = 0;	
		Ring->Ring_Head = Ring->Ring_Tail = Ring->Ring;
	}
	*Ring->Ring_Tail = data;
	Ring->Ring_Tail++;
	if(Ring->Ring_Tail>=(Ring->Ring+Ring->len))
	{
		Ring->Ring_Tail = Ring->Ring;
	}
	Ring->count ++;
}
/*
@brief 取出数据,但是不修改缓冲区
@param Ring 缓冲区指针
@param Pos 索引(与缓冲区头的偏移距离)
@return 数据
*/
int RingPeek(RingBuffer* Ring)
{
	if(Ring->Ring_Head != Ring->Ring_Tail)
	{
		return *Ring->Ring_Head;
	}
	else
		return -1;
}
/*
@brief 取出数据,并且从缓冲区中去除
@param Ring 缓冲区指针
@return 数据,-1:缓冲区已经空了,0..255:有效数据
*/
int RingPop(RingBuffer* Ring)
{
	int rt = -1;

	//if(Ring->Ring_Head != Ring->Ring_Tail)
	if(Ring->count>0)
	{
		unsigned char d = *Ring->Ring_Head;
		Ring->Ring_Head++;
		if(Ring->Ring_Head>=(Ring->Ring+Ring->len))
			Ring->Ring_Head = Ring->Ring;
		Ring->count--;
		rt = d;
	}

	return rt;
}
/*
@brief 缓冲区中数据的个数
@param Ring 缓冲区指针
@return 数据个数
*/
__inline int RingLength(RingBuffer* Ring)
{
	return Ring->count;
/*	if(Ring->Ring_Head>=Ring->Ring_Tail) 
		return Ring->Ring_Head - Ring->Ring_Tail;
	else 
		return Ring->len + Ring->Ring_Head - Ring->Ring_Tail;*/
}
/*
@brief 放入多个数据
@param Ring 缓冲区指针
@param date 数据区头指针
@param len 数据区长度
*/
void RingSetBuffer(RingBuffer* Ring,unsigned char* date,int len)
{
	while(len--)
	{
		RingSet(Ring,*date);
		date++;
	}
}
/*
@brief 取出多个数据
@param Ring 缓冲区指针
@param date 数据区头指针
@param len 数据区长度
@return data 取出的数据的个数
*/
int RingGetBuffer(RingBuffer* Ring,unsigned char* date,int len)
{
	int d,count = 0;
	while(len--)
	{
		d = RingPop(Ring);
		if(d == -1)
			break;
		*date = d;
		date++;
		count++;
	}
	return count;
}
