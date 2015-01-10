/*******************************************************************************
* Copyright (c) 2013, 深圳思达仪表有限公司  研发中心 固件研发部
* All rights reserved.
* 
* 文件名称：SPI.c
* 文件标识：见配置管理计划书
* 
* 当前版本：V1.0
* 作    者：ffq  and   NO: F06553 (Vendetta)
* 完成日期：2013年11月10日
*
* 取代版本：1.1 
* 作    者：输入原作者（或修改者）名字
* 完成日期：XXXX年XX月XX日
*
* 源代码说明： 
*                        
*******************************************************************************/

#include "Include.h"


/************************** 全局常量定义 **********************/


/************************* 全局变量定义 ***********************/

/************************  内部函数申明 ***********************/
static void RFExt_IntCallback(void);

// uint32 RX_num_cnt;  //test

/*********************************************************** 
函数功能：RF外部中斷初始化函数
入口参数：
出口参数：
备注说明:
***********************************************************/
void RFExtInt_Init(void)
{
	EXTI_DisableInt(EXTI_CH8);
	IO_EnableFunc(IO_PORT3, IO_PINx5);
	IO_ConfigFuncINTxPin(IO_EXT_INT_CH8, IO_INTx_INTx_1);
	IO_ConfigGPIOPin(IO_PORT3, IO_PINx5, IO_DIR_INPUT, IO_PULLUP_ON);
	EXTI_SetIntDetectMode(EXTI_CH8, EXTI_EDGE_FALLING);  //采用 RF IRQ 下降沿 (接收完成产生) 
	EXTI_ClrIntFlag(EXTI_CH8);
	EXTI_EnableInt(EXTI_CH8,RFExt_IntCallback);
	NVIC_EnableIRQ(EXINT8_31_IRQn);
}
/*********************************************************** 
函数功能：RF外部中断函数
入口参数：
出口参数：
备注说明:
***********************************************************/
void Disable_ExtInt(void)
{
    EXTI_DisableInt(EXTI_CH8);
}

/*********************************************************** 
函数功能：RF外部中断函数
入口参数：
出口参数：
备注说明:  上升沿(同步字)  下降沿(接收成功)
***********************************************************/
static void RFExt_IntCallback(void)
{
      RX_FIG = 1; 
      EXTI_ClrIntFlag(EXTI_CH8);
      // RX_num_cnt++; //test
}            

/*********************************************************** 
函数功能：RF-I0口初始化函数
入口参数：
出口参数：
备注说明:
***********************************************************/
void RF_IO_Init(void)
{
   /*  IO_DisableFunc(IO_PORT4,IO_PINx9);
     IO_DisableFunc(IO_PORT4,IO_PINxA);
     IO_DisableFunc(IO_PORT4,IO_PINx8);
     IO_DisableFunc(IO_PORT4,IO_PINxB);  // I/O pin
     IO_ConfigGPIOPin(IO_PORT4,IO_PINx9,IO_DIR_INPUT, IO_PULLUP_OFF);
     IO_ConfigGPIOPin(IO_PORT4,IO_PINxA,IO_DIR_INPUT, IO_PULLUP_OFF);
     IO_ConfigGPIOPin(IO_PORT4,IO_PINxB,IO_DIR_INPUT, IO_PULLUP_OFF);// input

	 __disable_irq();
     IO_WriteGPIOPin(IO_PORT4,IO_PINx9,IO_BIT_CLR);
     IO_WriteGPIOPin(IO_PORT4,IO_PINxA,IO_BIT_CLR);
     IO_WriteGPIOPin(IO_PORT4,IO_PINxB,IO_BIT_CLR);   //
     __enable_irq();
     IO_ConfigGPIOPin(IO_PORT4,IO_PINx8,IO_DIR_INPUT, IO_PULLUP_OFF);  // SO  input
     */
}

/**************************************************
* 函数功能:  SPI  SO_IN 拉低测试
* 输入参数: 
* 输出参数:  i        
* 函数说明: tsd 适用于地址和数据字节之间以及数据字节之间
*           延时 > 80 ns, 
*           CSN 拉低时 MCU要等
*           SOMI 拉低 才能进行 data 操作  delay 250 us
* 使用资源: 
**************************************************/
uint8 TestSOMI(void)
{
    uint8 i;
	PublicDelayUs(10);
	for (i=25; i>0; i--)      //250 us
    {
        PublicDelayUs(10);
        if ((SO_IN == 0)) 
            break;
    } 
	
    return i;
}

/**************************************************
* 函数功能: SPI 写寄存器   
* 输入参数: addr 寄存器地址  value 写入的值
* 输出参数:        
* 函数说明: tsd 适用于地址和数据字节之间以及数据字节之间
*           延时 > 80 ns, 
*           CSN 拉低时 MCU要等
*           SOMI 拉低 才能进行 data 操作  delay 250 us
* 使用资源: 
**************************************************/
void SpiWriteReg(uint8 addr, uint8 value)
{
     SCS_OUT_L;              //CSN=0;
     //while (1 == SO_IN);      //while (SOMI);SOMI=0表明晶体正在运作中，电压调制器已经稳定
     if (TestSOMI())
     {    
         SPI_byte(addr);	     //写地址
         SPI_byte(value);        //写入data
     }
     else
     {
          SOMI_false = 1;    // TestSOMI 错误
     }    
     SCS_OUT_H;              //CSN = 1;         
}

