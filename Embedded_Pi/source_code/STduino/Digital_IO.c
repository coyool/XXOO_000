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
* 源代码说明：board -- Embedded Pi
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/
static const PORT_PIN_TYPE PORT_pin_tab[PD2+1] =
{
    {GPIOA,GPIO_Pin_0},{GPIOA,GPIO_Pin_1},{GPIOA,GPIO_Pin_2},{GPIOA,GPIO_Pin_3},
    {GPIOA,GPIO_Pin_4},{GPIOA,GPIO_Pin_5},{GPIOA,GPIO_Pin_6},{GPIOA,GPIO_Pin_7},
    {GPIOA,GPIO_Pin_8},{GPIOA,GPIO_Pin_9},{GPIOA,GPIO_Pin_10},
    {GPIOB,GPIO_Pin_0},
    {GPIOB,GPIO_Pin_1},{GPIOB,GPIO_Pin_5},{GPIOB,GPIO_Pin_6},{GPIOB,GPIO_Pin_7},
    {GPIOB,GPIO_Pin_8},{GPIOB,GPIO_Pin_9},{GPIOB,GPIO_Pin_10},{GPIOB,GPIO_Pin_11},
    {GPIOB,GPIO_Pin_12},{GPIOB,GPIO_Pin_13},{GPIOB,GPIO_Pin_14},{GPIOB,GPIO_Pin_15},
    {GPIOC,GPIO_Pin_0},{GPIOC,GPIO_Pin_1},{GPIOC,GPIO_Pin_2},{GPIOC,GPIO_Pin_3},
    {GPIOC,GPIO_Pin_4},{GPIOC,GPIO_Pin_5},{GPIOC,GPIO_Pin_6},{GPIOC,GPIO_Pin_7},
    {GPIOC,GPIO_Pin_8},{GPIOC,GPIO_Pin_9},{GPIOC,GPIO_Pin_10},{GPIOC,GPIO_Pin_11},
    {GPIOC,GPIO_Pin_12},{GPIOC,GPIO_Pin_13},
    {GPIOD,GPIO_Pin_2}
};


/*** extern variable declarations ***/





/*******************************************************************************
* Description : Configures the specified pin to behave either as an input 
*               or an output. 
* Syntax      : pinMode_ALL(port, pin, mode, speed)
* Parameters I: port: 
*               pin: the number of the pin whose mode you wish to set
*               mode: INPUT, OUTPUT, or INPUT_PULLUP.
*               speed:
* Parameters O: None
* return      : None
*******************************************************************************/
void pinMode_ALL(GPIO_TypeDef* PORT, u16 pin, 
             GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

//    /* Enable the GPIO Clock */
//    RCC_APB2PeriphClockCmd(PORT, ENABLE);    
    
    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Mode = mode;  
    GPIO_InitStructure.GPIO_Speed = speed;
    
    GPIO_Init(PORT, &GPIO_InitStructure);
}

/*******************************************************************************
* Description : Write a HIGH or a LOW value to a digital pin.
* Syntax      : digitalWrite_ALL(pin, value)  digitalWrite -- ALL
* Parameters I: pin: the pin number
*               value: HIGH or LOW
* Parameters O: none
* return      : none
*******************************************************************************/
void digitalWrite_ALL(GPIO_TypeDef* PORT, u16 pin, BitAction BitVal)
{
    /* Set the GPIOA port pin 15 */ 
    GPIO_WriteBit(PORT, pin, BitVal); 
}

/*******************************************************************************
* Description : Reads the value from a specified digital pin, either HIGH or LOW
* Syntax      : digitalRead_ALL(pin)
* Parameters I: pin: the number of the digital pin you want to read (int)
* Parameters O: none
* return      : HIGH or LOW
*******************************************************************************/
u8 digitalRead_ALL(GPIO_TypeDef* PORT, u16 pin)
{
    /* Reads the seventh pin of the GPIOB and store it in ReadValue 
    variable */ 
    u8 ReadValue; 
    ReadValue = GPIO_ReadInputDataBit(PORT, pin); 
    
    return ReadValue;
}

/*******************************************************************************
* Description : Configures the specified pin to behave either as an input 
*               or an output.  
*               default: speed 10MHz     NOTE --  
*                       
* Syntax      : pinMode(pin, mode)
* Parameters I: port: 
*               pin: the number of the pin whose mode you wish to set
*               mode: INPUT, OUTPUT, or INPUT_PULLUP.
*               speed:
* Parameters O: None
* return      : None
*******************************************************************************/
void pinMode(STM32F103x_PIN_TYPE pin, GPIOMode_TypeDef mode)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

//    /* Enable the GPIO_LED Clock */
//    RCC_APB2PeriphClockCmd(PORT, ENABLE);    
    
    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Pin = PORT_pin_tab[pin].pinx;
    GPIO_InitStructure.GPIO_Mode = mode;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    
    GPIO_Init(PORT_pin_tab[pin].PORTx, &GPIO_InitStructure);
}

/*******************************************************************************
* Description : Write a HIGH or a LOW value to a digital pin.
* Syntax      : digitalWrite(pin, value) 
* Parameters I: pin: the pin number
*               value: HIGH or LOW
* Parameters O: none
* return      : none
*******************************************************************************/
void digitalWrite(STM32F103x_PIN_TYPE pin, BitAction BitVal)
{
    /* Set the GPIOA port pin 15 */ 
    GPIO_WriteBit(PORT_pin_tab[pin].PORTx,
                  PORT_pin_tab[pin].pinx, 
                  BitVal); 
}

/*******************************************************************************
* Description : Reads the value from a specified digital pin, either HIGH or LOW
* Syntax      : digitalRead(pin)
* Parameters I: pin: the number of the digital pin you want to read (int)
* Parameters O: none
* return      : HIGH or LOW
*******************************************************************************/
u8 digitalRead(STM32F103x_PIN_TYPE pin)
{
    /* Reads the seventh pin of the GPIOB and store it in ReadValue 
    variable */ 
    u8 ReadValue; 
    ReadValue = GPIO_ReadInputDataBit(PORT_pin_tab[pin].PORTx,
                                      PORT_pin_tab[pin].pinx); 
    return ReadValue;
}




