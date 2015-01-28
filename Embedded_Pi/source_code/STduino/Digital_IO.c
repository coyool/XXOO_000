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
#ifndef EMBEDDED_PI
static const PORT_PIN_TYPE PORT_pin_tab[PD2+1] =  //结构体数组
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
#endif 

/* JP6 JP7 JP8 JP9 map */
static const PORT_PIN_TYPE pinMap[46] =  //结构体数组
{
    {GPIOC, GPIO_Pin_11},  //0  PC11/USART3_RX
    {GPIOC, GPIO_Pin_10},  //1  PC10/USART3_TX
    {GPIOC, GPIO_Pin_12},  //2  PC12/USART3_CK
    {GPIOC, GPIO_Pin_6},   //3  PC6/TIM3_CH1
    {GPIOC, GPIO_Pin_7},   //4  PC7/TIM3_CH2
    {GPIOC, GPIO_Pin_8},   //5  PC8/TIM3_CH3
    {GPIOC, GPIO_Pin_9},   //6  PC9/TIM3_CH4
    {GPIOD, GPIO_Pin_2},   //7  PD2/TIM3_ETR
    {GPIOA, GPIO_Pin_15},  //8  PA15/JTDI/TIM2_CH1_ETR/SPI1_NSS
    {GPIOA, GPIO_Pin_8},   //9  PA8/USART1_CK/TIM1_CH1/MCO
    {GPIOB, GPIO_Pin_12},  //10 PB12/SPI2_NSS/I2C2_SMBAI/USART3_CK/TIM1_BKIN 
    {GPIOB, GPIO_Pin_15},  //11 PB15/SPI2_MOSI/TIM1_CH3N
    {GPIOB, GPIO_Pin_14},  //12 PB14/SPI2_MISO/USART3_RTS/TIM1_CH2N 
    {GPIOB, GPIO_Pin_13},  //13 PB13/SPI2_SCK/USART3_CTS/TIM1_CH1N
    {GPIOB, GPIO_Pin_7},   //14 PB7/I2C1_SDA/TIM4_CH2/USART1_RX
    {GPIOB, GPIO_Pin_6},   //15 PB6/I2C1_SCL/TIM4_CH1/USART1_TX
    {GPIOC, GPIO_Pin_0},   //16 PC0/ADC10
    {GPIOC, GPIO_Pin_1},   //17 PC1/ADC11
    {GPIOC, GPIO_Pin_2},   //18 PC2/ADC12
    {GPIOC, GPIO_Pin_3},   //19 PC3/ADC13
    {GPIOC, GPIO_Pin_4},   //20 PC4/ADC14
    {GPIOC, GPIO_Pin_5},   //21 PC5/ADC15
    {GPIOA, GPIO_Pin_3},   //22 PA3/USART2_RX/ADC3/TIM2_CH4 
    {GPIOA, GPIO_Pin_2},   //23 PA2/USART2_RX/ADC2/TIM2_CH3
    {GPIOA, GPIO_Pin_1},   //24 PA1/USART2_RTS/ADC1/TIM2_CH2 
    {GPIOA, GPIO_Pin_0},   //25 PA0-WKUP/USART2_CTS/ADC0/TIM2_CH1_ETR
    {GPIOA, GPIO_Pin_9},   //26 PA9/USART1_TX/TIM1_CH2
    {GPIOB, GPIO_Pin_0},   //27 PB0/ADC8/TIM3_CH3/TIM1_CH2N
    {GPIOA, GPIO_Pin_10},  //28 PA10/USART1_RX/TIM1_CH3
    {GPIOB, GPIO_Pin_1},   //29 PB1/ADC9/TIM3_CH4/TIM1_CH3N
    {GPIOB, GPIO_Pin_8},   //30 PB8/TIM4_CH3/I2C1_SCL/CANRX
    {GPIOB, GPIO_Pin_9},   //31 PB9/TIM4_CH4/I2C1_SCA/CANTX
    {GPIOA, GPIO_Pin_4},   //32 PA4/SPI1_NSS/USART2_CK/ADC4
    {GPIOA, GPIO_Pin_7},   //33 PA7/SPI1_MOSI/ADC7/TIM3_CH2/TIM1_CH1N
    {GPIOA, GPIO_Pin_6},   //34 PA6/SPI1_MISO/ADC6/TIM3_CH1/TIM1_BKIN
    {GPIOA, GPIO_Pin_5},   //35 PA5/SPI1_SCK/ADC5
    {GPIOC, GPIO_Pin_13},  //36 PC13/ANT1_TAMP 
    {GPIOB, GPIO_Pin_5},   //37 PB5/I2C1_SMBAI/TIM3_CH2/SPI1_MOSI
    {GPIOB, GPIO_Pin_11},  //38 PB11/I2C2_SDA/USART3_RX/TIM2_CH4 
    {GPIOB, GPIO_Pin_10},  //39 PB10/I2C2_SCL/USART3_TX/TIM2_CH3 
    {GPIOC, GPIO_Pin_0},   //40 PC0/ADC10
    {GPIOC, GPIO_Pin_1},   //41 PC1/ADC11
    {GPIOC, GPIO_Pin_2},   //42 PC2/ADC12
    {GPIOC, GPIO_Pin_3},   //43 PC3/ADC13
    {GPIOC, GPIO_Pin_4},   //44 PC4/ADC14
    {GPIOC, GPIO_Pin_5},   //45 PC5/ADC15
};

