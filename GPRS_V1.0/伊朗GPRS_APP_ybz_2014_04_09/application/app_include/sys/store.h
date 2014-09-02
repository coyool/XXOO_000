#ifndef __STORE_H
#define __STORE_H




//FLASHǰ10����������ƽ̨�ã�Ӧ�ò�ֻ�ܴӵ�10��������ʼ
#define FLASH_PARA_SECTOR_BASE			10								//����ռ��16������
#define FLASH_ALARM_SECTOR_BASE 		(FLASH_PARA_SECTOR_BASE+16)		//�澯ռ��20������
#define FLASH_CURVE_SECTOR_BASE			(FLASH_ALARM_SECTOR_BASE+20)		//����ռ��1472������
#define FLASH_DAYFRE_SECTOR_BASE		(FLASH_CURVE_SECTOR_BASE+1472)	//�ն�������ռ��224������
#define FLASH_CPYDAYFRE_SECTOR_BASE 	(FLASH_DAYFRE_SECTOR_BASE+224)	//�����ն�������ռ��14������
#define FLASH_MONTHFRE_SECTOR_BASE		(FLASH_CPYDAYFRE_SECTOR_BASE+14)	//�¶���ռ��91������
#define FLASH_TERMDAYFRE_SECTOR_BASE	(FLASH_MONTHFRE_SECTOR_BASE+91)	//�ն���ռ��32������
#define FLASH_TERMMONTHFRE_SECTOR_BASE	(FLASH_TERMDAYFRE_SECTOR_BASE+32)	//�ն���ռ��13������
#define FLASH_UPDATE_SECTOR_BASE		(FLASH_TERMMONTHFRE_SECTOR_BASE+13)//���������1892������ʼ
//#define FLASH_UPDATE_SECTOR_END			2047
#define FLASH_UPDATE_SECTOR_END			 (FLASH_UPDATE_SECTOR_BASE+32)


typedef union {
	unsigned int u;
	unsigned short us[2];
	struct {
		unsigned char tick;  // 15minute = 1
		unsigned char day;
		unsigned char month;
		unsigned char year;
	} s;
} dbtime_t;





#endif


