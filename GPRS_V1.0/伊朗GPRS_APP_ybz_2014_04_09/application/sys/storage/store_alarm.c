#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "app_include/sys/store.h"
#include "hal_include/flash.h"
#include "app_include/lib/crc.h"
#include "hal_include/switch.h"


/*
1.告警区组成，第1和2个扇区作控制和控制和备份，从第3个开始是数据区


              -------------
   block2    |  sector 4   |
              -------------
   block1    |  sector 3   |
			  -------------      /
   block0    |  sector 2   |    /
              -------------    /               -------------
控制备份区   |  sector 1   |                  |     数据    |
              -------------                    -------------
    控制区   |  sector 0   |                  |     8       | CHKHEAD_SIZE
              ------------- -----------       -------------  ........................

2 block
block,是数据的擦除单元，实际上每个block就量个扇区。每个block包括n个cell
block的排列形式:
			  -------------
             |  cell 3     |
              -------------
		     |  cell 2     |
              -------------
             |  cell 1     |
			  -------------
             |  cell 0     |
              -------------
3 cell
cell是最小的数据单元，每次读写必须以cell为一个单位，
cell排列形式：
                  -------------
          数据区 | data        |
                  -------------
    head(长度的) |    8        | CHKHEAD_SIZE
                  -------------
报警数据每个元素ALARM_CELL_SIZE字节，元素是指一次告警
共存储ALARMS次的数据，
ALARM_CELL_SIZE取100,ALARMS取500,每个扇区存40次告警数据，计算得12个扇区存告警数据
还要加一个扇区以供擦除的时候，存储器里最少有12个扇区的数据。
还要两个控制扇区，一共15个扇区

*/
//#define CELL_FLAG_SIZE		4
#define ALARM_SIZE(X)		(ALARM##X##_DATA_SIZE+CHKHEAD_SIZE)
#define ALARM_CELLS(X)		(FLASH_SECTOR_SIZE/ALARM##X##_CELL_SIZE)//每个block内的cell数量
#define ALARM_BLOCKS(X)		(ALARM##X##_NEEDS/ALARM_CELLS(X)+(ALARM##X##_NEEDS%ALARM_CELLS(X)?1:0)+1)	//block总数量是实际需求的量再加1



#define FLASH_ALARM1_SECTOR_BASE	FLASH_ALARM_SECTOR_BASE
#define ALARM1_DATA_SIZE	100
#define ALARM1_CELL_SIZE	ALARM_SIZE(1)		//108字节节
#define ALARM1_NEEDS				256				//ARLAM1的总数据
#define ALARM1_CELLS		ALARM_CELLS(1)	//每个擦除单元能存37条信息
#define ALARM1_BLOCKS		ALARM_BLOCKS(1)	//共有8个block,所以实际存有8*37296条告警
#define ALARM1_SECTORS		(ALARM1_BLOCKS+2)//加两个控制区，共10个扇区

#define FLASH_ALARM2_SECTOR_BASE	FLASH_ALARM_SECTOR_BASE+ALARM1_SECTORS
#define ALARM2_DATA_SIZE	100
#define ALARM2_CELL_SIZE	ALARM_SIZE(2)		//112字节节
#define ALARM2_NEEDS		256				//ARLAM1的总数据
#define ALARM2_CELLS		ALARM_CELLS(2)	//每个擦除单元能存34条信息
#define ALARM2_BLOCKS		ALARM_BLOCKS(2)	//共有9个block
//两类告警数据，占20个扇区


struct alarmctrl
{
	int dd;
	unsigned int count;		//从上电至今总条数
	unsigned short crc;		//校验，主要用在上电后RAM丢失
};
#define ALARM_MAX 2
static struct alarmctrl alarm_ctrl[ALARM_MAX];

struct alarmitf
{
	unsigned int blocks;
	unsigned int cells;
	unsigned int csize;
	unsigned int dsize;
	unsigned int needs;
	unsigned int sbase;
	struct alarmctrl *ctrl;
};
static const struct alarmitf alarm_interface[]={
	{ALARM1_BLOCKS,ALARM1_CELLS,ALARM1_CELL_SIZE,ALARM1_DATA_SIZE,ALARM1_NEEDS,FLASH_ALARM1_SECTOR_BASE,&alarm_ctrl[0]},
	{ALARM2_BLOCKS,ALARM2_CELLS,ALARM2_CELL_SIZE,ALARM2_DATA_SIZE,ALARM2_NEEDS,FLASH_ALARM2_SECTOR_BASE,&alarm_ctrl[1]},
};

//#if ALARM_MAX!=(sizeof(alarm_interface)/sizeof(alarm_interface[0]))
//	#error alarm number error!
//#endif


