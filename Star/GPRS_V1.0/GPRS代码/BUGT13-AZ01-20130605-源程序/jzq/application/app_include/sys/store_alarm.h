#ifndef __STORE_ALARM_H
#define __STORE_ALARM_H

int alarm_save(unsigned char type,const void *pbuf,unsigned short bytes);
int alarm_read(unsigned char type,unsigned int cnt,void *pbuf,unsigned short bytes);
int alarm_rm(void);
int alarm_count(unsigned char type);
#endif


