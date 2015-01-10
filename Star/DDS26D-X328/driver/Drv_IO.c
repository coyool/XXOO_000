
/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：drv_bt.h 
* 文件标识：见配置管理计划书 
* 摘要：基本定时器移植层

* 当前版本：1.0.0 
* 作者：张玉清
* 完成日期：20131024
*******************************************************************/
#include "Include.h"

/*--------------------全局常量定义---------------------*/

/*--------------------全局变量定义---------------------*/
uint32 testIOState=0;

/*--------------------内部函数申明-------------------------*/

/*********************************************************** 
函数功能：公用的IO初始化
入口参数：
出口参数：
备注说明：上电调用
***********************************************************/
void DRV_IO_Init(void)
{
      #if (_debug_ == 0)
	  IO_ConfigFuncJtagPin(IO_STATE_DISABLE, IO_STATE_DISABLE);
      IO_DisableFunc(IO_PORT0,IO_PINx1);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINx1,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT4,IO_PINxC,IO_BIT_SET);
      
      IO_DisableFunc(IO_PORT0,IO_PINx3);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINx3,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINx3,IO_BIT_CLR);
      #endif
      
      //Idle I0
      //P1F
      IO_DisableAnalogInput(IO_AN15);
      IO_DisableFunc(IO_PORT1,IO_PINxF);
      IO_ConfigGPIOPin(IO_PORT1,IO_PINxF,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT1,IO_PINxF,IO_BIT_CLR);
      //P00
      IO_DisableFunc(IO_PORT0,IO_PINx0);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINx0,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINx0,IO_BIT_CLR);
      //P02
      IO_DisableFunc(IO_PORT0,IO_PINx2);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINx2,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINx2,IO_BIT_CLR);
      //P04
      IO_DisableFunc(IO_PORT0,IO_PINx4);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINx4,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINx4,IO_BIT_CLR);
	  //P09
      IO_DisableFunc(IO_PORT0,IO_PINx9);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINx9,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINx9,IO_BIT_CLR);
      //P0C
      IO_DisableFunc(IO_PORT0,IO_PINxC);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINxC,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINxC,IO_BIT_CLR);
      //P0D
      IO_DisableFunc(IO_PORT0,IO_PINxD);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINxD,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINxD,IO_BIT_CLR);
      //P0E
      IO_DisableFunc(IO_PORT0,IO_PINxE);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINxE,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINxE,IO_BIT_CLR);
      //P0F
      IO_DisableFunc(IO_PORT0,IO_PINxF);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINxF,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINxF,IO_BIT_CLR);
      //P62
      IO_DisableFunc(IO_PORT6,IO_PINx2);
      IO_ConfigGPIOPin(IO_PORT6,IO_PINx2,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT6,IO_PINx2,IO_BIT_CLR);

	  //====================================================================
	  //注意:下面四组IO口在老表上作为FLASH,新表上没有焊接FLASH。
	  IO_DisableFunc(IO_PORT4,IO_PINx2);              //SCK
	  IO_ConfigGPIOPin(IO_PORT4,IO_PINx2,IO_DIR_OUTPUT, IO_PULLUP_OFF);
	  IO_WriteGPIOPin(IO_PORT4,IO_PINx2,IO_BIT_CLR);
	  
   	  IO_DisableFunc(IO_PORT4,IO_PINx3);              //S0
   	  IO_ConfigGPIOPin(IO_PORT4,IO_PINx3,IO_DIR_OUTPUT, IO_PULLUP_OFF);
	  IO_WriteGPIOPin(IO_PORT4,IO_PINx3,IO_BIT_CLR);
	  
      IO_DisableFunc(IO_PORT4,IO_PINx4);              //SI
      IO_ConfigGPIOPin(IO_PORT4,IO_PINx4,IO_DIR_OUTPUT, IO_PULLUP_OFF);
	  IO_WriteGPIOPin(IO_PORT4,IO_PINx4,IO_BIT_CLR);
	  
      IO_DisableFunc(IO_PORT4,IO_PINx5);              //CS  
      IO_ConfigGPIOPin(IO_PORT4,IO_PINx5,IO_DIR_OUTPUT, IO_PULLUP_OFF);  
	  IO_WriteGPIOPin(IO_PORT4,IO_PINx5,IO_BIT_CLR);
	  
	  //====================================================================
	  //注意:下面两组IO口在老表上有红外通讯，新表上没有红外通讯，没有配置红外通讯，输出低
	  //IR_RXD
	  IO_DisableFunc(IO_PORT0, IO_PINxA); 
	  IO_ConfigGPIOPin(IO_PORT0, IO_PINxA, IO_DIR_OUTPUT, IO_PULLUP_OFF);
	  IO_WriteGPIOPin(IO_PORT0,IO_PINxA,IO_BIT_CLR);
	  //IR_TXD
	  IO_DisableFunc(IO_PORT0, IO_PINxB); 
	  IO_ConfigGPIOPin(IO_PORT0, IO_PINxB, IO_DIR_OUTPUT, IO_PULLUP_OFF);
	  IO_WriteGPIOPin(IO_PORT0,IO_PINxB,IO_BIT_CLR);
      //====================================================================
	  //P4C注意:老表上此IO口是空闲口，新表上此IO口作为M-RXD-LED(白工要用)
      IO_DisableFunc(IO_PORT4,IO_PINxC);
      IO_ConfigGPIOPin(IO_PORT4,IO_PINxC,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT4,IO_PINxC,IO_BIT_CLR);

	  //P3F注意:老表上此IO口是用做控制DCDC电源的，新表上此IO口作为M-TXD-LED(白工要用)
	  IO_DisableFunc(IO_PORT3,IO_PINxF);	
	  IO_ConfigGPIOPin(IO_PORT3,IO_PINxF,IO_DIR_OUTPUT,IO_PULLUP_OFF); 
	  IO_WriteGPIOPin(IO_PORT3,IO_PINxF,IO_BIT_CLR);

	  //P60注意:老表上此IO口是COVERCHK,新表上此IO口是空闲口
	  IO_DisableFunc(IO_PORT6,IO_PINx0);		//cover
      IO_ConfigGPIOPin(IO_PORT6,IO_PINx0,IO_DIR_OUTPUT,IO_PULLUP_OFF);
	  IO_WriteGPIOPin(IO_PORT6,IO_PINx0,IO_BIT_CLR);

	  //P40注意:老表上作为报警灯，新表上把无功灯作为报警灯，这个灯悬空
	  IO_DisableFunc(IO_PORT4,IO_PINx0);		//cover
      IO_ConfigGPIOPin(IO_PORT4,IO_PINx0,IO_DIR_OUTPUT,IO_PULLUP_OFF);
	  IO_WriteGPIOPin(IO_PORT4,IO_PINx0,IO_BIT_CLR);

	  //P61注意:老表上此IO口是空闲口,新表上此IO口作为V-LCD
	  //此IO口配置为输入(除高温情况下配置为输出低)
      IO_DisableFunc(IO_PORT6,IO_PINx1);
      IO_ConfigGPIOPin(IO_PORT6,IO_PINx1,IO_DIR_INPUT,IO_PULLUP_OFF);
}
/*********************************************************** 
函数功能：公用的IO初始化
入口参数：
出口参数：
备注说明：电池供电调用
***********************************************************/
void DRV_IO_PowerDownInit(void)
{	 
      //PPM
      IO_DisableFunc(IO_PORT4,IO_PINx1);
      IO_ConfigGPIOPin(IO_PORT4,IO_PINx1,IO_DIR_OUTPUT,IO_PULLUP_OFF);
	  IO_WriteGPIOPin(IO_PORT4,IO_PINx1,IO_BIT_CLR);
      
      #if (_debug_ == 0)
      IO_ConfigFuncJtagPin(IO_STATE_DISABLE, IO_STATE_DISABLE);
      IO_DisableFunc(IO_PORT0,IO_PINx1);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINx1,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINx1,IO_BIT_SET);                              ///!!!
      
      IO_DisableFunc(IO_PORT0,IO_PINx3);
	  IO_ConfigGPIOPin(IO_PORT0,IO_PINx3,IO_DIR_OUTPUT,IO_PULLUP_OFF);            //!!!
      IO_WriteGPIOPin(IO_PORT0,IO_PINx3,IO_BIT_CLR);
      #endif   
      
      //Idle I0   
      //P1F
      IO_DisableAnalogInput(IO_AN15);
      IO_DisableFunc(IO_PORT1,IO_PINxF);
      IO_ConfigGPIOPin(IO_PORT1,IO_PINxF,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT1,IO_PINxF,IO_BIT_CLR);
      //P00
      IO_DisableFunc(IO_PORT0,IO_PINx0);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINx0,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINx0,IO_BIT_CLR);
      //P02
      IO_DisableFunc(IO_PORT0,IO_PINx2);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINx2,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINx2,IO_BIT_CLR);
      //P04
      IO_DisableFunc(IO_PORT0,IO_PINx4);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINx4,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINx4,IO_BIT_CLR);
	  //P09
      IO_DisableFunc(IO_PORT0,IO_PINx9);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINx9,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINx9,IO_BIT_CLR);
      //P0C
      IO_DisableFunc(IO_PORT0,IO_PINxC);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINxC,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINxC,IO_BIT_CLR);
      //P0D
      IO_DisableFunc(IO_PORT0,IO_PINxD);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINxD,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINxD,IO_BIT_CLR);
      //P0E
      IO_DisableFunc(IO_PORT0,IO_PINxE);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINxE,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINxE,IO_BIT_CLR);
      //P0F
      IO_DisableFunc(IO_PORT0,IO_PINxF);
      IO_ConfigGPIOPin(IO_PORT0,IO_PINxF,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT0,IO_PINxF,IO_BIT_CLR);
      //P62
      IO_DisableFunc(IO_PORT6,IO_PINx2);
      IO_ConfigGPIOPin(IO_PORT6,IO_PINx2,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT6,IO_PINx2,IO_BIT_CLR);

	  //====================================================================
	  //注意:下面四组IO口在老表上作为FLASH,新表上没有焊接FLASH。
	  IO_DisableFunc(IO_PORT4,IO_PINx2);              //SCK
	  IO_ConfigGPIOPin(IO_PORT4,IO_PINx2,IO_DIR_OUTPUT, IO_PULLUP_OFF);
	  IO_WriteGPIOPin(IO_PORT4,IO_PINx2,IO_BIT_CLR);
	  
   	  IO_DisableFunc(IO_PORT4,IO_PINx3);              //S0
   	  IO_ConfigGPIOPin(IO_PORT4,IO_PINx3,IO_DIR_OUTPUT, IO_PULLUP_OFF);
	  IO_WriteGPIOPin(IO_PORT4,IO_PINx3,IO_BIT_CLR);
	  
      IO_DisableFunc(IO_PORT4,IO_PINx4);              //SI
      IO_ConfigGPIOPin(IO_PORT4,IO_PINx4,IO_DIR_OUTPUT, IO_PULLUP_OFF);
	  IO_WriteGPIOPin(IO_PORT4,IO_PINx4,IO_BIT_CLR);
	  
      IO_DisableFunc(IO_PORT4,IO_PINx5);              //CS  
      IO_ConfigGPIOPin(IO_PORT4,IO_PINx5,IO_DIR_OUTPUT, IO_PULLUP_OFF);  
	  IO_WriteGPIOPin(IO_PORT4,IO_PINx5,IO_BIT_CLR);

	  //====================================================================
	  //注意:下面两组IO口在老表上有红外通讯，新表上没有红外通讯，没有配置红外通讯，输出低
	  //IR_RXD
	  IO_DisableFunc(IO_PORT0, IO_PINxA); 
	  IO_ConfigGPIOPin(IO_PORT0, IO_PINxA, IO_DIR_OUTPUT, IO_PULLUP_OFF);
	  IO_WriteGPIOPin(IO_PORT0,IO_PINxA,IO_BIT_CLR);
	  //IR_TXD
	  IO_DisableFunc(IO_PORT0, IO_PINxB); 
	  IO_ConfigGPIOPin(IO_PORT0, IO_PINxB, IO_DIR_OUTPUT, IO_PULLUP_OFF);
	  IO_WriteGPIOPin(IO_PORT0,IO_PINxB,IO_BIT_CLR);
	  //====================================================================
	  //P4C注意:老表上此IO口是空闲口，新表上此IO口作为M-RXD-LED(白工要用)
      IO_DisableFunc(IO_PORT4,IO_PINxC);
      IO_ConfigGPIOPin(IO_PORT4,IO_PINxC,IO_DIR_OUTPUT,IO_PULLUP_OFF);
      IO_WriteGPIOPin(IO_PORT4,IO_PINxC,IO_BIT_CLR);

	  //P3F注意:老表上此IO口是用做控制DCDC电源的，新表上此IO口作为M-TXD-LED(白工要用)
	  IO_DisableFunc(IO_PORT3,IO_PINxF);	
	  IO_ConfigGPIOPin(IO_PORT3,IO_PINxF,IO_DIR_OUTPUT,IO_PULLUP_OFF); 
	  IO_WriteGPIOPin(IO_PORT3,IO_PINxF,IO_BIT_CLR);

	  //P60注意:老表上此IO口是COVERCHK,新表上此IO口是空闲口
	  IO_DisableFunc(IO_PORT6,IO_PINx0);		//cover
      IO_ConfigGPIOPin(IO_PORT6,IO_PINx0,IO_DIR_OUTPUT,IO_PULLUP_OFF);
	  IO_WriteGPIOPin(IO_PORT6,IO_PINx0,IO_BIT_CLR);

	  //P40注意:老表上作为报警灯，新表上把无功灯作为报警灯，这个灯悬空
	  IO_DisableFunc(IO_PORT4,IO_PINx0);		//cover
      IO_ConfigGPIOPin(IO_PORT4,IO_PINx0,IO_DIR_OUTPUT,IO_PULLUP_OFF);
	  IO_WriteGPIOPin(IO_PORT4,IO_PINx0,IO_BIT_CLR);

	  //P61注意:老表上此IO口是空闲口,新表上此IO口作为V-LCD
	  //此IO口配置为输入(除高温情况下配置为输出低)
      IO_DisableFunc(IO_PORT6,IO_PINx1);
      IO_ConfigGPIOPin(IO_PORT6,IO_PINx1,IO_DIR_INPUT,IO_PULLUP_OFF);
}




void TestIO(void)
{
	IO_DisableFunc(IO_PORT_LED_ALARM,IO_PIN_LED_ALARM);
  	IO_ConfigGPIOPin(IO_PORT_LED_ALARM,IO_PIN_LED_ALARM,IO_DIR_OUTPUT,IO_PULLUP_OFF);

	if(testIOState=!testIOState)
	{
  		IO_WriteGPIOPin(IO_PORT_LED_ALARM,IO_PIN_LED_ALARM,IO_BIT_CLR);
	}
	else
	{
  		IO_WriteGPIOPin(IO_PORT_LED_ALARM,IO_PIN_LED_ALARM,IO_BIT_SET);
	}
}


