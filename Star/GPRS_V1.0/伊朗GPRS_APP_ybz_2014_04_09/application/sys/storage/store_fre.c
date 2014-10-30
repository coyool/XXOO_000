#include <stdio.h>

#include "app_include/sys/store.h"
#include "hal_include/flash.h"
#include "hal_include/rtc.h"
#include "hal_include/switch.h"

/*
1.block
每个类型的存储数据是以很多个block组成的，数据是以block为单位进行擦除的。block由数
个扇区组成，block完整地占用扇区
如日冻结数据，需要32天的日冻结数据，以一天的日冻结数据作为一个block，共32个block.

每个block又是由一个时标系统和很多个part组成
写数据的时候如果写日期与存储在这里的时标不同，正常情况下是就会把整个block擦除。读
数据时如果读日期和这里存储的日期不同就会返回错误。其实所谓时标系统就是总次数，二
者内在是一样的。比如日数据的第1000日就是第1000次。

某一类存储的排列形式为:             block内的排列形式为：
              -------------          /         -------------      
	         |  block 4    |     	/	   数 |  part 2     |                扇
              -------------        /           -------------                 区
             |  block 3    |      /        据 |  part 1     |                1
			  -------------      /             -------------   ......................
             |  block 2    |    /          区 |  part 0     |                扇
              -------------    /               -------------                 区
             |  block 1    |  /          时标 |     4       | PART_TIME_SIZE 0
              -------------                    -------------                   
             |  block 0    |     head(时标的) |     8       | CHKHEAD_SIZE
              -------------   -----------      -------------  ........................
2.part
part由很多个cell组成，part在flash中自由分配不受扇区限制
对于曲线数据，一个part就是一个整点数据。其它类型数据只有一个part
part可以跨扇区，一部分在这个扇区，另一部分在相临的扇区。

part的排列形式:  
			  -------------   
             |  cell 3     |  
              -------------   
		     |  cell 2     |
              ------------- 
             |  cell 1     |  
			  -------------   
             |  cell 0     |  
              -------------   

3.cell
在cell中，头8字节是标志的校验头。接下来的4字节是标志：低2字节是长度，高2字节是删
除标志。再接下来的8字节是数据区的校验头，之后是数据区
写数据的时候如果数据“长度和删除标志”不是0xffffffff，表明已被写过，返回错误.
读数据的时候如果数据长度无效或且删除标志不是0xffff0000，表明数据无效返回错误

cell排列形式：
                  -------------
          数据区 | data        |
	      		  -------------
    head(数据的) |    8        | CHKHEAD_SIZE
				  -------------
  长度和删除标志 |    4        | CELL_FLAG_SIZE
                  -------------
    head(长度的) |    8        | CHKHEAD_SIZE
                  -------------

*/
#define PART_TIME_SIZE	4	//part时标长度
#define CELL_FLAG_SIZE	4	//cell内长度和删除标志的长度
#define CELL_SIZE(X)	(CHKHEAD_SIZE*2+CELL_FLAG_SIZE+X##_DATA_SIZE)
#define PART_SIZE(X)	(X##_CELL_SIZE*X##_CELLS)
#define BLOCK_SIZE(X)	(CHKHEAD_SIZE+PART_TIME_SIZE+X##_PART_SIZE*X##_PARTS)
#define BLOCK_SECTORS(X)	(X##_BLOCK_SIZE/FLASH_SECTOR_SIZE+(X##_BLOCK_SIZE%FLASH_SECTOR_SIZE?1:0))
//#define CHKHEAD_SIZE		sizeof(struct store_head)	//校验系统长度


