#include <stdio.h>
#include "hal_include/flash.h"
#include "app_include/sys/store.h"
#include "hal_include/switch.h"
#include "plat_include/debug.h"
/*
包括一个存储区和一个备份区,参数共占用16个扇区
*/

#define PARA1_SECTOR_BASE	FLASH_PARA_SECTOR_BASE
#define PARA1_SECTORS		1
#define PARA2_SECTOR_BASE	(PARA1_SECTOR_BASE+PARA1_SECTORS*2)
#define PARA2_SECTORS		1
#define PARA3_SECTOR_BASE	(PARA2_SECTOR_BASE+PARA2_SECTORS*2)
#define PARA3_SECTORS		2
#define PARA4_SECTOR_BASE	(PARA3_SECTOR_BASE+PARA3_SECTORS*2)
#define PARA4_SECTORS		1
#define PARA5_SECTOR_BASE	(PARA4_SECTOR_BASE+PARA4_SECTORS*2)
#define PARA5_SECTORS		1
#define PARA6_SECTOR_BASE	(PARA5_SECTOR_BASE+PARA5_SECTORS*2)
#define PARA6_SECTORS		1
#define PARA7_SECTOR_BASE	(PARA6_SECTOR_BASE+PARA6_SECTORS*2)
#define PARA7_SECTORS		1
struct paraitf
{
	unsigned int sbase;	//sector base
	unsigned int ss;	//sectors
};
static const struct paraitf para_interface[]={
	{PARA1_SECTOR_BASE,PARA1_SECTORS},
	{PARA2_SECTOR_BASE,PARA2_SECTORS},
	{PARA3_SECTOR_BASE,PARA3_SECTORS},
	{PARA4_SECTOR_BASE,PARA4_SECTORS},
	{PARA5_SECTOR_BASE,PARA5_SECTORS},
	{PARA6_SECTOR_BASE,PARA6_SECTORS},
	{PARA7_SECTOR_BASE,PARA7_SECTORS},
};
#define PARA_MAX	sizeof(para_interface)/sizeof(para_interface[0])
/*
 *参数存储
 *@type:类型1~7，除3是两个扇区，其余都只占一个扇区
 *@pbuf:数据指针
 *len:长度
 *return:-1 失败
		>=0 成功，正常情况下就是len
 */
int para_save(unsigned char type,const void *pbuf,unsigned int bytes)
{
	unsigned int addr1,addr2;
	const struct paraitf *itf;
	if((type==0)||(type>PARA_MAX))
		return -1;
	type--;
	
	itf = &para_interface[type];
	if(bytes > (itf->ss*FLASH_SECTOR_SIZE))
		return -1;

	addr1 = itf->sbase;
	addr2 = addr1+itf->ss;
	PrintLog(LOGTYPE_SHORT,"save para type:%d,in sectore:%d and %d,use %d sectors\n",type,addr1,addr2,itf->ss);
	f2_write(addr1,addr2,pbuf,bytes);
	return bytes;
}

/*
 *读取参数
 *@type:类型1~7，除3是两个扇区，其余都只占一个扇区
 *@pbuf:数据指针
 *len:长度
 *return:-1 失败
		>=0 成功，正常情况下就是len
 */
int para_read(unsigned char type,void *pbuf,unsigned int bytes)
{
	unsigned int addr1,addr2;
	const struct paraitf *itf;
	if((type==0)||(type>PARA_MAX))
		return -1;
	type--;
	itf = &para_interface[type];
	if(bytes > (itf->ss*FLASH_SECTOR_SIZE))
		return -1;
	
	addr1 = itf->sbase;
	addr2 = addr1+itf->ss;
	PrintLog(LOGTYPE_SHORT,"read para type:%d,in sectore:%d and %d,use %d sectors\n",type,addr1,addr2,itf->ss);
	return f2_read(addr1,addr2,pbuf,bytes);
}

int para_rm(unsigned char type)
{
	unsigned int i,addr;
	const struct paraitf *itf;
	if((type==0)||(type>PARA_MAX))
		return -1;
	type--;
	itf = &para_interface[type];
	
	addr = itf->sbase;
	for(i=0;i<itf->ss;i++){
		addr += i;
		watchdog_feed();
		flash_sector_erase(addr);
		PrintLog(LOGTYPE_SHORT,"erase para type:%d,in sectore:%d\n",type,addr);
	}
	addr = itf->sbase+itf->ss;
	for(i=0;i<itf->ss;i++){
		addr += i;
		watchdog_feed();
		flash_sector_erase(addr);
		PrintLog(LOGTYPE_SHORT,"erase para type:%d,in sectore:%d\n",type,addr);
	}
	return 0;
}

