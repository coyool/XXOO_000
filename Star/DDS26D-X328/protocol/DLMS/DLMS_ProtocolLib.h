#ifndef __DLMS_PROTOCOLLIB__H
#define __DLMS_PROTOCOLLIB__H

#include "Include.h"

#define API_MAX_NAME_LEN				4
#define API_MAX_DAY_TARIFF				8
#define API_MAX_WEEK_TRIFF				8
#define API_MAX_SEASON_TRIFF			12
#define API_MAX_PERIOD_NUM				12
#define API_SPECIAL_DAY_NUM				36	

#define API_IAP_ACTIVE_INFO_SZIE		30

#ifndef my_memset
#define my_memset(x,y,l)  	memset(x,y,l)
#endif
#ifndef my_memcpy
#define my_memcpy(x,y,l)	memcpy(x,y,l)
#endif
#ifndef my_memcmp
#define my_memcmp(x,y,l)	memcmp(x,y,l)	
#endif

/********IC FUN**************************************/
#define DLMS_IC_01_FUN			
#define DLMS_IC_03_FUN
//#define DLMS_IC_04_FUN
//#define DLMS_IC_05_FUN
//#define DLMS_IC_06_FUN
#define DLMS_IC_07_FUN			
#define DLMS_IC_08_FUN	
//#define DLMS_IC_09_FUN
//#define DLMS_IC_11_FUN
#define DLMS_IC_15_FUN
//#define DLMS_IC_18_FUN
#define DLMS_IC_19_FUN
//#define DLMS_IC_20_FUN
//#define DLMS_IC_21_FUN
//#define DLMS_IC_22_FUN
//#define DLMS_IC_23_FUN
//#define DLMS_IC_70_FUN
//#define DLMS_IC_71_FUN

/*external typedef-------------------------------------------------*/
/* COSEM对象数据类型asn1编码信息 */
typedef enum
{
	DT_NULL_DATA                 = 0,
	DT_ARRAY                     = 1,
	DT_STRUCTURE                 = 2,
	DT_BOOLEAN                   = 3,
	DT_BIT_STRING                = 4,
	DT_DOUBLE_LONG               = 5,
	DT_DOUBLE_LONG_UNSIGNED      = 6,
	DT_FLOATING_POINT            = 7,
	DT_OCTET_STRING              = 9,
	DT_VISIBLE_STRING            = 10,
	DT_BCD                       = 13,
	DT_INTEGER                   = 15,   // int8
	DT_LONG                      = 16,   // int16
	DT_INTEGER_UNSIGNED          = 17,   // uint8
	DT_LONG_UNSIGNED             = 18,   // uint16
	DT_COMPACT_ARRAY             = 19,
	DT_LONG64                    = 20,
	DT_LONG64_UNSIGNED           = 21,
	DT_ENUM                      = 22,
	DT_FLOAT32                   = 23,
	DT_FLOAT64                   = 24,
	DT_DATE_TIME                 = 25,
	DT_DATE                      = 26,
	DT_TIME                      = 27,
	/* 自已加的自定义电表数据类型 */
	DT_BILLING_DAY               = 200,
	DT_DONOT_CARE                = 255
}eCosemDataType;

/* COSEM 对象数据单位类型定义 */
typedef enum
{
	UNIT_YEAR         = 1,
	UNIT_MONTH        = 2,
	UNIT_WEEK         = 3,
	UNIT_DAY          = 4,
	UNIT_HOUR         = 5,
	UNIT_MINUTE       = 6,
	UNIT_SECOND       = 7,
	UNIT_ANGLE        = 8,    // 度
	UNIT_TEMP         = 9,    // 温度
	UNIT_W            = 27,   // 有功功率,W
	UNIT_UA           = 28,   // 视在功率,W
	UNIT_UAR          = 29,   // 无功功率,W
	UNIT_WH           = 30,   // 有功电能,WH
	UNIT_UAH          = 31,   // 视在电能,WH
	UNIT_UARH         = 32,   // 无功电能,WH
	UNIT_AMP          = 33,   // A
	UNIT_VOL          = 35,   // V
	UNIT_HZ           = 44,   // Hz
	UNIT_100          = 56,   // %
	UNIT_UNDEF        = 255
}eClassUnit;

typedef enum
{
	DA_SUCCESS                   = 0,
	DA_HARDWARE_FAULT            = 1,
	DA_TEMPORARY_FAILURE         = 2,
	DA_READ_WRITE_DENIED         = 3,
	DA_OBJECT_UNDEFINED          = 4,
	DA_OBJECT_CLASS_INCONSISTENT = 9,
	DA_OBJECT_UNAVAILABLE        = 11,
	DA_TYPE_UNMATCHED            = 12,
	DA_SCOPE_OF_ACCESS_VIOLATED  = 13,
	DA_DATA_BLOCK_UNAVAILABLE    = 14,
	DA_LONG_GET_ABORTED          = 15,
	DA_NO_LONG_GET_IN_PROGRESS   = 16,
	DA_LONG_SET_ABORTED          = 17,
	DA_NO_LONG_SET_IN_PROGRESS   = 18,
	DA_DATA_BLOCK_NUMBER_INVALID = 19,
	DA_OTHER_REASON              = 250
}eDataAccessResult;

