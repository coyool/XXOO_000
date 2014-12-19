/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：ISR
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



/*** extern variable declarations ***/




/*******************************************************************************
* Description : Basic Timer Interrupt Service Routine  5ms
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
#pragma vector=BASICTIMER_VECTOR 
__interrupt void BasicTimer_ISR(void)
{ 
    systick_cnt++;
//    RF_timeOverCnt++;
    if (50 == systick_cnt)
    {
        systick_cnt = 0u;   
    }  
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
#pragma vector=UART0TX_VECTOR
__interrupt void Serial_TX_ISR(void)
{	
	UartSend();
	U0IFG &=~UTXIFG0;	
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
#pragma vector=UART0RX_VECTOR
__interrupt void Serial_RX_ISR (void)
{
	UartReceive(U0RXBUF);	
	U0IFG &=~URXIFG0;	
}
