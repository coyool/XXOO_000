
/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：drv_rtc.h 
* 文件标识：见配置管理计划书 
* 摘要：基本定时器移植层

* 当前版本：1.0.0 
* 作者：张玉清
* 完成日期：20131104
*******************************************************************/
#include "Include.h"

/*--------------------全局常量定义---------------------*/
#define EE_DATE_TIME_ADDR			EE_BAK_STRUCT_ADDR(EE_DATE_TIME)	//eep地址

/*--------------------全局变量定义---------------------*/
uint8 SecFlag;
uint8 RTCFlashFlag;
uint8 MinFlag;
uint8 HourFlag;
tEE_DATE_TIME G_CurDateTime;
__no_init tEE_DATE_TIME CalcPpmDateTime;
__no_init  SOFTCALIPPM_TYPE  SoftCaliPPM;  //停电后,是软复位,这个值不能丢失,所以申明不初始化变量
/*--------------------内部函数申明-------------------------*/
void UserRTCSecIntCallback(void);
void UserRTCSecIntCallbackPowerDown(void);
void UserRTCMinIntCallback(void);
void UserRTCHourIntCallback(void);
/*********************************************************** 
函数功能：  停电下累计PPM 计算 秒的误差
入口参数：
出口参数：
备注说明：
//配置： 上电要配置 32760开RTC硬件校准  停电要配置为32768 软件计算秒的偏差 
//声明不初始化变量---- 数据合法性-----加校验和
//为了更精确, PPM 保留1位小数点
//不足点：
1：低功耗下, 在分钟中断累计PPM ，秒累加功耗大 ,首次下电, ppm累计有误差
2: 实际显示的秒存在误差，最大实际读出来时钟 0-999ms  +  修正的 0-999 ms <2S的样子
   但是因为 SoftCaliPPM.RTC_Error_ACC  这个值不会被清零, 所以修正0--999ms按道理是不会增加了
3：此结构体在校验和错误 和  校时的时候会被清零。
***********************************************************/
void DRV_RTC_CalcSecDeviation(void)
{	
    tDateTime TempCalcPpmDateTime;
    RTC_TimeInfoT	CurTime;
	RTC_DateInfoT	CurDate;
	RTC_DayInfoT	CurDay;
	int32 second;
	int8 secondAddFlag=0;
    
    if(!IsCheckSumRight(SoftCaliPPM)) 
    {
		memset((uint8*)&SoftCaliPPM,0,sizeof(SoftCaliPPM));
		return ;		
	}
	RTC_ReadDateTimeDay(&CurDate,&CurTime,&CurDay);
	TempCalcPpmDateTime.Second=CurTime.Sec;
	TempCalcPpmDateTime.Minute=CurTime.Min;
	TempCalcPpmDateTime.Hour=CurTime.Hour;
	if(CurDay==RTC_DAY_SUN)//week转换
	{
		TempCalcPpmDateTime.Week=7;
	}
    else
    {
        TempCalcPpmDateTime.Week=(uint8)CurDay;
    }
    TempCalcPpmDateTime.Day=CurDate.Date;
    TempCalcPpmDateTime.Month=CurDate.Mon;
    TempCalcPpmDateTime.HighByteYear=(uint8)((CurDate.Year+2000)>>8);
    TempCalcPpmDateTime.LowByteYear=(uint8)(CurDate.Year+2000);
    
    second = GetDuration(&TempCalcPpmDateTime,&CalcPpmDateTime.dateTime);
	if((second<0)||(second>80))
	{
		second = 0;
	}
    memcpy((uint8 *)&CalcPpmDateTime.dateTime, (uint8 *)&TempCalcPpmDateTime, sizeof(TempCalcPpmDateTime));
	CalcCheckSum(CalcPpmDateTime);	
	SoftCaliPPM.RTC_Error_ACC +=(SoftCaliPPM.ResultCaliPPM+2441) *second; //符号数相加 //xx.x  一位小数点 1min计算, 秒更新功耗大。瞬间有70UA了
    
	if(SoftCaliPPM.RTC_Error_ACC <0)
	{
		if(SoftCaliPPM.RTC_Error_ACC <-ONESECODERTCPPM) //扩大10倍
		{
			SoftCaliPPM.RTC_Error_ACC +=ONESECODERTCPPM;
			
            secondAddFlag = 1;
			//SoftCaliPPM.ErrorSeconds++; 
		}
	}
	else
	{
		if(SoftCaliPPM.RTC_Error_ACC >ONESECODERTCPPM)
		{
			SoftCaliPPM.RTC_Error_ACC -=ONESECODERTCPPM;
            secondAddFlag = -1;
			//SoftCaliPPM.ErrorSeconds--; 
		}
	}
    
    if(secondAddFlag)
    {
    	__disable_irq();
        SoftCaliPPM.ErrorSeconds += secondAddFlag;
		__enable_irq();
    }
    
	CalcCheckSum(SoftCaliPPM);
}
/*********************************************************** 
函数功能  即使校验和对了,也要判断一下数据合法性
入口参数：
出口参数：
备注说明：累计的PPM 在-10000000 和 10000000范围内
          累计更新的秒 最大 认为 一天相差 100s  停电3年 
***********************************************************/
uint8  DRV_RTC_CheckSoftCailValue(void)
{
	uint8 flag =0;
	
	if(SoftCaliPPM.RTC_Error_ACC >ONESECODERTCPPM)
	{
		flag =1;
	}
	if(SoftCaliPPM.RTC_Error_ACC <-ONESECODERTCPPM)
	{
		flag =1;
	}
	if(SoftCaliPPM.ErrorSeconds >MAXSECODECALI)
	{
		flag =1;
	}
	if(SoftCaliPPM.ErrorSeconds <-MAXSECODECALI)
	{
		flag =1;
	}
	return flag;
}
/*********************************************************** 
函数功能：RTC参数复位
入口参数：
出口参数：
备注说明：配置后rtc模式寄存器将被复位
***********************************************************/
uint8 CheckRTCSfr(void)
{
	if(FM3_RTC->WTCOSEL == 0x01)
    {
      	if(FM3_RTC->WTBR == ((RTC_CLOCK_PRECCALER_VALUE/4)-1))
      	{
			if((FM3_RTC->WTCR1_f.ST == 1) && (FM3_RTC->WTCLKS == 0x0))
			{
				return 0;
			}
		}
    }
	return 1;
}

