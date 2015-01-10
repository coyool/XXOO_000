/*!
 ******************************************************************************
 **
 ** \file dr_uart.c
 **
 ** \brief ADC drivers
 **
 ** \author  ffq
 **
 ** \version V0.10
 **
 ** \date 2013-10-10
 **
 ** \attention   
 **
 ** (C) Copyright 
 **
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */
 
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "Include.h"

uint16 AD_Sample[5][5];        //ad采样值，可以保存5个历史采样数据

static void ScanIntCallback(void);
void TempNTCPower(uint8 ON_OFF);

/*******************************************************************************
 * @function_name: Init_Adc
 * @function_file: Drv_Adc.c
 * @描述：初始化ADC配置
 * @入口参数： 
   powerOn 1-on 0-off
 * @出口参数： 无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
void DRV_ADC_Init(uint8 powerOn)
{
  //温度采样电路电源控制IO，其它在AD中初始化
	IO_DisableFunc(IO_PORT4,IO_PINxE);
	IO_ConfigGPIOPin(IO_PORT4,IO_PINxE,IO_DIR_OUTPUT,IO_PULLUP_OFF);
    
    IO_EnableAnalogInput(IO_AN00);    //TEMP
    IO_EnableAnalogInput(IO_AN01);    // TEMP_A
    IO_EnableAnalogInput(IO_AN02);    //V_1.8V
	IO_EnableAnalogInput(IO_AN03);//V_CHK
	IO_EnableAnalogInput(IO_AN04);//BATCHK
    /*------------------- Global setting -----------------------*/
    /* Set sample time */
    ADC12_SelSampleTimeReg(ADC12_UNIT0, ADC12_CH0, ADC12_SAMPLE_TIME_SEL_REG0);
    ADC12_SelSampleTimeReg(ADC12_UNIT0, ADC12_CH1, ADC12_SAMPLE_TIME_SEL_REG0);
    ADC12_SelSampleTimeReg(ADC12_UNIT0, ADC12_CH2, ADC12_SAMPLE_TIME_SEL_REG0);
	ADC12_SelSampleTimeReg(ADC12_UNIT0, ADC12_CH3, ADC12_SAMPLE_TIME_SEL_REG0);
	ADC12_SelSampleTimeReg(ADC12_UNIT0, ADC12_CH4, ADC12_SAMPLE_TIME_SEL_REG0);
    ADC12_SetSampleTime0(ADC12_UNIT0, 9, ADC12_SAMPLE_TIME_N4);
    /* Set compare time */
    ADC12_SetCompareTime(ADC12_UNIT0, 2);
    /* Start ADC operation */
    ADC12_EnableOpt(ADC12_UNIT0);
    while(SET != ADC12_GetOptStat(ADC12_UNIT0));
    /*------------------- Scan mode setting -----------------------*/
    /* Select channel */
    ADC12_SelScanCh(ADC12_UNIT0, ((1ul<<ADC12_CH0) 
    								| (1ul<<ADC12_CH1) 
    								| (1ul<<ADC12_CH2) 
    								| (1ul<<ADC12_CH3) 
    								| (1ul<<ADC12_CH4)));
    /* Clear FIFO */
    ADC12_ClrScanFIFO(ADC12_UNIT0);
    /* Set FIFO stage */
    ADC12_SetScanFIFOStage(ADC12_UNIT0, (ADC12_ScanFIFOStageT)(USER_ADC_SCAN_CH_NUM-1));

    /* Set Scan mode */
    ADC12_SetScanMode(ADC12_UNIT0 , ADC12_SCAN_MODE_ONE_SHOT);
    /* Enable scan interrupt */
    ADC12_EnableScanInt(ADC12_UNIT0, ScanIntCallback);
    /* Enable ADC IRQ */
    NVIC_EnableIRQ((IRQn_Type)(ADC0_IRQn + (ADC12_UNIT0-ADC12_UNIT0)));
    /* Start scan */
    //ADC12_StartScan(ADC12_UNIT0);

	//连续采样5次，填充滤波缓存。
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

    if(powerOn)
    {
      DRV_ADC_On();
      //定时1s采样
      //TempNTCPower(OFF);
      TaskAdd(DRV_ADC_StartTask, 0, 100, 1);
    }
    else
    {
      DRV_ADC_Off();
    }
}

/*********************************************************** 
函数功能：温度采样电路电源控制
入口参数：1:ON	0:OFF
出口参数：
备注说明:
***********************************************************/
void TempNTCPower(uint8 ON_OFF)
{
	IO_WriteGPIOPin(IO_PORT4,IO_PINxE,ON_OFF);//输出高电平
}

/*******************************************************************************
 * @function_name: Init_Adc
 * @function_file: Drv_Adc.c
 * @描述：初始化ADC配置
 * @入口参数： 无
 * @出口参数： 无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
void DRV_ADC_Off(void)
{
	ADC12_DisableOpt(ADC12_UNIT0);
    TempNTCPower(OFF);
}

/*******************************************************************************
 * @function_name: Init_Adc
 * @function_file: Drv_Adc.c
 * @描述：初始化ADC配置
 * @入口参数： 无
 * @出口参数： 无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
void DRV_ADC_On(void)
{
    TempNTCPower(ON);
    /* Start ADC operation */
    ADC12_EnableOpt(ADC12_UNIT0);
    while(SET != ADC12_GetOptStat(ADC12_UNIT0));
}

