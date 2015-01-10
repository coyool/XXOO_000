
/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：powermanage.h
* 文件标识：见配置管理计划书 
* 摘要：电源管理移植层

* 当前版本：1.0.0 
* 作者：ffq
* 完成日期：20131029
*******************************************************************/
#include "Include.h"



/*--------------------全局常量定义-------------------------*/

/*--------------------全局变量定义-------------------------*/
uint8 scrKeyIntFlag;
uint8 VccChkIntFlag;
uint8 lvdIntFlag;
uint32 wakeUpTimer;
uint8 BattLowState=0;
uint32 SleepDispTimer;	//停电停显时间定时
/*--------------------内部函数申明-------------------------*/
void LvdOpen(void);
void LvdClose(void);
static void Lvd_IntHandler(void);
IntStatusT LvdGetManaul(void);
void PowerExtInt_Init(void);
void ScrExtInt_Init(void);
static void ScrExt_IntCallback(void);
void DRV_ADC_Open(uint8_t ch);
uint16_t DRV_ADC_SampleOneTime(uint8_t ch);
void DRV_ADC_Close(uint8 ch);
uint8 PowerUpDetect(void);
uint8 PowerDownDetect(void);
uint16 PowerUpInit(void);
void PowerUpBattInit(void);
void PowerDownInit(void);
uint16 PowerDowmMain(void);
void PowerManage(void);
void PowerManage_Mode(void);
void PowerWakeTierRst(void);
void SleepDispTimerRst(void);
void PowerDownClockCopy(void);

/*********************************************************** 
函数功能：打开LVD掉电检测
入口参数：
出口参数：
备注说明：1 上电初始化中首先打开掉电检测
		  2 发送掉电后会产生LVD中断
		  3 检测电压4.0V
***********************************************************/
void LvdOpen(void)
{
	lvdIntFlag=0;
    NVIC_DisableIRQ(LVD_IRQn);		//!!
    LVD_ClrIntFlag(); 
    LVD_UnlockCtrlReg();
    LVD_SetDetectVolt(LVD_VOL_VAL_4_0);
    LVD_EnableInt(Lvd_IntHandler);
    while(SET != LVD_GetIntRdyStat());
    LVD_LockCtrlReg();    
    NVIC_EnableIRQ(LVD_IRQn);
}

/*********************************************************** 
函数功能：关闭LVD掉电检测
入口参数：
出口参数：
备注说明：1 发送LVD中断后需要关闭LVD，否则一直进入LVD中断
		  2 停电后不能启用LPC_LOW_POWER_MODE，否则用LVD检测上电时间会很长
***********************************************************/
void LvdClose(void)
{
    LVD_ClrIntFlag();
    LVD_UnlockCtrlReg();                   
    LVD_DisableInt();
    LVD_DisableReset();
    //LVD_ConfigLowPowerMode(LPC_LOW_POWER_MODE);		//LPC_LOW_POWER_MODE下时间会很长
    LVD_LockCtrlReg(); 
    NVIC_DisableIRQ(LVD_IRQn);
}

/*********************************************************** 
函数功能：LVD中断回调函数
入口参数：
出口参数：
备注说明：1 置lvdIntFlag标志，供PowerDownDetect调用
		  2 发送LVD中断后需要关闭LVD，否则一直进入LVD中断
***********************************************************/
static void Lvd_IntHandler(void)
{
	lvdIntFlag=1;
    LvdClose();
}

/*********************************************************** 
函数功能：手动获取LVD状态
入口参数：
出口参数：
RESET = 高于门限
SET = 低于门限
备注说明：
1 
***********************************************************/
IntStatusT LvdGetManaul(void)
{
	IntStatusT temp;
	
    NVIC_DisableIRQ(LVD_IRQn);		//!!
    //LVD_ClrIntFlag(); 
    bFM3_LVD_LVD_CLR_LVDCL = 0;
    //LVD_UnlockCtrlReg();	
    FM3_LVD->LVD_RLR = LVD_UNLOCK;
    //LVD_SetDetectVolt(VoltLevel);
    FM3_LVD->LVD_CTL &= ~(15ul<<2);
    FM3_LVD->LVD_CTL |= (LVD_VOL_VAL_4_2<<2);
    //LVD_EnableInt(Lvd_IntHandler);
    bFM3_LVD_LVD_CTL_LVDIE = 1;
    //while(SET != LVD_GetIntRdyStat());
    while(SET != (IntStatusT)bFM3_LVD_LVD_STR2_LVDIRDY);
	//temp=LVD_GetIntFlag();
	temp=(IntStatusT)bFM3_LVD_LVD_STR_LVDIR;
    //LVD_DisableInt();
    bFM3_LVD_LVD_CTL_LVDIE = 0;
    //LVD_LockCtrlReg();
    FM3_LVD->LVD_RLR = 0;
	return temp;
}


