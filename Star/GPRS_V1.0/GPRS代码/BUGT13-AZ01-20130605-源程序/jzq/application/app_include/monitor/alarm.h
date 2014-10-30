/**
* alarm.h -- 事件告警接口
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-15
* 最后修改时间: 2009-5-15
*/

#ifndef _ALARM_H
#define _ALARM_H

#define MAXLEN_ALMDATA    20
typedef struct {
	unsigned char erc;
	unsigned char len;
	unsigned char min;
	unsigned char hour;
	unsigned char day;
	unsigned char mon;
	unsigned char year;
	unsigned char data[MAXLEN_ALMDATA];
} alarm_t;

#define ALMFLAG_ABNOR    0
#define ALMFLAG_NORMAL    1
#define ALMFLAG_IMPORTANT    0x80
#define ALMFLAG_NOIMPORTANT		0x40
void MakeAlarm(unsigned char flag, alarm_t *pbuf);

void ClearAlarm(unsigned char idx);
void GetAlarmEc(unsigned char *ec);


#endif /*_ALARM_H*/

