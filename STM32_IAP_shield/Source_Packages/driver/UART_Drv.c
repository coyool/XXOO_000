/*******************************************************************************
* Copyright 2014      深圳思达仪表  固件部  
* All right reserved
*
* 文件名称：UART API 
*
* 文件标识：UART.c
* 摘    要：STM32F10x Serial API
*
* 当前版本：V1.0
* 作    者：F06553
* 完成日期：2014-7-25
* 编译环境：IAR_for_ARM
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/
USART_TypeDef* USART[COMn] = 
{
    USART1, 
    USART2, 
    USART3
}; 

GPIO_TypeDef*  USART_GPIO[COMn] = 
{
    USART1_GPIOA, 
    USART2_GPIOA, 
    USART3_GPIOB
};
 
const uint32_t USART_CLK[COMn] = 
{
    RCC_APB2Periph_USART1, 
    RCC_APB1Periph_USART2, 
    RCC_APB1Periph_USART3
};

const uint32_t USART_GPIO_CLK[COMn] = 
{
    RCC_APB2Periph_GPIOA, 
    RCC_APB2Periph_GPIOA, 
    RCC_APB2Periph_GPIOB
};

const uint16_t USART_TX_PIN[COMn] = 
{
    USART1_TxPin, 
    USART2_TxPin,
    USART3_TxPin  
};

const uint16_t USART_RX_PIN[COMn] = 
{
    USART1_RxPin, 
    USART2_RxPin,
    USART3_RxPin
};

const uint16_t USART_IRQn[COMn]=
{
    USART1_IRQn,      /* 37 */
    USART2_IRQn,      /* 38 */
    USART3_IRQn       /* 39 */
};

/*** extern variable declarations ***/







/*******************************************************************************
* 函数名称: UART setup
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能:  The UART_setup() function is called when a sketch starts. 
*            Use it to initialize variables, pin modes, start using libraries, etc. 
*            The setup function will only run once, after each powerup or reset of the board 
*******************************************************************************/
void UART_setup((USART_TypeDef *) COM, const baudrate)
{
    USART_InitTypeDef USART_InitStructure;
         
    
    /*** 
    USARTx configured as follow:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits
    - Stop Bit = 1 bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled
    ***/

    USART_InitStructure.USART_BaudRate = baudrate;   /* 57600 */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
    
    /* USART configuration */
    USART_Init(USART[COM], USART_InitStruct);
    
    
    /* Enable USARTy Receive and Transmit interrupts */
    USART_ITConfig(USARTy, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USARTy, USART_IT_TXE, ENABLE);

    /* Enable USARTz Receive and Transmit interrupts */
    USART_ITConfig(USARTz, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USARTz, USART_IT_TXE, ENABLE);

    
    /* Enable USART Periph */
    USART_Cmd(USART[COM], ENABLE);
    
}

/*******************************************************************************
* 函数名称:  UART disable 
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能:  choose disable USARTx  
*******************************************************************************/
void UART_end((USART_TypeDef *) COM)
{
    /* Enable USART Periph */
    USART_Cmd(USART[COM], ENABLE);
}





