/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：Serial    duino SDK for STM32F103RB
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-9-19
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/






/*******************************************************************************
* Description : Sets the data rate in bits per second (baud) for serial data 
*               transmission.
*               {start: 1bit; data: 8bit; stop: 1bit; parity: none}
*               For communicating with the computer, use one of these rates: 
*               300, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 
*               or 115200. 
* Syntax      : Serial.begin(speed)   
* Parameters I: speed: in bits per second (baud) - long
* Parameters O: none
* return      : nothing
*******************************************************************************/
void Serial_bigen()
{
   /* config USART1 clock */  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    sPinTypeUART(serialPort);
    
    
    xUARTConfigSet(serialPort, baud, (xUART_CONFIG_WLEN_8 | xUART_CONFIG_STOP_1 | xUART_CONFIG_PAR_NONE));
    xUARTEnable(serialPort, (xUART_BLOCK_UART | xUART_BLOCK_TX | xUART_BLOCK_RX));
    
    xUARTIntCallbackInit(serialPort, uartCallback);  
    xUARTIntEnable(serialPort, xUART_INT_RX);
    xIntEnable(xSysCtlPeripheralIntNumGet(serialPort));

    transmitting = false;
}

/*******************************************************************************
* Description : Disables serial communication, allowing the RX and TX pins to 
*               be used for general input and output. 
*               To re-enable serial communication, call Serial.begin().
* Syntax      : Seria1.end() Seria.end()
* Parameters I: none
* Parameters O: none
* return      : nothing
*******************************************************************************/
void end(void)
{
    
}




