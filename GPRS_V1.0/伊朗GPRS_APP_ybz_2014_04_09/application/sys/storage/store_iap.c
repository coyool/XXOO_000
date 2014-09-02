/*
在线升级代码存储空间分配,256K代码空间共需64个扇区，存储器中只有分配1892~2047扇区
作为升级代码存储
存储区第一个扇区作为记录区，从第二个扇区开始是代码区
记录区内容如下
第一部分存放升级标志，升级标志4字节，校验头8字节，共12字节
每二部分存放代码长度，长度4字节，校验头8字节，共12字节
第三部分由很多个记录组成，每个记录由校验验头(8)，起始字节(4)，长度(4)和代码校验(2)组成

                   ------------                 
				  |			   |              /  -----------
				  |iap_record2 |           /    |数据区校验 |
                   ------------          /       -----------
				  |			   |	   /        |  长度     |
				  |iap_record1 |   /             -----------
				   ------------ /               | 起始字节数|
	起始地址和长度|			   |	数据形式	 -----------
				  |iap_record0 |				|	校验头	| 
				   -----------  ---------------  -----------
				  |			   |				
	代码长度      | iap_flag   |	校验头、长度、记录数
				  -----------  ---------------  -----------
				  |			   |				
	升级标志校验  | iap_flag   |	校验头加标志			
				   -----------					

*/
#include <stdlib.h>
#include <stdio.h>
#include "hal_include/flash.h"
#include "hal_include/mx25l64.h"
#include "app_include/sys/store.h"
#include "app_include/sys/store_iap.h"

#include "app_include/lib/crc.h"
#include "hal_include/switch.h"

#include "app_include/sys/iap_save.h"
#include "plat_include/debug.h"



static struct iap_record	irecord;
static struct iap_count	icount;

int store_iap(const void *pbuf,unsigned short len)
{
	unsigned int addr;
	if(icount.size+len>CODE_SIZE){
		//PrintLog(LOGTYPE_SHORT,"iap flash overflow\n");
		return -1;
	}
	if(icount.records>=UPDATE_RECORDS){
		//PrintLog(LOGTYPE_SHORT,"iap records overflow\n");
		return -1;
	}
	
	//代码写到Flash-------------------------------------------------------------
	addr = UPDATE_CODE_ADDR+icount.size;
	flash_program(addr,pbuf,len);
	
	
	//写记录数据----------------------------------------------------------------
	irecord.offset = icount.size;
	irecord.bytes = len;
	irecord.codecrc = calculate_crc(pbuf,len);
	addr = UPDATE_RECORD_ADDR;
	addr = UPDATE_RECORD_ADDR+UPDATE_RECORD_SIZE*icount.records;
	f_program(addr,&irecord,sizeof(irecord));
	
	icount.size+=len;
	icount.records++;
	//PrintLog(LOGTYPE_SHORT,"save code frame:%d,size:%d",icount.records,icount.size);
	return 0;
}


void store_iap_start(void)
{
	unsigned int i;
	//PrintLog(LOGTYPE_SHORT,"ipa save code start\n");
	for(i=FLASH_UPDATE_SECTOR_BASE;i<=FLASH_UPDATE_SECTOR_END;i++){
		watchdog_feed();
		flash_sector_erase(i);
		//printf("erase flash sector %d",i);
	}
	icount.size = 0;
	icount.records = 0;
}


static const unsigned int update_flag=UPDATE_FLAG;
void store_iap_end(void)
{	
	//存代码长度
	f_program(UPDATE_COUNT_ADDR,&icount,sizeof icount);
	//存升级标志
	f_program(UPDATE_FLAG_ADDR,&update_flag,sizeof update_flag);
	//PrintLog(LOGTYPE_SHORT,"iap save code end\n");
	//exit(0);
}



#if 0
int is_iap_legal(void);
unsigned char iapbuffer[1000];
#include "string.h"
#include "delay.h"
void test_iap(void)
{
	unsigned int i,addr;
	//unsigned char *pbuf;
	store_iap_start();
	for(i=0;i<150;i++){
		memset(iapbuffer,i,sizeof(iapbuffer));
		store_iap(iapbuffer,sizeof(iapbuffer));
		delay_us(30000);
	}
	store_iap_end();
	
	memset(&icount,0,sizeof icount);
	
	if(is_iap_legal()==0){
		PrintLog(LOGTYPE_SHORT,"iap unlegal\n");
		return;
	}

	for(i=0;i<icount.records;i++){
		//先读record-------------------------------------------------------------
		addr = UPDATE_RECORD_ADDR+i*UPDATE_RECORD_SIZE;
		if(f_read(addr,&irecord,sizeof(irecord))!=sizeof(irecord)){
			PrintLog(LOGTYPE_SHORT,"read iap record error\n");
			exit(0);
		}
		PrintLog(LOGTYPE_SHORT,"read iap record %d ok\n",i);
		//读数据区---------------------------------------------------------------
		addr = UPDATE_CODE_ADDR+irecord.offset;
		flash_read(addr,iapbuffer,irecord.bytes);
		if(calculate_crc(iapbuffer,irecord.bytes)!=irecord.codecrc){
			PrintLog(LOGTYPE_SHORT,"read code error\n");//代码区校验出错
			exit(0);
		}
		PrintLog(LOGTYPE_SHORT,"read iap code %d ok\n",i);
	}
}
static unsigned int flag;
int is_iap_legal(void)
{
	if(sizeof(update_flag)!=f_read(UPDATE_FLAG_ADDR,&flag,sizeof(flag))){
		return 0;
	}
	if(flag!=UPDATE_FLAG)
		return 0;
	if(sizeof(icount)!=f_read(UPDATE_COUNT_ADDR,&icount,sizeof(icount)))
		return 0;
	if(icount.size>CODE_SIZE)
		return 0;
	if(icount.records>UPDATE_RECORDS)
		return 0;
	return 1;
}


#endif
