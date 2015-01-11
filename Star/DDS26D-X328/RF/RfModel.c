/*******************************************************************
Copyright (c) 2013,深圳市思达仪表有限公司 
All rights reserved. 
 
文件名称：FRModel.c 
摘要：RF模式切换模块
作者：ffq
当前版本：1.0.0 -20140625

历史记录：
********************************************************************/

#include "Include.h"

/*---------------------------------------------------------------------------*/
/*内部常数和宏定义                                                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*内部结构体、枚举和类声明                                                   */
/*---------------------------------------------------------------------------*/
volatile t_RFModel RFModel;



/*---------------------------------------------------------------------------*/
/*变量定义                                                                   */
/*---------------------------------------------------------------------------*/
uint8 lowVoltageFlag = 0;
/*---------------------------------------------------------------------------*/
/*内部函数声明                                                               */
/*---------------------------------------------------------------------------*/
void RFModelCheckProcess(void);
void RFIOPowerUpInit(void);
void RFExtInt_Init(void);


/*---------------------------------------------------------------------------*/
/*函数定义                                                                   */
/*---------------------------------------------------------------------------*/
void RFModeChangeInit(void)
{	
	memset((uint8 *)&RFModel,0,sizeof(RFModel));
	RFModel.Module = RF_MESH;                 
    RFModel.RFInitTimes =RF_INITTIME; 
	TaskAdd(RFModelCheckProcess,0,5,1);
}

#pragma optimize=none
void RFModelCheckProcess(void)
{
	CHK_M_INPUT();
	if((CHK_M_PIN == 0))  // mesh
	{
        RFModel.PTPCounts =0;
		if(++RFModel.MeshCounts >= RF_MODEL_DELAY_TIME)
		{
			RFModel.MeshCounts = RF_MODEL_DELAY_TIME;
			if(RFModel.Module == RF_PTP)
			{			
				RFIOPowerUpInit();              //关GPIO中断防止进入白工程序-----------
                Disable_ExtInt();               //关RF中断         
				ProtocolUartInit(UART_MASH,USART_7E1,BAUDE_300BPS,commRxBffer[UART_MASH],sizeof(commRxBffer[UART_MASH]),HalfDouble);
				RFModel.Module = RF_MESH;
			}
		}
	}
	else  //PTP
	{
        RFModel.MeshCounts =0;
        if(++RFModel.PTPCounts >= RF_MODEL_DELAY_TIME)
          {
              RFModel.PTPCounts = RF_MODEL_DELAY_TIME;
              if(RFModel.Module == RF_MESH)
              {                                                                                    
           //     CC1101_init();  //ybz 
                memset((uint8 *)commRxBffer[UART_MASH], 0, sizeof(commRxBffer[UART_MASH]));
                LDRV_UsartClose(UART_MASH); //关mesh串口配置,防止进入白工程序接收中断----------- 
                RFModel.Module = RF_PTP;  
                CC1101_init();  //ybz 
              }
          }         			
	}
    if(RFModel.Module == RF_PTP)  //超时处理
    {   
        if(RFModel.RFInitTimes)
        {    
            RFModel.RFInitTimes--;
            if(!RFModel.RFInitTimes)
            {
                RFModel.RFInitTimes =RF_INITTIME; 
                CC1101_init();  //ybz 
            }
        }        
    }
    else
    {
        RFModel.RFInitTimes =RF_INITTIME;    
    }
    //通过这个指示,可以知道当前模块的状态
/*    if(RFModel.Module == RF_MESH)
    {
        if(G_CurDateTime.dateTime.Second%2)
        {
              IO_WriteGPIOPin(IO_PORT4,IO_PINxC,IO_BIT_CLR);   //M-RXD-LED
              IO_WriteGPIOPin(IO_PORT3,IO_PINxF,IO_BIT_SET);   //M-TXD-LED          
        } 
        else
        {
              IO_WriteGPIOPin(IO_PORT4,IO_PINxC,IO_BIT_SET);   //M-RXD-LED
              IO_WriteGPIOPin(IO_PORT3,IO_PINxF,IO_BIT_CLR);   //M-TXD-LED              
        }   
    }  
 */ 
}

/*********************************************************** 
函数功能：函数功能
入口参数：包括所有形参、访问的全局变量、修改的全局变量说明
出口参数：包括返回值说明
备注说明：包括功能概要，关键算法，调用说明
***********************************************************/
/*void DisableRF_ExtInt(void)
{
	EXTI_DisableInt(EXTI_CH8);
}
*/
/*********************************************************** 
函数功能：函数功能
入口参数：包括所有形参、访问的全局变量、修改的全局变量说明
出口参数：包括返回值说明
备注说明：包括功能概要，关键算法，调用说明
***********************************************************/
void RFIOPowerUpInit(void)
{
	//SCLK
    IO_DisableFunc(IO_PORT3,IO_PINx8);                  
    IO_ConfigGPIOPin(IO_PORT3,IO_PINx8,IO_DIR_INPUT, IO_PULLUP_OFF);

	//SCN	
    IO_DisableFunc(IO_PORT3,IO_PINxD);                  //SCN
    IO_ConfigGPIOPin(IO_PORT3,IO_PINxD,IO_DIR_INPUT, IO_PULLUP_OFF);

	//CTR_PA
	IO_DisableFunc(IO_PORT3,IO_PINx4);  
    IO_ConfigGPIOPin(IO_PORT3,IO_PINx4,IO_DIR_OUTPUT,IO_PULLUP_OFF); //CTRL_PA 上电默认为输出
    IO_WriteGPIOPin(IO_PORT3,IO_PINx4,IO_BIT_CLR);  //CTRL_PA

	//GDO2
	IO_DisableFunc(IO_PORT3,IO_PINx5);
	IO_ConfigGPIOPin(IO_PORT3, IO_PINx5, IO_DIR_INPUT, IO_PULLUP_ON);
}

