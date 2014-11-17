/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：PLC module
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-10-31
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：MCU -- M0516 ZDN 
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/
#define PLLCON_SETTING  FREQ_50MHZ



/*** static variable declarations ***/



/*** extern variable declarations ***/
__IO u32 systick_cnt = 0u;
__IO u32 systick_ms = 0u;

const u32 systick_fixedTime = 1000u;


void SYS_Init(void)
{
//    /* 解锁保护的寄存器 */
//    SYS_UnlockReg();
    
/*----------------------------------------------------------------------------*/
/* Init System Clock                                                          */
/*----------------------------------------------------------------------------*/
    /* 使能 内部高速时钟 */
    CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

    /* 等待时钟稳定 */
    CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

    /* 切换HCLK时钟源 分频1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));
    
    /* 使能 XTL12M */
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);
    
    /* 使能 PLL, 设置PLL频率, HLCK = PLLOUT */
    CLK_SetCoreClock(PLLCON_SETTING);  

    /* 等待时钟稳定 */
    CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk
                       |CLK_CLKSTATUS_PLL_STB_Msk
                       |CLK_CLKSTATUS_OSC22M_STB_Msk);

    /* 切换HCLK和SysTick的时钟源 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_PLL, CLK_CLKDIV_HCLK(1)); // HLCK=PLLOUT,不分频可以不需要这句
    CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLK_S_HCLK_DIV2); // SysTick = HLCK/2
    /* !!! SysTick被设定为来自"CPU", 当开始该时钟时，
       请在SysTick->CTRL中使用SysTick_CTRL_CLKSOURCE_Msk位。*/
    
    /* 使能IP模块的时钟源 APB */
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(PWM23_MODULE);
    CLK_EnableModuleClock(PWM45_MODULE);
    CLK_EnableModuleClock(TMR1_MODULE);

    /* 切换IP模块的时钟源 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_PLL , CLK_CLKDIV_UART(1));
    CLK_SetModuleClock(PWM23_MODULE, CLK_CLKSEL1_PWM23_S_HCLK  , 0);
    CLK_SetModuleClock(PWM45_MODULE, CLK_CLKSEL2_PWM45_S_HCLK  , 0);
    CLK_SetModuleClock(TMR1_MODULE , CLK_CLKSEL1_TMR1_S_HCLK   , 0); 
    
    /* 现在可以安全的关闭没使用的时钟了！*/
    CLK_DisableXtalRC(CLK_PWRCON_OSC22M_EN_Msk | CLK_PWRCON_OSC10K_EN_Msk);  
 
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    SystemCoreClockUpdate();

    /* Reset IP Module */
    SYS_ResetModule(UART0_RST);
//    SYS_ResetModule(PWM03_RST);
//    SYS_ResetModule(PWM47_RST);
    SYS_ResetModule(TMR1_RST);

    /*------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                */
    /*------------------------------------------------------------------------*/

    /* Set P3 multi-function pins for UART0 RXD and TXD */
    SYS->P3_MFP &= ~(SYS_MFP_P30_Msk | SYS_MFP_P31_Msk);
    SYS->P3_MFP |= (SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0);
    /* Set P2 multi-function pins for PWMA Channel0 */
    SYS->P2_MFP = SYS_MFP_P23_PWM3; 
    
//   /* 锁定保护的寄存器 */
//    SYS_LockReg();
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
* --返回值: 
* 函数功能: --
*******************************************************************************/
void main(void)
{ 
    /* power on setup */ 
    setup();
    
    __enable_irq();   //EA = 1
    
    //PLC_Tx_begin(PLC_Tx_AA);
    while (1)
    {
//        BlinkWithoutDelay(36);
        PLC_Tx_begin(PLC_Tx_AA);
        delayMs(500);

    }//end while
}