/*********************************************************** 
函数功能：RTC中断配置
入口参数：
出口参数：
备注说明：1 配置RTC中断 秒、分、时
***********************************************************/
void DRV_RTC_IntConfig(void)
{
	//RTC中断配置
	//由于RTC控制寄存器在软复位的情况下不会被清除,所以不使用的中断这里全部关掉
	//如果一个中断被使能,但是又没有入口,会进入硬件错误中断
	RTC_ClrAllIntFlag();
	RTC_DisableAlarmInt();
	RTC_DisableHourInt();
	RTC_DisableMinInt();
	RTC_DisableSecInt();
	RTC_DisableSubSecInt();
	RTC_DisableTimerInt();	  

	/* Enable 1s interrupt */
	SecFlag=0;
	RTCFlashFlag=0;
	RTC_EnableSecInt(UserRTCSecIntCallback);  //系统时钟测量
	MinFlag=0;
	RTC_EnableMinInt(UserRTCMinIntCallback);
	HourFlag=0;
	RTC_EnableHourInt(UserRTCHourIntCallback);
	NVIC_EnableIRQ(OSC_PLL_WC_RTC_IRQn);
}

/*********************************************************** 
函数功能：RTC中断停电配置
入口参数：
出口参数：
备注说明：1 配置RTC中断 秒、分、时，用于唤醒
***********************************************************/
void DRV_RTC_IntConfigPowerDown(void)
{
	//RTC中断配置
	//由于RTC控制寄存器在软复位的情况下不会被清除,所以不使用的中断这里全部关掉
	//如果一个中断被使能,但是又没有入口,会进入硬件错误中断
	RTC_ClrAllIntFlag();
	RTC_DisableAlarmInt();
	RTC_DisableHourInt();
	RTC_DisableMinInt();
	RTC_DisableSecInt();
	RTC_DisableSubSecInt();
	RTC_DisableTimerInt();	  

	/* Enable 1s interrupt */
	SecFlag=0;
	RTCFlashFlag=0;
	RTC_EnableSecInt(UserRTCSecIntCallbackPowerDown);  //秒唤醒源
	MinFlag=0;
	RTC_EnableMinInt(UserRTCMinIntCallback);
	HourFlag=0;
	RTC_EnableHourInt(UserRTCHourIntCallback);
	NVIC_EnableIRQ(OSC_PLL_WC_RTC_IRQn);
}

