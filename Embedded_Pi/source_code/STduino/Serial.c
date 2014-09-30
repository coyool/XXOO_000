/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：Serial    duino SDK for STM32F103RB
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-9-19
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/






/*******************************************************************************
* Description : Sets the data rate in bits per second (baud) for serial data 
*               transmission.
*               {start: 1bit; data: 8bit; stop: 1bit; parity: none}
*               For communicating with the computer, use one of these rates: 
*               300, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 
*               or 115200. 
*               UART1 
* Syntax      : Seria.begin(speed)   
* Parameters I: speed: in bits per second (baud) - long
* Parameters O: none
* return      : nothing
*******************************************************************************/
void Serial_bigen()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    
   /* config USART1 clock */  
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    /* USART1 GPIO config */
    /* Configure USART1 Tx (PA.09) as alternate function pushpull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    /* USART1 configured as follow:
        - BaudRate = 230400 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - Even parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
    */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    
    USART_Cmd(USART1, ENABLE);

//    transmitting = false;
}

/*******************************************************************************
* Description : Disables serial communication, allowing the RX and TX pins to 
*               be used for general input and output. 
*               To re-enable serial communication, call Serial.begin().
* Syntax      : Seria.end()
* Parameters I: none
* Parameters O: none
* return      : nothing
*******************************************************************************/
void Serial_end(void)
{    
    /* Disable the USART Transmit interrupt */
//    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    /* Disable USART */
    USART_Cmd(USART1, DISABLE);
    /* Disable USART Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);
    
    /* Clear Serial buff */
    //
}






