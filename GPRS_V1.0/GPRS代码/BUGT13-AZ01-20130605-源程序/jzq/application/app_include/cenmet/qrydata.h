/**
* qrydata.h -- 查询数据
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-6-2
* 最后修改时间: 2009-6-2
*/

#ifndef _QRY_DATA_H
#define _QRY_DATA_H

int ReadMdb(unsigned short metpid, unsigned short itemid, unsigned char *buf,int len,int *pactlen);
int ReadPlMdb(unsigned short metpid, unsigned short itemid, unsigned char *buf,int len,int *pactlen);

#endif /*_QRY_DATA_H*/

