/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：BSP
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-9-18
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/
static void NVIC_setup(void);
static void EXTI_setup(void);
static void SysTick_setup(void);

/*** static variable declarations ***/



/*** extern variable declarations ***/
TIMER_TYPE timer;






/*******************************************************************************
* Description : NVIC_setup 配置嵌套向量中断控制器 NVIC
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void NVIC_setup(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 

    /* 抢占式优先级别设置为无抢占优先级 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
    
    /* configuration A7139 Priority */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; /* invalid */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;     
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;      
    NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Description : EXTI_setup
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void EXTI_setup(void)
{
    EXTI_InitTypeDef  EXTI_InitStructure;

    /* configuration A7139 to generate an interrupt on falling edge */  
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;                 
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;   
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;        
    EXTI_Init(&EXTI_InitStructure);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5); 
    
    // PA7
    EXTI_InitStructure.EXTI_Line = EXTI_Line7;                 
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;   
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;        
    EXTI_Init(&EXTI_InitStructure);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7); 
}

/*******************************************************************************
* Description : SysTick
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void SysTick_setup(void)
{
    /* 
    * SystemFrequency / 100     10ms 
    * SystemFrequency / 1000    1ms  
    * SystemFrequency / 100000  10us   
    * SystemFrequency / 1000000 1us   
    */   
    u32 reload = 0u;
    
    reload = SystemCoreClock / 100;
    if (SysTick_Config(reload))  
    {    
        /* Capture error */    
        //printf("reload greater than MAX")
        while(1);
    }   

    SysTick_ENABLLE(DISABLE);
}


/*******************************************************************************
* 函数名称: BSP
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: 
*******************************************************************************/
void BSP(void)
{
/* Initialize LEDs, Key Button, LCD and COM port(USART) available on
   STM3210X-EVAL board ******************************************************/  
    
    /* Deinit */
    /* Deinitialize the RCC registers */ 
    /* 1. 该函数不改动寄存器RCC_CR的HSITRIM[4:0]位。
       2. 该函数不重置寄存器RCC_BDCR和寄存器RCC_CSR。
       3. IO默认高 */
//    RCC_DeInit();

    /* Resets the GPIO peripheral registers to their default reset 
    values */ 
    GPIO_DeInit(GPIOA); 
    GPIO_DeInit(GPIOB); 
    GPIO_DeInit(GPIOC); 
    GPIO_DeInit(GPIOD); 
    /* Resets the Alternate functions registers to their default reset 
    values */ 
    GPIO_AFIODeInit();
    
    /* Enable the GPIOx Clock */
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    
    
    /* Enable AFIO Clock Alternate function */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    /* NVIC configuration */
    NVIC_setup();
    
    /* EXTI configuration */
    EXTI_setup();
    
    /* SysTick configuration 10ms one time */  
    SysTick_setup();  
    
    /* Embedded Pi setup */
    LED_setup(LED1);  /* SPI SCK pin */
    //Serial_begin();
    
    
    /* shield setup */
    u8 temp = 0u;
    temp = RF_A7139_setup();
    if (temp != NORMAL)
    {
        //printf(" RF A7139 setup failed ");
    }  
    
    /* Enable */
    __enable_irq();                  /* EA = 1 */
    SysTick_ENABLLE(ENABLE);         /* systick enable */

    
    /* power on action */
    memset(&timer, 0, sizeof(&timer));
    
    LED_Blink(LED1, 10, 100);
    printf("\r\n ("__DATE__ "-" __TIME__ ") \r\n");
   
}