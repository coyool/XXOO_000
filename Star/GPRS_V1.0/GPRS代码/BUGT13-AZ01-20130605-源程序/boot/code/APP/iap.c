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
�ƻ�IAP��־�ķ����ǲ�����־���ڵ�����
*/
static void destroy_iap_flag(void)
{
	flash_sector_erase(FLASH_UPDATE_SECTOR_BASE);
}

static int code_acquire(unsigned char *pbuf,unsigned int i)
{
	unsigned int addr;

	//�ȶ�record-------------------------------------------------------------
	addr = UPDATE_RECORD_ADDR+i*UPDATE_RECORD_SIZE;
	if(f_read(addr,&irecord,sizeof(irecord))!=sizeof(irecord)){
		return -1;
	}
	
	//��������---------------------------------------------------------------
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
	//��У��һ�飬ȫ�����붼��ȷ���ܸ������д���
	for(i=0;i<icount.records;i++){
		watchdog_feed();
//		led_alarm_flash();
		if(code_acquire(iapbuf,i)<0)	//��У������˳�
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
			SysReset();		//����ʧ�ܣ����������ٶȳ���
		}
		block += j;
		pbuf = iapbuf;
		//ÿ��д�������һ��256��512��1024��4096������ѡ512�ֽ�
		while(block>=CRF_SIZE){
			if(copy_ram_to_flash(faddr,(unsigned char*)pbuf,CRF_SIZE)<0)
				SysReset();
			faddr += CRF_SIZE;
			pbuf += CRF_SIZE;
			block -= CRF_SIZE;
		}
		//�������ʣ�಻��CFR_SIZE
		if(block>0){
			memmove(iapbuf,pbuf,block);
			//���һ��record�Ѷ��꣬�����ٴ��ⲿFLASH��ȡ
			if(i==(icount.records-1)){
				//����Ч��ȫ����FFH
				memset(iapbuf+block,0xff,CRF_SIZE-block);
				copy_ram_to_flash(faddr,(unsigned char*)iapbuf,CRF_SIZE);
			}
		}
		//�´ζ�ȡ������ŵ�RAM��ַ
		pbuf = iapbuf+block;	
	}
	FLASH_If_finish();
	SerialPutString("\n");
	SerialPutString("Update finshed!\n");
	//led_online_on();
	//���IAP���ƻ�IAP��־
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
	//����볤��
	f_program(UPDATE_COUNT_ADDR,&icount,sizeof icount);
	//��������־
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
	
	//����д��Flash-------------------------------------------------------------
	addr = UPDATE_CODE_ADDR+icount.size;
	flash_program(addr,pbuf,len);
	
	
	//д��¼����----------------------------------------------------------------
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
