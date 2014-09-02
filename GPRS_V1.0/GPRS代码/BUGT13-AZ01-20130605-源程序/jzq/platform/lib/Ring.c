#include <stdio.h>
#include "plat_include/ring.h"
#include "ucos_ii.h"
/*
@brief ���λ�������������
*/
struct RingBuffer{
unsigned char* Ring;
int len;
unsigned char* Ring_Head;
unsigned char* Ring_Tail;
int count;
};
/*
@brief ���λ�����ʵ���б�
*/
static struct RingBuffer RingBufferList[RING_COUNT];
/*
@brief ��һ�����õĻ��λ�����ʵ��
*/
static int index = 0;

/*
@brief ��ʼ�����λ�����
@param buf Ԥ����Ļ������ռ�
@param len ����������
@return �ɹ�������ָ��, ����NULL(�ɵ���RING_COUNT)
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
@brief �建��
*/
void RingClear(RingBuffer* Ring)
{
	Ring->Ring_Head = Ring->Ring_Tail = Ring->Ring;
	Ring->count = 0;
}

/*
@brief ��������
@param Ring ������ָ��
@param data Ҫ���������
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
@brief ȡ������,���ǲ��޸Ļ�����
@param Ring ������ָ��
@param Pos ����(�뻺����ͷ��ƫ�ƾ���)
@return ����
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
@brief ȡ������,���Ҵӻ�������ȥ��
@param Ring ������ָ��
@return ����,-1:�������Ѿ�����,0..255:��Ч����
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
@brief �����������ݵĸ���
@param Ring ������ָ��
@return ���ݸ���
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
@brief ����������
@param Ring ������ָ��
@param date ������ͷָ��
@param len ����������
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
@brief ȡ���������
@param Ring ������ָ��
@param date ������ͷָ��
@param len ����������
@return data ȡ�������ݵĸ���
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