/*********************************************************** 
函数功能：RTC复位加载
入口参数：
出口参数：
备注说明：
***********************************************************/
void DRV_RTC_ValueRest(void)
{
	tEE_DATE_TIME G_PowerDownDateTime;
	eTimeCmpResult Result;
	
	uint8  PowerdownTimeErr=false;
	uint8  CurClockErr=false;


	if(ResetCause & RST_PONR)  //POR 复位
	{
		RTCResetRecord[0]++;
        ResetCause&=~RST_PONR;
	}
	if(ResetCause & RST_HWDT)  //LVD 低压复位
	{
		RTCResetRecord[1]++;
        ResetCause&=~RST_HWDT;
	}	
	if((NvmBytesRead(EE_DATE_TIME_ADDR, &G_PowerDownDateTime, sizeof(G_PowerDownDateTime)))==FALSE)  //先读出掉电时间
	{
		  PowerdownTimeErr=true;    // err
	} 
    else
    {
        if(0==(DRV_RTC_Chk(&G_PowerDownDateTime.dateTime)))   //读出掉电时间正确,需要符合时间格式,否则也认为时间有问题
        {
          PowerdownTimeErr =true;	 //err	
        }   
    } 
	
	RTCFlashFlag=1;
	DRV_RTC_FlashTask();						//判断当前时间格式是否正确
	if(0==(DRV_RTC_Chk(&G_CurDateTime.dateTime)))
	{
		CurClockErr =true;		 //err
	}
	if(CurClockErr==true)
	{
		if(PowerdownTimeErr==true) //掉电时钟和当前时钟都错误
		{                            
			G_CurDateTime.dateTime.HighByteYear = 0x07;
			G_CurDateTime.dateTime.LowByteYear = 0xDE;
			G_CurDateTime.dateTime.Month = 07;
			G_CurDateTime.dateTime.Day = 25;
			G_CurDateTime.dateTime.Week = 5;
			G_CurDateTime.dateTime.Hour = 0;
			G_CurDateTime.dateTime.Minute = 0;
			G_CurDateTime.dateTime.Second = 0;
			
			G_CurDateTime.dateTime.ClockStatus=0;
			G_CurDateTime.dateTime.ClockStatus|=RTC_STATUS_INVALID_VALUE;
			RTCResetRecord[2]++;
		}
		else            //掉电时钟正确和当前时钟错误
		{	
			memcpy((uint8 *)&G_CurDateTime, (uint8 *)&G_PowerDownDateTime, sizeof(G_PowerDownDateTime));
 			
			G_CurDateTime.dateTime.ClockStatus=0;
			G_CurDateTime.dateTime.ClockStatus|=RTC_STATUS_INVALID_VALUE;		
			RTCResetRecord[3]++;
		}		
		CalcCheckSum(G_CurDateTime);
		DRV_RTC_Set(&G_CurDateTime.dateTime);
	}
	else
	{
		if(PowerdownTimeErr==false)	//时钟正确和掉电时间正确,判断掉电时间与当前时间大小,掉电时间如果大于当前时间认为时钟有错
		{		    
			Result = ClockCmp(&G_PowerDownDateTime.dateTime, &G_CurDateTime.dateTime);
			if(Result ==TIME1_LARGE_TIME2)
			{
				 //memcpy((uint8 *)&G_CurDateTime, (uint8 *)&G_PowerDownDateTime, sizeof(G_PowerDownDateTime));
				 //G_CurDateTime.dateTime.ClockStatus=0;
				 //G_CurDateTime.dateTime.ClockStatus|=RTC_STATUS_INVALID_VALUE;				 
				 RTCResetRecord[4]++;			 
			  	 //CalcCheckSum(G_CurDateTime);
			 	 //DRV_RTC_Set(&G_CurDateTime.dateTime);
			}
		}	
	}
}

