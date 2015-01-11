/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：MeterInfo.c
* 文件标识：
* 摘要：电表自身相应数据信息的管理

* 当前版本：1.0.0 
* 作者：冯秋雄
* 完成日期：20130930
*******************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Include.h"
/* Private define---------------------------------------------------------------*/
#define EE_ALL_KEY_ADDR					EE_BAK_STRUCT_ADDR(EE_MeterInfoBak.MeterKey)
#define EE_MASTER_KEY_ADDR 				EE_BAK_STRUCT_ADDR(EE_MeterInfoBak.MeterKey.MasterKek)
#define EE_AUTH_KEY_ADDR 				EE_BAK_STRUCT_ADDR(EE_MeterInfoBak.MeterKey.Akey)
#define EE_METER_COMM_INFO_ADDR			EE_BAK_STRUCT_ADDR(EE_MeterInfoBak.MeterCommInfo)

#define EE_METER_IMPORTINFO_ADDR		EE_BAK_STRUCT_ADDR(EE_MeterInfoBak.MeterImportInfo)
#define EE_METER_GEOGRAPHICPOS_ADDR     EE_BAK_STRUCT_ADDR(EE_MeterInfoBak.MeterGeographicInfo)

/* Private typedef---------------------------------------------------------------*/
typedef struct
{
        uint8 Low;
	uint8 High;
}tStatus;

typedef struct
{
	tStatus Status1;
	tStatus Status2;
	tStatus Status3;
	tStatus Status4;
	tStatus Status5;
	tStatus Status6;
	tStatus Status7;
}tMeterStatus;


/* Private variable-------------------------------------------------------------*/
static tMeterCommLockInfo sMeterCommLockInfo;

static const uint8 sap_assignment_list[] = {LOGIC_DEVICE_NAME1};

//-----------------版本号-------------------
static const uint8 sSoftWare_Versions_ASCLL[32]=
{
	'D','D','S','2','6','D','-','X','3','2',
	'8','-','2','0','1','4','0','9','2','4',
	'-','V','1',' ',' ',' ',' ',' ',' ',' ',
	' ',' '
};			//  DTS27										

static const uint8 sHardWare_Versions_ASCLL[32]=
{
	'D','D','S','2','6','D','-','X','3','2',
	'8','-','E','-','2','0','1','-','2','0',
	'1','4','0','6','2','6',' ',' ',' ',' ',
	' ',' '
};

static const uint8 AutoPasswordCode[]=
{
    1, 2, 3, 4, 5, 6, 7, 8
};

flag MeterState[2];                       //电表状态

/*
BIT7： 1：电表掉电标志------------------判断此标志,关掉RF功能,进低功耗。
BIT6： 1：电压低于70%或者高于140%UN关闭RF功能,减少表的功耗。
BIT5： 电表判断外置模块的状态  0： PTP模块   1：升级模块/MESH模块
BIT4： 窃电标志----电流不平衡 1：正在窃电 0：不窃电
BIT3： 电池欠压标志   1：欠压  0：不欠压
BIT2： 当前计量路反向标志 1：反向  0：正向
BIT1： 当前计量路标志  1：第二路 0：第一路
BIT0:	时钟错误标志
*/

static void MeterStatusGet(void)
{
    RFModuleCheckProcess();
	if(1 == PowerDownDetect())
	{
		POWERDOWNFLAG = 1;                              
	}
	else
	{		
		POWERDOWNFLAG = 0;
	}
	
	if(1 == lowVoltageFlag)
	{
		VOLTAGELOWCLOSERFFLAG = 1;
	}
	else
	{
		VOLTAGELOWCLOSERFFLAG = 0;
	}
	
	if(1 == ModelSelGet())
	{
		MODELSELECTFLAG = 1;
	}
	else
	{
		MODELSELECTFLAG = 0;
	}
	
	if(1 == Get_EMU_Measure_Data(CurrentImbalance))
	{
		CURRENTIMBALANCEFLAG = 1;
	}
	else
	{
		CURRENTIMBALANCEFLAG = 0;
	}
	
	if(1 == BattLowStateGet())
	{
		BATTERYLOWFLAG = 1;                 //时钟电池欠压
	}
	else
	{
		BATTERYLOWFLAG = 0;
	}
	
	if(1 == Get_EMU_Measure_Data(POWER_DIRECTION))
	{
		REVERSEFLAG = 1;
	}
	else
	{
		REVERSEFLAG = 0;
	}
	
	if(1 == Get_EMU_Measure_Data(MEASURE_CHNSEL))
	{
		CURRENTMETERCHANNEL = 1;					//第2路计量
	}
	else
	{
		CURRENTMETERCHANNEL = 0;                    //第1路计量
	}
	
	if(1 == ClockStateGet())
	{
		CLOCKFAULTFLAG = 1;                		    //时钟故障
	}
	else
	{
		CLOCKFAULTFLAG = 0;
	}
	
}