/**************************************************
* 函数功能: SPI 读寄存器   
* 输入参数: addr 读取寄存器地址
* 输出参数:        
* 函数说明: 
* 使用资源: 
**************************************************/
uint8 SpiReadReg(uint8 addr)
{
    uint8 temp;
    uint8 value;

    temp = addr | READ_SINGLE;    //读寄存器命令
    SCS_OUT_L;                    //CSN=0;
   // while (1 == SO_IN);          //while (SOMI);SOMI=0表明晶体正在运作中，电压调制器已经稳定
    if (TestSOMI())
    {    
        SPI_byte(temp);
        value = SPI_byte(0);
    } 
    else
    {
          SOMI_false = 1;    // TestSOMI 错误
    }    
    SCS_OUT_H;                    //CSN=1
    
    return value;
}

/**************************************************
* 函数功能: SPI  写多个字节  
* 输入参数: addr 寄存器地址  *buff 写入data指针   count 写入长度
* 输出参数:        
* 函数说明: 
* 使用资源: 
**************************************************/
void SpiWriteBurstReg(uint8 addr, uint8 *buffer, uint8 count)
{
     uint8 i;
     uint8 temp;
  
     temp = addr | WRITE_BURST;
     SCS_OUT_L;                     //CSN=0;
     //while (1 == SO_IN);            //while (SOMI);SOMI=0表明晶体正在运作中，电压调制器已经稳定
     if (TestSOMI())
     {    
         SPI_byte(temp);                //addr 
         for (i=0; i<count; i++)
         {
             SPI_byte(buffer[i]);        
         }
     }
     else
     {
          SOMI_false = 1;    // TestSOMI 错误
     }    
     SCS_OUT_H;                     //CSN=1;
}

/**********************************************************
* 函数功能: SPI  读多个字节  
* 输入参数: addr 寄存器地址   *buff 存放空间指针  count 读出长度
* 输出参数:        
* 函数说明: 
* 使用资源: 
**********************************************************/
void SpiReadBurstReg (uint8 addr, uint8 *buffer, uint8 count)
{
    uint8 i;
    uint8 temp;
    
    temp = addr | READ_BURST;	        //写入要读的配置寄存器地址和读命令
    SCS_OUT_L;                          //CSN=0;
   // while (1 == SO_IN);                 //while (SOMI);SOMI=0表明晶体正在运作中，电压调制器已经稳定
    if (TestSOMI())
    {    
        SPI_byte(temp);   
        for (i=0; i<count; i++) 
        {
            buffer[i] = SPI_byte(0);
        }
    }
    else
    {
          SOMI_false = 1;    // TestSOMI 错误
    }    
    SCS_OUT_H;                          //CSN=1;
}

/**************************************************
* 函数功能: SPI 写控制命令   
* 输入参数: addr 写入控制命令地址
* 输出参数:        
* 函数说明: 
* 使用资源: 
**************************************************/
void SpiWriteStrobe(uint8 strobe)
{
    SCS_OUT_L;               //CSN=0;
    //while (1 == SO_IN);      //while (SOMI);SOMI=0表明晶体正在运作中，电压调制器已经稳定
    if (TestSOMI())
    {    
        SPI_byte(strobe);        //写入命令
    } 
    else
    {
          SOMI_false = 1;    // TestSOMI 错误
    }    
    SCS_OUT_H;               //CSN=1;
}

/**************************************************
* 函数功能: SPI 读控制命令   
* 输入参数: addr 读出控制命令地址
* 输出参数: 输出 状态寄存器 data        
* 函数说明: 
* 使用资源: 
**************************************************/
uint8 SpiReadStatus(uint8 addr)
{
    uint8 value;
    uint8 temp;
  
    temp = addr | READ_BURST;	//写入要读的状态寄存器的地址同时写入读命令
    SCS_OUT_L;                  //CSN=0;
    //while(1 == SO_IN);         //while (SOMI);SOMI=0表明晶体正在运作中，电压调制器已经稳定
    if (TestSOMI())
    {    
        SPI_byte(temp);
        value = SPI_byte(0);
    }
    else
    {
          SOMI_false = 1;    // TestSOMI 错误
    }    
    SCS_OUT_H;                 //CSN=1;
  
    return value;
}

/**************************************************
* 函数功能: SPI 总线时序 
* 输入参数: SPI_byte(addr)  SPI_byte(data) 。。。   
* 输出参数: SPI_byte(0)  读出data         
* 函数说明:  
* 使用资源: 
**************************************************/
uint8 SPI_byte(uint8 dat)
{
    uint8 i;
    
    for (i=0; i<8; i++)
    {       
        if (dat & 0x80)
        {    
            SI_OUT_H;      //SIMO = 1;   high level write data 
        } 
		else
		{
			SI_OUT_L;          //SIMO = 0;
		}
        SCK_OUT_L;         //SCK = 0; 
        PublicDelayUs(3);
	 
        dat = dat << 1;
        SCK_OUT_H;         //SCK = 1;
     
       // dat = dat << 1;
        if (1 == SO_IN)    
        {    
            dat++;         //high level read data 
        }    

        
    }//end for SPI SYN serial
    
    return dat;
}

