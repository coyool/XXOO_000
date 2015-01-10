#ifndef __DATASTRUCTTAB_H
#define __DATASTRUCTTAB_H
//#include "MeterInfo.h"
#include "Include.h"

#define EE_SPACE_MAX_SIZE				(64*128)		//单位为byte
#define EE_BAK_BASIC_ADDR				 0					//带备份数据EEPROM开始地址
#define EE_BAK_SPACE_SIZE				(EE_SPACE_MAX_SIZE*2/4)

#define EE_ADDR_BASIC					(EE_BAK_SPACE_SIZE)	//不带备份数据的EEPROM起始基址

#define FLASH_SPACE_MAX_SIZE			((uint32)4*1024*1024)	//单位为byte
#define FLASH_ADDR_DISTINGUISH_ADDR		0x40000000		//flash区分地址即第30位置1
#define FLASH_BASIC_ADDR				0					//FLASH基址

#define FLASH_IAP_BASIC_ADDR			0x3D0000		//最后128K

typedef struct
{
	uint8 member1[10];
	uint32 member2[10];
	uint32 crc16;
}DST_Test;



typedef struct
{
	//-------------时钟信息--------
	tEE_ClockPara EE_ClockPara;
	//---------------月结---------
	tEE_MonthBillingData EE_MonthBillingData;
//	tEE_MonthInfo EE_MonthInfo;
	//--------------事件记录---------
//	tEE_EventParaInfo EE_EventParaInfo;
	//--------------负荷曲线--------
	//tEE_LoadProfileInfo EE_LoadProfileInfo;
	//-------------需量--------
//	tDemand_Para_All EE_Demand_Para_All;
//	tDemand_Recovery EE_Demand_Recovery;
	//------------日冻结---
	tEE_FreezeDataInfo EE_FreezeDataInfo;
	//------------电表清零记录信息-------
	tEE_MeterClearRecordInfo EE_MeterClearRecordInfo;

	DST_Test DST_TestEEP;
	uint8 Reserve;
}tEepromStructTab;

typedef struct
{
	//-------------校表数据--------
    tEE_CALIBRATE_METER EE_CalibrateBak;
	//-------------电能数据--------
	tRunData   EE_EnergyBak;
	//-------------电表信息--------
	tEE_MeterInfoBak EE_MeterInfoBak;
	//---------------月结---------
	tEE_MonthBillingBak EE_MonthBillingBak;
	//--------------费率--------
//	tEE_TariffParaBak EE_TariffParaBak;
	//--------------负荷曲线--------
	//tEE_LoadProfileBak EE_LoadProfileBak;
	tEE_FreezeBak EE_FreezeBak;
	//--------------事件记录---------
//	tEE_EventInfoBak EE_EventInfoBak;
    tEE_EventParaInfo EE_EventParaInfo;
	//--------------显示模式参数---------
	tEE_DisplayAlternate EE_DisplayAlternate;
    //--------------RTC---------
	tEE_DATE_TIME EE_DATE_TIME;
    //--------------RTC_Cal---------
    tEE_RTC_Calibrate EE_CalParameter;
        
	DST_Test DST_TestEEPBak;
	uint8 Reserve;
}tEepromBakStructTab;

typedef struct
{
	//------------月结数据-----------
//	tFLASH_MonthBillingData FLASH_MonthBillingData;
	//---------------事件记录---------
//	tFLASH_EventDataInfo FLASH_EventDataInfo;
	//--------------冻结数据------------
//	tFLASH_FreezeDataInfo FLASH_FreezeDataInfo;
//	//--------------负荷曲线--------
	//tFLASH_LoadProfileDataInfo FLASH_LoadProfileDataInfo;

	DST_Test DST_TestFlash;
	uint8 Reserve;
}tFlashStructTab;


/* ------------ Exported macro -----------------------------------------------*/
#define OffsetOfStruct(StructType, member)  	(uint32)(&(((StructType *)0)->member))      //取结构成员的偏移量； StructType为结构类型，member为结构成员
#define EE_STRUCT_ADDR(x)                     	(OffsetOfStruct(tEepromStructTab, x)+EE_ADDR_BASIC)
#define EE_BAK_STRUCT_ADDR(x)					(OffsetOfStruct(tEepromBakStructTab, x)+EE_BAK_BASIC_ADDR)
#define FLASH_STRUCT_ADDR(x)                    (OffsetOfStruct(tFlashStructTab, x)+FLASH_BASIC_ADDR+FLASH_ADDR_DISTINGUISH_ADDR)
#endif

