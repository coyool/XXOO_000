#ifndef __MX25L64_H
#define __MX25L64_H

#define MX25L64_SECTOR_SIZE	(4096)
#define MX25L64_SECTORS		(2048)
#define MX25L64_SIZE		(MX25L64_SECTOR_SIZE*MX25L64_SECTORS)

void ssp3_init(void);


unsigned int mx25l64_read(unsigned int addr,void *buf,unsigned int bytes);
unsigned int mx25l64_program(unsigned int addr,const void *buf,unsigned int bytes);
unsigned int mx25l64_sector_erase(unsigned int addr);

#if 0

#define flash_read	mx25l64_read
#define flash_program mx25l64_program
#define flash_sector_erase mx25l64_sector_erase

#endif

#endif
