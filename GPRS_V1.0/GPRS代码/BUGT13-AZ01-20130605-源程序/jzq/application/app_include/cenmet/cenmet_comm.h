/**
* cenmet_comm.h -- 表计通信
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-17
* 最后修改时间: 2009-5-17
*/

#ifndef _CENMET_COMM_H
#define _CENMET_COMM_H

#include "app_include/param/capconf.h"

int CenMetRead(unsigned short metid, unsigned short itemid, unsigned char *buf, int *plen);
int CenMetReadTime(unsigned short metid, sysclock_t *pclk);

void CenMetProc(void);
void CenMetFindMetProc(void);

void ReadImmMdbCur(unsigned short mid, const unsigned short *pitems, unsigned short num,unsigned char user);
int CMetCommInit(unsigned char mid);

void CenMetCommCheck(void);

typedef struct{
	unsigned char cnt_comerr;  //通信错误及标识
	unsigned char rs485_time[5];
	unsigned char rs485_enepa[5];
	unsigned char rs485_enepi[4];
}metcommstic_t;

#ifndef DEFINE_PARAMETP
extern metcommstic_t metcommstic[MAX_CENMETP];
#endif
#define comflg_readitem_sig    0x80
//#define comflg_readitem_mark   0x40
#define readitemidsig(mid)  (((metcommstic[mid].cnt_comerr&comflg_readitem_sig))?1:0)
#define metcommchk(mid)  (((metcommstic[mid].cnt_comerr&0x0f)>=2)?1:0)
#define metcommclr(mid)   metcommstic[mid].cnt_comerr &= 0xf0;

#define MAX_READITEM_NUM  100
extern unsigned short MetReadItemsChk(unsigned short mid, unsigned short *itemid,unsigned char type);
#endif /*_CENMET_COMM_H*/