/*********************************************************** 
函数功能：掉电将RFIO配置输出低
入口参数：
出口参数：包括返回值说明
备注说明：包括功能概要，关键算法，调用说明
***********************************************************/
void RFIOPowerDownInit(void)
{
	RFModel.Module = RF_MESH;   //电池上电和掉电默认为MESH.
	//CTR_PA
	IO_DisableFunc(IO_PORT3,IO_PINx4);  
    IO_ConfigGPIOPin(IO_PORT3,IO_PINx4,IO_DIR_OUTPUT,IO_PULLUP_OFF); 
    IO_WriteGPIOPin(IO_PORT3,IO_PINx4,IO_BIT_CLR); 

	//GDO2
	IO_DisableFunc(IO_PORT3, IO_PINx5);
	IO_ConfigGPIOPin(IO_PORT3,IO_PINx5,IO_DIR_OUTPUT,IO_PULLUP_OFF); 
    IO_WriteGPIOPin(IO_PORT3,IO_PINx5,IO_BIT_CLR);

	//SO
	IO_DisableFunc(IO_PORT3, IO_PINx6);
	IO_ConfigGPIOPin(IO_PORT3,IO_PINx6,IO_DIR_OUTPUT,IO_PULLUP_OFF); 
    IO_WriteGPIOPin(IO_PORT3,IO_PINx6,IO_BIT_CLR);

	//SI
	IO_DisableFunc(IO_PORT3, IO_PINx7);
	IO_ConfigGPIOPin(IO_PORT3,IO_PINx7,IO_DIR_OUTPUT,IO_PULLUP_OFF); 
    IO_WriteGPIOPin(IO_PORT3,IO_PINx7,IO_BIT_CLR);

	//SCLK
	IO_DisableFunc(IO_PORT3, IO_PINx8);
	IO_ConfigGPIOPin(IO_PORT3,IO_PINx8,IO_DIR_OUTPUT,IO_PULLUP_OFF); 
	IO_WriteGPIOPin(IO_PORT3,IO_PINx8,IO_BIT_CLR);

	//SCN
	IO_DisableFunc(IO_PORT3, IO_PINxD);
	IO_ConfigGPIOPin(IO_PORT3,IO_PINxD,IO_DIR_OUTPUT,IO_PULLUP_OFF); 
	IO_WriteGPIOPin(IO_PORT3,IO_PINxD,IO_BIT_CLR);

	//CHK_M
	IO_DisableFunc(IO_PORT3, IO_PINxE);
	IO_ConfigGPIOPin(IO_PORT3,IO_PINxE,IO_DIR_OUTPUT,IO_PULLUP_OFF); 
	IO_WriteGPIOPin(IO_PORT3,IO_PINxE,IO_BIT_CLR);
}

/*********************************************************** 
函数功能：RF 模块检测	低压关闭
入口参数：电压低于70%或者高于140%UN关闭RF功能,减少表的功耗。 
出口参数：包括返回值说明
备注说明：包括功能概要，关键算法，调用说明
***********************************************************/
void RFModuleCheckProcess(void)
{
	uint16  Vol_A;
	int32   Un; 
	Un = Get_EMU_Measure_Data(SYS_PARAMETER_UN); //读取系统电压规格 兼容
	VariableParaGet(0x03800902, (uint8*)&Vol_A); //L1 电压
	
	if ( (Vol_A<(Un*7/10)) || (Vol_A>(Un*14/10)) ) 
	{
		if(++RFModel.volCounts >= RF_VOLTAGE_JUDGE_TIME)
		{		
			RFModel.volCounts = 0;
			lowVoltageFlag = 1;		
		}
	}
	else
	{
		RFModel.volCounts = 0;
		lowVoltageFlag = 0;	
	}
}

/*********************************************************** 
函数功能：RF 模块检测	低压关闭
入口参数：电压低于70%或者高于140%UN关闭RF功能,减少表的功耗。 
出口参数：包括返回值说明
备注说明：包括功能概要，关键算法，调用说明
***********************************************************/
uint8 ModelSelGet(void)
{
	if(RFModel.Module == RF_PTP)
	{	
		return 0;
	}
	else if(RFModel.Module == RF_MESH)
	{
		return 1;
	}
	return 0;
}
/*********************************************************** 
函数功能：RF 模块检测	低压关闭
入口参数：电压低于70%或者高于140%UN关闭RF功能,减少表的功耗。 
出口参数：包括返回值说明
备注说明：包括功能概要，关键算法，调用说明
***********************************************************/

void RFMeshMain(void)
{
    u8 temp = 0u;
    
    if(RFModel.Module==RF_PTP)   //是PTP模块才执行
    {
       temp = CC1101_available(RF_macRecvBuff, RF_shortPayloadSize); 
       if (1u == temp)
       {
            temp = 0u;
            RF_linkLayerProtocolStack();
       }
    }
}



