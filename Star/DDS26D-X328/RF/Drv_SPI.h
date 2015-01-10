/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：drv_extint.h 
* 文件标识：见配置管理计划书 
* 摘要：外部中断移植层

* 当前版本：1.0.0 
* 作者：ffq
* 完成日期：20131024
*******************************************************************/
#ifndef _DRV_EXT_H_
#define _DRV_EXT_H_

/*--------------------宏定义-------------------------------*/
#define  SCS_OUT_L        IO_ConfigGPIOPin(IO_PORT3,IO_PINxD,IO_DIR_OUTPUT,IO_PULLUP_OFF)
#define  SCS_OUT_H       IO_ConfigGPIOPin(IO_PORT3,IO_PINxD,IO_DIR_INPUT,IO_PULLUP_OFF)

#define  SI_OUT_L        IO_ConfigGPIOPin(IO_PORT3,IO_PINx7,IO_DIR_OUTPUT,IO_PULLUP_OFF)
#define  SI_OUT_H       IO_ConfigGPIOPin(IO_PORT3,IO_PINx7,IO_DIR_INPUT,IO_PULLUP_OFF)

#define  SCK_OUT_L       IO_ConfigGPIOPin(IO_PORT3,IO_PINx8,IO_DIR_OUTPUT,IO_PULLUP_OFF)
#define  SCK_OUT_H      IO_ConfigGPIOPin(IO_PORT3,IO_PINx8,IO_DIR_INPUT,IO_PULLUP_OFF)

#define  SO_IN            IO_ReadGPIOPin(IO_PORT3,IO_PINx6) 

#define	 RF_SI(x)	 ( (x) ? (IO_ConfigGPIOPin(IO_PORT3,IO_PINx7,IO_DIR_INPUT, IO_PULLUP_OFF)) : (IO_ConfigGPIOPin(IO_PORT3,IO_PINx7,IO_DIR_OUTPUT, IO_PULLUP_OFF)) )                      // SPI输出

#define  PA_OUT_L          {FM3_GPIO->PFR3&= ~IO_PINx4;FM3_GPIO->DDR3|= IO_PINx4;FM3_GPIO->PDOR3 &= ~IO_PINx4;}                            
#define  PA_OUT_H          {FM3_GPIO->PFR3&= ~IO_PINx4;FM3_GPIO->DDR3&= ~ IO_PINx4;}  

/*--------------------外部常量申明-------------------------*/

/*--------------------外部变量申明-------------------------*/


/*--------------------外部函数申明-------------------------*/
uint8 SPI_byte(uint8 dat); //
void SpiWriteBurstReg(uint8 addr, uint8 *buffer, uint8 count);
uint8 SpiReadReg(uint8 addr)	;
void SpiReadBurstReg (uint8 addr, uint8 *buffer, uint8 count);
uint8 SpiReadStatus(uint8 addr);
void SpiWriteStrobe(uint8 strobe);
void SpiWriteReg(uint8 addr, uint8 value);
void RF_IO_Init(void);
void RFExtInt_Init(void);
void Disable_ExtInt(void);    
extern uint8 TestSOMI(void);











#endif
