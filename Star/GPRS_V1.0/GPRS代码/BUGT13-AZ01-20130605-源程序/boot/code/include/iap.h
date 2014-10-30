#ifndef __IAP_H
#define __IAP_H

#define FLASH_BASE		0x08000000
#define APP_LOCATION	(unsigned int)0x08003000
#define SECTOR_END		64	//芯片中的flash结束扇区


int iap(void);

void store_iap_start(void);
void store_iap_end(void);
int store_iap(const void *pbuf,unsigned short len);

#endif
