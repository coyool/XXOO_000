#include <string.h>
#include <stdint.h>
#include "iap_save.h"
#include "iap.h"
#include "flash.h"
#include "iapcmd.h"
#include "crc.h"	
#include "switch.h"
#include "uart.h"




static unsigned int flag;
static struct iap_count icount;
static struct iap_record	irecord;

static int is_iap_legal(void)
{
	if(sizeof(flag)!=f_read(UPDATE_FLAG_ADDR,&flag,sizeof(flag))){
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

/*
破坏IAP标志的方法是擦除标志所在的扇区
*/
static void destroy_iap_flag(void)
{
	flash_sector_erase(FLASH_UPDATE_SECTOR_BASE);
}

static int code_acquire(unsigned char *pbuf,unsigned int i)
{
	unsigned int addr;

	//先读record-------------------------------------------------------------
	addr = UPDATE_RECORD_ADDR+i*UPDATE_RECORD_SIZE;
	if(f_read(addr,&irecord,sizeof(irecord))!=sizeof(irecord)){
		return -1;
	}
	
	//读数据区---------------------------------------------------------------
	addr = UPDATE_CODE_ADDR+irecord.offset;
	flash_read(addr,pbuf,irecord.bytes);
	if(calculate_crc(pbuf,irecord.bytes)!=irecord.codecrc){
		return -1;
	}
	return irecord.bytes;
}
unsigned char iapbuf[3000];
int iap(void)
{
	unsigned char *pbuf;
	unsigned int i,block,faddr;
	int j;
	
	SerialPutString("Update Program Check...\n");
	if(is_iap_legal()==0)
		return -1;
	SerialPutString("Prepare Update...\n");
	//先校验一遍，全部代码都正确才能覆盖现有代码
	for(i=0;i<icount.records;i++){
		watchdog_feed();
//		led_alarm_flash();
		if(code_acquire(iapbuf,i)<0)	//有校验错误，退出
		{
			SerialPutString("Crc check error!\n");
			return -1;
		}
	}
	SerialPutString("Crc check ok!\n");
//	led_alarm_on();
	FLASH_If_Init();

	if(erase_sector(APP_LOCATION,FLASH_BASE+(SECTOR_END*FLASH_PAGE_SIZE))){
		SerialPutString("Earse flash Error!Reboot...\n");		
		SysReset();
	}
	
	block = 0;
	pbuf = iapbuf;
	faddr = APP_LOCATION;
	SerialPutString("Updating ");
	for(i=0;i<icount.records;i++){
		watchdog_feed();
		//led_online_flash();
		SerialPutString(".");
		j = code_acquire(pbuf,i);
		if(j<0)
		{
			SerialPutString("Crc check error!Reboot...\n");
			SysReset();		//升级失败，将重启后再度偿试
		}
		block += j;
		pbuf = iapbuf;
		//每次写入必须是一个256或512或1024或4096，这里选512字节
		while(block>=CRF_SIZE){
			if(copy_ram_to_flash(faddr,(unsigned char*)pbuf,CRF_SIZE)<0)
				SysReset();
			faddr += CRF_SIZE;
			pbuf += CRF_SIZE;
			block -= CRF_SIZE;
		}
		//如果还有剩余不满CFR_SIZE
		if(block>0){
			memmove(iapbuf,pbuf,block);
			//最后一个record已读完，无需再从外部FLASH读取
			if(i==(icount.records-1)){
				//非有效区全部置FFH
				memset(iapbuf+block,0xff,CRF_SIZE-block);
				copy_ram_to_flash(faddr,(unsigned char*)iapbuf,CRF_SIZE);
			}
		}
		//下次读取代码后存放的RAM地址
		pbuf = iapbuf+block;	
	}
	FLASH_If_finish();
	SerialPutString("\n");
	SerialPutString("Update finshed!\n");
	//led_online_on();
	//完成IAP，破坏IAP标志
	if(i==icount.records){
		//printf("in application programming completed");
		destroy_iap_flag();
	}
	return 0;
}

void store_iap_start(void)
{
	unsigned int i;
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

	//exit(0);
}
int store_iap(const void *pbuf,unsigned short len)
{
	unsigned int addr;
	if(icount.size+len>CODE_SIZE){
		return -1;
	}
	if(icount.records>=UPDATE_RECORDS){
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
	return 0;
}
