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
uint8_t temp = 0;
uint8_t Rev_C = 0;



/*******************************************************************************
* 函数名称: delay 
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
void delay_ms(uint32_t Cnt)
{
    uint32_t i;
    
    for (; Cnt ; Cnt--)
    {    
        for (i=SystemCoreClock/4000; i; i--);
    }    
}

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
    //UART_setup();
    UART_Port_init();                        /* UART IO init */             
    UARTConfigMode(&tUARTModeConfigT);       /* UART setup */  
    
    for (i=0; i<10; i++)
    {
        bFM3_GPIO_PDOR0_PC = 0;     /* LED202 on */   
        bFM3_GPIO_PDOR0_PD = 0;     /* LED201 on */ 
        delay_ms(100);
        bFM3_GPIO_PDOR0_PC = 1;     /* LED202 off */
        bFM3_GPIO_PDOR0_PD = 1;     /* LED201 off */ 
        delay_ms(100);
    }
    
    /* enable peripheral fuction */
//    //MFS_UARTEnableRX(InUseCh);
//    MFS_UARTEnableTX(InUseCh);
//    Put_char('Z');
//    //delay_ms(1000); //1.252S
//    Receive_Byte(&i, Rev_timeout); 
//    Put_char('Z');
//    _NOP();
            
    /* printf versions informations */     
    // ...
}


int main(void)
{
    
    BSP();
    IAP();
    
//    while (1)
//    {
//        temp = Receive_Byte(&Rev_C, Rev_timeout);
//        if (temp == 0)
//        {
//            __NOP();
//        }
//    
//        if (Rev_C == 0x43)
//        {
//            _NOP();
//        } 
//    }  
     
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
