#include <stdio.h>

#include "app_include/sys/store.h"
#include "hal_include/flash.h"
#include "hal_include/rtc.h"
#include "hal_include/switch.h"

/*
1.block
ÿ�����͵Ĵ洢�������Ժܶ��block��ɵģ���������blockΪ��λ���в����ġ�block����
��������ɣ�block������ռ������
���ն������ݣ���Ҫ32����ն������ݣ���һ����ն���������Ϊһ��block����32��block.

ÿ��block������һ��ʱ��ϵͳ�ͺܶ��part���
д���ݵ�ʱ�����д������洢�������ʱ�겻ͬ������������Ǿͻ������block��������
����ʱ��������ں�����洢�����ڲ�ͬ�ͻ᷵�ش�����ʵ��νʱ��ϵͳ�����ܴ�������
��������һ���ġ����������ݵĵ�1000�վ��ǵ�1000�Ρ�

ĳһ��洢��������ʽΪ:             block�ڵ�������ʽΪ��
              -------------          /         -------------      
	         |  block 4    |     	/	   �� |  part 2     |                ��
              -------------        /           -------------                 ��
             |  block 3    |      /        �� |  part 1     |                1
			  -------------      /             -------------   ......................
             |  block 2    |    /          �� |  part 0     |                ��
              -------------    /               -------------                 ��
             |  block 1    |  /          ʱ�� |     4       | PART_TIME_SIZE 0
              -------------                    -------------                   
             |  block 0    |     head(ʱ���) |     8       | CHKHEAD_SIZE
              -------------   -----------      -------------  ........................
2.part
part�ɺܶ��cell��ɣ�part��flash�����ɷ��䲻����������
�����������ݣ�һ��part����һ���������ݡ�������������ֻ��һ��part
part���Կ�������һ�����������������һ���������ٵ�������

part��������ʽ:  
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
��cell�У�ͷ8�ֽ��Ǳ�־��У��ͷ����������4�ֽ��Ǳ�־����2�ֽ��ǳ��ȣ���2�ֽ���ɾ
����־���ٽ�������8�ֽ�����������У��ͷ��֮����������
д���ݵ�ʱ��������ݡ����Ⱥ�ɾ����־������0xffffffff�������ѱ�д�������ش���.
�����ݵ�ʱ��������ݳ�����Ч����ɾ����־����0xffff0000������������Ч���ش���

cell������ʽ��
                  -------------
          ������ | data        |
	      		  -------------
    head(���ݵ�) |    8        | CHKHEAD_SIZE
				  -------------
  ���Ⱥ�ɾ����־ |    4        | CELL_FLAG_SIZE
                  -------------
    head(���ȵ�) |    8        | CHKHEAD_SIZE
                  -------------

*/
#define PART_TIME_SIZE	4	//partʱ�곤��
#define CELL_FLAG_SIZE	4	//cell�ڳ��Ⱥ�ɾ����־�ĳ���
#define CELL_SIZE(X)	(CHKHEAD_SIZE*2+CELL_FLAG_SIZE+X##_DATA_SIZE)
#define PART_SIZE(X)	(X##_CELL_SIZE*X##_CELLS)
#define BLOCK_SIZE(X)	(CHKHEAD_SIZE+PART_TIME_SIZE+X##_PART_SIZE*X##_PARTS)
#define BLOCK_SECTORS(X)	(X##_BLOCK_SIZE/FLASH_SECTOR_SIZE+(X##_BLOCK_SIZE%FLASH_SECTOR_SIZE?1:0))
//#define CHKHEAD_SIZE		sizeof(struct store_head)	//У��ϵͳ����


