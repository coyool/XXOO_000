/**
* dl645_1997.h -- DL/T645-1997协议分析
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-21
* 最后修改时间: 2009-5-21
*/

#ifndef _DL645_1997_H
#define _DL645_1997_H

int Dl1997MakeReadPkt(const unsigned char *addr, unsigned short itemid, unsigned char *buf, int len);
int Dl1997MakeWritePkt(const unsigned char *addr, const struct  plwrite_config_t *pconfig, unsigned char *buf, int len);
int Dl1997MakeChkTimePkt(const sysclock_t *pclock, unsigned char *buf, int len);
int Dl1997CheckRead(const unsigned char *addr, unsigned short itemid, unsigned char *buf, int len);
int Dl1997CheckWrite(const unsigned char *addr, unsigned short itemid, const unsigned char *buf, int len);

int SepDl1997MakeReadPkt(const unsigned char *owneraddr, unsigned char *addr, unsigned short itemid, unsigned char *buf, int len);
int SepDl1997CheckRead(const unsigned char *addr, unsigned short itemid, unsigned char *buf, int len);
#ifndef DL645_UNSUPPORT_ITEM
#define DL645_UNSUPPORT_ITEM		-100
#endif

#endif /*_DL645_1997_H*/