/*********************************************************** 
函数功能：市電外部中斷初始化
入口参数：
出口参数：
备注说明:
***********************************************************/
void PowerExtInt_Init(void)
{
    IO_DisableAnalogInput(IO_AN03);
    IO_EnableFunc(IO_PORT1, IO_PINx3);
    IO_ConfigGPIOPin(IO_PORT1, IO_PINx3, IO_DIR_INPUT, IO_PULLUP_OFF);
}

/*********************************************************** 
函数功能：轮显按键中斷初始化
入口参数：
出口参数：
备注说明:
***********************************************************/
void ScrExtInt_Init(void)
{
    scrKeyIntFlag=0;
    EXTI_DisableInt(EXTI_CH1);
    bFM3_LCDC_LCDCC3_VE3 = 0;
    IO_EnableFunc(IO_PORT5, IO_PINx1);
    IO_ConfigFuncINTxPin(IO_EXT_INT_CH1, IO_INTx_INTx_0);
    IO_ConfigGPIOPin(IO_PORT5, IO_PINx1, IO_DIR_INPUT, IO_PULLUP_OFF);
    EXTI_SetIntDetectMode(EXTI_CH1, EXTI_EDGE_FALLING);
    EXTI_ClrIntFlag(EXTI_CH1);
    EXTI_EnableInt(EXTI_CH1,ScrExt_IntCallback);
    NVIC_EnableIRQ(EXINT0_7_IRQn);
}


/*********************************************************** 
函数功能：轮显按键外部中断回调函数
入口参数：
出口参数：
备注说明:
***********************************************************/
static void ScrExt_IntCallback(void)
{
    if(IO_BIT_CLR == IO_ReadGPIOPin(IO_PORT5,IO_PINx1))
    {
		scrKeyIntFlag= 1;
    }     
}

/*********************************************************** 
函数功能：上电检测
入口参数：
出口参数：0:没上电  1:上电
备注说明：1 在复位后、掉电模式下调用
		  2 LVD检测VCC-B是否高于4.2V
***********************************************************/
uint8 PowerUpDetect(void)
{
	if(LvdGetManaul() == RESET)
	{//上电
		if(LvdGetManaul() == RESET)
		{//上电
			return 1;
		}
	}
	//没上电
        lvdIntFlag=1;
	return 0;

/*	//test
	if(IO_ReadGPIOPin(IO_PORT3, IO_PINx4)==1)
	{//上电
		return 1;
	}
	else
	{//没上电
		return 0;
	}*/

}

/*********************************************************** 
函数功能：自动掉电检测
入口参数：
出口参数：0:没掉电  1:掉电
备注说明：1 在上电模式下调用
		  2 通过LVD判断
		  
***********************************************************/
uint8 PowerDownDetect(void)
{
	if(lvdIntFlag)
	{//掉电
		return 1;
	}
	else
	{//没掉电
		return 0;
	}
	
/*	//test
	if(IO_ReadGPIOPin(IO_PORT3, IO_PINx4)==0)
	{//掉电
		return 1;
	}
	else
	{//没掉电
		return 0;
	}*/
}

