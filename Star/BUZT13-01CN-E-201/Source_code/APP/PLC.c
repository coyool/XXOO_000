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
* 源代码说明：载波频率范围： 270±15kHz
*             通信系统载波的中心频率： 270kHz
*             调制解调方式： BFSK
*             频带宽度： 30kHz
*             载波通信速率： 100bps～800bps 变速
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/
static s32 Freq_cnt = 0u;    
//static const u32 MAX_285K = 760u;
//static const u32 MAX_255K = 680u;
static const u32 MAX_285K = 10;
static const u32 MAX_255K = 10;

static const u32 PLC_Tx_byte = 64u;
static u32 PLC_Tx_byteCnt = 0u;
static u32 PLC_Tx_bitCnt = 0u;


/*** extern variable declarations ***/
u8 PLC_Tx_PN9[64] =
{
    0xAA,0x83,0xDF,0x17,0x32,0x09,0x4E,0xD1,
    0xE7,0xCD,0x8A,0x91,0xC6,0xD5,0xC4,0xC4,
    0x40,0x21,0x18,0x4E,0x55,0x86,0xF4,0xDC,
    0x8A,0x15,0xA7,0xEC,0x92,0xDF,0x93,0x53,
    0x30,0x18,0xCA,0x34,0xBF,0xA2,0xC7,0x59,
    0x67,0x8F,0xBA,0x0D,0x6D,0xD8,0x2D,0x7D,
    0x54,0x0A,0x57,0x97,0x70,0x39,0xD2,0x7A,
    0xEA,0x24,0x33,0x85,0xED,0x9A,0x1D,0xE0
};

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
    u8 temp_bit = 0u;
    
//    if(TIMER_GetIntFlag(TIMER1) == 1)
    if ((TIMER1->TISR & TIMER_TISR_TIF_Msk) == 1)
    {
        /* Clear Timer1 time-out interrupt flag */
        //TIMER_ClearIntFlag(TIMER1);
        TIMER1->TISR = TIMER_TISR_TIF_Msk;

//        TimerIntCnt[1]++;  
        Freq_cnt--;
//        Freq_cnt ^= 1;
        
        P36 ^= HIGH;                             //P0.1 = 1 SCCOUT
        /* Tx running */
        if (Freq_cnt < 0)
        {
            PLC_Tx_bitCnt++;
            if (PLC_Tx_bitCnt >= 8u)
            {
                PLC_Tx_bitCnt = 0u;
                PLC_Tx_byteCnt++;
            }
            
            temp_bit = (PLC_Tx_PN9[PLC_Tx_byteCnt] << PLC_Tx_bitCnt) & 0x80;
            if (0x80 == temp_bit)
            {
//                TIMER_SET_PRESCALE_VALUE(TIMER1, 0);
                TIMER_SET_CMP_VALUE(TIMER1, 98);       //285KHz(0,98)
                Freq_cnt = MAX_285K;
            }
            else
            {
//                TIMER_SET_PRESCALE_VALUE(TIMER1, 0);
                TIMER_SET_CMP_VALUE(TIMER1, 500);       //255KHz(0,88)
                Freq_cnt = MAX_255K;
            }
        } 
        else
        {
        }
        
        /* check Tx ending */
        if (PLC_Tx_byte  <= PLC_Tx_byteCnt)
        {
            /* Stop Timer1 counting */
            TIMER_Close(TIMER1);
            P36 = 0;               //P0.0 = 0
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
//    pinMode_ALL(P0, 0, OUTPUT);     //SCC
//    pinMode_ALL(P0, 1, OUTPUT);     //SCCOUT
    pinMode_ALL(P3, 6, OUTPUT);     //SCCOUT
    
    /* PLC Tx Timer default setting */
    TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 1);  //中心频率 270KHz
//    TIMER_SET_PRESCALE_VALUE(TIMER1, 0);
//    TIMER_SET_CMP_VALUE(TIMER1, 88);
    
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
    PLC_Tx_bitCnt = 0u;
    PLC_Tx_byteCnt = 0u;
    
    if (((*data) & 0x80) == 0x80)
    {
        TIMER_SET_PRESCALE_VALUE(TIMER1, 0);
        TIMER_SET_CMP_VALUE(TIMER1, 98);       //285KHz(0,98)
        Freq_cnt = MAX_285K;
    }
    else
    {
        TIMER_SET_PRESCALE_VALUE(TIMER1, 0);
        TIMER_SET_CMP_VALUE(TIMER1, 500);       //255KHz(0,88)
        Freq_cnt = MAX_255K;
    }   
   
//    digitalWrite(0, HIGH);                    //P0.0 = 1 SCC
//    digitalWrite(1, HIGH);                    //P0.1 = 1 SCCOUT
    digitalWrite(36, HIGH);                    //P0.1 = 1 SCCOUT
//    Freq_cnt = 0;
    
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