/*** extern variable declarations ***/



#ifndef EMBEDDED_PI

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
    /* Reads the seventh pin of the GPIOB and store it in ReadValue variable */ 
    u8 ReadValue; 
    ReadValue = GPIO_ReadInputDataBit(PORT, pin); 
    
    return ReadValue;
}

#endif 

#ifndef EMBEDDED_PI
 
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
void pinMode_Px(STM32F103x_PIN_TYPE pin, GPIOMode_TypeDef mode)
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
void digitalWrite_Px(STM32F103x_PIN_TYPE pin, BitAction BitVal)
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
u8 digitalRead_Px(STM32F103x_PIN_TYPE pin)
{
    /* Reads the seventh pin of the GPIOB and store it in ReadValue variable */ 
    u8 ReadValue; 
    ReadValue = GPIO_ReadInputDataBit(PORT_pin_tab[pin].PORTx,
                                      PORT_pin_tab[pin].pinx); 
    return ReadValue;
}

#endif 

#ifdef EMBEDDED_PI

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
void pinMode(EMBEDDED_PI_PIN_TYPE pin, GPIOMode_TypeDef mode)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

//    /* Enable the GPIO_LED Clock */
//    RCC_APB2PeriphClockCmd(PORT, ENABLE);    
    
    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Pin = pinMap[pin].pinx;
    GPIO_InitStructure.GPIO_Mode = mode;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    
    GPIO_Init(pinMap[pin].PORTx, &GPIO_InitStructure);
}

/*******************************************************************************
* Description : Write a HIGH or a LOW value to a digital pin.
* Syntax      : digitalWrite(pin, value) 
* Parameters I: pin: the pin number
*               value: HIGH or LOW
* Parameters O: none
* return      : none
*******************************************************************************/
void digitalWrite(EMBEDDED_PI_PIN_TYPE pin, BitAction BitVal)
{
    /* Set the GPIOA port pin 15 */ 
    GPIO_WriteBit(pinMap[pin].PORTx,
                  pinMap[pin].pinx, 
                  BitVal); 
}

/*******************************************************************************
* Description : Reads the value from a specified digital pin, either HIGH or LOW
* Syntax      : digitalRead(pin)
* Parameters I: pin: the number of the digital pin you want to read (int)
* Parameters O: none
* return      : HIGH or LOW
*******************************************************************************/
u8 digitalRead(EMBEDDED_PI_PIN_TYPE pin)
{
    /* Reads the seventh pin of the GPIOB and store it in ReadValue variable */ 
    u8 ReadValue; 
    ReadValue = GPIO_ReadInputDataBit(pinMap[pin].PORTx,
                                      pinMap[pin].pinx); 
    return ReadValue;
}

#endif
