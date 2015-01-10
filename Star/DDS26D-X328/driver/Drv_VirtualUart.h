/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：
* 文件标识：
* 摘要：虚拟串口

* 当前版本：1.0.0 
* 作者：
* 完成日期：20140630
*******************************************************************/
#ifndef _DRV_VIRTUALUART_H_
#define _DRV_VIRTUALUART_H_

//虚拟串口 

typedef enum
{
  VIRTUAL_DIREC_CLEAR  		=0,
  VIRTUAL_METER_TO_RF  		=1,
  VIRTUAL_RF_TO_METER  		=2,
  
}VIRTUAL_UART_DIREC;



typedef enum
{
  VIRTUAL_PROTOCOL_Tm       =0,
  VIRTUAL_PROTOCOL_97       =1,
  VIRTUAL_PROTOCOL_07       =2,  
  VIRTUAL_PROTOCOL_21C      =0xd0,
  VIRTUAL_PROTOCOL_21E      =0xd1,
  VIRTUAL_PROTOCOL_STARRF   =0xe0,	

}VIRTUAL_UART_PROTOCOL;


void  VirtualUartRx(void);
void  VirtualUartTx(COM_TYPEDEF com, uint8 * file, uint16 len);
void VirtualUartInit(void);
void VirtualUartTest(void);

#endif