/*********************************************************** 
函数功能：上电模式初始化
入口参数：
出口参数：
备注说明：1 市电上电时调用
          2 调用每个模块初始化函数，初始化函数包括所有的硬件初始化、数据初始化、任务初始化
***********************************************************/
uint16 PowerUpInit(void)
{
	DRV_WD_FeedDog();		//喂狗
	//切换到高速时钟
    SwitchToMainPLL(CLOCK_SUB_OSC_ON,BaseSysetmClockDIV1,PLLMultiplier2);
    DRV_RTC_CalcSecDeviation();           //上电马上进行补偿
	/*设置中断优先级*/
	Config_SystemClock_Priority();
    LvdOpen();		//打开LVD
    EXTI_DisableInt(EXTI_CH1);
	if(PowerDownDetect() != 0)
	{
		return 0;
	}
	/*初始化主任务*/
	TaskInit(SYS_TICK_LOW, SYS_TICK_HIGH);	//只初始化，不启动		
	/*初始化驱动模块*/
	DRV_IO_Init();
	Drv_Calibrate_Init();
	DRV_Calibrate_PRO_SWI_INIT();			//used for calibrate
	IIcInit();	
	//MX25L3206_IOInt();
	KeyInit();
	DRV_ADC_Init(1);
    DRV_ADC_On();
//	DRV_RTC_CalcSecDeviation();
    DRV_RTC_Init();  		//rtc初始化
    InitCaliVal();      	//RTC温补
    DRV_RTCSecPulseInit();  //rtc秒脉冲初始化
	DRV_LCD_Init(0);
	DRV_LCD_FillAll();
	LEDInit();
	ProtocolManageInit();	//通讯串口打开
	//RF 初始化
	RFModeChangeInit();     //RF切换模块
	RFIOPowerUpInit();
    
	if(0 == DRV_EMU_PowerUpInit())//used for EMU,load cailibrate data
	{
		return POWER_UP_INIT_FAIL;
	}
	//初始化应用模块
	ClockPowerUpInit();
//	if(POWER_UP_INIT_FAIL == TariffInit())
//	{
//		return POWER_UP_INIT_FAIL;
//	}
	//DRV_WD_FeedDog();		//?11·
	//DemandDataClear();
	//DemandInit();
	if(POWER_UP_INIT_FAIL == EnergyDataInit())
	{
		return POWER_UP_INIT_FAIL;
	}
	DRV_WD_FeedDog();		//?11·
	if(POWER_UP_INIT_FAIL == BillingPowerUpInit())
	{
		return POWER_UP_INIT_FAIL;
	}
	DRV_WD_FeedDog();		//?11·
//	if(POWER_UP_INIT_FAIL == LoadProfilePowerUpInit())
//	{
//		return POWER_UP_INIT_FAIL;
//	}
	if(POWER_UP_INIT_FAIL == FreezeInit())
	{
		return POWER_UP_INIT_FAIL;
	}
	DRV_WD_FeedDog();		//?11·
	if(POWER_UP_INIT_FAIL == EventPowerUpInit())
	{
		return POWER_UP_INIT_FAIL;
	}
	MeterInfoPowerUpInit();
	//DLMS_AA_Init();
	//------end----------
   //==============================================================
	Init_Disp();
   //==============================================================
    MeterPowerUPClearAll();         //全清，生产用
	//任务起动
   	MFT_FRTStart(MFT_UNIT0, FRT_CH2);
	TaskStart();
	//RF1MSStart();
	return POWER_UP_INIT_SUCESS;
}

