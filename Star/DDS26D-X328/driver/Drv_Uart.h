/*!
 ******************************************************************************
 **
 ** \file uart_fm3.h
 **
 ** \brief UART drivers head file
 **
 ** \author 
 **
 ** \version V0.10
 **
 ** \date 2013-10-09
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */ 

#ifndef _DR_UART_H
#define _DR_UART_H

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/

#define  MAXCHNELNUMBER        6           //最大通道数量:    0:MASH 1:IR 2:RS485 3:5EMU1 4:5EMU2
#define  RX_TIMEOUT_CNT	       12         //接收超时12*5ms
#define  TX_TIMEOUT_CNT	       12         //发送超时12*5ms
#define  RT_INTERVAL_MIN       4          //应答最小间隔4*5ms
#define  RX_IDLE_RST	       360000    //接收空闲复位间隔360000*5ms=30min
/*****************************************************************************/
//COM口的状态
/*****************************************************************************/
typedef enum 
{ 
	IDLE=0,	    //空闲
    READY,		//帧准备就绪，等待发送时机
    ING,		//帧收发中
    FINISH,		//帧收发完成
}COMM_STATE;

/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/
typedef enum 
{ 
    HalfDouble = 0, 	//半双工
    FullDouble,			//全双工
} COM_DoubleMode;

/*!
******************************************************************************
** 串口缓存结构体
******************************************************************************
*/
typedef struct s_com 
{ 
    uint8   channel;
    COMM_STATE   ucStt;              // 通讯状态_发送
    COMM_STATE   ucStR;              // 通讯状态_接收
    uint8   ucRecvTmr;              // 接收帧超时，单位10ms,相当于调用周期为10ms
    uint8   ucTxTmr;                // 发送帧超时，单位10ms,相当于调用周期为10ms
    uint8   ucOverTmr;              // 应答延时，单位10ms,相当于调用周期为10ms,保护用
    uint32   ucResetTmr;			// 串口空闲定时复位，防止串口死
    uint16  ucRXLen;                // 接收的数据总长度
    uint16  ucTXLen;                // 发送的数据总长度
    uint16  ucRxPos;                // 当前接受处理数据的位置,相当于指向ucBuf的一个指针
    uint16  ucTxPos;                // 当前发送处理数据的位置,相当于指向ucBuf的一个指针
    uint8*  ucRxBuf;                // 通讯接受数据缓冲区
    uint8*  ucTxBuf;                // 通讯發送数据缓冲区
    COM_DoubleMode   doubleMode;	//全双工
}S_COM;


//============================================================================
//==============应用层应用====================================================
//============================================================================
/*!
******************************************************************************
** 选择通讯类型结构类型
******************************************************************************
*/
typedef enum
{
	UART_VT = 0,
    UART_MASH,
    UART_IR,
    UART_485, 
	UART_EMU_L1, 
	UART_EMU_L2, 
}COM_TYPEDEF;

/*!
******************************************************************************
** 选择通讯波特率结构类型
******************************************************************************
*/
typedef enum 
{
    BAUDE_300BPS = 0, 
    BAUDE_600BPS, 
    BAUDE_1200BPS, 
    BAUDE_2400BPS, 
    BAUDE_4800BPS,	
    BAUDE_9600BPS, 
    BAUDE_19200BPS, 
    BAUDE_38400BPS, 
    BAUDE_57600BPS, 
    BAUDE_115200BPS,
}COM_BaudeType;

/*!
******************************************************************************
** 选择通讯波特率结构类型
******************************************************************************
*/
typedef enum 
{ 
    USART_7E1 = 0,       //7 bits data, 1 stop bit, EVEN 
    USART_8E1, 
    USART_7O1, 
    USART_8O1, 
    USART_7N1, 
    USART_8N1,
} COM_DataFormat;

extern uint8 Rxbuffer[250];
extern S_COM Comm[MAXCHNELNUMBER];        //chy     

//============================================================================
//==============================应用层应用====================================
//============================================================================


/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/* Config */
void LDRV_UsartOpen(COM_TYPEDEF com,COM_DataFormat type,COM_BaudeType baude,uint8 * rxfile,uint16 size, COM_DoubleMode doubleM);
void LDRV_UsartClose(COM_TYPEDEF com);
uint16 LDRV_UsartRecv(COM_TYPEDEF com);
void LDRV_UsartSend(COM_TYPEDEF com, uint8 * file, uint16 len);
void LDRV_CommRxEnable(COM_TYPEDEF com);
void LDRV_CommTxEnable(COM_TYPEDEF com);
COMM_STATE LDRV_UsartGetStateRx(COM_TYPEDEF com);
COMM_STATE LDRV_UsartGetStateTx(COM_TYPEDEF com);
void LDRV_UsartRestStateTx(COM_TYPEDEF com);

void COMRxMonitoringTask(void);
void COMTxMonitoringTask(void);
void LDRV_Test(void);
void COMTxMonitoringEMUTask(void);





  
  
#ifdef __cplusplus
}
#endif

#endif