/*********************************************************** 
函数功能：RTC参数复位
入口参数：
出口参数：
备注说明：单片机复位时调用
		  配置后rtc寄存器将被复位，
***********************************************************/
void DRV_RTC_Rest(uint8 flag)
{
    RTC_TimeInfoT   Time = {0};
    RTC_DateInfoT   Date = {0};
    RTC_DayInfoT   	Day = RTC_DAY_SUN;

    //RTC_ReadTime(&Time);//先读取，防止时钟退回到掉电时刻!!!!!!!!!!!!!!!!!!!
    //RTC_ReadDate(&Date);
    //RTC_ReadDay(&Day);
    RTC_ReadDateTimeDay(&Date,&Time,&Day);

    ////配置时钟输出引脚
    //InitRTCIO();
    //清除RTC中断标志
    RTC_ClrAllIntFlag();
    //暂停RTC
    RTC_Stop();
    //RTC时钟源为副时钟
    RTC_ClkInSel(RTC_CLOCK_IN_SUB_CLOCK);
    //关闭RTC时钟源分频
    RTC_DisableSuboutDiv();
//#if (RTC_CONFIG_TYPE == RTC_TYPE_A)    
//    //设置原时钟分频值－TYPE_A
//    RTC_SetSuboutDivVal(RTC_DIV_RATIO_SETTING_512);
//#else
    //设置原时钟分频值－TYPE_B
    RTC_SetSuboutDivVal(RTC_DIV_RATIO_SETTING_1);
//#endif    
    //使能RTC源时钟分频
    RTC_EnableSuboutDiv();
//#if (RTC_CONFIG_TYPE == RTC_TYPE_B)        
    //RTCCO输出1Hz
    RTC_COSel(RTC_CO_SEL_1Hz);
//#endif    
    //RTC计数模块的时钟分频
    RTC_SetCntCycle(((RTC_CLOCK_PRECCALER_VALUE/4)-1));
    //关闭RTC校准功能
    RTC_DisableCali();
//#if (RTC_CONFIG_TYPE == RTC_TYPE_B)  
    //设置校准周期
    RTC_SetCaliCycle(RTC_CLOCK_CALIBRATION_CYCLE);
//#endif    
    //设置RTC时钟校准值
    RTC_SetCaliVal(RTC_CAL_CENTER_VAL);
    //使能RTC校准功能
    RTC_EnableCali();
    
    //初始化RTC时间
    //Time.Sec = RTC_TIME_SEC;
    //Time.Min = RTC_TIME_MIN;
    //Time.Hour = RTC_TIME_HOUR;
    RTC_InitTime(&Time);
    
    /* 初始化RTC日期 */
    //Date.Date = RTC_DATE_DAY;
    //Date.Mon = RTC_DATE_MON;
    //Date.Year = (RTC_DATE_YEAR-2000);
    RTC_InitDate(&Date);	
	
    RTC_InitDay(Day);
    //启动RTC计数模块
    RTC_Start();

}

/*********************************************************** 
函数功能：RTC初始化
入口参数：
出口参数：
备注说明：上电时调用
		  配置rtc中断
		  刷新RTC，供其它模块初始化时调用
		  时钟刷新任务添加
***********************************************************/
void DRV_RTC_Init(void)
{
	
    RTCFlashFlag=1;
	DRV_RTC_FlashTask();	//刷新RTC，供其它模块初始化时调用
	DRV_RTC_IntConfig();
	TaskAdd(DRV_RTC_FlashTask, 0, 10, 1);		//时钟刷新任务添加
}

