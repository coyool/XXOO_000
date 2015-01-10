#ifndef __EVENT_H
#define __EVENT_H

/*Public define----------------------------------*/
typedef enum
{
	EVENT_ACT_POWER_REVERSE_STR_L1,				//潮流反向
	EVENT_ACT_POWER_REVERSE_END_L1,
	EVENT_CURRENT_Unbalanced_STR,           //两路不平衡开始
	EVENT_CURRENT_Unbalanced_END,           //两路不平衡结束
	EVENT_METER_CLEAR,
	EVENT_CLEAR_EVENT,
    EVENT_SYNCHRONIZATION_STR,						//校时
	EVENT_SYNCHRONIZATION_END,
	EVENT_NULL
}eEventCode;


typedef enum
{
	STATE_ACT_POWER_REVERSE_L1 =0,	
	STATE_CURRENT_Unbalanced,  //两路不平衡
	STATE_ACT_POWER_REVERSE_L1_LED,
	STATE_CURRENT_Unbalanced_LED,
	STATE_EVENT_NULL = 31,
}eEventState;





#pragma pack(push, 1)

typedef struct
{
	uint32 CurrentUnbalancedTotalNum;
	uint32 ActivePowerReverseNum;
	uint16 CheckSum;
}tEE_EventRecordNum;

#pragma pack(pop)

typedef struct
{
	tEE_EventRecordNum EE_EventRecordNum; 
}tEE_EventParaInfo;





/*Public functions----------------------------------*/
uint16 EventPowerUpInit(void);
void EventSecondProcess(void);
void EventTimeCnt(void);
void EventClear(void);
uint16 EventParaGet(uint32 DataId, uint8 *pBuf);
uint8 EventParaSet(uint32 DataId, uint8 *pBuf, uint16 Len);
void EventInsertLog(eEventCode EventCode, uint8 *pBuf);
uint8 EventGetState(eEventState State);
void EventFirmwarSwitchSave(void);
void EventActivePowerReverseNumAdd(void);
void EventCurrentUnbalancedNumAdd(void);
#endif