static BOOL_B MeterInfoGetPreKeyStatus(void)
{
	if(!IO_ReadGPIOPin(IO_PORT_PRO_SWI,IO_PIN_PRO_SWI))
	{
		PublicDelayMs(2);
		if(!IO_ReadGPIOPin(IO_PORT_PRO_SWI,IO_PIN_PRO_SWI))
		{
			return TRUE;
		}
	}
	return FALSE;
}

static uint16 MeterInfoKeyGet(uint32 DataId, uint8 *pBuf)
{
	tMeterKey MeterKey;
	uint16 Result = 0x8000+DA_READ_WRITE_DENIED;

	//if(MeterInfoGetPreKeyStatus())		//短接后使用默认密码
	//{
	//	memset(&MeterKey, 1, sizeof(MeterKey));
	//}
	if(!NvmBytesRead(EE_ALL_KEY_ADDR, &MeterKey, sizeof(MeterKey)))
	{
		//memset(&MeterKey, 1, sizeof(MeterKey));
		memcpy((uint8 *)MeterKey.MasterKek, AutoPasswordCode, sizeof(AutoPasswordCode));
		memcpy((uint8 *)MeterKey.Akey, AutoPasswordCode, sizeof(AutoPasswordCode));
		memcpy((uint8 *)MeterKey.LlsKey, AutoPasswordCode, sizeof(AutoPasswordCode));
	}
	switch(DataId)
	{
		case 0x0F000107:		//LLS key
			memcpy(pBuf, MeterKey.LlsKey, KEY_LEN);
			Result = KEY_LEN;
			break;
		case 0x0F000207:		//HLS key
			memcpy(pBuf, MeterKey.Akey, KEY_LEN);
			Result = KEY_LEN;
			break;
		case 0x01A00002:		//Master key
			memcpy(pBuf, MeterKey.MasterKek, KEY_LEN);
			Result = KEY_LEN;
			break;
		default:
			break;
	}
	return Result;
}

static uint8 MeterInfoKeySet(uint32 DataId, uint8 *pBuf, uint16 Len)
{
	tMeterKey MeterKey;
	uint8 Result = DA_READ_WRITE_DENIED;

	if(!NvmBytesRead(EE_ALL_KEY_ADDR, &MeterKey, sizeof(MeterKey)))
	{
		//memset(&MeterKey, 1, sizeof(MeterKey));
		memcpy((uint8 *)MeterKey.MasterKek, AutoPasswordCode, sizeof(AutoPasswordCode));
		memcpy((uint8 *)MeterKey.Akey, AutoPasswordCode, sizeof(AutoPasswordCode));
		memcpy((uint8 *)MeterKey.LlsKey, AutoPasswordCode, sizeof(AutoPasswordCode));
	}
	if(Len != KEY_LEN)
	{
		return DA_OTHER_REASON;
	}
	switch((uint16)(DataId>>8))
	{
		case 0xA000:		//master key
			memcpy(MeterKey.MasterKek, pBuf, Len);
			Result = DA_SUCCESS;
			break;
		case 0x0001:		//LLS key
			memcpy(MeterKey.LlsKey, pBuf, Len);
			Result = DA_SUCCESS;
			break;
		case 0x0002:		//HLS key
			memcpy(MeterKey.Akey, pBuf, Len);
			Result = DA_SUCCESS;
			break;
		default:
			break;
	}
	if(Result == DA_SUCCESS)
	{
		NvmBytesWrite(EE_ALL_KEY_ADDR, &MeterKey, sizeof(MeterKey));
	}
	return Result;
}

