/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：Event.h 
* 文件标识：
* 摘要：事件记录

* 当前版本：1.0.0 
* 作者：冯秋雄
* 完成日期：20130927
*******************************************************************/
//#include "Event.h"
#include "Include.h"

/* Private typedef---------------------------------------------*/
static struct
{
	uint8 Flag;
	uint8 Time;
}sEventLedAlarm[2];

/* Private define-----------------------------------------------*/
#define EVENT_Un								220	//(V)
#define EVENT_Ib								5	//(A)
#define EVENT_Imax								80	//(A)
#define EVENT_POWER								(EVENT_Un*EVENT_Ib)	//(W)
                                                     //功率反向
#define EVENT_ACTIVE_P_RERESERVE_STR_TIME		    3600
#define EVENT_ACTIVE_P_RERESERVE_END_TIME		    3
#define EVENT_ACTIVE_P_RERESERVE_LED_STR_TIME		5
#define EVENT_ACTIVE_P_RERESERVE_LED_END_TIME		3

#define EVENT_CURRENT_Unbalanced_STR_TIME           3600 
#define EVENT_CURRENT_Unbalanced_END_TIME           3
#define EVENT_CURRENT_Unbalanced_LED_STR_TIME       60 
#define EVENT_CURRENT_Unbalanced_LED_END_TIME       3

#define EE_EVENT_RECORDNUM_SAVE_ADDR				EE_BAK_STRUCT_ADDR(EE_EventParaInfo.EE_EventRecordNum)

/* Private variable----------------------------------------------*/

static uint16  sCurrentUnbalancedCnt;
static uint32 sCurrentUnbalancedNum;
static uint16  sActivePowerReverseCnt;
static uint32 sActivePowerReverseNum;

/*程序升级切换需要保存的数据*/

static uint32 sEventState;

/******end***********/

/* Private functions-------------------------------------------------------------------*/
/*****************************************************************************************
*   Action:    记录事件
*   Input:     pBuf--传入事件需要特殊记录的参数，如修改参数事件所修改数据的OBIS
*   Output:    无
*****************************************************************************************/
static void EventStateUpdate(uint8 Flag, eEventState State)
{
	if(Flag == 0)
	{
		sEventState &= (~((uint32)1<<State));
	}
	else
	{
		sEventState |= ((uint32)1<<State);
	}
}

static void EventActivePowerReverseProcess(void)
{
    
	if(1 == EventGetState(STATE_ACT_POWER_REVERSE_L1))
	{
		if(Get_EMU_Measure_Data(POWER_DIRECTION)==1)	
		{
			sActivePowerReverseCnt= 0;
		}
		else
		{
			if(++sActivePowerReverseCnt>= EVENT_ACTIVE_P_RERESERVE_END_TIME)
			{
                sActivePowerReverseCnt=0;
				EventStateUpdate(FALSE, STATE_ACT_POWER_REVERSE_L1);
			}
		}
	}
	else
	{
		if(Get_EMU_Measure_Data(POWER_DIRECTION)!=1)
		{
			sActivePowerReverseCnt= 0;
		}
		else
		{
			if(++sActivePowerReverseCnt>= EVENT_ACTIVE_P_RERESERVE_STR_TIME)
			{
                               sActivePowerReverseCnt=0;
				EventStateUpdate(TRUE, STATE_ACT_POWER_REVERSE_L1);
				EventActivePowerReverseNumAdd();	
			}
		}
	}
}


static void EventCurrentUnbalancedProcess(void)
{
    
	if(1 == EventGetState(STATE_CURRENT_Unbalanced))
	{
		if(Get_EMU_Measure_Data(CurrentImbalance)==1)	
		{
			sCurrentUnbalancedCnt= 0;
		}
		else
		{
			if(++sCurrentUnbalancedCnt>= EVENT_CURRENT_Unbalanced_END_TIME)
			{
                                 sCurrentUnbalancedCnt=0;
				 EventStateUpdate(FALSE, STATE_CURRENT_Unbalanced);
			}
		}
	}
	else
	{
		if(Get_EMU_Measure_Data(CurrentImbalance)!=1)
		{
			sCurrentUnbalancedCnt= 0;
		}
		else
		{
			if(++sCurrentUnbalancedCnt>= EVENT_CURRENT_Unbalanced_STR_TIME)
			{
                sCurrentUnbalancedCnt=0;
				EventStateUpdate(TRUE, STATE_CURRENT_Unbalanced);
				EventCurrentUnbalancedNumAdd();	
			}
		}
	}
}

