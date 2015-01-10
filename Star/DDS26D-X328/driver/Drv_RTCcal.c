
/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：drv_bt.h 
* 文件标识：见配置管理计划书 
* 摘要：基本定时器移植层

* 当前版本：1.0.0 
* 作者：张玉清
* 完成日期：20131024
*******************************************************************/
#include "Include.h"

/*--------------------全局常量定义---------------------*/
#define EE_CalParameter_ADDR			EE_BAK_STRUCT_ADDR(EE_CalParameter)	//eep地址

/*! \brief thrice compensation (*10) at each temperation point */
//const int s_ThriceComp[][2] = 
//{
//    {-450, 70},
//    {-400, 60},   //-45
//    {-350, 45},   //-40
//    {-300, 30},
//    {-250, 20},//
//    {-200, 10},
//    {-150, 5},
//    {-100, 0},
//    {-50,   0},
//    {0,     0},
//    {50,    0},
//    {100,   0},
//    {150,   0},
//    {200,   0},
//    {250,   0},
//    {300,   0},
//    {350,   0},
//    {400,   0},
//    {450,   0},
//    {500,  -10},//-5
//    {550, -15},//-10
//    {600, -20},//-15
//    {650, -35},//-20
//    {700, -50},//-30
//    {750, -55},//-35
//    {800, -60},
//    {850, -60},
//    {900, -60},
//    {950, -60},
//    {1000,-60},
//};
const int s_ThriceComp[][2] =       //晶振温度曲线
{
    {-450,  75},
    {-400,  65},   //-45
    {-350,  30},   //-40
    {-300,   5},
    {-250,  -8},//
    {-200, -18},
    {-150, -22},
    {-100, -25},
    {-50,  -24},
    {0,    -24},
    {50,   -20},
    {100,  -17},
    {150,  -12},
    {200,   -7},
    {250,   -3},
    {300,    0},
    {350,    1},
    {400,    1},
    {450,    3},
    {500,    6},//-5
    {550,   -1},//-10
    {600,   -7},//-15
    {650,  -13},//-20
    {700,  -19},//-30
    {750,  -43},//-35
    {800,  -50},
    {850,  -55},
    {900,  -60},
    {950,  -65},
    {1000, -70},
};

/*--------------------全局变量定义---------------------*/
tEE_RTC_Calibrate  CalParameter;//RTC校准参数

/*--------------------内部函数申明-------------------------*/
static int16 GetThriceCompValue(int16 Temp);
void DRV_RTCCalLoad(void);
/*!
 ******************************************************************************
 ** \brief Initialize the calibration value
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
 void InitCaliVal(void)
 {
   if(!IsCheckSumRight(SoftCaliPPM))
   {
		memset((uint8*)&SoftCaliPPM,0,sizeof(SoftCaliPPM));	
   }
   //校准参数加载
   DRV_RTCCalLoad();
/*  CalParameter.Slope=250;
  CalParameter.T0=470;
  CalParameter.Vol_adc=894;
  CalParameter.Ntc_adc=232;
  CalParameter.R_value=10000;
  CalParameter.Ntc_R_25=1000;
  CalParameter.CryFactor=-339;
  CalParameter.Crytemp=250;
  CalParameter.CryOffset=-5500;
  CalParameter.CryOffsetPPM=-5;
  CalParameter.RTC_state=90;
  CalParameter.CRC=0;*/
  
	//RTC计数模块的时钟分频
   //RTC_SetCntCycle(((RTC_CLOCK_PRECCALER_VALUE/4)-1));  //停电因为配置为了32768而且关掉了校准功能,所以上电要打开
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

   TaskAdd(CaliProcessTask, 4250, 60000, 1);
 }

 /*********************************************************** 
函数功能：停电下关闭校准,使用软件来计算秒的偏差
入口参数：
出口参数：
备注说明:
***********************************************************/
void PowerDownInitCail(void)
{
	//RTC计数模块的时钟分频
   //RTC_SetCntCycle(((32768/4)-1));  //停电不进行修正,使用晶振本来频偏
	//关闭RTC校准功能
   RTC_DisableCali();
}
/*********************************************************** 
函数功能：RTC顶点误差设置
入口参数：
出口参数：
备注说明:
***********************************************************/
void DRV_RTCCryOffsetSet(int16 CryOffset)
{
  CalParameter.CryOffset=CryOffset;
  CalcCheckSum(CalParameter);
  NvmBytesWrite(EE_CalParameter_ADDR, &CalParameter, sizeof(CalParameter));
}

