#include "flash.h"

struct iap_record
{
	unsigned int offset;	//������¼��ʼƫ��
	unsigned short bytes;		//������¼�ֽ���
	unsigned short codecrc;		//������У��
};
struct iap_count
{
	unsigned int size;		//�����С
	unsigned int records;	//record������
};

#define FLASH_UPDATE_SECTOR_BASE		1892//(FLASH_TERMMONTHFRE_SECTOR_BASE+13)//���������1892������ʼ
#define FLASH_UPDATE_SECTOR_END			2047

#define UPDATE_FLAG	0x25137089
//������󳤶�638976
#define CODE_SIZE			((FLASH_UPDATE_SECTOR_END-FLASH_UPDATE_SECTOR_BASE+1)*FLASH_SECTOR_SIZE)
//���������־
#define UPDATE_FLAG_ADDR	(FLASH_UPDATE_SECTOR_BASE*FLASH_SECTOR_SIZE)
//������ʶֻ����һ��int��,SIZE��12
#define UPDATE_FLAG_SIZE	(CHKHEAD_SIZE+sizeof(unsigned int))
//��Ŵ��������ȣ�
#define UPDATE_COUNT_ADDR	(UPDATE_FLAG_ADDR+UPDATE_FLAG_SIZE)	
//SIZE��16
#define UPDATE_COUNT_SIZE	(CHKHEAD_SIZE+sizeof(struct iap_count))
//��¼��
#define UPDATE_RECORD_ADDR		(UPDATE_COUNT_ADDR+UPDATE_COUNT_SIZE)
//ÿ����¼size
#define UPDATE_RECORD_SIZE		(CHKHEAD_SIZE+sizeof(struct iap_record))
#define UPDATE_RECORD_SECTOR_NUM	4
//#define UPDATE_RECORD_SIZE		(CHKHEAD_SIZE)
//��¼����255
#define UPDATE_RECORDS		(((FLASH_SECTOR_SIZE*UPDATE_RECORD_SECTOR_NUM)-UPDATE_FLAG_SIZE-UPDATE_COUNT_SIZE)/UPDATE_RECORD_SIZE)
//������
#define UPDATE_CODE_ADDR		((FLASH_UPDATE_SECTOR_BASE+UPDATE_RECORD_SECTOR_NUM)*FLASH_SECTOR_SIZE)
