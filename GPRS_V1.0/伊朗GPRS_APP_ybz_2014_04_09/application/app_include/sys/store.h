#ifndef __STORE_H
#define __STORE_H




//FLASH前10个扇区留给平台用，应用层只能从第10个扇区开始
#define FLASH_PARA_SECTOR_BASE			10								//参数占用16个扇区
#define FLASH_ALARM_SECTOR_BASE 		(FLASH_PARA_SECTOR_BASE+16)		//告警占用20个扇区
#define FLASH_CURVE_SECTOR_BASE			(FLASH_ALARM_SECTOR_BASE+20)		//曲线占用1472个扇区
#define FLASH_DAYFRE_SECTOR_BASE		(FLASH_CURVE_SECTOR_BASE+1472)	//日冻结数据占用224个扇区
#define FLASH_CPYDAYFRE_SECTOR_BASE 	(FLASH_DAYFRE_SECTOR_BASE+224)	//抄表日冻结数据占用14个扇区
#define FLASH_MONTHFRE_SECTOR_BASE		(FLASH_CPYDAYFRE_SECTOR_BASE+14)	//月冻结占用91个扇区
#define FLASH_TERMDAYFRE_SECTOR_BASE	(FLASH_MONTHFRE_SECTOR_BASE+91)	//终端日占用32个扇区
#define FLASH_TERMMONTHFRE_SECTOR_BASE	(FLASH_TERMDAYFRE_SECTOR_BASE+32)	//终端月占用13个扇区
#define FLASH_UPDATE_SECTOR_BASE		(FLASH_TERMMONTHFRE_SECTOR_BASE+13)//升级程序从1892扇区开始
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


