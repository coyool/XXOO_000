/**
* align.h -- ���ݶ���
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-6-11
* ����޸�ʱ��: 2009-6-11
*/

#ifndef _LIB_ALIGN_H
#define _LIB_ALIGN_H

unsigned short MAKE_SHORT(const unsigned char* buf);

void DEPART_SHORT(unsigned short usv,unsigned char* buf);

unsigned long MAKE_LONG(const unsigned char* buf);

void DEPART_LONG(unsigned long ulv,unsigned char* buf);

#endif /*_LIB_ALIGN_H*/