/*********************************************************** 
函数功能：RTC初始化
入口参数：
出口参数：
备注说明：电池上电时调用
		  配置rtc中断
		  刷新RTC，供其它模块初始化时调用
***********************************************************/
void DRV_RTC_InitUpBatt(void)
{
	RTCFlashFlag=1;
	DRV_RTC_FlashTask();	//刷新RTC，供其它模块初始化时调用
	DRV_RTC_IntConfigPowerDown();	//配置RTC中断唤醒
}

/*********************************************************** 
函数功能：RTC初始化
入口参数：
出口参数：
备注说明：停电时调用
		  配置rtc中断
		  刷新RTC，供其它模块初始化时调用
		  保存掉电时间
***********************************************************/
void DRV_RTC_InitPowerDown(void)
{
	RTCFlashFlag=1;
	DRV_RTC_FlashTask();	//刷新RTC，供其它模块初始化时调用
	NvmBytesWrite(EE_DATE_TIME_ADDR, &G_CurDateTime, sizeof(G_CurDateTime));	//保存掉电时间
	DRV_RTC_IntConfigPowerDown();	//配置RTC中断唤醒
}

/*********************************************************** 
函数功能：RTC获取停电时间
入口参数：
出口参数：
备注说明:
***********************************************************/
void DRV_RTCPowerDownTimeGet(tDateTime *dateTime)
{
	tEE_DATE_TIME temp;
	
	NvmBytesRead(EE_DATE_TIME_ADDR, &temp, sizeof(temp));
	*dateTime = temp.dateTime;
}

/*********************************************************** 
函数功能：RTC秒定时中断回调函数
入口参数：
出口参数：
备注说明:
***********************************************************/
void UserRTCSecIntCallback(void)
{
  SecFlag=1;
  RTCFlashFlag=1;
  //Hcr_Calib.HcfCnt = 0xFFFF - FM3_BT7_RT->TMR;    //获取捕获值
  Hcr_Calib.HcfCnt = 0xFFFF-BT_RTGet16bitCount(BT_CH_7);// 
  //        FM3_BT7_RT->TMCR &=~(TMCR_CTEN);
  BT_RTDisableCount(BT_CH_7);				
  FM3_BT7_RT->PCSR  = 0xFFFF;
  //        FM3_BT7_RT->TMCR |= (TMCR_CTEN); 
  BT_RTEnableCount(BT_CH_7);				
  //        FM3_BT7_RT->TMCR |= (TMCR_STRG);            //重新启动定时器
  BT_RTStartSoftTrig(BT_CH_7);
  
  if(SoftCaliPPM.ErrorSeconds)
  {
      DRV_RTC_FlashTask();
  }
}

/*********************************************************** 
函数功能：RTC秒定时中断回调函数
入口参数：
出口参数：
备注说明:
***********************************************************/
void UserRTCSecIntCallbackPowerDown(void)
{
  SecFlag=1;
  RTCFlashFlag=1;
  if(SoftCaliPPM.ErrorSeconds)
  {
      DRV_RTC_FlashTask();
  }
}

/*********************************************************** 
函数功能：RTC分定时中断回调函数
入口参数：
出口参数：
备注说明:
***********************************************************/
void UserRTCMinIntCallback(void)
{
  MinFlag=1;
}
/*********************************************************** 
函数功能：RTC小时定时中断回调函数
入口参数：
出口参数：
备注说明:
***********************************************************/
void UserRTCHourIntCallback(void)
{
  HourFlag=1;
}

/*********************************************************** 
函数功能：RTC获取秒状态
入口参数：
出口参数：1 发生秒中断  0没有
备注说明:读后清零
***********************************************************/
uint8 DRV_RTC_GetSecState(void)
{
  if(SecFlag)
  {
    SecFlag=0;
    return 1;
  }
  else
  {
    return 0;
  }
}

