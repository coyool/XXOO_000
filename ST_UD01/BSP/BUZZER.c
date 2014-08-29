/*******************************************************************************
* Copyright 2014      深圳思达仪表  固件部  
* All right reserved
*
* 文件名称：button
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：
* 完成日期：
* 编译环境：
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"






/*******************************************************************************
* 函数名称: BUZZ setup
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: -- 
*******************************************************************************/
void BUZZ_setup(void)
{ 
   IO_ConfigGPIOPin(IO_PORT3,IO_PINxA,IO_DIR_OUTPUT,IO_PULLUP_OFF);
   IO_WriteGPIOPin(IO_PORT3,IO_PINxA,IO_BIT_CLR);
}

/*******************************************************************************
* 函数名称: 
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: -- 
*******************************************************************************/
void oneSound(uint8_t cnt,uint16_t Voicedelay)
{
  uint8_t i,j;
  
  for(j=0;j<cnt;j++)
  {
      for(i=0; i<50; i++)
      {
          IO_WriteGPIOPin(IO_PORT3,IO_PINxA,IO_BIT_CLR);
          PublicDelayUs(200);
          IO_WriteGPIOPin(IO_PORT3,IO_PINxA,IO_BIT_SET);
          PublicDelayUs(200);
      } 
      delay_ms(Voicedelay);
  }  

}

/*******************************************************************************
* 函数名称: 
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
void BuzzSound(uint8_t mode)
{
    if(mode == VERSION_LOW)
    {
        oneSound(1,0); 
    }
    else if(mode == VERSION_EQU)
    {
       oneSound(5,100); 
    }  
    else  // mode == VERSION_HIGH  
    {
        _NOP();
    }    
}