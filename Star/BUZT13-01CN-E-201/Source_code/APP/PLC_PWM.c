/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：PLC_PWM
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-11-12
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/







/**
 * @brief       PWMA IRQ Handler
 *
 * @param       None
 *
 * @return      None
 *
 * @details     ISR to handle PWMA interrupt 
 */
/*******************************************************************************
* Description : PWMA IRQ Handler
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : ISR to handle PWMA interrupt 
*******************************************************************************/
void PWMA_IRQHandler(void)
{
    u32 PWM_IntFlag;

    /* Handle PWMA Timer function */
    PWM_IntFlag = PWMA->PIIR;

    /* PWMB channel 0 PWM timer interrupt */
    if (PWM_IntFlag & PWM_PIIR_PWMIF0_Msk)
    {
        PWMA->PIIR = PWM_PIIR_PWMIF0_Msk;

    }
}

/*******************************************************************************
* Description : PLC_setup
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void PLC_setup(void)
{
    /* Assume PWM output frequency is 270KHz and duty ratio is 50%, user can calculate PWM settings by follows.
       duty ratio = (CMR+1)/(CNR+1)
       cycle time = CNR+1
       High level = CMR+1
       PWM clock source frequency = HLCK 50MHz
       (CNR+1) = PWM clock source frequency/prescaler/clock source divider/PWM output frequency
               = 50M/1/1/270K = 
       (Note: CNR is 16 bits, so if calculated value is larger than 65536, user should increase prescale value.)
       CNR = 
       duty ratio = 50% ==> (CMR+1)/(CNR+1) = 50% ==> CMR = (CNR+1)*0.5-1 = X*50/100 - 1
       CMR = 
       Prescale value is 1 : prescaler = 1
       Clock divider is PWM_CSR_DIV1 : clock divider = 1
    */


    /* Enable PWM Generator link to Output pin */
    PWM_EnableOutput(PWMA, PWM3_OUTPIN_LINK);

    /* PWM configuration PWM generator and get the nearest frequency in 
    edge aligned auto-reload mode */
    PWM_ConfigOutputChannel(PWMA, PWM_CH3, PWM_Freq, PWM_dutyRatio_50);

    /* Enable Timer period Interrupt */
    PWM_EnablePeriodInt(PWMA, PWM_CH0, PWM_PERIOD_INT_UNDERFLOW); //边沿对齐方式

    /* Enable PWMA NVIC */
    NVIC_EnableIRQ((IRQn_Type)(PWMA_IRQn));

    /* Enable PWM Timer CH3EN = 1 */
    PWM_Start(PWMA, PWM3_BIT3); 
  
}

/*******************************************************************************
* Description : PLC_end
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void PLC_end(void)
{
    /*--------------------------------------------------------------------------------------*/
    /* Stop PWM Timer (Recommended procedure method 2)                                      */
    /* Set PWM Timer counter as 0, When interrupt request happen, disable PWM Timer         */
    /* Set PWM Timer counter as 0 in Call back function                                     */
    /*--------------------------------------------------------------------------------------*/

    /* Disable PWMA NVIC */
    NVIC_DisableIRQ((IRQn_Type)(PWMA_IRQn));

    /* Wait until PWMB channel 0 Timer Stop */
    while(PWMA->PDR0 != 0);

    /* Disable the PWM Timer */
    PWM_Stop(PWMA, PWM3_BIT3);

    /* Disable PWM Generator broken link to Output pin */
    PWM_DisableOutput(PWMA, PWM3_OUTPIN_LINK);  
}
