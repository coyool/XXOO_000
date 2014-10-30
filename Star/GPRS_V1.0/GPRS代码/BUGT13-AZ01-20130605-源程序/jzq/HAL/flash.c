//#include "func.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "ucos_ii.h"
#include "app_include/lib/crc.h"
#include "hal_include/flash.h"
#include "plat_include/debug.h"




/**
 带检验的读取
 *@addr:地址
 *@pbuf:数据指针
 *@bytes:数据长度
 *@return:-1 出错，>=0 有效字节数
*/
int f_read(unsigned int addr,void *pbuf,unsigned int bytes)
{
	struct save_head head;
	flash_read(addr,&head,sizeof(head));
	if(head.headcrc!=calculate_crc((const unsigned char*)&head,offsetof(struct save_head,headcrc)))
		return -1;
	if(head.len!=bytes)
		return -1;
	flash_read(addr+sizeof(head),pbuf,head.len);
	if(head.datacrc!=calculate_crc(pbuf,head.len))
		return -1;
	return head.len;
}

int f_program(unsigned int addr,const void *pbuf,unsigned int bytes)
{
	struct save_head head;

	head.datacrc = calculate_crc(pbuf,bytes);
	head.len = bytes;
	head.headcrc = calculate_crc((const unsigned char*)&head,offsetof(struct save_head,headcrc));
	flash_program(addr,&head,sizeof(head));
	flash_program(addr+sizeof(head),pbuf,bytes);
	return bytes;
}

int f2_read(unsigned int sct1,unsigned int sct2,void *pbuf,unsigned int bytes)
{
	sct1 *= FLASH_SECTOR_SIZE;
	if(f_read(sct1,pbuf,bytes)>=0){
		return bytes;
	}
	sct2 *= FLASH_SECTOR_SIZE;
	if(f_read(sct2,pbuf,bytes)>=0){
	
		return bytes;
	}
	return -1;
}
int f2_write(unsigned int sct1,unsigned int sct2,const void *pbuf,unsigned int bytes)
{
	unsigned int i;
	for(i=0;i<(bytes/FLASH_SECTOR_SIZE+(bytes%FLASH_SECTOR_SIZE?1:0));i++)
	{
		flash_sector_erase(sct1+i);
		flash_sector_erase(sct2+i);
	}
	
	sct1 *= FLASH_SECTOR_SIZE;
	f_program(sct1,pbuf,bytes);
	sct2 *= FLASH_SECTOR_SIZE;
	f_program(sct2,pbuf,bytes);
	return bytes;
}


