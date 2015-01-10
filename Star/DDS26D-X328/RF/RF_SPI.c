
/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：RF software sPI
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-12-2
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include "Include.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/







/*******************************************************************************
* 函数功能:  SPI  SO_IN 拉低测试
* 输入参数: 
* 输出参数:  i        
* 函数说明: tsd 适用于地址和数据字节之间以及数据字节之间
*           延时 > 80 ns, 
*           CSN 拉低时 MCU要等
*           SOMI 拉低 才能进行 data 操作  delay 250 us
* 使用资源: 
*******************************************************************************/
uint8_t TestSOMI(void)
{
    uint8_t i;
    //Delay100Us();
    NOP();
    NOP();
    for (i=10; i>0; i--)      //250 us
    {
        delayUs(100);
        if (SO_IN_L) 
        {
            break;
        }    
    } 
	
    return i;
}

/*******************************************************************************
* 函数功能: SPI 写寄存器   
* 输入参数: addr 寄存器地址  value 写入的值
* 输出参数:        
* 函数说明: tsd 适用于地址和数据字节之间以及数据字节之间
*           延时 > 80 ns, 
*           CSN 拉低时 MCU要等
*           SOMI 拉低 才能进行 data 操作  delay 250 us
* 使用资源: 
*******************************************************************************/
void SpiWriteReg(uint8_t addr, uint8_t value)
{
     CSN_OUT_L;              //CSN=0;
     if (TestSOMI())
     {    
         SPI_byte(addr);	     //写地址
         SPI_byte(value);        //写入data
     }
     else
     {
          RF.SOMI_false = 1;    // TestSOMI 错误
     }    
     CSN_OUT_H;              //CSN = 1;         
}

/*******************************************************************************
* 函数功能: SPI 读寄存器   
* 输入参数: addr 读取寄存器地址
* 输出参数:        
* 函数说明: 
* 使用资源: 
*******************************************************************************/
/*
uint8_t SpiReadReg(uint8_t addr)
{
    uint8_t temp = 0;
    uint8_t value = 0;

    temp = addr | READ_SINGLE;    //读寄存器命令
    CSN_OUT_L;                    //CSN=0;
     if (TestSOMI())
    {    
        SPI_byte(temp);
        value = SPI_byte(0);
    } 
    else
    {
          RF.SOMI_false = 1;    // TestSOMI 错误
          value = 0;
    }    
    CSN_OUT_H;                    //CSN=1
    
    return value;
}
*/
/**************************************************
* 函数功能: SPI  写多个字节  
* 输入参数: addr 寄存器地址  *buff 写入data指针   count 写入长度
* 输出参数:        
* 函数说明: 
* 使用资源: 
**************************************************/
void SpiWriteBurstReg(uint8_t addr, uint8_t *buffer, uint8_t count)
{
     uint8_t i = 0;
     uint8_t temp = 0;
  
     temp = addr | WRITE_BURST;
     CSN_OUT_L;                     //CSN=0;
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
          RF.SOMI_false = 1;    // TestSOMI 错误
     }    
     CSN_OUT_H;                     //CSN=1;
}

/**********************************************************
* 函数功能: SPI  读多个字节  
* 输入参数: addr 寄存器地址   *buff 存放空间指针  count 读出长度
* 输出参数:        
* 函数说明: 
* 使用资源: 
**********************************************************/
void SpiReadBurstReg (uint8_t addr, uint8_t *buffer, uint8_t count)
{
    uint8_t i = 0;
    uint8_t temp = 0;
    
    temp = addr | READ_BURST;	        //写入要读的配置寄存器地址和读命令
    CSN_OUT_L;                          //CSN=0;
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
          RF.SOMI_false = 1;    // TestSOMI 错误
    }    
    CSN_OUT_H;                          //CSN=1;
}

/**************************************************
* 函数功能: SPI 写控制命令   
* 输入参数: addr 写入控制命令地址
* 输出参数:        
* 函数说明: 
* 使用资源: 
**************************************************/
void SpiWriteStrobe(uint8_t strobe)
{
    CSN_OUT_L;               //CSN=0;
    if (TestSOMI())
    {    
        SPI_byte(strobe);        //写入命令
    } 
    else
    {
          RF.SOMI_false = 1;    // TestSOMI 错误
    }    
    CSN_OUT_H;               //CSN=1;
}

/**************************************************
* 函数功能: SPI 读控制命令   
* 输入参数: addr 读出控制命令地址
* 输出参数: 输出 状态寄存器 data        
* 函数说明: 
* 使用资源: 待改进
**************************************************/
uint8_t SpiReadStatus(uint8_t addr)
{
    uint8_t value = 0;
    uint8_t temp = 0;
  
    temp = addr | READ_BURST;	//写入要读的状态寄存器的地址同时写入读命令
    CSN_OUT_L;                  //CSN=0;
    if (TestSOMI())
    {    
        SPI_byte(temp);
        value = SPI_byte(0);
    }
    else
    {
          RF.SOMI_false = 1;    // TestSOMI 错误
          value = 0;
    }    
    CSN_OUT_H;                 //CSN=1;
  
    return value;
}

/**************************************************
* 函数功能: SPI 总线时序 
* 输入参数: SPI_byte(addr)  SPI_byte(data) 。。。   
* 输出参数: SPI_byte(0)  读出data         
* 函数说明:  上升沿读写data 
* 使用资源: 
**************************************************/
uint8_t SPI_byte(uint8_t dat)
{
    uint8_t i;
    
    for (i=0; i<8; i++)
    {
        if (dat & 0x80)
        {    
            SI_OUT_H;      // SIMO = 1;  high level write data 
        }
		else
		{
		    SI_OUT_L;      // SIMO = 0;
		} 
        SCLK_OUT_L;         // SCK = 0;
        
        delayUs(5);        // ARM    delayUs(5)
        
        dat = dat << 1;
        SCLK_OUT_H;         // SCK = 1; 
        if (SO_IN_H)       //1 
        {    
            dat++;         // high level read data 
        }    
        
    }//end for SPI SYN serial
    
    return dat;
}