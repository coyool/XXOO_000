#ifndef _RUNSTATE_H
#define _RUNSTATE_H

#include "app_include/param/capconf.h"
#include "plat_include/timeal.h"

#define LEN_MALM_STAT	68

typedef struct {
	unsigned char head[2];	//��¼ͷ
	unsigned char cur[2];	//��ǰ��¼����
	unsigned char snd[2];	//�����ϱ���������
	unsigned char stat[8];
} alarm_stat_t;

typedef struct {
	unsigned char flag_acd;
	alarm_stat_t alarm;
	unsigned char cnt_snderr[2];
	unsigned short crc;
} runstate_t;

extern runstate_t RunState;
//RunState�޸�
#define RunStateModify	RunState

void SaveRunState(void);
int IsSelfReboot(void);
#endif /*_RUNSTATE_H*/

