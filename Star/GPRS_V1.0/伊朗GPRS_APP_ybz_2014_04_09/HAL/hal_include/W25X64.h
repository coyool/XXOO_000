#ifndef _W25X64_H_
#define _W25X64_H_
/*
spi flash �豸��ʼ��
*/
void w25x64_init(void);
/*
������
*/
void w25x64_read(unsigned int addr,void *pbuf,unsigned int bytes);
/*
��������
*/
void w25x64_sector_erase(unsigned int addr);
/*
д����
*/
void w25x64_program(unsigned int addr,const void *pbuf,unsigned int bytes);
/*
����
*/
void W25X64_Test(void);
#endif
