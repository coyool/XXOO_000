/*******************************************************************************
* Copyright (c) 2014, ����˼���Ǳ����޹�˾  �з����� �̼��з���
* All rights reserved.
* 
* �ļ����ƣ�RF_APP.c
* �ļ���ʶ��
* 
* ��ǰ�汾��V1.0
* ��    �ߣ�F06553����  
* ������ڣ�2014�� 12�� 17��
*
* ȡ���汾��V 1.1 
* ��    �ߣ�����ԭ���ߣ����޸��ߣ�����
* ������ڣ�XXXX��XX��XX��
*
* Դ����˵���� 
*                      
* NOTE:   support DL645 (Q351 Q352)  
*
*******************************************************************************/
#include "all_header_file.h"

/*** static function prototype declarations ***/
#define PLLCON_SETTING  FREQ_50MHZ



/*** static variable declarations ***/



/*** extern variable declarations ***/
__IO u32 systick_cnt = 0u;
__IO u32 systick_ms = 0u;

const u32 systick_fixedTime = 1000u;


void SYS_Init(void)
{
    u32 i; 
//---------------------------------------------
    // Before we go to high speed we need to make sure the supply voltage is adequate. 
    SVSCTL |= (SVSON | 0x60);       // 0x60 ����ѹΪ2.5V
    // Wait for adequate voltage to run at full speed 
    while ((SVSCTL & SVSOP))
    // dummy loop ;
    // The voltage should now be OK to run the CPU at full speed. Now it should
    // be OK to use the SVS as a reset source. 
     SVSCTL |= PORON;               // ��ѹ��ʱ�������λ
//----------------------------------------------
    // Configure load caps 
	FLL_CTL0 |= XCAP14PF;
    // Loop until 32kHz crystal stabilizes
    do
    {
        IFG1 &= ~OFIFG;                 // Clear oscillator fault flag
        for (i = 50000; i>0; i--);        // Delay
    }
    while (IFG1 & OFIFG);               // Test osc fault flag
	
#define MAIN_FREQ  8
#if (8 == MAIN_FREQ)              //main frequency 8 MHz
    SCFI0 = FN_3 | FLLD_4;        //��ָ�� Լ���� 119 ns (ʵ����΢����)
    SCFQCTL = 64 - 1;               
#else
	SCFI0 = 0x00;  
    SCFQCTL |= 32-1;        
#endif    
    FLL_CTL0 |= DCOPLUS;  
    
    for (u32 i=0; i<10000; i++)
    {
      _NOP();
    }  
}

/*******************************************************************************
* Description : systick init  systick clock source is STCLK_S. fixed time = 1ms
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void SysTick_setup(u32 us)
{
    u32 cnt = 0u;
    
    /* check Parameters */
    ASSERT(us < SysTick_LOAD_RELOAD_Msk);

    systick_cnt = 0u;
    cnt = CyclesPerUs * us;
    SysTick_Config(cnt); 
}

/*******************************************************************************
* Description : peripheral setup
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void setup(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, IP clock and multi-function I/O */
    SYS_Init();
    /* Lock protected registers */
    SYS_LockReg();
    
    /* global variable init */
    systick_cnt = 0u;
    
    /* Peripheral and Sheild setup */
    SysTick_setup(systick_fixedTime); 
    Serial_begin();
    LED_init();
    
    /*------------------------------------------------------------------------*/
    /* pwer on action                                                         */
    /*------------------------------------------------------------------------*/
    /* LED */
//    Blink(25, 10);
//    Blink(26, 10);
    Blink(36, 10);
//    u32 i;
//    for (i=0; i<15; i++)
//    {
//        digitalWrite(25, HIGH);
//        digitalWrite(26, HIGH);  
//        delayMs(100);              
//        digitalWrite(25, LOW);
//        digitalWrite(26, LOW);
//        delayMs(100);  
//    }  



    /* printf log */
    printf(" programing information: \r\n");
    printf("\r\n ("__DATE__ "  " __TIME__ ") \r\n");
    printf("version: BUZT13-01CN V1.0 \r\n");
    printf("\r\nCPU @ %d Hz\r\n", SystemCoreClock);
    printf("\r\n");
}



/*******************************************************************************
* 函数名称: main
* 输入参数: 
* 输出参数: 
* --返回�? 
* 函数功能: --
*******************************************************************************/
void main(void)
{ 


}