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
static const u32 PLC_FreqHigh = 285000u;
static const u32 PLC_FreqLow = 255000u;

static const u32 PLC_Tx_byteMax = 64u;
static u32 PLC_Tx_byteCnt = 0u;
static u32 PLC_Tx_bitCnt = 0u;
static __IO s32 PLC_baudRateCnt = 0u; 



/*** extern variable declarations ***/
u8 PLC_Tx_PN9[64] =
{
    0xFF,0x83,0xDF,0x17,0x32,0x09,0x4E,0xD1,
    0xE7,0xCD,0x8A,0x91,0xC6,0xD5,0xC4,0xC4,
    0x40,0x21,0x18,0x4E,0x55,0x86,0xF4,0xDC,
    0x8A,0x15,0xA7,0xEC,0x92,0xDF,0x93,0x53,
    0x30,0x18,0xCA,0x34,0xBF,0xA2,0xC7,0x59,
    0x67,0x8F,0xBA,0x0D,0x6D,0xD8,0x2D,0x7D,
    0x54,0x0A,0x57,0x97,0x70,0x39,0xD2,0x7A,
    0xEA,0x24,0x33,0x85,0xED,0x9A,0x1D,0xE0
};

u8 PLC_Tx_AA[64] =
{
    0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
    0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
    0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
    0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
    0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
    0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
    0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
    0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
};





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

/*******************************************************************************
* Description : PWMA IRQ Handler
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : ISR to handle PWMA interrupt 
*******************************************************************************/
void PWMA_IRQHandler(void)
{
    u32 PWM_IntFlag = 0u;
    u8 temp_bit = 0u;

    /* Handle PWMA Timer function */
    PWM_IntFlag = PWMA->PIIR;
    
    /* PWMB channel 3 PWM timer interrupt */
    if (PWM_IntFlag & PWM_PIIR_PWMIF3_Msk)
    {
        PWMA->PIIR = PWM_PIIR_PWMIF3_Msk;
        
        PLC_baudRateCnt--; 
        
        if (PLC_baudRateCnt < 0)
        {
            PLC_Tx_bitCnt++;
            if (PLC_Tx_bitCnt >= 8u)
            {
                PLC_Tx_bitCnt = 0u;
                PLC_Tx_byteCnt++;
                if (PLC_Tx_byteCnt >= PLC_Tx_byteMax)
                {
                    //PLC Tx end
                    noTone(PWMA, PWM_CH3, PWMA_IRQn); //后期改进，内部有死循环
                }
            }
            
//            temp_bit = (PLC_Tx_PN9[PLC_Tx_byteCnt] << PLC_Tx_bitCnt) & 0x80;
            temp_bit = (PLC_Tx_AA[PLC_Tx_byteCnt] << PLC_Tx_bitCnt) & 0x80;
            if (0x80 == temp_bit)
            {
                PWM_SET_CMR(PWMA, PWM_CH3, 43);  
                PWM_SET_CNR(PWMA, PWM_CH3, 86);  //285K HZ
//                PLC_baudRateCnt = 570;
                PLC_baudRateCnt = 10;
            }
            else
            {
                PWM_SET_CMR(PWMA, PWM_CH3, 48);  
                PWM_SET_CNR(PWMA, PWM_CH3, 97);  //255K Hz
//                PLC_baudRateCnt = 510;
                PLC_baudRateCnt = 10;
            } 
        }//end if (PLC_baudRateCnt < 0)
    }
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
    PLC_Tx_bitCnt = 0u;
    PLC_Tx_byteCnt = 0u;
    
    pinMode_ALL(P0, 0, OUTPUT);     //SCC
    digitalWrite(0, HIGH);                    //P0.0 = 1 SCC
    
    if (((*data) & 0x80) == 0x80)
    {
        //285K Hz CNR 86(+1) CMR 43(+1)  prescale=1+1  Drv=1
        tone(PWMA, PWM_CH3, PWMA_IRQn, PLC_FreqHigh); 
//        PLC_baudRateCnt = 570;    //baud rate 500bps
        PLC_baudRateCnt = 10;
    }
    else
    {
        //255K Hz CNR 97(+1) CMR48(+1) prescale=1+1  Drv=1
        tone(PWMA, PWM_CH3, PWMA_IRQn, PLC_FreqLow);  
//        PLC_baudRateCnt = 510;    //baud rate 500bps
        PLC_baudRateCnt = 10;
        
    }      
}
