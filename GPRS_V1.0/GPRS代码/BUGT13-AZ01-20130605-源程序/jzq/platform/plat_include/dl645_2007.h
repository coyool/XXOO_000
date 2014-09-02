/**
* dl645_2007.h -- DL/T645-2007协议分析
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-21
* 最后修改时间: 2009-5-21
*/

#ifndef _DL645_2007_H
#define _DL645_2007_H
struct plwrite_config_t;

int Dl2007MakeReadPkt(const unsigned char *addr, unsigned short itemid, unsigned char *buf, int len);
int Dl2007MakeWritePkt(const unsigned char *addr, const struct  plwrite_config_t *pconfig, unsigned char *buf, int len);
int Dl2007MakeChkTimePkt(const sysclock_t *pclock, unsigned char *buf, int len);
int Dl2007CheckRead(const unsigned char *addr, unsigned short itemid, unsigned char *buf, int len);
int Dl2007CheckWrite(const unsigned char *addr, unsigned short itemid, const unsigned char *buf, int len);

unsigned short Dl2007ConverItemid(unsigned long lid);
unsigned long Dl2007LongId(unsigned short sid);
int Dl2007MakeReadPktRaw(const unsigned char *addr, unsigned long itemid, unsigned char *buf, int len);
int SepDl2007MakeReadPkt(const unsigned char *owneraddr, unsigned char *addr, unsigned short itemid, unsigned char *buf, int len);
int SepDl2007CheckRead(const unsigned char *addr, unsigned short itemid, unsigned char *buf, int len);
#ifndef DL645_UNSUPPORT_ITEM
#define DL645_UNSUPPORT_ITEM		-100
#endif
int SepDl2007CheckFrame(unsigned char *addr, unsigned char *code, unsigned char *buf, int len);
int Dl2007MakeWritePktRaw(const unsigned char *addr, unsigned int Itemid, unsigned char *data,unsigned char data_len,unsigned char *buf);

#endif /*_DL645_2007_H*/
