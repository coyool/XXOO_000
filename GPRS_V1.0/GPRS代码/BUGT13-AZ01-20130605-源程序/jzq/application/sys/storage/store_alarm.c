#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "app_include/sys/store.h"
#include "hal_include/flash.h"
#include "app_include/lib/crc.h"
#include "hal_include/switch.h"


/*
1.�澯����ɣ���1��2�����������ƺͿ��ƺͱ��ݣ��ӵ�3����ʼ��������


              -------------
   block2    |  sector 4   |
              -------------
   block1    |  sector 3   |
			  -------------      /
   block0    |  sector 2   |    /
              -------------    /               -------------
���Ʊ�����   |  sector 1   |                  |     ����    |
              -------------                    -------------
    ������   |  sector 0   |                  |     8       | CHKHEAD_SIZE
              ------------- -----------       -------------  ........................

2 block
block,�����ݵĲ�����Ԫ��ʵ����ÿ��block������������ÿ��block����n��cell
block��������ʽ:
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
cell����С�����ݵ�Ԫ��ÿ�ζ�д������cellΪһ����λ��
cell������ʽ��
                  -------------
          ������ | data        |
                  -------------
    head(���ȵ�) |    8        | CHKHEAD_SIZE
                  -------------
��������ÿ��Ԫ��ALARM_CELL_SIZE�ֽڣ�Ԫ����ָһ�θ澯
���洢ALARMS�ε����ݣ�
ALARM_CELL_SIZEȡ100,ALARMSȡ500,ÿ��������40�θ澯���ݣ������12��������澯����
��Ҫ��һ�������Թ�������ʱ�򣬴洢����������12�����������ݡ�
��Ҫ��������������һ��15������

*/
//#define CELL_FLAG_SIZE		4
#define ALARM_SIZE(X)		(ALARM##X##_DATA_SIZE+CHKHEAD_SIZE)
#define ALARM_CELLS(X)		(FLASH_SECTOR_SIZE/ALARM##X##_CELL_SIZE)//ÿ��block�ڵ�cell����
#define ALARM_BLOCKS(X)		(ALARM##X##_NEEDS/ALARM_CELLS(X)+(ALARM##X##_NEEDS%ALARM_CELLS(X)?1:0)+1)	//block��������ʵ����������ټ�1



#define FLASH_ALARM1_SECTOR_BASE	FLASH_ALARM_SECTOR_BASE
#define ALARM1_DATA_SIZE	100
#define ALARM1_CELL_SIZE	ALARM_SIZE(1)		//108�ֽڽ�
#define ALARM1_NEEDS				256				//ARLAM1��������
#define ALARM1_CELLS		ALARM_CELLS(1)	//ÿ��������Ԫ�ܴ�37����Ϣ
#define ALARM1_BLOCKS		ALARM_BLOCKS(1)	//����8��block,����ʵ�ʴ���8*37296���澯
#define ALARM1_SECTORS		(ALARM1_BLOCKS+2)//����������������10������

#define FLASH_ALARM2_SECTOR_BASE	FLASH_ALARM_SECTOR_BASE+ALARM1_SECTORS
#define ALARM2_DATA_SIZE	100
#define ALARM2_CELL_SIZE	ALARM_SIZE(2)		//112�ֽڽ�
#define ALARM2_NEEDS		256				//ARLAM1��������
#define ALARM2_CELLS		ALARM_CELLS(2)	//ÿ��������Ԫ�ܴ�34����Ϣ
#define ALARM2_BLOCKS		ALARM_BLOCKS(2)	//����9��block
//����澯���ݣ�ռ20������


struct alarmctrl
{
	int dd;
	unsigned int count;		//���ϵ�����������
	unsigned short crc;		//У�飬��Ҫ�����ϵ��RAM��ʧ
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
	//�Ȳ鿴RAM�������
	if(itf->ctrl->crc==calculate_crc((const unsigned char*)itf->ctrl,offsetof(struct alarmctrl,crc)))
		return;
	//�ٲ鿴flash�������
	if(f2_read(itf->sbase,itf->sbase+1,itf->ctrl,sizeof(struct alarmctrl))>0)
		if(itf->ctrl->crc==calculate_crc((const unsigned char*)itf->ctrl,offsetof(struct alarmctrl,crc)))
			return;
	//��ʼ��
	itf->ctrl->count=0;
	//itf->ctrl->count--;//�洢��ʱ���0��ʼ
	itf->ctrl->crc = calculate_crc((const unsigned char*)itf->ctrl,offsetof(struct alarmctrl,crc));
	f2_write(itf->sbase,itf->sbase+1,itf->ctrl,sizeof(struct alarmctrl));
}

/*
 *����澯���ݣ�ÿ��ֻ�ܴ�һ��
 *@type:���ͣ�ֻ����0����1
 *@pbuf:����ָ��
 *@bytes:���������ݳ���
 *@return:-1 ʧ��
		  >=0 ʵ��д������ݣ���������¾���bytes
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


	addr = itf->sbase+2;	//��������ʼ����
	n = itf->ctrl->count % ALARM1_NEEDS;    //ת��
	addr += (n/itf->cells)%itf->blocks;	//����block������
	i = n % itf->cells;	//��ĳһ��������ƫ��
	printf("save alarm %d int sector:%d,offset:%d,alarm number:%d\n",type,addr,i,itf->ctrl->count);
	if(i==0){//������
		flash_sector_erase(addr);
		printf("alarm to next sector,ther sectore is %d\n",addr);
	}
	addr *= FLASH_SECTOR_SIZE;	//����������ַ
	addr += i*itf->csize;	//��д��ַ
	f_program(addr,pbuf,bytes);

	itf->ctrl->count++;		
	itf->ctrl->crc = calculate_crc((const unsigned char*)itf->ctrl,offsetof(struct alarmctrl,crc));
	f2_write(itf->sbase,itf->sbase+1,itf->ctrl,sizeof(struct alarmctrl));
	return bytes;
}

/*
 *��ȡ�澯���ݣ�ÿ��ֻ�ܶ�һ��
 *@type:���ͣ�0����1
 *@cnt:��ȡ�ڼ��� 0~255
 *@pbuf:����ָ��
 *@bytes:����������
 *@return:-1 ʧ��
          >=0 ʵ����Ч���ݳ���
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
	if(cnt>=itf->needs)	//���ܴ����������ֵ
		return -1;
	alarm_ctrl_check(type);

	addr = itf->sbase+2;
	addr += (cnt/itf->cells)%itf->blocks;//��ǰ��������
	i = cnt%itf->cells;//��ĳһ��������ƫ��
	printf("read alarm %d int sector:%d,offset:%d,alarm number:%d\n",type,addr,i,cnt);
	addr *= FLASH_SECTOR_SIZE;	//������ַ
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
		//itf->ctrl->count--;//�洢��ʱ���0��ʼ
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