/*********************************************************** 
函数功能：ADC软件定时启动 1000msTask
入口参数：
出口参数：
备注说明:
***********************************************************/
void DRV_ADC_StartTask(void)
{
	//TempNTCPower(ON);
	//PublicDelayMs(2);
	ADC12_StartScan(ADC12_UNIT0);
}

/*******************************************************************************
 * @function_name: Start_Adc
 * @function_file: Drv_Adc.c
 * @描述：获取采样值指定通道
 * @入口参数：
ch通道0-15
index缓存索引0-4
 * @出口参数： 无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
uint16 DRV_ADC_Get(uint8 ch)
{
	uint16 temp;
	NVIC_DisableIRQ((IRQn_Type)(ADC0_IRQn));
	temp=AD_Sample[ch][0]>>4;
    NVIC_EnableIRQ((IRQn_Type)(ADC0_IRQn));
        
	//printf( "<Ch%d> ADC value:%d\n\r", ch, temp);

	return temp;
}

/*******************************************************************************
 * @function_name: Start_Adc
 * @function_file: Drv_Adc.c
 * @描述：获取指定通道采样平均值，去掉最大最小取平均
 * @入口参数：
ch通道0-15
 * @出口参数： 无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
uint16 DRV_ADC_GetAvg(uint8 ch)
{
    uint8 i;
	uint16 temp[5];
    uint16 max,min;
    uint32 avg;

	NVIC_DisableIRQ((IRQn_Type)(ADC0_IRQn));
	temp[0]=AD_Sample[ch][0]>>4;
	temp[1]=AD_Sample[ch][1]>>4;
	temp[2]=AD_Sample[ch][2]>>4;
	temp[3]=AD_Sample[ch][3]>>4;
	temp[4]=AD_Sample[ch][4]>>4;
	NVIC_EnableIRQ((IRQn_Type)(ADC0_IRQn));
	
    max=temp[0];
    for(i=1;i<5;i++)
    {
      if(temp[i]>max)
      {
        max=temp[i];
      }
    }
    min=temp[0];
    for(i=1;i<5;i++)
    {
      if(temp[i]<min)
      {
        min=temp[i];
      }
    }
    avg=0;
    for(i=0;i<5;i++)
    {
      avg+=temp[i];
    }
    avg-=max;
    avg-=min;
    avg=avg/3;
	return avg;
}

/*******************************************************************************
 * @function_name: ScanIntCallback
 * @function_file: Drv_Adc.c
 * @描述：ADC中断回调函数
 * @入口参数： 无
 * @出口参数： 无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
static void ScanIntCallback(void)
{
    ADC_ScanFIFOInfoT info;
    uint8 cnt;
//    uint16 data;
    cnt = USER_ADC_SCAN_CH_NUM;
    while(cnt--)
    {                         
        ADC12_GetScanFIFOInfo(ADC12_UNIT0, &info);
        if(info.Ch == ADC12_CH0)
        {
            if(info.DataValid == ADC12_SCAN_DATA_VALID)
            {
                AD_Sample[0][4] = AD_Sample[0][3];
                AD_Sample[0][3] = AD_Sample[0][2];
                AD_Sample[0][2] = AD_Sample[0][1];
                AD_Sample[0][1] = AD_Sample[0][0];
                AD_Sample[0][0] = ADC12_GetScanFIFOData(ADC12_UNIT0);
            }
        }
        else if(info.Ch == ADC12_CH1)
        {
            if(info.DataValid == ADC12_SCAN_DATA_VALID)
            {
                AD_Sample[1][4] = AD_Sample[1][3];
                AD_Sample[1][3] = AD_Sample[1][2];
                AD_Sample[1][2] = AD_Sample[1][1];
                AD_Sample[1][1] = AD_Sample[1][0];
                AD_Sample[1][0] = ADC12_GetScanFIFOData(ADC12_UNIT0);
            }
        }
        else if(info.Ch == ADC12_CH2)
        {
            if(info.DataValid == ADC12_SCAN_DATA_VALID)
            {
                AD_Sample[2][4] = AD_Sample[2][3];
                AD_Sample[2][3] = AD_Sample[2][2];
                AD_Sample[2][2] = AD_Sample[2][1];
                AD_Sample[2][1] = AD_Sample[2][0];
                AD_Sample[2][0] = ADC12_GetScanFIFOData(ADC12_UNIT0);
            }
        }
        else if(info.Ch == ADC12_CH3)
        {
            if(info.DataValid == ADC12_SCAN_DATA_VALID)
            {
                AD_Sample[3][4] = AD_Sample[3][3];
                AD_Sample[3][3] = AD_Sample[3][2];
                AD_Sample[3][2] = AD_Sample[3][1];
                AD_Sample[3][1] = AD_Sample[3][0];
                AD_Sample[3][0] = ADC12_GetScanFIFOData(ADC12_UNIT0);
            }
        }
        else if(info.Ch == ADC12_CH4)
        {
            if(info.DataValid == ADC12_SCAN_DATA_VALID)
            {
                AD_Sample[4][4] = AD_Sample[4][3];
                AD_Sample[4][3] = AD_Sample[4][2];
                AD_Sample[4][2] = AD_Sample[4][1];
                AD_Sample[4][1] = AD_Sample[4][0];
                AD_Sample[4][0] = ADC12_GetScanFIFOData(ADC12_UNIT0);
            }
        }
    }
    ADC12_ClrScanFIFO(ADC12_UNIT0); 
	//TempNTCPower(OFF);
}

