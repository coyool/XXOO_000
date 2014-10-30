/**
* crc.h -- 计算CRC函数头文件
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-4-1
* 最后修改时间: 2009-4-1
*/

#ifndef _LIB_CRC_H
#define _LIB_CRC_H

unsigned short CalculateCRC(const unsigned char *buffer, int count);
void CalculateCRCStep(const unsigned char *buffer, int count, unsigned short *pcrc);
#define calculate_crc CalculateCRC
#define PPPINITFCS16 0xffff 
#define PPPGOODFCS16 0xf0b8 
unsigned short pppfcs16(unsigned short fcs, unsigned char *cp, int len);
#endif /*_LIB_CRC_H*/

