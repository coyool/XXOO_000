/*******************************************************************************
* Copyright 2015      Jr 
* All right reserved
*
* 文件名称：
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：Jr
* 完成日期：2015-2-2
* 编译环境：D:\software\IAR_for_ARM\arm   IAR for ARM 6.50 
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/







/*******************************************************************************
* Description : ADC_setup
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void ADC1_setup(void)
{
    /* Check the parameters */
//    ASSERT ();
    
    /* Resets ADC1 */
    ADC_DeInit(ADC1);
    
    /* 41 pin(PC1) 配置为模拟输入,输入时不用设置速率 */
    pinMode(A41, INPUT_ANALOG);
    
    ADC_InitTypeDef ADC_InitStructure;

    /*** ADC1 configuration ***/
    /* Enable ADC1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    /* 独立 ADC 模式 */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    /* 禁止扫描模式，扫描模式用于多通道采集 */
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    /* 开启连续转换模式，即不停地进行 ADC 转换 */
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    /* 不使用外部触发转换 */
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    /* 采集数据右对齐  0000 D11 - D0 */
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    /* 要转换的通道数目 1*/
    ADC_InitStructure.ADC_NbrOfChannel = 1;

    ADC_Init(ADC1, &ADC_InitStructure);

    /*配置 ADC 时钟，为 PCLK2 的 8 分频，即 9Hz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    /*配置 ADC1 的通道 11 为 55.5 个采样周期 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);
    //...
    
    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    
    /*复位校准寄存器 */
    ADC_ResetCalibration(ADC1);
    
    /*等待校准寄存器复位完成 */
    while(ADC_GetResetCalibrationStatus(ADC1));

    /* ADC 校准 */
    ADC_StartCalibration(ADC1);
    /* 等待校准完成*/
    while(ADC_GetCalibrationStatus(ADC1));

    /* 使用软件触发 ADC 转换 */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


