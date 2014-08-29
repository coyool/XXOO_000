/*******************************************************************************
* Copyright 2014      深圳思达仪表  固件部  
* All right reserved
*
* 文件名称：updata module
*
* 文件标识：main.c
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-8-15
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/





#include    "all_header_file.h"

/*** static function prototype declarations ***/
static void UART_setup(void);
static void BSP(void);

/*** static variable declarations ***/
// UART config 
static MFS_UARTModeConfigT tUARTModeConfigT =
{
    57600,                   /* 4800 or 19200  57600 */
    UART_DATABITS_8,        
    UART_STOPBITS_1,        
    UART_PARITY_NONE,       
    UART_BITORDER_LSB,
    UART_NRZ,               /* level inversion */  
};


/*** extern variable declarations ***/




/*******************************************************************************
* 函数名称: BSP
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
static void BSP(void)
{
    uint8_t i;
    
    BUTTON_KEY_setup();
    LED_setup();
    BUZZ_setup();
    //UART_setup();
    UART_Port_init();                        /* UART IO init */             
    UARTConfigMode(InUseCh,&tUARTModeConfigT);       /* UART setup */  
    UARTConfigMode(UartUSBCh,&tUARTModeConfigT);

   // UARTConfigMode(&tUART300ModeConfigT);  
    
    /* power on LED201 LED202 Blink */
    for (i=0u; i<10u; i++)
    {
        bFM3_GPIO_PDOR0_PC = 0u;     /* LED202 on */   
        bFM3_GPIO_PDOR0_PD = 0u;     /* LED201 on */      
        delay_ms(100u);
        bFM3_GPIO_PDOR0_PC = 1u;     /* LED202 off */
        bFM3_GPIO_PDOR0_PD = 1u;     /* LED201 off */        
        delay_ms(100u);
    } 
    
    /* power on BUZZER buzz */
    oneSound(1,0); 
    delay_ms(1000);
    oneSound(5,100); 
  
#define  TEST_1    
#ifdef   TEST_1    
    MFS_UARTEnableRX(InUseCh);
    MFS_UARTEnableTX(InUseCh);
    Put_char('Z');
    //delay_ms(1000); //1.252S
    Receive_Byte(&i, Rev_timeout); 
    Put_char('Z');
    _NOP();
#endif    
         
    /* enable peripheral fuction */
    //  null
    
    
    /* printf versions informations */     
    // ...
}


/*******************************************************************************
* 函数名称: main
* 输入参数: 
* 输出参数: 
* --返回值: 0
* 函数功能: --
*******************************************************************************/
int main(void)
{
    BSP();
    IAP();    

   return 0u;
}


/*******************************************************************************
* 函数名称: 
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: 
*******************************************************************************/
//int main()
//{
//    while (1)
//    {
//        switch (task_seq)
//        {
//            case 0:
//                task1();   
//                task_seq = 0;
//                break;
//            case 1:
//                task2();
//                task_seq = 1;
//                break;
//            case 2:
//                task3();
//                task_seq = 2;
//                break;
//            case 3:
//                task4();
//                task_seq = 3;
//                break;
//                
//            default:
//                __NOP();
//                break;
//        }//end switch
//    }    
//    
//    
//    return 0u;
//}
