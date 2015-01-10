/**********************************************************************
Led.h
**********************************************************************/
#ifndef  _DISPLAY_H_
#define  _DISPLAY_H_

/*--------------------外部常量申明---------------------*/

/*--------------------外部变量申明---------------------*/

/*--------------------宏定义---------------------------*/

//#define unit_℃	9
#define unit_money      10
#define unit_W		27
#define unit_VA	        28
#define unit_var	29
#define unit_Wh  	30
#define unit_Vah	31
#define unit_Varh       32
#define unit_A		33
#define unit_V		35
#define unit_Hz		44
#define unit_Rac	45
#define unit_Rre	46
#define unit_Rap	47
#define unit_KW	        127
#define unit_KVA	128
#define unit_Kvar	129
#define unit_Kwh	130
#define unit_Kvah	131
#define unit_Kvarh	132
#define unit_KKW	227
#define unit_KKVA	228
#define unit_KKvar	229
#define unit_KKwh	230
#define unit_KKvah	231
#define unit_KKvarh	232
#define unit_COS	255

#define    BATTERY_SCALER     (-2)  

/*--------------------宏定义---------------------------*/
#define    LongkeyPageScreen                        3
#define    MaxObjctSumAlternate                     20
/*--------------------外部常量申明---------------------*/


/*--------------------外部变量申明---------------------*/
/*--------------------外部变量申明---------------------*/
typedef struct			//显示模式参数
{
    uint8 Decimal; 
    uint8 time;			//捕获周期
    uint8 objectSum;	       //dataId实际总数
    uint32 DI[MaxObjctSumAlternate];		//dataId
    uint16 CheckSum;    //校验和
}tEE_DisplayAlternate;

extern tEE_DisplayAlternate displayAlternate;		//自动轮显参数


typedef enum
{
	DIS_METER_NO	= 0,
	DIS_DATA_NO,
	DIS_TIME_NO,
	DIS_ALARM_RF_NEL,	
	DIS_ALARM_RF_SPI,
	DIS_ALARM_CLEAR_OK,
    DIS_ALARM_ERROR,
}eDisplayAlarm;

typedef enum
{
	DIS_CLEAR_OK	= 0,
	DIS_ERROR,	
}eDisplayFlag;


/*--------------------外部函数申明-------------------------*/



/*--------------------外部函数申明-------------------------*/

void Init_Disp(void);
void Init_DispPowerUpBatt(void);
void Init_DispPowerDown(void);
void Disp_Auto_Cycle_Task(void);
void Set_Short_KeyDisp_Code(void);
void Set_Long_KeyDisp_Code(void);
void UnitDateGet(uint8 data);
void Display_id(uint8*Date_id);
void DispSceenSet(void);
void Disp_Task(void);
void PowerUpDisplayAlarmPro(void);
void PowerDownDisplayAlarmPro(void);
void Disp_Message(const uint8 *Message);
void DisplayAlarm(eDisplayAlarm Alarm, uint8 Type);
void Display_YMDHMS(tDateTime *DateTime);
void DisplayFlagAlarm(void);
void Disp_Obis_Message(const uint8 *Message);
uint8 DispParaSet(uint32 DataId, uint8 *pBuf, uint16 Len);
uint16 DispParaGet(uint32 DataId, uint8 *pBuf);
void DispParaClear(void);

#endif