/*1. 曲线数据
曲线数据最小时间间隔是1小时。
曲线数据的block是一天的数据，part是一个整点数据，cell某一整点在某一天的数据
每一天在flash里面存储位置都是固定，以总天数轮循。技术要求里面是要求31天的数据。正
常换天时如果走过第31天，轮循回到第1天，就会擦掉第1天的数据，这时终端里面只有30天
的数据了。为了保证最少有31数据，应该共分配32天的空间。
*/
//曲线数据最长只允许100字节
#define CURVE_DATA_SIZE	100
//cell的长度分配为120：数据区长度100字节+数据区头长度8+长度标识区头长度8+长度标识区4。
#define CURVE_CELL_SIZE		CELL_SIZE(CURVE)
//每个block的cell数量，即保存曲线数据的测量点数量
#define CURVE_CELLS 64
//part的长度 120*64=7680
#define CURVE_PART_SIZE 	PART_SIZE(CURVE)
//part数量是24，对应24小时
#define CURVE_PARTS		24
//block长度为184332字节 头长度8+时间4+7680*24
#define CURVE_BLOCK_SIZE 	BLOCK_SIZE(CURVE)
//每个block用扇区数是46
#define CURVE_BLOCK_SECTORS 	BLOCK_SECTORS(CURVE)
//保存的曲线数据的总天数是32
#define CURVE_BLOCKS 32
//所以曲线数据占用的总扇区数是32*46=1472个扇区


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*2.日冻结数据
日冻结数据的block是一天的日冻结数据。
技术要求31天的日冻结，正常换天，如果走过第31天，再存数据就应擦除第1天的数，这时
终端里面有31的日冻结数据了。为了保证任何时候最少有31天数据，应该多存储一天。 
*/
//cell中数据长度分配400字节
#define DAYFRE_DATA_SIZE	400
//cell长度分配420字节
#define DAYFRE_CELL_SIZE	CELL_SIZE(DAYFRE)
//保存日冻结数据的测量点数
#define DAYFRE_CELLS	64
//part长度26880
#define DAYFRE_PART_SIZE	PART_SIZE(DAYFRE)
#define DAYFRE_PARTS	1
//block为26892字节
#define DAYFRE_BLOCK_SIZE		BLOCK_SIZE(DAYFRE)
//每个擦除单元占用扇区数是7
#define DAYFRE_BLOCK_SECTORS	BLOCK_SECTORS(DAYFRE)
//保存32天的日冻结
#define DAYFRE_BLOCKS	32
//日冻结数据共有7*32=224个扇区

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*3.抄表日冻结
抄表日冻结数据的block是一天的冻结数据
技术要求1个抄表日冻结，为了保证任何时候最少有1个抄表日数据，终端内最多保存2个
抄表日数据。
 */
//cell中数据长度分配400字节
#define CPYDAYFRE_DATA_SIZE	400
//cell长度分配420字节
#define CPYDAYFRE_CELL_SIZE		CELL_SIZE(CPYDAYFRE)
//保存抄表日冻结数据的测量点数
#define CPYDAYFRE_CELLS	64
//part长度26880
#define CPYDAYFRE_PART_SIZE		PART_SIZE(CPYDAYFRE)
#define CPYDAYFRE_PARTS		1
//block长度26892
#define CPYDAYFRE_BLOCK_SIZE		BLOCK_SIZE(CPYDAYFRE)
//每个擦除单元占用扇区数是7
#define CPYDAYFRE_BLOCK_SECTORS		BLOCK_SECTORS(CPYDAYFRE)
//保存2个抄表日冻结
#define CPYDAYFRE_BLOCKS	2
//日冻结数据共有7*2=14个扇区

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*4.月冻结数据
月冻结数据的block是某一月的冻结数据
技术要求12月的月冻结，正常换天，如果走过第12月，再存数据就应擦除第1月的数，这时
终端里面有11个月冻结数据了。为了保证任何时候最少有12月数据，应该多存储13月。
 */
//cell中数据长度分配400字节
#define MONTHFRE_DATA_SIZE	400
//cell长度分配420字节
#define MONTHFRE_CELL_SIZE	CELL_SIZE(MONTHFRE)
//保存月冻结数据的测量点数
#define MONTHFRE_CELLS	64
#define MONTHFRE_PART_SIZE	PART_SIZE(MONTHFRE)
#define MONTHFRE_PARTS	1
//月冻结每个擦除单元是以月数据为单元，为26892字节
#define MONTHFRE_BLOCK_SIZE		BLOCK_SIZE(MONTHFRE)
//每个擦除单元占用扇区数是7
#define MONTHFRE_BLOCK_SECTORS	BLOCK_SECTORS(MONTHFRE)
//保存13月的月冻结
#define MONTHFRE_BLOCKS	13
//日冻结数据共有7*13=91个扇区

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*
终端日冻结数
 */
//cell中数据长度分配36字节
#define TERMDAYFRE_DATA_SIZE	36
//cell长度分配56字节
#define TERMDAYFRE_CELL_SIZE	CELL_SIZE(TERMDAYFRE)
//保存测量点数
#define TERMDAYFRE_CELLS	64
//每个part为3584字节
#define TERMDAYFRE_PART_SIZE	PART_SIZE(TERMDAYFRE)
#define TERMDAYFRE_PARTS		1
//每个擦除单元3596占用扇区数是1
#define TERMDAYFRE_BLOCK_SIZE		BLOCK_SIZE(TERMDAYFRE)
#define TERMDAYFRE_BLOCK_SECTORS	BLOCK_SECTORS(TERMDAYFRE)
//保存32个擦除单元
#define TERMDAYFRE_BLOCKS	32
//终端日冻结数据共有32*1=32个扇区

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*
终端月冻结数据
 */
