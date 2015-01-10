
/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：ProtocolManage.h 
* 文件标识：见配置管理计划书 
* 摘要：协议管理

* 当前版本：1.0.0 
* 作者：张玉清
* 完成日期：20131028
*******************************************************************/
#include "Include.h"

/*--------------------全局常量定义---------------------*/
uint8 commRxBffer[MAXCHNELNUMBER-2][MAXCOMBUFFER];
static uint16 sCommRxLen[MAXCHNELNUMBER-2] = {0};
static uint8 *spCommRxPoint[MAXCHNELNUMBER-2] = {NULL};
uint8 comModeSel;
/*--------------------全局变量定义---------------------*/

/*--------------------内部函数申明-------------------------*/
void ProtocolManageTask(void);

/*********************************************************** 
函数功能：协议管理初始化	
入口参数：
出口参数：
备注说明：上电时调用
***********************************************************/
void ProtocolVirtualUartInit(COM_TYPEDEF com,uint8 * rxfile)
{
	spCommRxPoint[com] = rxfile;
	memset((uint8 *)&Comm[com],0,sizeof(Comm[com]));
	memset((uint8 *)&commRxBffer[com], 0,sizeof(commRxBffer[com]));
	//VirtualUartInit();
}

void ProtocolManageInit(void)
{
	uint8 i,j=0;
	comModeSel = IEC62056_21MODE;
	//多次判断防止抖动
	for(i=0;i<=50;i++)
	{
		if(IO_ReadGPIOPin(IO_PORT_PRO_SWI,IO_PIN_PRO_SWI))
		{
			j++;
		}
		else
		{
			if(j)
			{
				j--;
			}
		}
		PublicDelayMs(1);
	}
	DRV_WD_FeedDog();
	if(j>25)
	{
		comModeSel = IEC62056_21MODE;
	}
	else
	{
	    comModeSel = DL645_07MODE;
	}	
	//硬件串口初始化
	
	memset((uint8 *)commRxBffer[UART_MASH], 0, sizeof(commRxBffer[UART_MASH]));
	memset((uint8 *)commRxBffer[UART_IR], 0, sizeof(commRxBffer[UART_IR]));
	memset((uint8 *)commRxBffer[UART_VT], 0, sizeof(commRxBffer[UART_VT])); 	
	ProtocolUartInit(UART_MASH,USART_7E1,BAUDE_300BPS,commRxBffer[UART_MASH],sizeof(commRxBffer[UART_MASH]),HalfDouble);
	ProtocolUartInit(UART_IR,USART_7E1,BAUDE_300BPS,commRxBffer[UART_IR],sizeof(commRxBffer[UART_IR]),HalfDouble);
	
	if(comModeSel == IEC62056_21MODE)
	{	
		memset((uint8 *)commRxBffer[UART_485], 0, sizeof(commRxBffer[UART_485]));
		ProtocolUartInit(UART_485,USART_7E1,BAUDE_300BPS,commRxBffer[UART_485],sizeof(commRxBffer[UART_485]),HalfDouble);		
    }
	else if(comModeSel == DL645_07MODE)
	{	
		memset((uint8 *)commRxBffer[UART_485], 0, sizeof(commRxBffer[UART_485]));
		ProtocolUartInit(UART_485,USART_8E1,BAUDE_2400BPS,commRxBffer[UART_485],sizeof(commRxBffer[UART_485]),HalfDouble);
	}

	//虚拟串口初始化
//	ProtocolVirtualUartInit(UART_VT,commRxBffer[UART_VT]);
//	TaskAdd(VirtualUartRx, 0, 5 ,1);
	//串口驱动
	TaskAdd(COMRxMonitoringTask,0,5,1);
	TaskAdd(COMTxMonitoringTask,0,5,1);
	TaskAdd(ProtocolManageTask, 0, 5 ,1);
}

void ProtocolManagePowerDownInit(void)
{
	LDRV_UsartClose(UART_MASH);
	LDRV_UsartClose(UART_485);
    LDRV_UsartClose(UART_EMU_L1);
    LDRV_UsartClose(UART_EMU_L2);
}

void ProtocolUartInit(COM_TYPEDEF com,COM_DataFormat type,COM_BaudeType baude,uint8 * rxfile,uint16 size, COM_DoubleMode doubleM)
{
	spCommRxPoint[com] = rxfile;
	if(com == UART_IR)
	{
		return;
	}
	else
	{
		LDRV_UsartOpen(com, type, baude, rxfile, size, doubleM);
	}
	
}

/*********************************************************** 
函数功能：协议管理	5ms
入口参数：
出口参数：
备注说明：
***********************************************************/
void ProtocolManageTask(void)
{
	COM_TYPEDEF com;
	
	for(com=(COM_TYPEDEF)0; com<(COM_TYPEDEF)(MAXCHNELNUMBER-2);com++)
	{
		sCommRxLen[com] = LDRV_UsartRecv(com);
		//----Protocol handle start---
		if(com == UART_485)
		{
			if(comModeSel == DL645_07MODE)
        	{
				if(DLT645Main(com))
				{
					sCommRxLen[com] = 0;
				}
			}
			else if(comModeSel == IEC62056_21MODE)
			{			
				if(IEC62056ReceiveProcess(com))
				{
					sCommRxLen[com] = 0;
				}
	            BlockDataSend(com);
			}
		}
		else
		{
			if(IEC62056ReceiveProcess(com))
			{
				sCommRxLen[com] = 0;
			}
	        BlockDataSend(com);
		}
        
		//----Protocol handle end---
		if(sCommRxLen[com] != 0)
		{
			sCommRxLen[com] = 0;
			//无效帧丢弃，再次打开接收,
			if(com==UART_VT)
    		{
    			
			}
			else
			{
				LDRV_CommRxEnable(com);
			}		
		}
	}
}

/*********************************************************** 
函数功能：数据接收查询，
入口参数：pBuf--数据指针
出口参数：长度指针，如果是相应协议的数据则在
					处理完后需要把指针内的数据长度清零
备注说明：
***********************************************************/
uint16 ProtocolDataRec(COM_TYPEDEF Comm, uint8 **pBuf)
{
	*pBuf = spCommRxPoint[Comm];
	return sCommRxLen[Comm];
}

/*********************************************************** 
函数功能：获取通讯指针地址
入口参数：pBuf--数据指针
出口参数：
					
备注说明：
***********************************************************/
void GetCommRxPoint(COM_TYPEDEF Comm, uint8 **pBuf)
{
	*pBuf = spCommRxPoint[Comm];
}

