#include <stddef.h>
#include <string.h>
#include "plat_include/platpara.h"
#include "plat_include/sys_config.h"
#include "hal_include/flash.h"


struct platform_para platpara;


struct platparaitf
{
	void *point;
	unsigned int size;
};


static const struct platparaitf platpara_interface[]={
	{&platpara.printport,sizeof(platpara.printport)},
	{&platpara.ipaddr[0],sizeof(platpara.ipaddr)},
	{&platpara.ethaddr[0],sizeof(platpara.ethaddr)}
};
#define PLATPARA_MAX	(sizeof(platpara_interface)/sizeof(platpara_interface[0]))
//#define PLATPARA_ADDR1	(FLASH_PFPARA_SECTOR*FLASH_SECTOR_SIZE)
//#define PLATPARA_ADDR2	(FLASH_PFPARA_BACK_SECTOR*FLASH_SECTOR_SIZE)
void platpara_init(void)
{
	if((f2_read(FLASH_PFPARA_SECTOR,FLASH_PFPARA_BACK_SECTOR,&platpara,sizeof(platpara)))>0)
		return;
	platpara.printport = 3;

	platpara.ipaddr[0] = 192;
	platpara.ipaddr[1] = 168;
	platpara.ipaddr[2] = 0;
	platpara.ipaddr[3] = 100;
	platpara.ethaddr[0] = 0x00;
	platpara.ethaddr[1] = 0xff;
	platpara.ethaddr[2] = 0xff;
	platpara.ethaddr[3] = 0xff;
	platpara.ethaddr[4] = 0xff;
	platpara.ethaddr[5] = 0xff;
	f2_write(FLASH_PFPARA_SECTOR,FLASH_PFPARA_BACK_SECTOR,&platpara,sizeof(platpara));
}
/**
 *读平台参数
 @type:类型
 @pbuf:缓冲区
 @len:长度
	PLATPARA_UARTCFG
 @return 0失败，非0正常数据长度
 */

int platpara_read(unsigned char type,void *pbuf,unsigned int len)
{
	const struct platparaitf *itf;
	if(type>=PLATPARA_MAX)
		return -1;
	itf=&platpara_interface[type];
	if(len<itf->size)
		return -1;
	memcpy(pbuf,itf->point,itf->size);
	return itf->size;

}

int platpara_write(unsigned char type,const void *pbuf,unsigned int len)
{
	const struct platparaitf *itf;
	if(type>=PLATPARA_MAX)
		return -1;
	itf=&platpara_interface[type];
	if(len>itf->size)
		return -1;

	//参数相等，不必重写
	if(memcmp(itf->point,pbuf,len)==0)
		return 0;
	memcpy(itf->point,pbuf,len);
	f2_write(FLASH_PFPARA_SECTOR,FLASH_PFPARA_BACK_SECTOR,&platpara,sizeof(platpara));
	return itf->size;
}
