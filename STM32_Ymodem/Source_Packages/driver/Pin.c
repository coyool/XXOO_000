/*******************************************************************************
* Copyright 2014      深圳思达仪表  固件部  
* All right reserved
*
* 文件名称：pin interface
*
* 文件标识：pin.c
* 摘    要：pin API   digital I/O  Analog I/O
*
* 当前版本：V1.0
* 作    者：F06553
* 完成日期：2014-8-4
* 编译环境：IAR_for_ARM  6.50
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/



/***  
Digital I/O   
***/

/*******************************************************************************
* 函数名称: pin_mode   pinMode(pin, mode)
* 输入参数: pin: the number of the pin whose mode you wish to set
*           mode: INPUT, OUTPUT, or INPUT_PULLUP. 
* 输出参数: 
* --返回值: 
* 函数功能: Configures the specified pin to behave either as an input or an output.
*******************************************************************************/
void pin_mode(void)
{
    _NOP();
    
}

/*******************************************************************************
* 函数名称: pin_write   digitalWrite(pin, value)
* 输入参数: pin:   the pin number   
*           value: HIGH or LOW
* 输出参数: 
* --返回值: 
* 函数功能: Write a HIGH or a LOW value to a digital pin 
*******************************************************************************/
void pin_write(GPIO_TypeDef* Port, u16 Pin, BitAction value)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(Port));
    assert_param(IS_GET_GPIO_PIN(Pin));
    assert_param(IS_GPIO_BIT_ACTION(value)); 
    
    /* Set Value the GPIOx port pin xx */ 
    GPIO_WriteBit(Port, Pin, value);
}

/*******************************************************************************
* 函数名称: pin_read   digitalRead(pin)
* 输入参数: pin: the number of the digital pin you want to read (int)
* 输出参数: 
* --返回值: HIGH or LOW
* 函数功能: Reads the value from a specified digital pin, either HIGH or LOW.
*******************************************************************************/
//void pin_read(GPIO_TypeDef* Port, uint16_t Pin)
//{
//    /* Reads the seventh pin of the GPIOB and store it in ReadValue 
//    variable */ 
//    u8 ReadValue = 0x00;
//    
//    /* Check the parameters */
//    assert_param(IS_GPIO_ALL_PERIPH(Port));
//    assert_param(IS_GET_GPIO_PIN(Pin)); 
//  
//    ReadValue = GPIO_ReadInputDataBit(Port, Pin);
//}


/***
Analog I/O
***/