//cell中数据长度分配36字节
#define TERMMONTHFRE_DATA_SIZE	36
//cell长度分配56字节
#define TERMMONTHFRE_CELL_SIZE	CELL_SIZE(TERMMONTHFRE)
//保存测量点数
#define TERMMONTHFRE_CELLS	64
//每个擦除单元为3596字节
#define TERMMONTHFRE_PART_SIZE	PART_SIZE(TERMMONTHFRE)
#define TERMMONTHFRE_PARTS	1
//每个擦除单元占用扇区数是1
#define TERMMONTHFRE_BLOCK_SIZE		BLOCK_SIZE(TERMMONTHFRE)
#define TERMMONTHFRE_BLOCK_SECTORS	BLOCK_SECTORS(TERMMONTHFRE)
//保存13个擦除单元
#define TERMMONTHFRE_BLOCKS	13
//终端月冻结数据共有13*1=13个扇区
/*
static unsigned int avail_hour(dbtime_t dbtime)
{
	unsigned int time;
	struct sysclock clk;
	clk.year = bdtime.s.year;
	clk.month = dbtime.s.month;
	clk.day = dbtime.s.day;
	clk.hour = dbtime.s.tick/4;
	clk.minute = 0;
	clk.second = 0;
	time = sysclock_to_utime(&clk);
	time = time/3600;
	return time;
}
*/
static unsigned int hour_to_part(dbtime_t dbtime)
{
	if(dbtime.s.tick>=24*4)
		return 0;
	return dbtime.s.tick/4;
}
static unsigned int nopart(dbtime_t dbtime)
{
	dbtime=dbtime;
	return 0;
}

static unsigned int cal_days(dbtime_t dbtime)
{
	unsigned int time;
	struct sysclock clk;
	clk.year = dbtime.s.year;
	clk.month = dbtime.s.month;
	clk.day = dbtime.s.day;
	clk.hour = 0;
	clk.minute = 0;
	clk.second = 0;
	time = sysclock_to_utime(&clk);
	time = time/3600/24;
	return time;
}
unsigned int cal_months(dbtime_t dbtime)
{
	unsigned int time;
	time = (unsigned int)dbtime.s.year*12+dbtime.s.month-1;
	return time;
}

struct freitf
{	
	unsigned int blocks;	//每个part中block的数量
	unsigned int blockss;	//每个block占用的扇区数
	
	unsigned int parts;		//每一类数据中的part数量
	unsigned int psize;		//每个part占用的的长度
	
	unsigned int cells;		//每个block中cell的数据
	unsigned int csize;		//每个cell的长度
	unsigned int cdsize;		//每个cell中数据的长度
	
	unsigned int sbase;			//flash中的起始扇区
	unsigned int (*calblock)(dbtime_t time);	//时间转化为block的数量，part以时间为单位二者在内部是相通
	unsigned int (*calpart)(dbtime_t time);	//时间转化为part的数量，在block中的偏移
};

static const struct freitf freeze_interface[]=
{
	//日冻结
	{DAYFRE_BLOCKS,DAYFRE_BLOCK_SECTORS,
		DAYFRE_PARTS,DAYFRE_PART_SIZE,
		DAYFRE_CELLS,DAYFRE_CELL_SIZE,DAYFRE_DATA_SIZE,
		FLASH_DAYFRE_SECTOR_BASE,cal_days,nopart},
	//抄表日冻结
	{CPYDAYFRE_BLOCKS,CPYDAYFRE_BLOCK_SECTORS,
		CPYDAYFRE_PARTS,CPYDAYFRE_PART_SIZE,
		CPYDAYFRE_CELLS,CPYDAYFRE_CELL_SIZE,CPYDAYFRE_DATA_SIZE,
		FLASH_CPYDAYFRE_SECTOR_BASE,cal_days,nopart},
	//月冻结
	{MONTHFRE_BLOCKS,MONTHFRE_BLOCK_SECTORS,
		MONTHFRE_PARTS,MONTHFRE_PART_SIZE,
		MONTHFRE_CELLS,MONTHFRE_CELL_SIZE,MONTHFRE_DATA_SIZE,
		FLASH_MONTHFRE_SECTOR_BASE,cal_months,nopart},
	//曲线
	{CURVE_BLOCKS,CURVE_BLOCK_SECTORS,
		CURVE_PARTS,CURVE_PART_SIZE,
		CURVE_CELLS,CURVE_CELL_SIZE,CURVE_DATA_SIZE,
		FLASH_CURVE_SECTOR_BASE,cal_days,hour_to_part},
	//终端日冻结
	{TERMDAYFRE_BLOCKS,TERMDAYFRE_BLOCK_SECTORS,
		TERMDAYFRE_PARTS,TERMDAYFRE_PART_SIZE,
		TERMDAYFRE_CELLS,TERMDAYFRE_CELL_SIZE,TERMDAYFRE_DATA_SIZE,
		FLASH_TERMDAYFRE_SECTOR_BASE,cal_days,nopart},
	//终端月冻续
	{TERMMONTHFRE_BLOCKS,TERMMONTHFRE_BLOCK_SECTORS,
		TERMMONTHFRE_PARTS,TERMMONTHFRE_PART_SIZE,
		TERMMONTHFRE_CELLS,TERMMONTHFRE_CELL_SIZE,TERMMONTHFRE_DATA_SIZE,
		FLASH_TERMMONTHFRE_SECTOR_BASE,cal_months,nopart},
};