static void EventCurrentUnbalancedNumAdd(void)
{
    tEE_EventRecordNum EE_EventRecordNum;
	
    if(EventGetState(STATE_CURRENT_Unbalanced))
    {
        sCurrentUnbalancedNum++;
		if((sCurrentUnbalancedNum+sActivePowerReverseNum)>99999999)
        {
            sCurrentUnbalancedNum= 0;
			sActivePowerReverseNum=0;
        }
        EE_EventRecordNum.CurrentUnbalancedTotalNum= sCurrentUnbalancedNum;
    }
	EE_EventRecordNum.ActivePowerReverseNum= sActivePowerReverseNum;
	CalcCheckSum(EE_EventRecordNum);
    NvmBytesWrite(EE_EVENT_RECORDNUM_SAVE_ADDR, &EE_EventRecordNum, sizeof(EE_EventRecordNum));
}

static void EventActivePowerReverseNumAdd(void)
{
    tEE_EventRecordNum EE_EventRecordNum;
	
    if(EventGetState(STATE_ACT_POWER_REVERSE_L1))
    {
        sActivePowerReverseNum++;
		if((sCurrentUnbalancedNum+sActivePowerReverseNum)>99999999)
        {
            sCurrentUnbalancedNum= 0;
			sActivePowerReverseNum=0;
        }
		EE_EventRecordNum.ActivePowerReverseNum= sActivePowerReverseNum;
    }
	EE_EventRecordNum.CurrentUnbalancedTotalNum= sCurrentUnbalancedNum;
	CalcCheckSum(EE_EventRecordNum);
    NvmBytesWrite(EE_EVENT_RECORDNUM_SAVE_ADDR, &EE_EventRecordNum, sizeof(EE_EventRecordNum));
}



static void EventLEDAlarmProcess(void)
{
    if(sEventLedAlarm[0].Flag == FALSE)
    {
        if(Get_EMU_Measure_Data(POWER_DIRECTION) != 0)
        {
            if(++sEventLedAlarm[0].Time>=EVENT_ACTIVE_P_RERESERVE_LED_STR_TIME)
            {
                EventStateUpdate(TRUE, STATE_ACT_POWER_REVERSE_L1_LED);
                sEventLedAlarm[0].Time=0;
                sEventLedAlarm[0].Flag=TRUE;
            }
        }
        else
        {
            if(sEventLedAlarm[0].Time>0)
            {
			   sEventLedAlarm[0].Time--;
			}
        }
    }
    else
    {
        if(Get_EMU_Measure_Data(POWER_DIRECTION) == 0)
        {
            if(++sEventLedAlarm[0].Time>=EVENT_ACTIVE_P_RERESERVE_LED_END_TIME)
            {
                EventStateUpdate(FALSE, STATE_ACT_POWER_REVERSE_L1_LED);
                sEventLedAlarm[0].Time=0;
                sEventLedAlarm[0].Flag=FALSE;
            }
        }
        else
        {
            sEventLedAlarm[0].Time = 0;	
        }
    }
    if(sEventLedAlarm[1].Flag == FALSE)
    {
        if(Get_EMU_Measure_Data(CurrentImbalance) != 0)
        {
            if(++sEventLedAlarm[1].Time>=EVENT_CURRENT_Unbalanced_LED_STR_TIME)
            {
                EventStateUpdate(TRUE, STATE_CURRENT_Unbalanced_LED);
                sEventLedAlarm[1].Time=0;
                sEventLedAlarm[1].Flag=TRUE;
            }
        }
        else
        {
            if(sEventLedAlarm[1].Time>0)
            {
			   sEventLedAlarm[1].Time--;
			}
        }
    }
    else
    {
        if(Get_EMU_Measure_Data(CurrentImbalance) == 0)
        {
            if(++sEventLedAlarm[1].Time>=EVENT_CURRENT_Unbalanced_LED_END_TIME)
            {
                EventStateUpdate(FALSE, STATE_CURRENT_Unbalanced_LED);
                sEventLedAlarm[1].Time=0;
                sEventLedAlarm[1].Flag=FALSE;
            }
        }
        else
        {
            sEventLedAlarm[1].Time = 0;
        }
    }
    
} 
/*Public functions--------------------------------------------------*/
/*****************************************************************************************
*   Action:    事件记录初始化，主要为成对出现的事件进行相应的事件修补
*   Input:     pBuf--传入事件需要特殊记录的参数，如修改参数事件所修改数据的OBIS
*   Output:    无
*****************************************************************************************/
uint16 EventPowerUpInit(void)
{  
    tEE_EventRecordNum EE_EventRecordNum;
    
    memset(&sEventState, 0, sizeof(sEventState));
    memset(&sEventLedAlarm, 0, sizeof(sEventLedAlarm));
    memset(&sCurrentUnbalancedCnt, 0, sizeof(sCurrentUnbalancedCnt));
    memset(&sActivePowerReverseCnt,0, sizeof(sActivePowerReverseCnt));
	sCurrentUnbalancedNum=0;
	sActivePowerReverseNum=0;
    if(NvmBytesRead(EE_EVENT_RECORDNUM_SAVE_ADDR, &EE_EventRecordNum, sizeof(EE_EventRecordNum)))
    {
        sCurrentUnbalancedNum=EE_EventRecordNum.CurrentUnbalancedTotalNum;
        sActivePowerReverseNum=EE_EventRecordNum.ActivePowerReverseNum;
    }
	else
	{
		energyEepError = 1;
	}
    if(PowerDownDetect() != 0)
    {
        return POWER_UP_INIT_FAIL;
    }
	TaskAdd(EventSecondProcess, 0, 1000, 1);
    return POWER_UP_INIT_SUCESS;
}


