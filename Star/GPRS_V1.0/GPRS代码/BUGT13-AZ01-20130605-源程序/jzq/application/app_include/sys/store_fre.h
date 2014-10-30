#ifndef STORE_FRE_H
#define STORE_FRE_H

int fre_save(unsigned char type,dbtime_t dbtime,unsigned int mid,const void *pbuf,unsigned short bytes);
int fre_read(unsigned char type,dbtime_t dbtime,unsigned int mid,void *pbuf,unsigned short bytes);
int fre_rm(unsigned char type,unsigned int mid);
int fre_rm_time(unsigned char type,dbtime_t time);
int fre_rm_interval(dbtime_t time1,dbtime_t time2);
int fre_rm_all(void);
#endif
