#ifndef _W25X64_H_
#define _W25X64_H_
/*
spi flash 设备初始化
*/
void w25x64_init(void);
/*
读数据
*/
void w25x64_read(unsigned int addr,void *pbuf,unsigned int bytes);
/*
擦除扇区
*/
void w25x64_sector_erase(unsigned int addr);
/*
写数据
*/
void w25x64_program(unsigned int addr,const void *pbuf,unsigned int bytes);
/*
测试
*/
void W25X64_Test(void);
#endif