/*********************************************************** 
函数功能：电池上电模式初始化
入口参数：
出口参数：
备注说明：1 电池上电时调用
          2 调用每个模块初始化函数，初始化函数包括所有的硬件初始化、数据初始化、任务初始化
***********************************************************/
void PowerUpBattInit(void)
{
	//电池上电跑高速RC分频//后面有必要再调
	SwitchToHighCR(CLOCK_MAINPLL_OFF, CLOCK_MAIN_OSC_OFF, CLOCK_SUB_OSC_ON, BaseSysetmClockDIV8);
	//初始化主任务
	TaskInit(0, 0);
	RF1MSPowerDownInit();
	//关闭RF外部中断
	Disable_ExtInt();
	//硬件设置为低功耗模式
    LvdClose();				//关闭LVD
	DRV_IO_PowerDownInit();
	//存储器相关打开
	IIcPowerDownInit();	
	//MX25L3206_IOPowerDownInt();//!
	KeyPowerDownInit();
	DRV_ADC_Init(1);	//先开一下ADC
//	DRV_RTC_CalcSecDeviation();
    DRV_RTC_InitUpBatt();  //rtc中断初始化
    InitCaliVal();      //RTC温补
    PowerDownInitCail(); //停电下 不补偿
//  DRV_RTC_CalcSecDeviation();
    DRV_RTCSecPulsePowerDownInit();  //rtc秒脉冲初始化
    ClockBatteryUpInit();
    DRV_EMU_PowerDownInit();//used for EMU
    DRV_LCD_Init(1);    
	DRV_LCD_FillAll();
	LEDPowerDownInit();
	ProtocolManagePowerDownInit();//串口
    //SPI_POWER_DOWN_Init();
    RFIOPowerDownInit();
    DRV_Calibrate_PRO_SWI_POWER_DOWN_INIT();
    Feed_watchdog();
    //IIcInit();
	//存储器相关打开
	//IIcPowerDownInit();	
	//MX25L3206_IOPowerDownInt();//!
	//应用模块掉电初始化，保存数据等
   	EnergyDataInitBattery();
	//EventPowerBatteryInit();
	Init_DispPowerUpBatt();
	SleepDispTimerRst();


	//---------RTC校准处理
	DRV_ADC_StartTask();
    while(RESET != ADC12_GetScanStatusFlag(ADC12_UNIT0));
    DRV_ADC_StartTask();
    while(RESET != ADC12_GetScanStatusFlag(ADC12_UNIT0));
    DRV_ADC_StartTask();
    while(RESET != ADC12_GetScanStatusFlag(ADC12_UNIT0));
    DRV_ADC_StartTask();
    while(RESET != ADC12_GetScanStatusFlag(ADC12_UNIT0));
    DRV_ADC_StartTask();
    while(RESET != ADC12_GetScanStatusFlag(ADC12_UNIT0));
    CaliProcessTaskPowerDown();
	DRV_RTC_CalcSecDeviation();
	DRV_ADC_Init(0);	//关闭ADC
	//存储器相关关闭
	//设置唤醒源，进入休眠
    LCD_ConfigInputIOMode(LCD_INPUT_IO_CUTOFF);
    ScrExtInt_Init();		//轮显按键中断
    PowerExtInt_Init();
    
	//电池上电采用高速RC需要回到低速
	SwitchToLowCR(CLOCK_MAINPLL_OFF,CLOCK_MAIN_OSC_OFF,CLOCK_SUB_OSC_ON, BaseSysetmClockDIV1);
	LowPwrCon_GoToStandByMode(STB_TimerMode,STB_IO_KEEP);	//休眠
}

/*********************************************************** 
函数功能：掉电模式初始化
入口参数：
出口参数：
备注说明：1 硬件设置为低功耗模式
          2 应用模块掉电初始化，保存数据等
          3 数据保存完后，关闭存储器
          3 设置唤醒源，进入休眠
***********************************************************/
void PowerDownInit(void)
{	
	__disable_irq();		//关闭总中断，停电处理期间不再响应中断，避免占用停电处理时间
	//初始化主任务
	TaskInit(0, 0);
	RF1MSPowerDownInit();
	//关闭RF外部中断
    Disable_ExtInt();	
	//硬件设置为低功耗模式
	LvdClose();				//关闭LVD
    MFT_FRTStop(MFT_UNIT0, FRT_CH2);
	DRV_IO_PowerDownInit();
	KeyPowerDownInit();
	DRV_ADC_Init(0);
    DRV_ADC_Off();
//	DRV_RTC_CalcSecDeviation();
    DRV_RTC_InitPowerDown();  //rtc中断初始化
    DRV_RTCSecPulsePowerDownInit();  //rtc秒脉冲初始化
    PowerDownClockCopy();
    DRV_RTC_CalcSecDeviation();
    PowerDownInitCail(); //停电下 不补偿 
	DRV_EMU_PowerDownInit();//used for EMU
	DRV_LCD_Init(1);	
	LEDPowerDownInit();
	ProtocolManagePowerDownInit();//串口
	//SPI_POWER_DOWN_Init();
	RFIOPowerDownInit();
    DRV_Calibrate_PRO_SWI_POWER_DOWN_INIT();
	//应用模块掉电初始化，保存数据等
	EnergyPowerOffProcess();
	Init_DispPowerDown();
	SleepDispTimerRst();
	//BillingPowerDownProcess();
	//EventPowerdownInit();
	MeterInfoPowerDownInit();
	
	//存储器相关
	IIcPowerDownInit(); 
	//MX25L3206_IOPowerDownInt();//!


	//设置唤醒源，进入休眠
	ScrExtInt_Init();		//轮显按键中断
    PowerExtInt_Init();
    LCD_ConfigInputIOMode(LCD_INPUT_IO_CUTOFF);
	__enable_irq();
	SwitchToLowCR(CLOCK_MAINPLL_OFF,CLOCK_MAIN_OSC_OFF,CLOCK_SUB_OSC_ON, BaseSysetmClockDIV1);
	LowPwrCon_GoToStandByMode(STB_TimerMode,STB_IO_KEEP);	//休眠
  
}

