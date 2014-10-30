#ifndef __IAPCMD_H
#define __IAPCMD_H
#define CRF_SIZE	128
#define FLASH_PAGE_SIZE    ((unsigned short)0x800)

int erase_sector(unsigned int start,unsigned int end);
int copy_ram_to_flash(unsigned int dst,unsigned char* src,int length);

unsigned int cal_sector(unsigned int addr);
void FLASH_If_Init(void);
void FLASH_If_finish(void);
#endif
