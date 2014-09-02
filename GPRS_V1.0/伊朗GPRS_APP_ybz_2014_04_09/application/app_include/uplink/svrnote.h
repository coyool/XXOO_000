/**
* svrnote.h -- 服务器通信主动发送提醒
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-19
* 最后修改时间: 2009-5-19
*/

#ifndef _SVRCOMM_NOTE_H
#define _SVRCOMM_NOTE_H

#define SVREV_NOTE				1<<0
#define SVREV_SMS				1<<1
#define SVREV_CSQ				1<<2
#define SVREV_USER_SMS			1<<3
#define SVREV_ACTIVE			1<<4
#define SVREV_ALERM				1<<5





#define SVRNOTEMASK_ALARM		0x80000000
#define SVRNOTEMASK_TASKCLS2	0x00000001
#define SVRNOTEMASK_TASKCLS1	0x00000001

#define SVRNOTEID_ALARM		100
#define SVRNOTEID_TASKCLS2(id)		(unsigned char)(id+33)
#define SVRNOTEID_TASKCLS1(id)		(unsigned char)(id+1)

void SvrCommNote(unsigned char id);


#endif /*_SVRCOMM_NOTE_H*/

