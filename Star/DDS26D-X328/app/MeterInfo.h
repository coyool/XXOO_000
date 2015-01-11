#ifndef __METER_INFO_H
#define __METER_INFO_H
/* Includes ------------------------------------------------------------------*/
//#include "Include.h"

/*Export define---------------------------------------------------*/

//-----------------版本号-------------------
#define KEY_LEN							8
#define METER_ADDR_LEN					6
#define METER_GEOGRPINIF_LEN                            40

#define METER_COMM_LOCK					0X5A

#define METER_COMM_TYPE
#define METER_COMM_LLS_ERR				0X01
#define METER_COMM_LLS_OK				0X02
#define METER_COMM_HLS_ERR				0X10
#define METER_COMM_HLS_OK				0X20

/* 这里配置逻辑设备名, 只允许少于16个字符 */
#define LOGIC_DEVICE_NAME1           "STADDS26D-VN27"


extern flag MeterState[2];                       //电表状态

#define		POWERDOWNFLAG				MeterState[0].bits.bit7
#define		VOLTAGELOWCLOSERFFLAG		MeterState[0].bits.bit6
#define		MODELSELECTFLAG				MeterState[0].bits.bit5
#define		CURRENTIMBALANCEFLAG		MeterState[0].bits.bit4
#define		BATTERYLOWFLAG				MeterState[0].bits.bit3
#define		REVERSEFLAG					MeterState[0].bits.bit2
#define		CURRENTMETERCHANNEL			MeterState[0].bits.bit1
#define		CLOCKFAULTFLAG				MeterState[0].bits.bit0

#define     RFINITFLAG                  MeterState[1].bits.bit7



/*Export typedef-------------------------------------------------*/
typedef enum
{
	METER_LLS_COMM	=0,
	METER_HLS_COMM,
}eMeterCommType;

typedef struct
{
	uint8 MasterKek[KEY_LEN];	//Master key
	//uint8 Ekey[KEY_LEN];		//encryption key
	uint8 Akey[KEY_LEN];	//HLS authentication key
	uint8 LlsKey[KEY_LEN];	//LLS authentication key
	uint16 CheckSum;
}tMeterKey;

typedef struct
{
	uint8 MeterAddr[METER_ADDR_LEN];
	uint16 CheckSum;
}tMeterImportInfo;

typedef struct
{
	uint16 HlsLockDay;
	uint16 LlsLockDay;
	uint8 HlsCommErrNum;		
	uint8 LlsCommErrNum;	
	uint8 CommLockFlag;
}tMeterCommLockInfo;

typedef struct
{
	tMeterCommLockInfo MeterCommLockInfo;
	uint16 CheckSum;
}tMeterCommInfo;

typedef struct
{
	uint16 SoftvareVer;
	uint16 HardvareVer;
	uint16 ActiveConstant;
	uint16 ReactiveConstant;
	uint16 CheckSum;
}tMeterBasicInfo;

typedef struct
{
//	tMeterBasicInfo MeterBasicInfo;
	tMeterCommInfo MeterCommInfo;
}tEE_MeterInfo;

typedef struct
{
      uint8  GeographicInfo[METER_GEOGRPINIF_LEN];     //地理信息
      uint16 CheckSum;
}tMeterGeographicInfo;

typedef struct
{
	tMeterKey               MeterKey;
	tMeterImportInfo        MeterImportInfo;
	tMeterCommInfo          MeterCommInfo;
    tMeterGeographicInfo    MeterGeographicInfo;
}tEE_MeterInfoBak;


/*Export functions-------------------------------------------------*/
void MeterInfoPowerUpInit(void);
void MeterInfoPowerDownInit(void);
void MeterInfoProcess(void);
uint16 MeterInfoParaGet(uint32 DataId, uint8 *pBuf);
uint8 MeterInfoParaSet(uint32 DataId, uint8 *pBuf, uint16 Len);
uint8 ErrorStateGet(void);
void PassWordSetDefault(void);
void MeterAddressClear(void);

#endif