//#define PART_ADDR	
int fre_save(unsigned char type,dbtime_t dbtime,unsigned int mid,const void *pbuf,unsigned short bytes)
{
	unsigned int count,sector,addr,i;
	const struct freitf *itf;
	if(type>=sizeof(freeze_interface)/sizeof(freeze_interface[0]))
		return -1;
	itf = &freeze_interface[type];
	
	if(bytes > itf->cdsize)
		return -1;
	if(mid >= itf->cells)
		return -1;
	//~~~~~~~~~~block部分~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	count = (*itf->calblock)(dbtime);//其实count就是时间，二都一一对应
	sector = itf->sbase+(count%itf->blocks)*itf->blockss;
	printf("save fre %d count :%d,start sector:%d...\nin bpc mode block:%d ",
		type,count,sector,count%itf->blocks);
	addr = sector*FLASH_SECTOR_SIZE;
#if PART_TIME_SIZE!=4
	#error part time size err
#endif
	if(PART_TIME_SIZE==(f_read(addr,(unsigned char *)&i,PART_TIME_SIZE)))	//起始扇区头4+8字节，只有时间
		if(i==count)	//如果存的时间和传入时间相同，将会跳过擦除
			goto cellop;
	//擦除，重新写时标
	for(i=0;i<itf->blockss;i++){
		watchdog_feed();
		flash_sector_erase(sector+i);
	}
	f_program(addr,&count,PART_TIME_SIZE);
	printf("\nerase fre eare from sector %d to %d.\n",sector,sector+i-1);
	
cellop:
	//~~~~~~~~~~part部分~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	addr += PART_TIME_SIZE+CHKHEAD_SIZE;
	count  = (*itf->calpart)(dbtime);	//计算所在block
	addr += count*itf->psize;
	//~~~~~~~~~~cell部分~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	addr += mid*itf->csize;
	printf("part %d cell %d,in sector %d\n",count,mid,addr/FLASH_SECTOR_SIZE);
#if CELL_FLAG_SIZE!=4
	#error cell flag size err
#endif
	//flash_read(addr)
	if(CELL_FLAG_SIZE==f_read(addr,&i,CELL_FLAG_SIZE))	//有效验，说明已被写过
			return -1;
	
	i = bytes;
	i |= 0xffff0000;	//高二字节为0xFFFF，如果被擦除就标志擦除为非0xFFFF
	f_program(addr,&i,CELL_FLAG_SIZE);	//f_开头的存储有存储头
	addr+=(CELL_FLAG_SIZE+CHKHEAD_SIZE);
	f_program(addr,pbuf,bytes);
	return bytes;
}


