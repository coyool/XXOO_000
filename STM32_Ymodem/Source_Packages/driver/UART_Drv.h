#ifndef __UART_H_
#define __UART_H_


/*** define and type ***/

#define COMn                     3

/** 
 * Periph func 
*/
#define USART1_TxPin             GPIO_Pin_9
#define USART1_RxPin             GPIO_Pin_10
#define USART1_GPIOA             GPIOA
#define USART1_Tx_DMA_Channel    DMA1_Channel4
#define USART1_Rx_DMA_Channel    DMA1_Channel5
#define USART1_Tx_DMA_FLAG       DMA1_FLAG_TC4
#define USART1_Rx_DMA_FLAG       DMA1_FLAG_TC5
#define USARTy_DR_Base           0x40013804

#define USART2_TxPin             GPIO_Pin_2
#define USART2_RxPin             GPIO_Pin_3
#define USART2_GPIOA             GPIOA
#define USART2_Tx_DMA_Channel    DMA1_Channel7
#define USART2_Rx_DMA_Channel    DMA1_Channel6
#define USART2_Tx_DMA_FLAG       DMA1_FLAG_TC7
#define USART2_Rx_DMA_FLAG       DMA1_FLAG_TC6
#define USARTz_DR_Base           0x40004404

#define USART3_TxPin             GPIO_Pin_10
#define USART3_RxPin             GPIO_Pin_11
#define USART3_GPIOB             GPIOB
#define USART3_Tx_DMA_Channel    DMA1_Channel2
#define USART3_Rx_DMA_Channel    DMA1_Channel3
#define USART3_Tx_DMA_FLAG       DMA1_FLAG_TC2
#define USART3_Rx_DMA_FLAG       DMA1_FLAG_TC3
#define USARTz_DR_Base           0x40004804


/**
 * Pins Software Remapping 
 */ 

#define USART2_TX_PIN                 GPIO_Pin_5
#define USART2_RX_PIN                 GPIO_Pin_6
#define USART2_GPIOD                   GPIOD


typedef enum 
{
    UART_COM1 = 0,
    UART_COM2 = 1,
    UART_COM3 = 2
} UART_COM_TypeDef; 






/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern void UART_setup(void);


#endif


