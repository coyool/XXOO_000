#ifndef __EEPROM_H
#define __EEPROM_H


/* EERPOM类型定义 */
#define EEPROMTYPE               64

#if (EEPROMTYPE == 1024)
    #define EE_MAX_PAGE_LEN         128
	#define EE_MAX_PAGE_NUM         1024
#elif (EEPROMTYPE == 512)
    #define EE_MAX_PAGE_LEN         128
	#define EE_MAX_PAGE_NUM         512
#elif (EEPROMTYPE == 256)
    #define EE_MAX_PAGE_LEN         64
	#define EE_MAX_PAGE_NUM         512
#elif (EEPROMTYPE == 64)
    #define EE_MAX_PAGE_LEN         32
	#define EE_MAX_PAGE_NUM         256
#elif (EEPROMTYPE == 32)
    #define EE_MAX_PAGE_LEN         32
	#define EE_MAX_PAGE_NUM         128
#elif (EEPROMTYPE == 16)
    #define EE_MAX_PAGE_LEN         16
	#define EE_MAX_PAGE_NUM         128
#elif (EEPROMTYPE == 8)
    #define EE_MAX_PAGE_LEN         16
	#define EE_MAX_PAGE_NUM         64
#endif

#define EE_PAGELEN                 EE_MAX_PAGE_LEN
#define EEPROM_DEV                ((uint8)0xA0)
#define EE_TryTimes                200

#ifndef FALSE
#define FALSE 	0
#endif
#ifndef TRUE
#define TRUE	1
#endif


/*-Export functions------------------------------------*/
uint8 WriteEepromBytes(const void *buffer, uint16 addr, uint16 lenth);
uint8 ReadEepromBytes(void *buffer, uint16 addr, uint16 lenth);
BOOL_B CheckEepromBytes(const void *buffer, uint16 addr, uint16 lenth);
void EEp_Test_Task(void);
#endif
