/**
* qrydata.h -- ��ѯ����
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-6-2
* ����޸�ʱ��: 2009-6-2
*/

#ifndef _QRY_DATA_H
#define _QRY_DATA_H

int ReadMdb(unsigned short metpid, unsigned short itemid, unsigned char *buf,int len,int *pactlen);
int ReadPlMdb(unsigned short metpid, unsigned short itemid, unsigned char *buf,int len,int *pactlen);

#endif /*_QRY_DATA_H*/

