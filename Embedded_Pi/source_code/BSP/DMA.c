/*******************************************************************************
* Copyright 2015      Jr 
* All right reserved
*
* 文件名称：DMA
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：Jr
* 完成日期：2015-1-30
* 编译环境：D:\software\IAR_for_ARM\arm   IAR for ARM 6.50 
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/






/*******************************************************************************
* Description : DMA1_init
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void DMA1_setup(void)
{
	/* Check the parameters */
//	ASSERT ();
	
    DMA_InitTypeDef DMA_InitStructure;
	
    /*** DMA channel1 configuration ***/
    /* reset DMA */
    DMA_DeInit(DMA1_Channel1);
    /* Enable DMA clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /* ADC地址 */
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address; 
    /*内存地址*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&randomSeedValue; 
    /*外设为数据源*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
    DMA_InitStructure.DMA_BufferSize = 1;
    /*外设地址固定*/
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
    /*内存地址固定*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 
    /* 半字 */
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    /* 循环传输 */
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    /* Enable DMA channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);
}