/*1. ��������
����������Сʱ������1Сʱ��
�������ݵ�block��һ������ݣ�part��һ���������ݣ�cellĳһ������ĳһ�������
ÿһ����flash����洢λ�ö��ǹ̶�������������ѭ������Ҫ��������Ҫ��31������ݡ���
������ʱ����߹���31�죬��ѭ�ص���1�죬�ͻ������1������ݣ���ʱ�ն�����ֻ��30��
�������ˡ�Ϊ�˱�֤������31���ݣ�Ӧ�ù�����32��Ŀռ䡣
*/
//���������ֻ����100�ֽ�
#define CURVE_DATA_SIZE	100
//cell�ĳ��ȷ���Ϊ120������������100�ֽ�+������ͷ����8+���ȱ�ʶ��ͷ����8+���ȱ�ʶ��4��
#define CURVE_CELL_SIZE		CELL_SIZE(CURVE)
//ÿ��block��cell�������������������ݵĲ���������
#define CURVE_CELLS 64
//part�ĳ��� 120*64=7680
#define CURVE_PART_SIZE 	PART_SIZE(CURVE)
//part������24����Ӧ24Сʱ
#define CURVE_PARTS		24
//block����Ϊ184332�ֽ� ͷ����8+ʱ��4+7680*24
#define CURVE_BLOCK_SIZE 	BLOCK_SIZE(CURVE)
//ÿ��block����������46
#define CURVE_BLOCK_SECTORS 	BLOCK_SECTORS(CURVE)
//������������ݵ���������32
#define CURVE_BLOCKS 32
//������������ռ�õ�����������32*46=1472������


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*2.�ն�������
�ն������ݵ�block��һ����ն������ݡ�
����Ҫ��31����ն��ᣬ�������죬����߹���31�죬�ٴ����ݾ�Ӧ������1���������ʱ
�ն�������31���ն��������ˡ�Ϊ�˱�֤�κ�ʱ��������31�����ݣ�Ӧ�ö�洢һ�졣 
*/
//cell�����ݳ��ȷ���400�ֽ�
#define DAYFRE_DATA_SIZE	400
//cell���ȷ���420�ֽ�
#define DAYFRE_CELL_SIZE	CELL_SIZE(DAYFRE)
//�����ն������ݵĲ�������
#define DAYFRE_CELLS	64
//part����26880
#define DAYFRE_PART_SIZE	PART_SIZE(DAYFRE)
#define DAYFRE_PARTS	1
//blockΪ26892�ֽ�
#define DAYFRE_BLOCK_SIZE		BLOCK_SIZE(DAYFRE)
//ÿ��������Ԫռ����������7
#define DAYFRE_BLOCK_SECTORS	BLOCK_SECTORS(DAYFRE)
//����32����ն���
#define DAYFRE_BLOCKS	32
//�ն������ݹ���7*32=224������

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*3.�����ն���
�����ն������ݵ�block��һ��Ķ�������
����Ҫ��1�������ն��ᣬΪ�˱�֤�κ�ʱ��������1�����������ݣ��ն�����ౣ��2��
���������ݡ�
 */
//cell�����ݳ��ȷ���400�ֽ�
#define CPYDAYFRE_DATA_SIZE	400
//cell���ȷ���420�ֽ�
#define CPYDAYFRE_CELL_SIZE		CELL_SIZE(CPYDAYFRE)
//���泭���ն������ݵĲ�������
#define CPYDAYFRE_CELLS	64
//part����26880
#define CPYDAYFRE_PART_SIZE		PART_SIZE(CPYDAYFRE)
#define CPYDAYFRE_PARTS		1
//block����26892
#define CPYDAYFRE_BLOCK_SIZE		BLOCK_SIZE(CPYDAYFRE)
//ÿ��������Ԫռ����������7
#define CPYDAYFRE_BLOCK_SECTORS		BLOCK_SECTORS(CPYDAYFRE)
//����2�������ն���
#define CPYDAYFRE_BLOCKS	2
//�ն������ݹ���7*2=14������

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*4.�¶�������
�¶������ݵ�block��ĳһ�µĶ�������
����Ҫ��12�µ��¶��ᣬ�������죬����߹���12�£��ٴ����ݾ�Ӧ������1�µ�������ʱ
�ն�������11���¶��������ˡ�Ϊ�˱�֤�κ�ʱ��������12�����ݣ�Ӧ�ö�洢13�¡�
 */
//cell�����ݳ��ȷ���400�ֽ�
#define MONTHFRE_DATA_SIZE	400
//cell���ȷ���420�ֽ�
#define MONTHFRE_CELL_SIZE	CELL_SIZE(MONTHFRE)
//�����¶������ݵĲ�������
#define MONTHFRE_CELLS	64
#define MONTHFRE_PART_SIZE	PART_SIZE(MONTHFRE)
#define MONTHFRE_PARTS	1
//�¶���ÿ��������Ԫ����������Ϊ��Ԫ��Ϊ26892�ֽ�
#define MONTHFRE_BLOCK_SIZE		BLOCK_SIZE(MONTHFRE)
//ÿ��������Ԫռ����������7
#define MONTHFRE_BLOCK_SECTORS	BLOCK_SECTORS(MONTHFRE)
//����13�µ��¶���
#define MONTHFRE_BLOCKS	13
//�ն������ݹ���7*13=91������

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*
�ն��ն�����
 */
