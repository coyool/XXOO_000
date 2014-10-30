/*
������������洢�ռ����,256K����ռ乲��64���������洢����ֻ�з���1892~2047����
��Ϊ��������洢
�洢����һ��������Ϊ��¼�����ӵڶ���������ʼ�Ǵ�����
��¼����������
��һ���ִ��������־��������־4�ֽڣ�У��ͷ8�ֽڣ���12�ֽ�
ÿ�����ִ�Ŵ��볤�ȣ�����4�ֽڣ�У��ͷ8�ֽڣ���12�ֽ�
���������ɺܶ����¼��ɣ�ÿ����¼��У����ͷ(8)����ʼ�ֽ�(4)������(4)�ʹ���У��(2)���

                   ------------                 
				  |			   |              /  -----------
				  |iap_record2 |           /    |������У�� |
                   ------------          /       -----------
				  |			   |	   /        |  ����     |
				  |iap_record1 |   /             -----------
				   ------------ /               | ��ʼ�ֽ���|
	��ʼ��ַ�ͳ���|			   |	������ʽ	 -----------
				  |iap_record0 |				|	У��ͷ	| 
				   -----------  ---------------  -----------
				  |			   |				
	���볤��      | iap_flag   |	У��ͷ�����ȡ���¼��
				  -----------  ---------------  -----------
				  |			   |				
	������־У��  | iap_flag   |	У��ͷ�ӱ�־			
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
	//����볤��
	f_program(UPDATE_COUNT_ADDR,&icount,sizeof icount);
	//��������־
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
		//�ȶ�record-------------------------------------------------------------
		addr = UPDATE_RECORD_ADDR+i*UPDATE_RECORD_SIZE;
		if(f_read(addr,&irecord,sizeof(irecord))!=sizeof(irecord)){
			PrintLog(LOGTYPE_SHORT,"read iap record error\n");
			exit(0);
		}
		PrintLog(LOGTYPE_SHORT,"read iap record %d ok\n",i);
		//��������---------------------------------------------------------------
		addr = UPDATE_CODE_ADDR+irecord.offset;
		flash_read(addr,iapbuffer,irecord.bytes);
		if(calculate_crc(iapbuffer,irecord.bytes)!=irecord.codecrc){
			PrintLog(LOGTYPE_SHORT,"read code error\n");//������У�����
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
