/**
* datatask.c -- �������
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-8
* ����޸�ʱ��: 2009-5-8
*/

#include <stdio.h>
#include <string.h>
#define DEFINE_PARATASK
#include "app_include/param/datatask.h"
#include "app_include/param/operation.h"
#include "app_include/param/operation_inner.h"
#include "app_include/lib/bcd.h"
#include "app_include/uplink/datatask_timer.h"
#include "app_include/cenmet/sndtime.h"
#include "plat_include/debug.h"
#include "app_include/param/param_init.h"

#define ParaTask para_comb.uniq.ParaTaskCls1;



static int ParaOpertionTask(int flag, para_task_t *ptask, unsigned char *buf, int len, int *actlen)
{

	if(0 == flag) {

		*buf++ = ((ptask->mod_snd&0x03)<<6) + (ptask->dev_snd&0x3f);
		buf[0] = ptask->base_second;
		buf[1] = ptask->base_minute;
		buf[2] = ptask->base_hour;
		buf[3] = ptask->base_day;
		buf[4] = ptask->base_month;
		buf[5] = ptask->base_year;
		HexToBcd(buf, 6);
		buf[4] += ptask->base_week << 5;
	}
	else {
		ptask->mod_snd = (buf[0]>>6)&0x03;
		ptask->dev_snd = buf[0]&0x3f; buf++;
		ptask->base_second = *buf++;
		ptask->base_minute = *buf++;
		ptask->base_hour = *buf++;
		ptask->base_day = *buf++;
		ptask->base_month = *buf & 0x1f;
		ptask->base_week = (*buf++)>>5;
		ptask->base_year = *buf++;
		BcdToHex(&ptask->base_year, 6);
		SaveParaTerm();
	}

	return 0;
}

/**
* @brief �ն˲���F65����
* @param flag ������ʽ, 0-��, 1-д
* @param metpid �������
* @param buf ������ָ��
* @param len ����������
* @param actlen ��Ч���ݳ���(�ɺ�������)
* @return 0�ɹ�, ����ʧ��(�ο�POERR_XXX�궨��)
* @note ����ͬ������ͷ���ֵ��ͬ, �����ظ�ע��
*/
int ParaOperationF65(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	para_task_t *ptask = &para_comb.uniq.ParaTaskCls1[metpid-1];
	int rtn;

	rtn = ParaOpertionTask(flag, ptask, buf, len, actlen);

	if(flag && 0 == rtn) {
		if(ptask->valid) DataTaskCls1ReInit(metpid-1);
	}

	return rtn;
}


/**
* @brief �ն˲���F67����
*/
int ParaOperationF67(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	para_task_t *ptask = &para_comb.uniq.ParaTaskCls1[metpid-1];
	unsigned char old;
	PrintLog(LOGTYPE_UPLINK, "ParaOperationF67 id:%d,cmd:%x\r\n",metpid,buf[0]);
	if(0 == flag) {
		if(ptask->valid) buf[0] = 0x55;
		else buf[0] = 0xaa;
	}
	else 
	{
		old = ptask->valid;
		if(buf[0] == 0x55) ptask->valid = 1;
		else if(buf[0] == 0xaa) ptask->valid = 0;
		else return POERR_INVALID;

		if(old != ptask->valid) 
		{
			DataTaskCls1ReInit(metpid-1);
			SaveParaTerm();
		}
	}

	return 0;
}


