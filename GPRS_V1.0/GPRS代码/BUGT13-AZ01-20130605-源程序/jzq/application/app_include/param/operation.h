/**
* operation.h -- ��������ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-7
* ����޸�ʱ��: 2009-5-7
*/

#ifndef _PARAM_OPERATION_H

#include "app_include/uplink/svrmsg.h"
#include "plat_include/flashsave.h"

#define _PARAM_OPERATION_H

#define POERR_OK			0
#define POERR_INVALID		1
#define POERR_FATAL		2

int WriteParam(unsigned char *buf, int len, int *pactlen);

typedef struct {
	unsigned char *buf;   //���뻺����ָ��
	int len;   //����������
	int actlen;   //��Ч���ݳ���(�ɺ�������)
} para_readinfo_t;
int ReadParam(unsigned char *buf, int len, int *pactlen, para_readinfo_t *readinfo);

void ClearSaveParamFlag(void);
void SaveParam(void);

void ClearAllData(void);
void SaveCleanData(void);
void ClearAllParam(void);
void ClearFactoryParam(void);

#endif /*_PARAM_OPERATION_H*/