//cell�����ݳ��ȷ���36�ֽ�
#define TERMDAYFRE_DATA_SIZE	36
//cell���ȷ���56�ֽ�
#define TERMDAYFRE_CELL_SIZE	CELL_SIZE(TERMDAYFRE)
//�����������
#define TERMDAYFRE_CELLS	64
//ÿ��partΪ3584�ֽ�
#define TERMDAYFRE_PART_SIZE	PART_SIZE(TERMDAYFRE)
#define TERMDAYFRE_PARTS		1
//ÿ��������Ԫ3596ռ����������1
#define TERMDAYFRE_BLOCK_SIZE		BLOCK_SIZE(TERMDAYFRE)
#define TERMDAYFRE_BLOCK_SECTORS	BLOCK_SECTORS(TERMDAYFRE)
//����32��������Ԫ
#define TERMDAYFRE_BLOCKS	32
//�ն��ն������ݹ���32*1=32������

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*
�ն��¶�������
 */
//cell�����ݳ��ȷ���36�ֽ�
#define TERMMONTHFRE_DATA_SIZE	36
//cell���ȷ���56�ֽ�
#define TERMMONTHFRE_CELL_SIZE	CELL_SIZE(TERMMONTHFRE)
//�����������
#define TERMMONTHFRE_CELLS	64
//ÿ��������ԪΪ3596�ֽ�
#define TERMMONTHFRE_PART_SIZE	PART_SIZE(TERMMONTHFRE)
#define TERMMONTHFRE_PARTS	1
//ÿ��������Ԫռ����������1
#define TERMMONTHFRE_BLOCK_SIZE		BLOCK_SIZE(TERMMONTHFRE)
#define TERMMONTHFRE_BLOCK_SECTORS	BLOCK_SECTORS(TERMMONTHFRE)
//����13��������Ԫ
#define TERMMONTHFRE_BLOCKS	13
//�ն��¶������ݹ���13*1=13������
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
	unsigned int blocks;	//ÿ��part��block������
	unsigned int blockss;	//ÿ��blockռ�õ�������
	
	unsigned int parts;		//ÿһ�������е�part����
	unsigned int psize;		//ÿ��partռ�õĵĳ���
	
	unsigned int cells;		//ÿ��block��cell������
	unsigned int csize;		//ÿ��cell�ĳ���
	unsigned int cdsize;		//ÿ��cell�����ݵĳ���
	
	unsigned int sbase;			//flash�е���ʼ����
	unsigned int (*calblock)(dbtime_t time);	//ʱ��ת��Ϊblock��������part��ʱ��Ϊ��λ�������ڲ�����ͨ
	unsigned int (*calpart)(dbtime_t time);	//ʱ��ת��Ϊpart����������block�е�ƫ��
};