/*********************************************************** 
函数功能：RTC获取分状态
入口参数：
出口参数：1 发生分中断  0没有
备注说明:读后清零
***********************************************************/
uint8 DRV_RTC_GetMinState(void)
{
  if(MinFlag)
  {
    MinFlag=0;
    return 1;
  }
  else
  {
    return 0;
  }
}

/*********************************************************** 
函数功能：RTC获取小时状态
入口参数：
出口参数：1 发生小时中断  0没有
备注说明:读后清零
***********************************************************/
uint8 DRV_RTC_GetHourState(void)
{
  if(HourFlag)
  {
    HourFlag=0;
    return 1;
  }
  else
  {
    return 0;
  }
}

/*********************************************************** 
函数功能：RTC刷新 10ms
入口参数：
出口参数：
备注说明:
***********************************************************/
void DRV_RTC_FlashTask(void)
{
	RTC_TimeInfoT	CurTime;
	RTC_DateInfoT	CurDate;
	RTC_DayInfoT	CurDay;
	
    uint32 Seconds;	
	tDateTime Temp_CurDateTime;	
	
	if(RTCFlashFlag)
	{
		RTCFlashFlag=0;
        
        memcpy((uint8 *)&Temp_CurDateTime,(uint8 *)&G_CurDateTime,sizeof(Temp_CurDateTime));	//先保存一下, 不保存会出现校时候,时钟符号不消失问题
          
		//RTC_ReadTime(&CurTime);
		//RTC_ReadDate(&CurDate);
		//RTC_ReadDay(&CurDay);
        RTC_ReadDateTimeDay(&CurDate,&CurTime,&CurDay);
		Temp_CurDateTime.Second=CurTime.Sec;
		Temp_CurDateTime.Minute=CurTime.Min;
		Temp_CurDateTime.Hour=CurTime.Hour;
		if(CurDay==RTC_DAY_SUN)//week转换
		{
			Temp_CurDateTime.Week=7;
		}
		else
		{
			Temp_CurDateTime.Week=(uint8)CurDay;
		}
		Temp_CurDateTime.Day=CurDate.Date;
		Temp_CurDateTime.Month=CurDate.Mon;
		Temp_CurDateTime.HighByteYear=(uint8)((CurDate.Year+2000)>>8);
		Temp_CurDateTime.LowByteYear=(uint8)(CurDate.Year+2000);
		
		if(IsCheckSumRight(SoftCaliPPM)) //数据正确
		{   
			if(!DRV_RTC_CheckSoftCailValue())  //数据合法
			{
	            if(SoftCaliPPM.ErrorSeconds)  //秒修正有值
	            {
	                Seconds = DateTimeToSeconds(&Temp_CurDateTime) + SoftCaliPPM.ErrorSeconds;	//代数和 加减秒
	                SecondsToDateTime(&Temp_CurDateTime, Seconds);		
	                SoftCaliPPM.ErrorSeconds =0;
	                CalcCheckSum(SoftCaliPPM);
					
					CurTime.Sec=Temp_CurDateTime.Second;
					CurTime.Min=Temp_CurDateTime.Minute;
					CurTime.Hour=Temp_CurDateTime.Hour;
			
					if(Temp_CurDateTime.Week==7)//week转换
					{
						CurDay=RTC_DAY_SUN;
					}
					else
					{
						CurDay=(RTC_DayInfoT)Temp_CurDateTime.Week;
					}
					CurDate.Date=Temp_CurDateTime.Day;
					CurDate.Mon=Temp_CurDateTime.Month;
					CurDate.Year=((uint16)(Temp_CurDateTime.HighByteYear)<<8) + Temp_CurDateTime.LowByteYear - 2000;
					
					//RTC_WriteTime(&CurTime, RTC_CNT_RESET);
					//RTC_WriteDate(&CurDate, RTC_CNT_RESET);
					//RTC_WriteDay(CurDay, RTC_CNT_RESET);
                    RTC_WriteDateTimeDay(&CurDate,&CurTime,CurDay,RTC_CNT_CONTINUE);
	            }
			}
			else
			{
				memset((uint8*)&SoftCaliPPM,0,sizeof(SoftCaliPPM));
			}
		}	
		memcpy((uint8 *)&G_CurDateTime, (uint8 *)&Temp_CurDateTime, sizeof(Temp_CurDateTime));	 	
	}
    CalcCheckSum(G_CurDateTime);
}

