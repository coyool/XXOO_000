/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：PLC
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-11-7
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/
const u8 PLC_Tx_buffer[64] =
{
    0
};

const u32 remainder = 0x00000001;

/*** extern variable declarations ***/
volatile u32 TimerIntCnt[4] = {0};







/*******************************************************************************
* Description : PLC Tx ISR
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void TMR1_IRQHandler(void)
{
    u8 temp = 0u;
    
    if(TIMER_GetIntFlag(TIMER1) == 1)
    {
        /* Clear Timer1 time-out interrupt flag */
        TIMER_ClearIntFlag(TIMER1);

        TimerIntCnt[1]++;
        if (0 == (TimerIntCnt & 1))               //%2
        {
            digitalWrite(1,HIGH);                 //P1.0 = 1
            
            temp = data[TimerIntCnt>>3] <<= 1;
            if (1 == (temp & 0x80))
            {
                TIMER_SET_CMP_VALUE(TIMER1, 0);     //285KHz
            }
            else
            {
                TIMER_SET_CMP_VALUE(TIMER1, 0);     //255KHz
            }  
        } 
        else
        {
            GPIO_PIN_ADDR(0, 1) ^= HIGH; 
        }//end if  %2     
        
        if (TimerIntCnt >= (MAX-1))
        {
            TIMER_Close(TIMER1);
        }
    }
}

/*******************************************************************************
* Description : PLC  configuration
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void PLC_setup(void)
{
    /* PLC Tx Pin set */
    pinMode_ALL(P0, 0, OUTPUT);     //SCC
    pinMode_ALL(P0, 1, OUTPUT);     //SCCOUT
    
    /* PLC Tx Timer default setting */
    TIMER_Open(TIMER1, TIMER_CONTINUOUS_MODE, 285000);  //中心频率 270KHz
//    TIMER_SET_PRESCALE_VALUE(TIMER1, 0);
//    TIMER_SET_CMP_VALUE(TIMER1, 0);
    
    /* PLC Tx Timer IE enable */
    TIMER_EnableInt(TIMER1);           

    /* Enable Timer1 NVIC */
    NVIC_EnableIRQ(TMR1_IRQn);                     //中断优先级缺

    /* Clear Timer1 interrupt counts to 0 */
    TimerIntCnt[1] = 0u;

    /* Start Timer1 counting */
//    TIMER_Start(TIMER1);
}

/*******************************************************************************
* Description : PLC_Tx_begin
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void PLC_Tx_begin(u8 *data)
{
    /* Clear Timer1 interrupt counts to 0 */
    TimerIntCnt[1] = 0u;
   
    if ((data[0]&0x80) == 1)
    {
        TIMER_SET_PRESCALE_VALUE(TIMER1, 0);
        TIMER_SET_CMP_VALUE(TIMER1, 0);       //285KHz
    }
    else
    {
        TIMER_SET_PRESCALE_VALUE(TIMER1, 0);
        TIMER_SET_CMP_VALUE(TIMER1, 0);       //255KHz
    }   
   
    digitalWrite(0,HIGH);                    //P0.0 = 1
    digitalWrite(1,HIGH);                    //P0.1 = 1
    
    /* Start Timer1 counting */
    TIMER_Start(TIMER1);
}

/*******************************************************************************
* Description : PLC_Tx_begin
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void PLC_Tx_end(void)
{
    /* Stop Timer1 counting */
    TIMER_Close(TIMER1);
}


