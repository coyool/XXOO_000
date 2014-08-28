/*******************************************************************************
** 硬件平台: 富士通MB9BF218s学习板
** 固件库  ：V1.0
** 文件名  : UART.h
** 实现功能: UART头文件
** 作者    : 微控事业部
** 创建日期: 2013/8/28
** 版权    : 利尔达科技有限公司
** ----------------------------------------------------------------------------
** 修改者:
** 修改日期:
** 修改内容:
*******************************************************************************/
#include "pdl_header.h"
#include <stdio.h>

#ifndef __UART_H__
#define __UART_H__

/* Common routines */
#define IS_AF(c)  ((c >= 'A') && (c <= 'F'))
#define IS_af(c)  ((c >= 'a') && (c <= 'f'))
#define IS_09(c)  ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)  IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c)  IS_09(c)
#define CONVERTDEC(c)  (c - '0')

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)   (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))


/******************************************************************************/
/**                          以下是定义IO口             	     	      	 **/
/******************************************************************************/
#define InUseCh    MFS_Ch0// MFS_Ch5
#define UartUSBCh   MFS_Ch0


/*! \brief IO MFS channel */
#define   IO_MFS_CH             (InUseCh)
/*! \brief IO MFS port */
#define   IO_MFS_PORT           (IO_PORT3)
/*! \brief IO MFS SOT pin */
#define   IO_MFS_SOT_PIN        (IO_PINx7)
/*! \brief IO MFS SIN pin */
#define   IO_MFS_SIN_PIN        (IO_PINx6)
/*! \brief IO MFS SOT pin location */
#define   IO_MFS_SOT_PIN_LOC    (IO_MFS_SOTx_SOTx_2)
/*! \brief IO MFS SIN pin location */
#define   IO_MFS_SIN_PIN_LOC    (IO_MFS_SINx_SINx_2)

/******************************************************************************/
/**                          以下是内部函数申明                              **/
/******************************************************************************/
extern void UART_Port_init(void);
void UARTConfigMode(uint8_t Ch,MFS_UARTModeConfigT *pModeConfig);
void UARTPollRX(uint8_t *data, uint16_t size);		//轮询接收size个字符
void UARTPollTX(uint8_t Ch, uint8_t *data, uint16_t size);		//轮询发送size个字符
void UARTPollTX_string(uint8_t *data);				//轮询发送一个字符串
uint32_t Get_One_char(uint8_t *key);
uint8_t RX_char(uint8_t *key);
void Get_char_C(uint32_t timeout, uint8_t *key);
void Put_char(uint8_t data);                 	  //查询方式发送一个字符
int8_t Receive_Byte(uint8_t *c, uint32_t timeout);

void Int2Str(uint8_t* str, int32_t intnum);
uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum);

extern void UART_setup(void);

#endif
