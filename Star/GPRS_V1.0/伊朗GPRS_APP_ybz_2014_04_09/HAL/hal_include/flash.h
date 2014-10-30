#ifndef __FLASH_H
#define __FLASH_H
/*
unsigned int mx25l64_read(unsigned int addr,void *buf,unsigned int bytes);
unsigned int mx25l64_program(unsigned int addr,const void *buf,unsigned int bytes);
unsigned int mx25l64_sector_erase(unsigned int addr);
*/

struct save_head
{
	unsigned int len;
	unsigned short datacrc;
	unsigned short headcrc;
};
#define CHKHEAD_SIZE	sizeof(struct save_head)

#define FLASH_SECTOR_SIZE	4096
#define FLASH_SECTORS		2048
#define FLASH_SIZE			(FLASH_SECTOR_SIZE*FLASH_SECTORS)
/*
#define flash_read	mx25l64_read
#define flash_program mx25l64_program
#define flash_sector_erase mx25l64_sector_erase
*/
#include "W25X64.h"
#define flash_read	w25x64_read
#define flash_program w25x64_program
#define flash_sector_erase w25x64_sector_erase

int f_read(unsigned int addr,void *pbuf,unsigned int bytes);
int f_program(unsigned int addr,const void *pbuf,unsigned int bytes);

int f2_read(unsigned int sct1,unsigned int sct2,void *pbuf,unsigned int bytes);
int f2_write(unsigned int sct1,unsigned int sct2,const void *pbuf,unsigned int bytes);



#endif


