#include "flash.h"

struct iap_record
{
	unsigned int offset;	//本条记录起始偏移
	unsigned short bytes;		//本条记录字节数
	unsigned short codecrc;		//数据区校验
};
struct iap_count
{
	unsigned int size;		//代码大小
	unsigned int records;	//record总条数
};

#define FLASH_UPDATE_SECTOR_BASE		1892//(FLASH_TERMMONTHFRE_SECTOR_BASE+13)//升级程序从1892扇区开始
#define FLASH_UPDATE_SECTOR_END			2047

#define UPDATE_FLAG	0x25137089
//代码最大长度638976
#define CODE_SIZE			((FLASH_UPDATE_SECTOR_END-FLASH_UPDATE_SECTOR_BASE+1)*FLASH_SECTOR_SIZE)
//存放升级标志
#define UPDATE_FLAG_ADDR	(FLASH_UPDATE_SECTOR_BASE*FLASH_SECTOR_SIZE)
//升级标识只能是一个int型,SIZE是12
#define UPDATE_FLAG_SIZE	(CHKHEAD_SIZE+sizeof(unsigned int))
//存放代码区长度，
#define UPDATE_COUNT_ADDR	(UPDATE_FLAG_ADDR+UPDATE_FLAG_SIZE)	
//SIZE是16
#define UPDATE_COUNT_SIZE	(CHKHEAD_SIZE+sizeof(struct iap_count))
//记录区
#define UPDATE_RECORD_ADDR		(UPDATE_COUNT_ADDR+UPDATE_COUNT_SIZE)
//每个记录size
#define UPDATE_RECORD_SIZE		(CHKHEAD_SIZE+sizeof(struct iap_record))
#define UPDATE_RECORD_SECTOR_NUM	4
//#define UPDATE_RECORD_SIZE		(CHKHEAD_SIZE)
//记录条数255
#define UPDATE_RECORDS		(((FLASH_SECTOR_SIZE*UPDATE_RECORD_SECTOR_NUM)-UPDATE_FLAG_SIZE-UPDATE_COUNT_SIZE)/UPDATE_RECORD_SIZE)
//代码区
#define UPDATE_CODE_ADDR		((FLASH_UPDATE_SECTOR_BASE+UPDATE_RECORD_SECTOR_NUM)*FLASH_SECTOR_SIZE)
