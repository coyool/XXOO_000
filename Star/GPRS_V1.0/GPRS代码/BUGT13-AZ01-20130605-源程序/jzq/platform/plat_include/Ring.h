#ifndef _RING_H_
#define _RING_H_
/**
* Ring.h -- 环形缓冲区
* 
* 作者:  
* 创建时间: 2012-10-31
* 最后修改时间: 2012-10-31
*/
/*
@brief 环形缓冲区实例数量上限
*/
#define RING_COUNT 20
/*
@brief 环形缓冲区类型声明
*/
typedef struct RingBuffer RingBuffer;
/*
@brief 初始化环形缓冲区
@param buf 预分配的缓冲区空间
@param len 缓冲区长度
@return 成功缓冲区指针, 否则NULL(可调整RING_COUNT)
*/
RingBuffer* RingInit(unsigned char* buf,int len);
/*
@brief 放入数据
@param Ring 缓冲区指针
@param data 要放入的数据
*/
void RingSet(RingBuffer* Ring,unsigned char data);
/*
@brief 清缓存
*/
void RingClear(RingBuffer* Ring);
/*
@brief 取出数据,但是不修改缓冲区
@param Ring 缓冲区指针
@param Pos 索引(与缓冲区头的偏移距离)
@return 数据
*/
int RingPeek(RingBuffer* Ring);
/*
@brief 取出数据,并且从缓冲区中去除
@param Ring 缓冲区指针
@return 数据,-1:缓冲区已经空了,0..255:有效数据
*/
int RingPop(RingBuffer* Ring);
/*
@brief 缓冲区中数据的个数
@param Ring 缓冲区指针
@return 数据个数
*/
__inline int RingLength(RingBuffer* Ring);
/*
@brief 放入多个数据
@param Ring 缓冲区指针
@param date 数据区头指针
@param len 数据区长度
*/
void RingSetBuffer(RingBuffer* Ring,unsigned char* date,int len);
/*
@brief 取出多个数据
@param Ring 缓冲区指针
@param date 数据区头指针
@param len 数据区长度
@return data 取出的数据的个数
*/
int RingGetBuffer(RingBuffer* Ring,unsigned char* date,int len);
#endif