void EventSecondProcess(void)
{
	EventActivePowerReverseProcess();//功率反向
	EventCurrentUnbalancedProcess();//两路不平衡
	EventLEDAlarmProcess();         //LED灯时间处理
}

/*****************************************************************************************
*   Action:   清相应的事件
*   Input:     DataId--相应事件的ID, 0--清所有的事件且不记录清事件记录的事件(电表清零)
			0xFFFFFFFF--事件清零，除了电表与事件清零事件外其它事件全清
*   Output:    执行清事件命令的情况
*****************************************************************************************/
void EventClear(void)
{
    tEE_EventRecordNum EE_EventRecordNum;
    sCurrentUnbalancedNum=0;
    sActivePowerReverseNum=0;
    EE_EventRecordNum.CurrentUnbalancedTotalNum= sCurrentUnbalancedNum;
    EE_EventRecordNum.ActivePowerReverseNum= sActivePowerReverseNum;
	CalcCheckSum(EE_EventRecordNum);
    NvmBytesWrite(EE_EVENT_RECORDNUM_SAVE_ADDR, &EE_EventRecordNum, sizeof(EE_EventRecordNum));	 
}


/*****************************************************************************************
*   Action:   插入事件记录
*   Input:     EventCode--事件代码, pBuf--目前只有DataId的OBIS，把DataId强制转换成8位字符串的指针
		没有OBIS记录的则为NULL
*   Output:    无
*****************************************************************************************/
uint8 EventGetState(eEventState State)
{
	if((sEventState & ((uint32)1<<State)) != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*****************************************************************************************
*   Action:    冻结相关数据读取
*   Input:     DataId--数据ID；pBuf--数据存放的指针
*   Output:    读取数据的长度
*****************************************************************************************/
uint16 EventParaGet(uint32 DataId, uint8 *pBuf)
{
	uint16 Result = 0;
	uint32 Countnum=0;
        
    tEE_EventRecordNum EE_EventRecordNum;
    if(NvmBytesRead(EE_EVENT_RECORDNUM_SAVE_ADDR, &EE_EventRecordNum, sizeof(EE_EventRecordNum)))
    {
       Countnum=EE_EventRecordNum.CurrentUnbalancedTotalNum+EE_EventRecordNum.ActivePowerReverseNum;
    }
	switch(DataId)
	{
		case 0x0181A002://窃电总次数
			*(uint32*)pBuf = Countnum;
			Result = 4;
			break;
		default:
			break;
	}
	return Result;
}

