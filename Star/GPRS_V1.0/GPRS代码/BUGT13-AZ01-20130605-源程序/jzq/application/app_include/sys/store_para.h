#ifndef __STORE_PARA_H
#define __STORE_PARA_H

int para_save(unsigned char type,const void *pbuf,unsigned int bytes);
int para_read(unsigned char type,void *pbuf,unsigned int bytes);
int para_rm(unsigned char type);

#endif