/*********************************************************** 
函数功能：RTC获取
入口参数：
出口参数：
备注说明:1 不需要中断保护
		 2 也可以直接访问G_CurDateTime
***********************************************************/
void DRV_RTC_Get(tDateTime *dateTime)
{
	*dateTime = G_CurDateTime.dateTime;
}

/*********************************************************** 
函数功能：RTC设置
入口参数：
出口参数：
备注说明:
***********************************************************/
BOOL_B DRV_RTC_Set(tDateTime *dateTime)
{
	RTC_TimeInfoT	CurTime;
	RTC_DateInfoT	CurDate;
	RTC_DayInfoT	CurDay;

	if(!DRV_RTC_Chk(dateTime))
	{
		return false;
	}
	//重新配置参数
	DRV_RTC_Rest(1);
	DRV_RTC_IntConfig();
	
	dateTime->ClockStatus = G_CurDateTime.dateTime.ClockStatus;		//??
	G_CurDateTime.dateTime = *dateTime;
    CalcCheckSum(G_CurDateTime);
	CurTime.Sec=dateTime->Second;
	CurTime.Min=dateTime->Minute;
	CurTime.Hour=dateTime->Hour;
	if(dateTime->Week==7)//week转换
	{
		CurDay=RTC_DAY_SUN;
	}
	else
	{
		CurDay=(RTC_DayInfoT)dateTime->Week;
	}
	CurDate.Date=dateTime->Day;
	CurDate.Mon=dateTime->Month;
	CurDate.Year=((uint16)(dateTime->HighByteYear)<<8) + dateTime->LowByteYear - 2000;

	//RTC_WriteTime(&CurTime, RTC_CNT_RESET);
	//RTC_WriteDate(&CurDate, RTC_CNT_RESET);
	//RTC_WriteDay(CurDay, RTC_CNT_RESET);
    RTC_WriteDateTimeDay(&CurDate,&CurTime,CurDay,RTC_CNT_RESET);
    
	memset((uint8*)&SoftCaliPPM,0,sizeof(SoftCaliPPM));	//一校时此处停电 重新更新 软修正值  
	CalcCheckSum(SoftCaliPPM);
    
    memcpy((uint8 *)&CalcPpmDateTime.dateTime, (uint8 *)&G_CurDateTime.dateTime, sizeof(CalcPpmDateTime.dateTime));
	CalcCheckSum(CalcPpmDateTime);                      //校时更新校准比较时间
    
	return TRUE;
}

void DRV_RTC_ResetState(void)
{
	G_CurDateTime.dateTime.ClockStatus=0x00;		//夏令时由应用提供
    CalcCheckSum(G_CurDateTime);
}

/*********************************************************** 
函数功能：检查RTC数据合法性
入口参数：*c_rtc
出口参数：succ(1=succ;0=fail)
备 	  注：
***********************************************************/
uint8 DRV_RTC_Chk(tDateTime *c_rtc)
{
	uint16 Year = ((uint16)(c_rtc->HighByteYear) << 8) + c_rtc->LowByteYear;

	if((Year>=2012) && (Year<=2099))	//year:2012-2099
	{
		if((c_rtc->Month>=1) && (c_rtc->Month<=12))				//month:01-12
		{
			if((c_rtc->Day>=1) && (c_rtc->Day<=31))			//day:01-31
			{
//				if(c_rtc->Week<=6)							//week:00-06
				if(c_rtc->Week<=7)							//week:01-07
				{
					if((c_rtc->Hour<=23)) 			//hour:00-23
					{
						if((c_rtc->Minute<=59)) 	   //minute:00-59
						{
							if((c_rtc->Second<=59))    //second:00-59
							{
								return 1;
							}
						}
					}
				}
			}
		}
	}
	return 0;
}

