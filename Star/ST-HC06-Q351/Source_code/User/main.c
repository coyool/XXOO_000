/*******************************************************************************
* Copyright (c) 2014, 深圳思达仪表有限公司  研发中心 固件研发部
* All rights reserved.
* 
* 文件名称：RF_APP.c
* 文件标识：
* 
* 当前版本：V1.0
* 作    者：F06553代号  
* 完成日期：2014年 12月 17日
*
* 取代版本：V 1.1 
* 作    者：输入原作者（或修改者）名字
* 完成日期：XXXX年XX月XX日
*
* 源代码说明： 
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
    SVSCTL |= (SVSON | 0x60);       // 0x60 检测电压为2.5V
    // Wait for adequate voltage to run at full speed 
    while ((SVSCTL & SVSOP))
    // dummy loop ;
    // The voltage should now be OK to run the CPU at full speed. Now it should
    // be OK to use the SVS as a reset source. 
     SVSCTL |= PORON;               // 电压低时会产生复位
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
    SCFI0 = FN_3 | FLLD_4;        //单指令 约等于 119 ns (实际略微大于)
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
* 鍑芥暟鍚嶇О: main
* 杈撳叆鍙傛暟: 
* 杈撳嚭鍙傛暟: 
* --杩斿洖鍊? 
* 鍑芥暟鍔熻兘: --
*******************************************************************************/
void main(void)
{ 


}