/* COSEM对象类 */
typedef enum
{
	CLASS_DATA                 = 1,
	CLASS_REGISTER             = 3,
	CLASS_EXTENDED_REGISTER    = 4,
	CLASS_DEMAND_REGISTER      = 5,
	CLASS_REGISTER_ACTIVATION  = 6,
	CLASS_PROFILE_GENERIC      = 7,
	CLASS_CLOCK                = 8,
	CLASS_SCRIPT_TABLE         = 9,
	CLASS_SCHEDULE             = 10,
	CLASS_SPECIAL_DAYS_TABLE   = 11,
	CLASS_ASSOCIATION_SN       = 12,
	CLASS_ASSOCIATION_LN       = 15,
	CLASS_SAP_ASSIGNMENT       = 17,
	CLASS_IMAGE_TRANSFER	   = 18,
	CLASS_LOCAL_PORT_SETUP     = 19,
	CLASS_ACTIVITY_CALENDAR    = 20,
	CLASS_REGISTER_MONITOR     = 21,
	CLASS_SINGLE_ACTION_SCHEDULE = 22,
	CLASS_HDLC_SETUP           = 23,
	CLASS_TWISTED_PAIR_SETUP   = 24,
	CLASS_MBUS_SUPPORT         = 25,
	CLASS_UTILITY_TABLE        = 26,
	CLASS_PSTN_CONFIGURATION   = 27,
	CLASS_PSTN_AUTO_ANSWER     = 28,
	CLASS_PSTN_AUTO_DIAL       = 29,
	CLASS_PUSH_SETUP		   = 40,
	CLASS_TCPUDP_SETUP         = 41,
	CLASS_IPV4_SETUP           = 42,
	CLASS_ETHERNET_MAC_SETUP   = 43,
	CLASS_PPP_SETUP            = 44,
	CLASS_GPRS_MODEM_SETUP     = 45,
	CLASS_SMTP_SETUP           = 46,
	CLASS_MESSAGE_HANDLER      = 60,
	CLASS_REGISTER_TABLE       = 61,
	CLASS_STATUS_MAPPING       = 63,
	CLASS_SECURITY_SETUP	   = 64,
    CLASS_DISCONNECT_CONTROL   = 70,
    CLASS_LIMITER              = 71,
    CLASS_MBUS_CLIENT          = 72,
    CLASS_MBUS_MASTER_SETUP    = 74
}eClassId;

/* COSEM对象访问内存管理定义 */
typedef struct{
	uint32 apdu_blk_no;       // COSEM块传输编号
	uint16 apdu_size;         // COSEM APDU空间尺寸大小
	uint8 * apdu;             // COSEM APDU缓冲
	uint16 data_idx;          // data result索引
	uint16 data_len;          // data result长度
	uint16 result_size;       // data result空间尺寸大小
	uint8 * result;           // 接口类对象访问缓冲
	uint16 api_idx;           // 电表API接口已访问点索引
	uint16 api_ptr;           // 电表API接口已访问点指针
	uint16 api_point;         // 电表API接口需访问点数
	uint16 api_cnt;           // 
}tCosemMem;

/* COSEM对象请求信息定义 */
typedef struct tObjReq
{
	uint16 class_id;          // 类 id 号
	uint8 obis[6];            // OBIS码
	uint8 attr_id;            // 属性号
	uint16 para_len;          // 参数域长度
	uint8 * para;             // 参数内容
	uint8 ic_idx;             // ObjTable中的接口类索引
	const void * ele;         // 指向ObjTable的对象表中的实例
	struct tObjReq * next;    // 链表指向下一个请求
}tObjRequest;

/* 定义对象的单位量度asn1编码信息 */
typedef struct{
	int8 scaler;
	uint8 unit;
}tScalerUnitInfo;

/*************************************************/


typedef enum
{
    GetIdex        = 0,
    GetData        = 1,
    GetCurrentIdex = 2
}eAPI_DataInform;

typedef struct
{
	uint16 class_id;
	uint8 *obis;
	uint32 data_id;
	uint8 ic_idx;             // ObjTable中的接口类索引
	const void *ele;
}tObjFind;

typedef struct
{
    uint8 HighByteYear;
    uint8 LowByteYear;
    uint8 Month;
    uint8 Day;
    uint8 Week;
    uint8 Hour;
    uint8 Minute;
    uint8 Second;
    uint8 HundredthSecond;
    uint8 HighByteDeviation;
    uint8 LowByteDeviation;
    uint8 ClockStatus;
}tDateTime;

typedef struct
{
    uint16 StartIdex;       //所取数据的起始位置索引 ,从1开始
    uint16 OffSet;          //起始位置索引的偏移量，偏移量从0开始,也可做他用
    uint16 IdexNum;         //所取数据的索引个数
    uint16 MaxNum;
}tAPI_ProfileInformation;

