#ifndef _RING_H_
#define _RING_H_
/**
* Ring.h -- ���λ�����
* 
* ����:  
* ����ʱ��: 2012-10-31
* ����޸�ʱ��: 2012-10-31
*/
/*
@brief ���λ�����ʵ����������
*/
#define RING_COUNT 20
/*
@brief ���λ�������������
*/
typedef struct RingBuffer RingBuffer;
/*
@brief ��ʼ�����λ�����
@param buf Ԥ����Ļ������ռ�
@param len ����������
@return �ɹ�������ָ��, ����NULL(�ɵ���RING_COUNT)
*/
RingBuffer* RingInit(unsigned char* buf,int len);
/*
@brief ��������
@param Ring ������ָ��
@param data Ҫ���������
*/
void RingSet(RingBuffer* Ring,unsigned char data);
/*
@brief �建��
*/
void RingClear(RingBuffer* Ring);
/*
@brief ȡ������,���ǲ��޸Ļ�����
@param Ring ������ָ��
@param Pos ����(�뻺����ͷ��ƫ�ƾ���)
@return ����
*/
int RingPeek(RingBuffer* Ring);
/*
@brief ȡ������,���Ҵӻ�������ȥ��
@param Ring ������ָ��
@return ����,-1:�������Ѿ�����,0..255:��Ч����
*/
int RingPop(RingBuffer* Ring);
/*
@brief �����������ݵĸ���
@param Ring ������ָ��
@return ���ݸ���
*/
__inline int RingLength(RingBuffer* Ring);
/*
@brief ����������
@param Ring ������ָ��
@param date ������ͷָ��
@param len ����������
*/
void RingSetBuffer(RingBuffer* Ring,unsigned char* date,int len);
/*
@brief ȡ���������
@param Ring ������ָ��
@param date ������ͷָ��
@param len ����������
@return data ȡ�������ݵĸ���
*/
int RingGetBuffer(RingBuffer* Ring,unsigned char* date,int len);
#endif