void PowerUpInitManage(void)
{
	uint16 Flag = 0;
	
	while(1)
	{
		if(PowerUpDetect()==0)
		{//电池上电模式
			PowerUpBattInit();
			Flag = PowerDowmMain();	//循环函数，直到上电退出
		}
		else
		{//上电模式
			Flag = PowerUpInit();
		}
		if(Flag == POWER_UP_INIT_SUCESS)
		{
			return;
		}
	}
}

/*********************************************************** 
函数功能：按键唤醒初始化
入口参数：
出口参数：
备注说明：1 按键唤醒后调用
          2 初始化唤醒下的任务
          
***********************************************************/
void PowerSleepWakeInit(void)
{	
	//按键唤醒后跑高速RC//后面有必要再调
	SwitchToHighCR(CLOCK_MAINPLL_OFF, CLOCK_MAIN_OSC_OFF, CLOCK_SUB_OSC_ON, BaseSysetmClockDIV8);
	//初始化主任务
	TaskInit(SYS_TICK_LOW, 0);

	//硬件设置为低功耗模式
	KeyPowerDownInit();
	DRV_LCD_Init(1);	//7天后需要重新打开LCD
	KeyPowerDownInit();
    DRV_RTC_FlashTask();
	//Clock_CheckDST();
	//存储器相关
	IIcPowerDownInit(); 
	//MX25L3206_IOPowerDownInt();//!
    
	//2 应用模块掉电初始化，保存数据等
	//==============================================================
	Init_DispPowerDown();
	//KeyScrTaskAdd(PowerWakeTierRst);
	PowerWakeTierRst();
	//==============================================================
	SleepDispTimerRst();   //按键重新开启7天
	TaskStart();
}

/*********************************************************** 
函数功能：按键唤醒初始化
入口参数：
出口参数：
备注说明：1 按键唤醒后调用
          2 初始化唤醒下的任务
          
***********************************************************/
void PowerWakeSleepInit(void)
{	
	//初始化主任务
	TaskInit(0, 0);

	//硬件设置为低功耗模式
	//停电停显定时			
	if(!SleepDispTimer)
	{
		DRV_LCD_Uninit(); //关闭LCD
	}
	//存储器相关
	//MX25L3206_IOPowerDownInt();
	//2 应用模块掉电初始化，保存数据等
	Init_DispPowerDown();

	//设置唤醒源，进入休眠
	ScrExtInt_Init();		//轮显按键中断
    PowerExtInt_Init();
	//按键模式采用高速RC需要回到低速
	LCD_ConfigInputIOMode(LCD_INPUT_IO_CUTOFF);
	SwitchToLowCR(CLOCK_MAINPLL_OFF,CLOCK_MAIN_OSC_OFF,CLOCK_SUB_OSC_ON, BaseSysetmClockDIV1);
}

