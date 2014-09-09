/*******************************************************************************
* Copyright 2014      深圳思达仪表  固件部  
* All right reserved
*
* 文件名称：board support package
*
* 文件标识：BSP.c
* 摘    要：
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



/*** extern variable declarations ***/








/*******************************************************************************
* 函数名称: bus clock
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
static void RCC_Configuration(void)
{
    /* Enable GPIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    /* Enable USART Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
}

/*******************************************************************************
* 函数名称: NVIC config
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* Configure the NVIC Preemption Priority Bits */  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    /* Enable the USART Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* 函数名称: GPIO config
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
static void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 

    /* Configure USART Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = USART_RX_PIN[COM];
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(USART_GPIO[COM], &GPIO_InitStructure);

    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = USART_TX_PIN[COM];
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(USART_GPIO[COM], &GPIO_InitStructure);
}

/*******************************************************************************
* 函数名称: DMA config 
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
static void DMA_Configuration(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    /* USART1_Tx_DMA_Channel (triggered by USART1 Tx event) Config */
    DMA_DeInit(USART1_Tx_DMA_Channel);
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;   
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxBuffer1; // 
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;           
    DMA_InitStructure.DMA_BufferSize = TxBufferSize1;        //
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    
    DMA_Init(USART1_Tx_DMA_Channel, &DMA_InitStructure);

}

/*******************************************************************************
* 函数名称: BSP
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能:  setup LEDs, Key Button, LCD and COM port(USART)...  available 
*******************************************************************************/
void BSP(void)
{
    /* System Clocks Configuration */
    RCC_Configuration();
       
    /* NVIC configuration */
    NVIC_Configuration();

    /* Configure the GPIO ports */
    GPIO_Configuration();

    /* Configure the DMA */
    DMA_Configuration();


    /* Flash unlock */
    FLASH_Unlock();

    /* LED setup */
	/* LCD setup */
	/* KEY button setup */
	
	/* COM (USART) setup */
	//UART_setup(UART_COM1, 57600);
    
}


