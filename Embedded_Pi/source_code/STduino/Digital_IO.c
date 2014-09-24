/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：Digital I/O   duino SDK for STM32F103RB
*
* 文件标识：
* 摘    要：
*
* 当前版本：V1.0
* 作    者：F06553
* 完成日期：2014-9-18
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/





/*******************************************************************************
* Description : Configures the specified pin to behave either as an input 
*               or an output. 
* Syntax      : pinMode(pin, mode)
* Parameters I: pin: the number of the pin whose mode you wish to set
*               mode: INPUT, OUTPUT, or INPUT_PULLUP.
* Parameters O: None
* return      : None
*******************************************************************************/
void pinMode(GPIO_TypeDef* PORT, u16 pin, 
             GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

//    /* Enable the GPIO_LED Clock */
//    RCC_APB2PeriphClockCmd(PORT, ENABLE);    
    
    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Mode = mode;  /* 推挽输出 */
    GPIO_InitStructure.GPIO_Speed = speed;
    
    GPIO_Init(PORT, &GPIO_InitStructure);
}

/*******************************************************************************
* Description : Write a HIGH or a LOW value to a digital pin.
* Syntax      : digitalWrite(pin, value)
* Parameters I: pin: the pin number
*               value: HIGH or LOW
* Parameters O: none
* return      : none
*******************************************************************************/
void digitalWrite(GPIO_TypeDef* PORT, u16 pin, BitAction BitVal)
{
    /* Set the GPIOA port pin 15 */ 
    GPIO_WriteBit(PORT, pin, BitVal); 
}

/*******************************************************************************
* Description : Reads the value from a specified digital pin, either HIGH or LOW
* Syntax      : digitalRead(pin)
* Parameters I: pin: the number of the digital pin you want to read (int)
* Parameters O: none
* return      : HIGH or LOW
*******************************************************************************/
u8 digitalRead(GPIO_TypeDef* PORT, u16 pin)
{
    /* Reads the seventh pin of the GPIOB and store it in ReadValue 
    variable */ 
    u8 ReadValue; 
    ReadValue = GPIO_ReadInputDataBit(PORT, pin); 
    
    return ReadValue;
}