/*********************************************************** 
函数功能：停电模式循环
入口参数：
出口参数：
备注说明：1 空闲进入休眠，停显1屏，停电7天后关闭液晶显示
          2 定时1s唤醒，判断是否上电，喂狗，每分钟RTC温度补偿
          3 按键唤醒，低速运行一个轮显周期
          4 检测到上电后直接复位
***********************************************************/
uint16 PowerDowmMain(void)
{
	uint32 timer500ms=0;
	uint32 timer1000ms=0;
    int16 temp;
    volatile uint8 BatteryFlag = 0;
    volatile uint8 BatteryState;

	DRV_WD_FeedDog();
	//LvdGetManaul();		//设置LVD门限等
	while(1)
	{
		//按键中断唤醒
		if(scrKeyIntFlag)
		{
			scrKeyIntFlag=0;
			EXTI_DisableInt(EXTI_CH1);
            
			DRV_RTC_FlashTask();

			//adc
            DRV_ADC_On();
            
            DRV_ADC_StartTask();
            while(RESET != ADC12_GetScanStatusFlag(ADC12_UNIT0));
            DRV_ADC_StartTask();
            while(RESET != ADC12_GetScanStatusFlag(ADC12_UNIT0));
            DRV_ADC_StartTask();
            while(RESET != ADC12_GetScanStatusFlag(ADC12_UNIT0));
            DRV_ADC_StartTask();
            while(RESET != ADC12_GetScanStatusFlag(ADC12_UNIT0));
            DRV_ADC_StartTask();
            while(RESET != ADC12_GetScanStatusFlag(ADC12_UNIT0));
            
            DRV_ADC_Off();    
			BatteryState = BattLowStateGet();

			if(BatteryState == 0)
			{
				BatteryFlag = 0;
				PowerSleepWakeInit();
				//按键唤醒主循环
				while(1)
				{
					if(runMe)
					{
						//5ms任务
						runMe=0;
						timer500ms++;
						timer1000ms++;
						DRV_WD_FeedDog();
						if(FM3_GPIO->PDIR1&= IO_PINx3)
						{//上电外部中断唤醒
							if(PowerUpDetect())
							{
								PowerWakeSleepInit();//设置唤醒源，退出低速运行
								break;			
							}
						}
						Key_Scr_DetectPowerDown_Task();//轮显按键驱动
						//CoverDetectProcess();
						//EventBatteryUpdateStateFlagProcess();
						//500ms任务
						if(timer500ms>100)
						{
							timer500ms=0;
							//DRV_RTC_FlashTask();
							//Clock_CheckDST();
						}
						DRV_RTC_FlashTask();
						//1000ms任务
						if(timer1000ms>200)
						{
							timer1000ms=0;
                        	PowerDownDisplayAlarmPro();
                        	//LCD_Flag_Driver_Task();
                        	Disp_Auto_Cycle_Task();	//显示
							if(wakeUpTimer)		//定时回到休眠
							{
								wakeUpTimer--;
							}
							else
							{
								PowerWakeSleepInit();//设置唤醒源，退出低速运行
								break;
							}
						}
					}
				}	
			}
			else
			{
				DRV_LCD_Uninit();
			}
		}
		
		//1s定时中断唤醒
		if(SecFlag==1)
		{
			SecFlag=0;           
		}
        
        //1min定时中断唤醒
        if(MinFlag==1)
        {
            MinFlag=0;
                   
            //adc
            DRV_ADC_On();
          
            DRV_ADC_StartTask();
            while(RESET != ADC12_GetScanStatusFlag(ADC12_UNIT0));
            DRV_ADC_StartTask();
            while(RESET != ADC12_GetScanStatusFlag(ADC12_UNIT0));
            DRV_ADC_StartTask();
            while(RESET != ADC12_GetScanStatusFlag(ADC12_UNIT0));
            DRV_ADC_StartTask();
            while(RESET != ADC12_GetScanStatusFlag(ADC12_UNIT0));
            DRV_ADC_StartTask();
            while(RESET != ADC12_GetScanStatusFlag(ADC12_UNIT0));
          
            DRV_ADC_Off();
            //RTC温度补偿
            TempGet(&temp, 1);                    
            CalcPPM(temp);
            DRV_RTC_CalcSecDeviation();  //停电计算秒的误差
            
            if(BattLowStateGet())//电池
            {      
               SetSegment(SEG_battery2,0);
            }
            else
            {
           	   SetSegment(SEG_battery2,1); 
            }
			RefreshLCD();
            //电池电压检测
			BatteryState = BattLowStateGet();
			if(BatteryState == 1)
          	{
				if(BatteryFlag == 0)
				{
					Feed_watchdog();
					BatteryFlag =1;
					SleepDispTimer = 0;
				}
          	}
          	else if(BatteryFlag == 1)
          	{
                *(volatile unsigned long*)(0xE000ED0C)=0x05FA0004;
				while(1);                          //20140710加上
          	}
		    //停电停显定时          
		    if(SleepDispTimer)
		    {
			   SleepDispTimer--;
		    }
		    else
		    {
			   DRV_LCD_Uninit();	//关闭LCD
		    }  
        }

		//p13上电预判//尽量缩短检测时间
		if(FM3_GPIO->PDIR1&= IO_PINx3)
		{
			if(PowerUpDetect())
			{
				//if(POWER_UP_INIT_SUCESS == PowerUpInit())
				//{
				//	return POWER_UP_INIT_SUCESS;	//回到主循环
				//}
				//else
				//{
				//	PowerDownInit();
				//	Feed_watchdog();
					//LvdGetManaul();		//设置LVD门限等
				//}
				*(volatile unsigned long*)(0xE000ED0C)=0x05FA0004;
				while(1);                       //20140710加上
			}
		}
		//休眠
		if(BatteryState == 1)
        {
             LowPwrCon_GoToStandByMode(STB_RTCMode,STB_IO_KEEP);
        }
        else
        {
             LowPwrCon_GoToStandByMode(STB_TimerMode,STB_IO_KEEP);	
        }
		Feed_watchdog();
	}	
}