/*********************************************************** 
函数功能：RTC校准参数加载
入口参数：
出口参数：
备注说明:
***********************************************************/
void DRV_RTCCalLoad(void)
{
	//加载参数
	if(NvmBytesRead(EE_CalParameter_ADDR, &CalParameter, sizeof(CalParameter)))
	{
	}
	else
	{
		CalParameter.Slope=250;
		CalParameter.T0=470;
		CalParameter.Vol_adc=894;
		CalParameter.Ntc_adc=232;
		CalParameter.R_value=10000;
		CalParameter.Ntc_R_25=1000;
		CalParameter.CryFactor=-339;
		CalParameter.Crytemp=250;
//		CalParameter.CryOffset=-5500;
//		CalParameter.CryOffsetPPM=-5;
		CalParameter.CryOffset=0;
		CalParameter.CryOffsetPPM=0;
		CalParameter.RTC_state=90;
		CalParameter.CRC=0;
	}

}

/*!
 ******************************************************************************
 ** \brief Get thrice compensation value
 **
 ** \param Temp cuurent temperature
 **         
 ** \return None
 **
 ******************************************************************************
 */
static int16 GetThriceCompValue(int16 Temp)
{
    uint8 i;    
    int16 j,k, ThriceCompVal;
    int16 CurTemp = Temp;
       
    for(i=0;i<(sizeof(s_ThriceComp)/sizeof(s_ThriceComp[0])) -1; i++)
    {
        if((s_ThriceComp[i][0]) <= CurTemp && (s_ThriceComp[i+1][0] >= CurTemp))
        {
            break;
        }
    }
    j= s_ThriceComp[i+1][1]-s_ThriceComp[i][1];
    k = CurTemp-s_ThriceComp[i][0];

    ThriceCompVal = s_ThriceComp[i][1] + (j*k)/50;
    
    return ThriceCompVal;
}

/*!
 ******************************************************************************
 ** \brief Load normal temperature value from Flash
 **
 ** \param void
 **         
 ** \return None
 **
 ******************************************************************************
 */
//static uint32_t LoadNormalDemVal(void)
//{
//    return *((uint32_t*)RTC_NORMAL_DEC_VAL_ADDR);
//}
/*********************************************************** 
函数功能：  停电下PPM（晶振原始频偏 + 温度频偏）计算
入口参数：
出口参数：
备注说明：
***********************************************************/
void CalcPPM(int16 Temp)
{
    int64 Result = 0;
    int64 ResultPulse = 0;
    int16 ThriceVal=0;
//    uint32 SubNormalDecVal;
    /* Load the demarcation value from Flash */
//    SubNormalDecVal = LoadNormalDemVal();
    /* Calculate the thrice compensation value according to current temperature */
    ThriceVal = GetThriceCompValue(Temp);
    
    /* Calculate the frequency deviation by Frequency-Temperature formular*/
    /* |F-Fo| = B*(T-To)*(T-To) */

 
    if(Temp >(int)RTC_OSC_TURN_TEMP_A)
    {
      Result = (uint64)RTC_OSC_TWICE_PARA_A*((Temp) - RTC_OSC_TURN_TEMP_A)*((Temp) - RTC_OSC_TURN_TEMP_A);
    }
    else
    {
      Result = (uint64)RTC_OSC_TWICE_PARA_A*(RTC_OSC_TURN_TEMP_A - (Temp))*(RTC_OSC_TURN_TEMP_A - (Temp));
    }
    
    /* The original value has been amplified by 10^8 */
    /* Reduce 10^7 multiple first */
    Result = Result/10000000;
    //--------------------------------------------------------------------------
    ResultPulse = CalParameter.CryOffset/100 - Result + ThriceVal;

	SoftCaliPPM.ResultCaliPPM =(int16)ResultPulse;
	CalcCheckSum(SoftCaliPPM);
} 
/*!
 ******************************************************************************
 ** \brief Calculate the frequency deviation according to current temperature
 **
 ** \param Temp current temperature
 **         
 ** \return None
 **
 
 WTBR = (0.5 [s] ÷ (2 × MCLK cycles [s])) - 1 =8191
 WTCAL = {(Frequency before correction - (WTBR+1)×4) / Ideal frequency} × 220 
 ******************************************************************************
 */
