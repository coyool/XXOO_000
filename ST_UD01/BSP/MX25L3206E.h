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

#ifndef _DR_MX25L3206E_H
#define _DR_MX25L3206E_H

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/

#define MAX_ADDR		0x3FFFFF	// 定义芯片内部最大地址 
#define	SEC_MAX     	        1023	        // 定义最大扇区号
#define SEC_SIZE		0x1000      // 扇区大小
#define OK                      1
#define FAIL                    0

/****************************************************************************/
typedef	unsigned long	                        U32;
typedef	unsigned int	                        U16;
typedef	unsigned char	                        U08;

typedef        signed long				s32;
typedef        signed short			        s16;
typedef        signed char				s8;

typedef        unsigned long				u32;
typedef        unsigned short		        u16;
typedef        unsigned char				u8;


#define  Atsout         IO_ReadGPIOPin(IO_PORT4,IO_PINx3)                                                //SO口输入状态
#define  Atsin_data     IO_ReadGPIOPin(IO_PORT4,IO_PINx4)                                              //SI口输入状态

#define  Atsin_in	IO_ConfigGPIOPin(IO_PORT4,IO_PINx4,IO_DIR_INPUT,IO_PULLUP_ON)              //需要打开上拉吗？
#define  Atsin_Out      IO_ConfigGPIOPin(IO_PORT4,IO_PINx4,IO_DIR_OUTPUT,IO_PULLUP_ON)


#define	Atsck_Low	IO_WriteGPIOPin(IO_PORT4,IO_PINx2,IO_BIT_CLR) //IO_ConfigGPIOPin(IO_PORT4,IO_PINx2,IO_DIR_OUTPUT,IO_PULLUP_ON)          // sck时钟
#define	Atsck_High	IO_WriteGPIOPin(IO_PORT4,IO_PINx2,IO_BIT_SET) // IO_ConfigGPIOPin(IO_PORT4,IO_PINx2,IO_DIR_INPUT, IO_PULLUP_ON)         // sck时钟


#define	Atsin(x)	( (x) ? (IO_WriteGPIOPin(IO_PORT4,IO_PINx4,IO_BIT_SET)) : (IO_WriteGPIOPin(IO_PORT4,IO_PINx4,IO_BIT_CLR)))     //( (x) ? (IO_ConfigGPIOPin(IO_PORT4,IO_PINx4,IO_DIR_INPUT, IO_PULLUP_ON)) : (IO_ConfigGPIOPin(IO_PORT4,IO_PINx4,IO_DIR_OUTPUT, IO_PULLUP_ON)) )                      // SPI输出

#define	CE_Low()	 IO_WriteGPIOPin(IO_PORT4,IO_PINx5,IO_BIT_CLR)  //cs( IO_ConfigGPIOPin(IO_PORT4,IO_PINx5,IO_DIR_OUTPUT,IO_PULLUP_ON) )                                //片选使能
#define CE_High()	 IO_WriteGPIOPin(IO_PORT4,IO_PINx5,IO_BIT_SET)  //cs( IO_ConfigGPIOPin(IO_PORT4,IO_PINx5,IO_DIR_INPUT, IO_PULLUP_ON) )	                               //片选取消


/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/

typedef struct
{
    unsigned char B0:1;
    unsigned char B1:1;
    unsigned char B2:1;
    unsigned char B3:1;
    unsigned char B4:1;
    unsigned char B5:1;
    unsigned char B6:1;
    unsigned char B7:1;
} __BIT8;

union Byte_Bit
{
    unsigned char Byte;
    __BIT8 Bit;
};


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

void MX25L3206_IOInt(void);
void MX25L3206_IOPowerDownInt(void);
U08 MX25L3206_Read(u8* RcvBufPt,u32 Dst, U16 NByte);    
u8 MX25L3206_SecErase(u32 sec1);
U08 MX25L3206_Write(u32 Dst,  u8* SndbufPt,U16 NByte);
u8 MX25L3206_Erase(u16 sec1, u16 sec2);



void test_flash(void);


  
  
#ifdef __cplusplus
}
#endif

#endif
