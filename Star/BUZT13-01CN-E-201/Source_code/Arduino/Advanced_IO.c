/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：Advanced I/O
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-11-14
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/






/*******************************************************************************
* Description : Generates a square wave of the specified frequency 
*               (and 50% duty cycle) on a pin. A duration can be specified, 
*               otherwise the wave continues until a call to noTone()
* Syntax      : tone(pin, frequency, duration)   tone(PWMA, PWM_CH3, PWMA_IRQn, 270000) 
* Parameters I: pin -- the pin on which to generate the tone (only support PWM pins)
*               Freq --  the frequency of the tone in hertz  (uint Hz)
*               duration -- the duration of the tone in ms   (unit ms)
* Parameters O: 
* return      : 
*******************************************************************************/
void tone(PWM_T *PWMx, u32 PWM_CHx, IRQn_Type PWMx_IRQn, u32 frequency)
{
    /* Enable PWM Generator link to Output pin */
    PWM_EnableOutput(PWMx, (1u << PWM_CHx));

    /* PWM configuration PWM generator and get the nearest frequency in 
    edge aligned auto-reload mode */
    PWM_ConfigOutputChannel(PWMx, PWM_CHx, frequency, PWM_dutyRatio_50); //default 270000Hz
    
    /* INV */ 
    PWM_ENABLE_OUTPUT_INVERTER(PWMx, 
                               (PWM_PCR_CH0INV_Msk << (PWM_PCR_CH0INV_Pos * (PWM_CHx * 4))));
    
    /* Enable Timer period Interrupt */
    PWM_EnablePeriodInt(PWMx, PWM_CHx, PWM_PERIOD_INT_UNDERFLOW); //边沿对齐方式

    /* Enable PWM NVIC */
    NVIC_EnableIRQ((IRQn_Type)(PWMx_IRQn));

    /* Enable PWM Timer CH3EN = 1 */
    PWM_Start(PWMx, (1u << PWM_CHx)); 
}

/*******************************************************************************
* Description : Stops the generation of a square wave triggered by tone(). 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void noTone(PWM_T *PWMx, u32 PWM_CHx, IRQn_Type PWMx_IRQn)
{
    /*--------------------------------------------------------------------------------------*/
    /* Stop PWM Timer (Recommended procedure method 2)                                      */
    /* Set PWM Timer counter as 0, When interrupt request happen, disable PWM Timer         */
    /* Set PWM Timer counter as 0 in Call back function                                     */
    /*--------------------------------------------------------------------------------------*/

    /* Disable the PWM Timer CRN = 0 */
    PWM_Stop(PWMx, (1u << PWM_CHx));
    
    /* Disable PWMA NVIC */
    NVIC_DisableIRQ((IRQn_Type)(PWMx_IRQn));

    /* Wait until PWMB channel 3 Timer Stop */
//    while(PWMA->PDR3 != 0);  
    while ( *(u32 *)(0x40040014 + (PWM_CHx * 12)) != 0);

//    /* Disable the PWM Timer */
//    PWM_Stop(PWMA, (1u << PWM_CHx));

    /* Disable PWM Generator broken link to Output pin */
    PWM_DisableOutput(PWMA, (1u << PWM_CHx));  
    
    pinMode_ALL(P0, 0, OUTPUT);     //SCC
    digitalWrite(0, LOW);                    //P0.0 = 0 SCC
}