static uint16 MeterInfoParaClass1Get(uint32 DataId, uint8 *pBuf)
{
	uint16 Result = 0x8000+DA_READ_WRITE_DENIED;
	uint8 Attr = (uint8)DataId;
	
	if(Attr == 0x02)
	{
		switch((uint16)(DataId>>8))
		{
			case 0x0100:		//表号
			case 0x0200:		//HDLC地址
			{
				tMeterImportInfo MeterImportInfo;
				
				if(!NvmBytesRead(EE_METER_IMPORTINFO_ADDR, &MeterImportInfo, sizeof(MeterImportInfo)))
				{
					memset(&MeterImportInfo, 0, sizeof(MeterImportInfo));
				}
				Result = sizeof(MeterImportInfo.MeterAddr);
				memcpy(pBuf, MeterImportInfo.MeterAddr, Result);
				break;
			}
            case 0x0209:   //地理信息
            {
                tMeterGeographicInfo    MeterGeographicInfo;
                                
                if(!NvmBytesRead(EE_METER_GEOGRAPHICPOS_ADDR, &MeterGeographicInfo, sizeof(MeterGeographicInfo)))
				{
				   memset(&MeterGeographicInfo, 0, sizeof(MeterGeographicInfo));
				}
                memcpy(pBuf, &MeterGeographicInfo, sizeof(MeterGeographicInfo));
                break;
            }                         
			case 0x0300:		//logical name
				Result = sizeof(sap_assignment_list)-1;
				my_memcpy(pBuf,sap_assignment_list,Result);
				break;
			case 0xA000:			//master key
				Result = MeterInfoKeyGet(DataId, pBuf);
				break;
			case 0x2100:		//电表有功常数	
				Result = 4;
				*(uint32*)pBuf = Get_EMU_Measure_Data(SYS_PARAMETER_CONST_ACT);
				break;
			case 0x2200:		//电表无功常数
				Result = 4;
				*(uint32*)pBuf = Get_EMU_Measure_Data(SYS_PARAMETER_CONST_REA);
				break;		
			case 0x5100:		//软件版本号
				Result = sizeof(sSoftWare_Versions_ASCLL);
				memcpy(pBuf, sSoftWare_Versions_ASCLL, sizeof(sSoftWare_Versions_ASCLL));
				break;
			case 0x5200:		//硬件版本号
				Result = sizeof(sHardWare_Versions_ASCLL);
				memcpy(pBuf, sHardWare_Versions_ASCLL, sizeof(sHardWare_Versions_ASCLL));
				break;
			case 0x7000:
//				Result = MeterStatusGet(pBuf);
				break;
            case 0xA100:        //通信信息
            	if(MeterInfoGetPreKeyStatus())
            	{
					*pBuf = 0;
            	}
				else if(*pBuf == METER_LLS_COMM)
				{
					*pBuf = sMeterCommLockInfo.CommLockFlag & 0x0f;
				}
				else if(*pBuf == METER_HLS_COMM)
				{
					*pBuf = sMeterCommLockInfo.CommLockFlag & 0xf0;
				}
				Result = 1;
				break;
			default:
				break;
		}
	}
	return Result;
}

/* Public functions-------------------------------------------------------------*/
void MeterInfoPowerUpInit(void)
{
	tMeterCommInfo MeterCommInfo;

	if(!NvmBytesRead(EE_METER_COMM_INFO_ADDR, &MeterCommInfo, sizeof(tMeterCommInfo)))
	{
		memset(&MeterCommInfo, 0, sizeof(tMeterCommInfo));
	}
	sMeterCommLockInfo = MeterCommInfo.MeterCommLockInfo;
	TaskAdd(MeterInfoProcess, 0, 1000, 1);	
	TaskAdd(MeterStatusGet, 0, 1, 0);	
}

void MeterInfoPowerDownInit(void)
{
	tMeterCommInfo MeterCommInfo;

	MeterCommInfo.MeterCommLockInfo = sMeterCommLockInfo;
	NvmBytesWrite(EE_METER_COMM_INFO_ADDR, &MeterCommInfo, sizeof(tMeterCommInfo));
}