static const struct freitf freeze_interface[]=
{
	//�ն���
	{DAYFRE_BLOCKS,DAYFRE_BLOCK_SECTORS,
		DAYFRE_PARTS,DAYFRE_PART_SIZE,
		DAYFRE_CELLS,DAYFRE_CELL_SIZE,DAYFRE_DATA_SIZE,
		FLASH_DAYFRE_SECTOR_BASE,cal_days,nopart},
	//�����ն���
	{CPYDAYFRE_BLOCKS,CPYDAYFRE_BLOCK_SECTORS,
		CPYDAYFRE_PARTS,CPYDAYFRE_PART_SIZE,
		CPYDAYFRE_CELLS,CPYDAYFRE_CELL_SIZE,CPYDAYFRE_DATA_SIZE,
		FLASH_CPYDAYFRE_SECTOR_BASE,cal_days,nopart},
	//�¶���
	{MONTHFRE_BLOCKS,MONTHFRE_BLOCK_SECTORS,
		MONTHFRE_PARTS,MONTHFRE_PART_SIZE,
		MONTHFRE_CELLS,MONTHFRE_CELL_SIZE,MONTHFRE_DATA_SIZE,
		FLASH_MONTHFRE_SECTOR_BASE,cal_months,nopart},
	//����
	{CURVE_BLOCKS,CURVE_BLOCK_SECTORS,
		CURVE_PARTS,CURVE_PART_SIZE,
		CURVE_CELLS,CURVE_CELL_SIZE,CURVE_DATA_SIZE,
		FLASH_CURVE_SECTOR_BASE,cal_days,hour_to_part},
	//�ն��ն���
	{TERMDAYFRE_BLOCKS,TERMDAYFRE_BLOCK_SECTORS,
		TERMDAYFRE_PARTS,TERMDAYFRE_PART_SIZE,
		TERMDAYFRE_CELLS,TERMDAYFRE_CELL_SIZE,TERMDAYFRE_DATA_SIZE,
		FLASH_TERMDAYFRE_SECTOR_BASE,cal_days,nopart},
	//�ն��¶���
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
	//~~~~~~~~~~block����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	count = (*itf->calblock)(dbtime);//��ʵcount����ʱ�䣬����һһ��Ӧ
	sector = itf->sbase+(count%itf->blocks)*itf->blockss;
	printf("save fre %d count :%d,start sector:%d...\nin bpc mode block:%d ",
		type,count,sector,count%itf->blocks);
	addr = sector*FLASH_SECTOR_SIZE;
#if PART_TIME_SIZE!=4
	#error part time size err
#endif
	if(PART_TIME_SIZE==(f_read(addr,(unsigned char *)&i,PART_TIME_SIZE)))	//��ʼ����ͷ4+8�ֽڣ�ֻ��ʱ��
		if(i==count)	//������ʱ��ʹ���ʱ����ͬ��������������
			goto cellop;
	//����������дʱ��
	for(i=0;i<itf->blockss;i++){
		watchdog_feed();
		flash_sector_erase(sector+i);
	}
	f_program(addr,&count,PART_TIME_SIZE);
	printf("\nerase fre eare from sector %d to %d.\n",sector,sector+i-1);
	
cellop:
	//~~~~~~~~~~part����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	addr += PART_TIME_SIZE+CHKHEAD_SIZE;
	count  = (*itf->calpart)(dbtime);	//��������block
	addr += count*itf->psize;
	//~~~~~~~~~~cell����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	addr += mid*itf->csize;
	printf("part %d cell %d,in sector %d\n",count,mid,addr/FLASH_SECTOR_SIZE);
#if CELL_FLAG_SIZE!=4
	#error cell flag size err
#endif
	//flash_read(addr)
	if(CELL_FLAG_SIZE==f_read(addr,&i,CELL_FLAG_SIZE))	//��Ч�飬˵���ѱ�д��
			return -1;
	
	i = bytes;
	i |= 0xffff0000;	//�߶��ֽ�Ϊ0xFFFF������������ͱ�־����Ϊ��0xFFFF
	f_program(addr,&i,CELL_FLAG_SIZE);	//f_��ͷ�Ĵ洢�д洢ͷ
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
	//~~~~~~~~~~block����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	count = (*itf->calblock)(dbtime);//��ʵcount����ʱ�䣬����һһ��Ӧ
	sector = itf->sbase+(count%itf->blocks)*itf->blockss;
	addr = sector*FLASH_SECTOR_SIZE;
	printf("read fre %d count :%d,start sector:%d...\nin bpc mode block:%d ",
		type,count,sector,count%itf->blocks);
#if PART_TIME_SIZE!=4
	#error part time size err
#endif
	if(PART_TIME_SIZE!=(f_read(addr,(unsigned char *)&i,PART_TIME_SIZE)))	//������ʱ��
		return -1;
	if(i!=count)	//������ʱ��ʹ���ʱ�䲻ͬ
		return -1;
	
	//~~~~~~~~~~part����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	addr += PART_TIME_SIZE+CHKHEAD_SIZE;
	count  = (*itf->calpart)(dbtime);	//��������block
	addr += count*itf->psize;
	
	//~~~~~~~~~~cell����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	addr += mid*itf->csize;
	printf("part %d cell %d,in sector %d\n",count,mid,addr/FLASH_SECTOR_SIZE);
#if CELL_FLAG_SIZE!=4
	#error cell flag size err
#endif
	if(CELL_FLAG_SIZE !=f_read(addr,&i,CELL_FLAG_SIZE))	//���ļ�ͷ
		return -1;
	if((i&0xffff0000)!=0xffff0000)	//��������
		return -1;
	i &= 0x0000ffff;
	if(i > bytes)	//���Ȳ��ܴ���
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
		addr = itf->sbase+i*itf->blockss;	//bolck��������
		addr *= FLASH_SECTOR_SIZE;			//part���ڵ�ַ
		addr += PART_TIME_SIZE+CHKHEAD_SIZE;	//part��cell����ʼ��ַ
		for(j=0;j<itf->parts;j++){
			addr += j*itf->psize;					//cell���ڵ�ַ
			for(k=0;k<itf->cells;k++){
				watchdog_feed();
				addr += k*itf->csize;
				if(CELL_FLAG_SIZE !=f_read(addr,&i,CELL_FLAG_SIZE))	//���ļ�ͷ
					continue;		//������Ч����ɾ��
				if((i&0xffff0000)!=0xffff0000)
					continue;		//�����ѱ�ɾ��
				i &= 0xfffeffff;	//�����16λ��ʾɾ��
				f_program(addr,&i,CELL_FLAG_SIZE);	//д���־
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
	
	//~~~~~~~~~~block����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	count = (*itf->calblock)(dbtime);//��ʵcount����ʱ�䣬����һһ��Ӧ
	sector = itf->sbase+(count%itf->blocks)*itf->blockss;

	for(i=0;i<itf->blockss;i++){
		watchdog_feed();
		flash_sector_erase(sector+i);
	}
	printf("\nerase fre %d count %d eare from sector %d to %d.\n",type,count,sector,sector+i-1);
	return 0;
}

/*
  �Ѵ�time1��time2֮����������
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