int fre_read(unsigned char type,dbtime_t dbtime,unsigned int mid,void *pbuf,unsigned short bytes)
{
	unsigned int count,sector,addr,i;
	const struct freitf *itf;
	if(type>=sizeof(freeze_interface)/sizeof(freeze_interface[0]))
		return -1;
	itf = &freeze_interface[type];
	
	if(bytes > itf->cdsize)
		return -1;
	if(mid >= itf->cells)
		return -1;
	//~~~~~~~~~~block部分~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	count = (*itf->calblock)(dbtime);//其实count就是时间，二都一一对应
	sector = itf->sbase+(count%itf->blocks)*itf->blockss;
	addr = sector*FLASH_SECTOR_SIZE;
	printf("read fre %d count :%d,start sector:%d...\nin bpc mode block:%d ",
		type,count,sector,count%itf->blocks);
#if PART_TIME_SIZE!=4
	#error part time size err
#endif
	if(PART_TIME_SIZE!=(f_read(addr,(unsigned char *)&i,PART_TIME_SIZE)))	//读不到时间
		return -1;
	if(i!=count)	//如果存的时间和传入时间不同
		return -1;
	
	//~~~~~~~~~~part部分~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	addr += PART_TIME_SIZE+CHKHEAD_SIZE;
	count  = (*itf->calpart)(dbtime);	//计算所在block
	addr += count*itf->psize;
	
	//~~~~~~~~~~cell部分~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	addr += mid*itf->csize;
	printf("part %d cell %d,in sector %d\n",count,mid,addr/FLASH_SECTOR_SIZE);
#if CELL_FLAG_SIZE!=4
	#error cell flag size err
#endif
	if(CELL_FLAG_SIZE !=f_read(addr,&i,CELL_FLAG_SIZE))	//读文件头
		return -1;
	if((i&0xffff0000)!=0xffff0000)	//被擦除过
		return -1;
	i &= 0x0000ffff;
	if(i > bytes)	//长度不能大于
		return -1;
	addr+=(CELL_FLAG_SIZE+CHKHEAD_SIZE);
	return f_read(addr,pbuf,i);
}


int fre_rm(unsigned char type,unsigned int mid)
{
	int i,j,k;
	const struct freitf *itf;
	unsigned int addr;
	if(type>=sizeof(freeze_interface)/sizeof(freeze_interface[0]))
		return -1;
	itf = &freeze_interface[type];
	for(i=0;i<itf->blocks;i++){
		addr = itf->sbase+i*itf->blockss;	//bolck所在扇区
		addr *= FLASH_SECTOR_SIZE;			//part所在地址
		addr += PART_TIME_SIZE+CHKHEAD_SIZE;	//part内cell的起始地址
		for(j=0;j<itf->parts;j++){
			addr += j*itf->psize;					//cell所在地址
			for(k=0;k<itf->cells;k++){
				watchdog_feed();
				addr += k*itf->csize;
				if(CELL_FLAG_SIZE !=f_read(addr,&i,CELL_FLAG_SIZE))	//读文件头
					continue;		//数据无效不用删除
				if((i&0xffff0000)!=0xffff0000)
					continue;		//数据已被删除
				i &= 0xfffeffff;	//清掉第16位表示删除
				f_program(addr,&i,CELL_FLAG_SIZE);	//写清标志
			}
		}
	}
	return 0;
}

int fre_rm_time(unsigned char type,dbtime_t dbtime)
{
	unsigned int count,sector,i;
	const struct freitf * itf;
	if(type>=sizeof(freeze_interface)/sizeof(freeze_interface[0]))
		return -1;
	itf = &freeze_interface[type];;
	
	//~~~~~~~~~~block部分~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	count = (*itf->calblock)(dbtime);//其实count就是时间，二都一一对应
	sector = itf->sbase+(count%itf->blocks)*itf->blockss;

	for(i=0;i<itf->blockss;i++){
		watchdog_feed();
		flash_sector_erase(sector+i);
	}
	printf("\nerase fre %d count %d eare from sector %d to %d.\n",type,count,sector,sector+i-1);
	return 0;
}

/*
  把从time1到time2之间的区域擦掉
*/
int fre_rm_interval(dbtime_t time1,dbtime_t time2)
{
	unsigned int i,j,k,count1,count2,sector;
	const struct freitf *itf = &freeze_interface[0];
	for(i=0;i<sizeof(freeze_interface)/sizeof(freeze_interface[0]);i++){
		count1 = (*itf->calblock)(time1);
		count2 = (*itf->calblock)(time2);
		for(j=count1;j<count2;j++){
			sector = itf->sbase+(j%itf->blocks)*itf->blockss;
			for(k=0;k<itf->blockss;k++){
				watchdog_feed();
				flash_sector_erase(sector+k);
			}
		}
		itf++;
	}
	return 0;
}


int fre_rm_all(void)
{
	unsigned int i,j,k,sector;
	const struct freitf *itf = &freeze_interface[0];
	for(i=0;i<sizeof(freeze_interface)/sizeof(freeze_interface[0]);i++){
		for(j=0;j<itf->blocks;j++){
			sector = itf->sbase+j*itf->blockss;
			for(k=0;k<itf->blockss;k++){
				watchdog_feed();
				flash_sector_erase(sector+k);
			}
		}
		itf++;
	}
	return 0;
	
}