typedef struct
{
	uint8 Len;
	uint8 Name[API_MAX_NAME_LEN];
}tAPI_Name;

typedef struct
{
	tAPI_Name SeasonName;
	tDateTime SeasonStrTime;
	tAPI_Name WeekName;
}tAPI_SeasonProfile;

typedef struct
{
	tAPI_Name WeekName;
	uint8 DayId[7];
}tAPI_WeekProfile;

typedef struct
{
	uint8 Hour;
    uint8 Minute;
    uint8 Second;
	uint8 Hundredths;
}tDlmsTime;

typedef struct
{
	uint8 HighByteYear;
	uint8 LowByteYear;
	uint8 Month;
	uint8 DayOfMonth;
	uint8 DayOfWeek;
}tDlmsDate;

typedef struct
{
	tDlmsTime DlmsTime;
	tDlmsDate DlmsDate;
}tDlmsDateTime;

typedef struct
{
	tDlmsTime Time;
	uint16 ScriptSelector;
}tAPI_TariffProfile;

typedef struct
{
	uint8 DayId;
	uint8 PeriodNum;
	tAPI_TariffProfile Period[API_MAX_PERIOD_NUM];	
}tAPI_DayProfile;

typedef struct
{
    uint16 ScriptId;          	// script_identifier
    uint8  MaskNameLen;     	//最大值是8
    uint8  MaskName[API_MAX_NAME_LEN];     
}tAPI_TariffScript;

typedef struct
{
    uint8   MaskNameLen;     	// MaskName的长度
    uint8   MaskName[3];      	// 最长8个字节
    uint8   Index[8];         	//bit-string  1: 对应索引位置的数据项存在。0：不存在 
}tAPI_ActMask;

typedef struct
{
	uint8 HighByteYear;
	uint8 LowByteYear;
	uint8 Month;
	uint8 Day;
	uint8 Week;
}tAPI_Date;

typedef struct
{
    uint16 Index;
    tAPI_Date SpecDate;     //special day
    uint8 DayID;
}tAPI_SpecialDay;

typedef struct
{
	uint16 ProfileId;
	uint8 ActivationTime[12];
	uint32 Duration;
}tAPI_EmergencyProfile;

typedef struct
{
	uint32 DataId;
	uint16 Selector;
}tAPI_Script;

typedef struct
{
	uint8 Len;
	uint8 Data[API_IAP_ACTIVE_INFO_SZIE];
}tAPI_IapActInfoString;

typedef struct
{
	uint32 ImageActiveSize;
	tAPI_IapActInfoString ImageActveIndenfication;
	tAPI_IapActInfoString ImageActiveSinature;
}tAPI_IapImageActInfo;

/* Exported functions ------------------------------------------------------- */
void DLMS_AA_Init(void);
void DLMS_AA_Process(void);
BOOL_B ObjFindThroughDataId(tObjFind *req);

bool DLMS_HDLC_GetFirstConnectFlag(void);
void DLMS_HDLC_ClearFirstConnectFlag(void);

uint8 API_SetDataValue(uint32 DataId, void *pDataBuf, uint16 len);
uint16 API_GetDataValue(uint32 DataId, void *pDataBuf);

/***********************************************/
eDataAccessResult DLMS_IC_01_DataGet(const tObjRequest * req, tCosemMem * mem);
eDataAccessResult DLMS_IC_01_DataSet(const tObjRequest * req, tCosemMem * mem);
eDataAccessResult DLMS_IC_03_RegisterGet(const tObjRequest * req, tCosemMem * mem);
eDataAccessResult DLMS_IC_03_RegisterSet(const tObjRequest * req, tCosemMem * mem);
eDataAccessResult DLMS_IC_03_RegisterAction(const tObjRequest * req, tCosemMem * mem);
eDataAccessResult DLMS_IC_07_ProfileGet(const tObjRequest * req, tCosemMem * mem);
eDataAccessResult DLMS_IC_07_ProfileSet(const tObjRequest * req, tCosemMem * mem);
eDataAccessResult DLMS_IC_07_ProfileAction(const tObjRequest * req, tCosemMem * mem);
eDataAccessResult DLMS_IC_08_ClockGet(const tObjRequest * req, tCosemMem * mem);
eDataAccessResult DLMS_IC_08_ClockSet(const tObjRequest * req, tCosemMem * mem);
eDataAccessResult DLMS_IC_08_ClockAction(const tObjRequest * req, tCosemMem * mem);
eDataAccessResult DLMS_IC_15_AssociationLnGet(const tObjRequest * req, tCosemMem * mem);
eDataAccessResult DLMS_IC_15_AssociationLnSet(const tObjRequest * req, tCosemMem * mem);
eDataAccessResult DLMS_IC_15_AssociationLnAction(const tObjRequest * req, tCosemMem * mem);
eDataAccessResult DLMS_IC_19_IecPortSetupGet(const tObjRequest * req, tCosemMem * mem);
eDataAccessResult DLMS_IC_19_IecPortSetupSet(const tObjRequest * req, tCosemMem * mem);

#endif