void MeterInfoProcess(void)
{
	tDateTime DateTime;
	uint16 Days;

	Clock_GetTime(&DateTime);
	Days = CalcCalendarToDays(&DateTime);
	if((sMeterCommLockInfo.CommLockFlag & 0x0f) != 0)
	{
		if(sMeterCommLockInfo.LlsLockDay != Days)
		{
			sMeterCommLockInfo.CommLockFlag &= 0xf0;
		}
	}
	if((sMeterCommLockInfo.CommLockFlag & 0xf0) != 0)
	{
		if(sMeterCommLockInfo.HlsLockDay != Days)
		{
			sMeterCommLockInfo.CommLockFlag &= 0x0f;
		}
	}
}

uint8 ErrorStateGet(void)
{
	if((energyEepError == 1)||(caliEepError == 1))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void PassWordSetDefault(void)
{
	tMeterKey MeterKey;
	memcpy((uint8 *)MeterKey.MasterKek, AutoPasswordCode, sizeof(AutoPasswordCode));
	memcpy((uint8 *)MeterKey.Akey, AutoPasswordCode, sizeof(AutoPasswordCode));
	memcpy((uint8 *)MeterKey.LlsKey, AutoPasswordCode, sizeof(AutoPasswordCode));
	NvmBytesWrite(EE_ALL_KEY_ADDR, &MeterKey, sizeof(MeterKey));
}

/*****************************************************************************************
*   Action:   表地址清零
*   Input:    
*   Output:    
*****************************************************************************************/
void MeterAddressClear(void)
{
	tMeterImportInfo MeterImportInfo;
    tMeterGeographicInfo    MeterGeographicInfo;
         
	memset(&MeterImportInfo, 0, sizeof(MeterImportInfo));
	NvmBytesWrite(EE_METER_IMPORTINFO_ADDR, &MeterImportInfo, sizeof(MeterImportInfo));
        
    memset(&MeterGeographicInfo, 0, sizeof(MeterGeographicInfo));
    NvmBytesWrite(EE_METER_GEOGRAPHICPOS_ADDR, &MeterGeographicInfo, sizeof(MeterGeographicInfo));
}

/*****************************************************************************************
*   Action:   电表信息相关数据读取
*   Input:     DataId--数据ID；pBuf--数据存放的指针
*   Output:    读取数据的长度
*****************************************************************************************/
uint16 MeterInfoParaGet(uint32 DataId, uint8 *pBuf)
{
	uint16 Result = 0x8000+DA_READ_WRITE_DENIED;
	uint8 Attr = (uint8)DataId;
	int32 Data;

	switch((uint8)(DataId >> 24))
	{
		case 1:
			Result = MeterInfoParaClass1Get(DataId, pBuf);
			break;
		case 3:
			if(Attr == 2)
			{
				switch((uint16)(DataId>>8))
				{
					case 0xA001:		//额定电压
						Data = Get_EMU_Measure_Data(SYS_PARAMETER_UN)/10;
						*(uint16*)pBuf = Data;
						Result = 2;
						break;
					case 0xA002:		//额定电流
						Data = Get_EMU_Measure_Data(SYS_PARAMETER_IB)/1000;
						*(uint16*)pBuf = Data;
						Result = 2;
						break;
					case 0xA003:		//额定频率
						*(uint16*)pBuf = 5000;
						Result = 2;
						break;	
					case 0xA004:		//最大电流
						Data = Get_EMU_Measure_Data(SYS_PARAMETER_IMAX)/1000;
						*(uint16*)pBuf = Data;
						Result = 2;
						break;
					default:
						break;
				}
			}
			if(Attr == 3)
			{
				switch((uint16)(DataId>>8))
				{
					case 0xA001:		//额定电压
						*pBuf=0;		//scale
						*(pBuf+1)=35;	//second
						Result = 2;
						break;
					case 0xA002:		//额定电流
						*pBuf=0;		//scale
						*(pBuf+1)=33;	//second
						Result = 2;
						break;
					case 0xA003:		//额定频率
						*(int8*)pBuf=-2;		//scale
						*(pBuf+1)=44;	//second
						Result = 2;
						break;	
					case 0xA004:		//最大电流
						*pBuf=0;		//scale
						*(pBuf+1)=33;	//second
						Result = 2;
						break;
					default:
						break;
				}
			}
			break;
		case 15:
			Result = MeterInfoKeyGet(DataId, pBuf);
			break;
		case 19:
			if(Attr == 6)
			{
				tMeterImportInfo MeterImportInfo;
				
				if(!NvmBytesRead(EE_METER_IMPORTINFO_ADDR, &MeterImportInfo, sizeof(MeterImportInfo)))
				{
					memset(&MeterImportInfo, 0, sizeof(MeterImportInfo));
				}
				Result = sizeof(MeterImportInfo.MeterAddr);
				memcpy(pBuf, MeterImportInfo.MeterAddr, Result);
				break;
			}
			break;
		default:
			break;
	}
	return Result;
}

/*****************************************************************************************
*   Action:    电表信息相关数据设置
*   Input:     DataId--数据ID；pBuf--数据存放的指针,Len--所设数据的长度
*   Output:   0--成功; 其它--失败的原因
*****************************************************************************************/
uint8 MeterInfoParaSet(uint32 DataId, uint8 *pBuf, uint16 Len)
{
	uint8 Result = DA_READ_WRITE_DENIED;
	tDateTime DateTime;

	switch(DataId)
	{
		case 0x01020002:	//HDLC地址
			//if(MeterInfoGetPreKeyStatus())
			{
				tMeterImportInfo MeterImportInfo;

				if(Len != METER_ADDR_LEN)
				{
					break;
				}
				if(!NvmBytesRead(EE_METER_IMPORTINFO_ADDR, &MeterImportInfo, sizeof(MeterImportInfo)))
				{
					memset(&MeterImportInfo, 0, sizeof(MeterImportInfo));
				}
				memcpy(MeterImportInfo.MeterAddr, pBuf, Len);
				if(!NvmBytesWrite(EE_METER_IMPORTINFO_ADDR, &MeterImportInfo, sizeof(MeterImportInfo)))
				{
					break;
				}
				Result = 0;
			}
			break;
        case CurrentGeographicPos_IDCode:            //地理信息
            {
                  tMeterGeographicInfo    MeterGeographicInfo;
                  memset(&MeterGeographicInfo, 0, sizeof(MeterGeographicInfo));  //这个要清零,确保回读回来是0
                  memcpy(&MeterGeographicInfo.GeographicInfo, pBuf, Len);
                  if(!NvmBytesWrite(EE_METER_GEOGRAPHICPOS_ADDR, &MeterGeographicInfo, sizeof(MeterGeographicInfo)))
                  {
                          break;
                  }
                  Result = 0;
                  break;
            }
		case 0x0F000107:		//LLS key
		case 0x0F000207:		//HLS key
		case 0x01A00002:		//Master key
			Result = MeterInfoKeySet(DataId, pBuf, Len);
			break;
		case 0x01A10002:		//通信错误信息
			if(MeterInfoGetPreKeyStatus())
			{
				memset(&sMeterCommLockInfo, 0, sizeof(sMeterCommLockInfo));
				return 0;
			}
			Clock_GetTime(&DateTime);
			if(*pBuf == METER_LLS_COMM)
			{
				if(pBuf[1] != 0)
				{
					if((sMeterCommLockInfo.CommLockFlag&0x0f) == 0)
					{
						if(++sMeterCommLockInfo.LlsCommErrNum >= 3)
						{
							sMeterCommLockInfo.LlsCommErrNum = 0;
							sMeterCommLockInfo.CommLockFlag |= 0x05;
							sMeterCommLockInfo.LlsLockDay = CalcCalendarToDays(&DateTime);
						}
					}
				}
				else
				{
					sMeterCommLockInfo.LlsCommErrNum = 0;
				}
			}
			else if(*pBuf == METER_HLS_COMM)
			{
				if(pBuf[1] != 0)
				{
					if((sMeterCommLockInfo.CommLockFlag&0xf0) == 0)
					{
						if(++sMeterCommLockInfo.HlsCommErrNum >= 3)
						{
							sMeterCommLockInfo.HlsCommErrNum = 0;
							sMeterCommLockInfo.CommLockFlag |= 0x50; 
							sMeterCommLockInfo.HlsLockDay = CalcCalendarToDays(&DateTime);
						}
					}
				}
				else
				{
					sMeterCommLockInfo.HlsCommErrNum = 0;
				}
			}
			else
			{
				break;
			}
			Result = 0;
			break;
		default:
			break;
	}
	return Result;
}