uint16 CaliProcess(int16 Temp)
{
    int64 Result = 0;
    int64 ResultPulse = 0;
    int16 ThriceVal=0;
//    uint32 SubNormalDecVal;
    /* Load the demarcation value from Flash */
//    SubNormalDecVal = LoadNormalDemVal();
    /* Calculate the thrice compensation value according to current temperature */
    ThriceVal = GetThriceCompValue(Temp);
    
    /* Calculate the frequency deviation by Frequency-Temperature formular*/
    /* |F-Fo| = B*(T-To)*(T-To) */

 
    if(Temp >(int)RTC_OSC_TURN_TEMP_A)
    {
      Result = (uint64)RTC_OSC_TWICE_PARA_A*((Temp) - RTC_OSC_TURN_TEMP_A)*((Temp) - RTC_OSC_TURN_TEMP_A);
    }
    else
    {
      Result = (uint64)RTC_OSC_TWICE_PARA_A*(RTC_OSC_TURN_TEMP_A - (Temp))*(RTC_OSC_TURN_TEMP_A - (Temp));
    }
    
    /* The original value has been amplified by 10^8 */
    /* Reduce 10^7 multiple first */
    Result = Result/10000000;
    //--------------------------------------------------------------------------
    ResultPulse = CalParameter.CryOffset/100 - Result + ThriceVal;
    RtcWriteCoef(ResultPulse);
    
	SoftCaliPPM.ResultCaliPPM =(int16)ResultPulse; //上电涮新 
	CalcCheckSum(SoftCaliPPM);
    //--------------------------------------------------------------------------
    /* Interagte the normal temperature demarcation value and compensation value */
    Result = RTC_32768HZ_PPM_OFFSET_A + CalParameter.CryOffset/100 -Result;
    /* Regurate the frequency deviation by thrice compensation value */

    Result = Result + ThriceVal;

    /* Get the calibration step (*10) */
    Result = (Result*1000)/RTC_CALI_PPM_PER_STEP_A;
    /* Regulate the calibration step by round arithmetic */
    if(Result%10 >= 7)
    {
        Result = (Result/10) + 1;
    }
    else
    {
        Result = (Result/10);
    }

    /* If invalid value, don't make calibration */
    if(Result > 1024 || Result <0) return 0;
    /* Write the final value into frequency correction setting register */
    RTC_SetCaliVal((uint16)Result);
    return (uint16)Result;
}

/*********************************************************** 
函数功能：温度补偿任务 60s Task
入口参数：
出口参数：
备注说明:
***********************************************************/
void CaliProcessTask(void)
{
	int16 temp;
	
	TempGet(&temp, 1);
	CaliProcess(temp);//校准处理
}

/*********************************************************** 
函数功能：温度补偿任务 60s Task
入口参数：
出口参数：
备注说明:
***********************************************************/
void CaliProcessTaskPowerDown(void)
{
    int16 temp;
    TempGet(&temp, 0);
    CalcPPM(temp);
}

/*********************************************************** 
函数功能：顶点温度PPM调整
入口参数：
出口参数：
备注说明：根据当前电表温度和当前温度下晶振PPM求出顶点温度PPM
		  CalParameter.CryOffset=PPMTest-RTC_OSC_TWICE_PARA*(T-Ti)*(T-Ti)
***********************************************************/
#pragma optimize=none
void AdjustCryOffset(int16 PPMTest)
{
//    int16 tt;
//    int32 temp;

	if(PPMTest>=10000)
	{
		CalParameter.CryOffset=0;
		CalParameter.CryOffsetPPM=0;
	}
	else
	{
		//CalParameter.CryOffset=PPMTest-CalParameter.CryFactor*(T-Ti)*(T-Ti)
//	    TempGet(&tt, 1);   //由NTC获取的当前温度值
//	    temp = (int32)((int32)tt-(int32)RTC_OSC_TURN_TEMP_A) * (int32)((int32)tt-(int32)RTC_OSC_TURN_TEMP_A);    //CalParameter->Crytemp表示晶体的顶点温度(x10)
//	    temp*= -(int32)(RTC_OSC_TWICE_PARA*10000);   //CalParameter->CryFactor表示晶体二次项系数(x10000)
//	    temp = temp/(int32)1000;
//		CalParameter.CryOffset =(int32)PPMTest*(int32)100 - temp;		  //晶体顶点温度偏差值
//	    CalParameter.CryOffsetPPM= CalParameter.CryOffset/1000;         //晶体顶点温度偏差值

		//测试的就是带补偿的PPM，不用调整
		CalParameter.CryOffset =(int32)PPMTest*(int32)100;
	    CalParameter.CryOffsetPPM= CalParameter.CryOffset/1000;         //晶体顶点温度偏差值
	}
	//保存
	CalcCheckSum(CalParameter);
	NvmBytesWrite(EE_CalParameter_ADDR, &CalParameter, sizeof(CalParameter));

	//执行一次补偿
    
    
	CaliProcessTask();
}