/*********************************************************** 
函数功能：电源管理
入口参数：
出口参数：
备注说明：1 上电模式调用
		  2 上电模式下检测到掉电，做掉电处理，掉电模式初始化
		  3 PowerDowmMain中检测到上电重新做上电初始化后回到主循环
***********************************************************/
void PowerManage(void)
{
	if(PowerDownDetect())
	{
		PowerDownInit();
		PowerDowmMain();	//循环函数，上电后直接复位
	}
}

/*********************************************************** 
函数功能：唤醒倒计时复位
入口参数：
出口参数：
备注说明：按键处理调用
		  自动循显一圈的时间
***********************************************************/
void PowerWakeTierRst(void)
{
	wakeUpTimer=(uint32)displayAlternate.objectSum * (uint32)displayAlternate.time;
}

/*********************************************************** 
函数功能：停电停显时间服务
入口参数：
出口参数：
备注说明：按键处理调用
		  7天
***********************************************************/
void SleepDispTimerRst(void)
{
	SleepDispTimer=7*24*60;		//7*24*60 min
	//SleepDispTimer=1;
}

/*********************************************************** 
函数功能：获取电池电压  (xx.xx)
入口参数：
出口参数：
备注说明：Vtest=(ADtest/ADfull)*Vref
			   =(ADtest/ADfull) * (1.8*ADfull/AD1.8)
			   =ADtest * 1.8 / AD1.8
		  Vbatt=Vtest*(R1+R2)/R2
***********************************************************/
uint16 BattVoltageGet(void)
{
	uint16 temp;
	
	temp=(uint16)(((uint32)DRV_ADC_GetAvg(4)*(uint32)180)/DRV_ADC_GetAvg(2));	//2为小数
	temp*=2;		//分压换算
	return temp;
}

/*********************************************************** 
函数功能：获取电池欠压状态
入口参数：
出口参数：0-没有欠压，1-欠压
备注说明：需要有滞回门限，所以需要保存状态
***********************************************************/
uint8 BattLowStateGet(void)
{
	uint16 temp;
	
	temp=BattVoltageGet();
	if(temp<=300)
	{
		BattLowState=1;
	}
	else if(temp>=310)
	{
		BattLowState=0;
	}
	return BattLowState;
}

uint16 BattVoltageDataGet(uint32 DataId, uint8 *pBuf)
{
    uint16 temp,result=0x8000;
    if(DataId==0x03C10002)
    {
        temp=BattVoltageGet();
        if(temp<=250)//电池
        {
            temp=0;
        }
        if(temp>365)
        {
            temp=365;
        }
        *(uint16 *)pBuf=(uint16)temp; 
        result=2;   
    }
    return result;
}

void PowerDownClockCopy(void)
{
    memcpy((uint8 *)&CalcPpmDateTime.dateTime, (uint8 *)&G_CurDateTime.dateTime, sizeof(CalcPpmDateTime.dateTime));
	CalcCheckSum(CalcPpmDateTime);	
}