#define ALARM_CTRL_ADDR (FLASH_ALARM_SECTOR_BASE+ALARM_SECTOR_CTRL_OFFSET)*FLASH_SECTOR_SIZE)
#define ALARM_CTRL_BACK_ADDR (FLASH_ALARM_SECTOR_BASE+ALARM_SECTOR_CTRL_BACK_OFFSET)*FLASH_SECTOR_SIZE)
static void alarm_ctrl_check(unsigned char type)
{
	const struct alarmitf *itf;
	if(type>=ALARM_MAX)
		return;
	itf=&alarm_interface[type];
	//先查看RAM里的数据
	if(itf->ctrl->crc==calculate_crc((const unsigned char*)itf->ctrl,offsetof(struct alarmctrl,crc)))
		return;
	//再查看flash里的数据
	if(f2_read(itf->sbase,itf->sbase+1,itf->ctrl,sizeof(struct alarmctrl))>0)
		if(itf->ctrl->crc==calculate_crc((const unsigned char*)itf->ctrl,offsetof(struct alarmctrl,crc)))
			return;
	//初始化
	itf->ctrl->count=0;
	//itf->ctrl->count--;//存储的时候从0开始
	itf->ctrl->crc = calculate_crc((const unsigned char*)itf->ctrl,offsetof(struct alarmctrl,crc));
	f2_write(itf->sbase,itf->sbase+1,itf->ctrl,sizeof(struct alarmctrl));
}

/*
 *保存告警数据，每次只能存一条
 *@type:类型，只能是0或者1
 *@pbuf:数据指针
 *@bytes:待保存数据长度
 *@return:-1 失败
		  >=0 实际写入的数据，正常情况下就是bytes
 */
int alarm_save(unsigned char type,const void *pbuf,unsigned short bytes)
{
	const struct alarmitf *itf;
	unsigned int i,n,addr;
	if(type>=ALARM_MAX)
		return -1;
	itf=&alarm_interface[type];
	if(bytes>itf->dsize)
		return -1;
	alarm_ctrl_check(type);


	addr = itf->sbase+2;	//数据区起始扇区
	n = itf->ctrl->count % ALARM1_NEEDS;    //转换
	addr += (n/itf->cells)%itf->blocks;	//所在block的扇区
	i = n % itf->cells;	//在某一个扇区的偏移
	printf("save alarm %d int sector:%d,offset:%d,alarm number:%d\n",type,addr,i,itf->ctrl->count);
	if(i==0){//换扇区
		flash_sector_erase(addr);
		printf("alarm to next sector,ther sectore is %d\n",addr);
	}
	addr *= FLASH_SECTOR_SIZE;	//所在扇区地址
	addr += i*itf->csize;	//被写地址
	f_program(addr,pbuf,bytes);

	itf->ctrl->count++;		
	itf->ctrl->crc = calculate_crc((const unsigned char*)itf->ctrl,offsetof(struct alarmctrl,crc));
	f2_write(itf->sbase,itf->sbase+1,itf->ctrl,sizeof(struct alarmctrl));
	return bytes;
}

/*
 *读取告警数据，每次只能读一条
 *@type:类型，0或者1
 *@cnt:读取第几次 0~255
 *@pbuf:数据指针
 *@bytes:数据区长度
 *@return:-1 失败
          >=0 实际有效数据长度
 */
int alarm_read(unsigned char type,unsigned int cnt,void *pbuf,unsigned short bytes)
{
	unsigned int i,addr;
	const struct alarmitf *itf;
	if(type>=ALARM_MAX)
		return -1;
	itf=&alarm_interface[type];
	if(bytes>itf->dsize)
		return -1;
	if(cnt>=itf->needs)	//不能大于需求最大值
		return -1;
	alarm_ctrl_check(type);

	addr = itf->sbase+2;
	addr += (cnt/itf->cells)%itf->blocks;//当前被存扇区
	i = cnt%itf->cells;//在某一个扇区的偏移
	printf("read alarm %d int sector:%d,offset:%d,alarm number:%d\n",type,addr,i,cnt);
	addr *= FLASH_SECTOR_SIZE;	//扇区地址
	addr += i*itf->csize;
	return f_read(addr,pbuf,bytes);
}

int alarm_rm(void)
{
	int i,j,addr;
	const struct alarmitf *itf;
	for(i=0;i<ALARM_MAX;i++){
		itf = &alarm_interface[i];
		itf->ctrl->count=0;
		//itf->ctrl->count--;//存储的时候从0开始
		itf->ctrl->crc = calculate_crc((const unsigned char*)itf->ctrl,offsetof(struct alarmctrl,crc));
		f2_write(itf->sbase,itf->sbase+1,itf->ctrl,sizeof(struct alarmctrl));

		addr = itf->sbase+2;
		for(j=0;j<itf->blocks;j++){
			watchdog_feed();
			addr += j;
			flash_sector_erase(addr);
			printf("erase alarm  sector:%d\n",addr);
		}
	}
	return 0;
}

int alarm_count(unsigned char type)
{
	const struct alarmitf *itf;
	if(type>=ALARM_MAX)
		return -1;
	itf=&alarm_interface[type];
	alarm_ctrl_check(type);

	return itf->ctrl->count;
}
