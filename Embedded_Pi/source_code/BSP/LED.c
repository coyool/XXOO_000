/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：LED
*
* 文件标识：
* 摘    要：
*
* 当前版本：V1.0
* 作    者：F06553
* 完成日期：2014-9-19
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/
const EMBEDDED_PI_PIN_TYPE LED_pinMap[LEDn] = 
{
    D13
};



/*** static variable declarations ***/



/*** extern variable declarations ***/







/*******************************************************************************
* Description : LED_setup
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void LED_setup(LED_TYPEDEF LED)
{
    pinMode(LED_pinMap[LED], OUTPUT);
}

/*******************************************************************************
* Description : Turn an LED on and off   used blocking delay
* Syntax      : LED_Blink(LED1, 10, 100);
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void LED_Blink(LED_TYPEDEF LED, __IO u32 cnt, const u32 interval)
{   
    while (cnt--)
    {
        LED_on(LED1);
        delay_ms(interval);
        LED_off(LED1);
        delay_ms(interval);
    }    
}

/*******************************************************************************
* Description : LED on
* Syntax      : 
* Parameters I: LED number
* Parameters O: 
* return      : 
*******************************************************************************/
void LED_on(LED_TYPEDEF LED)
{
    digitalWrite(LED_pinMap[LED], HIGH);
}

/*******************************************************************************
* Description : LED off
* Syntax      : 
* Parameters I: LED number
* Parameters O: 
* return      : 
*******************************************************************************/
void LED_off(LED_TYPEDEF LED)
{
    digitalWrite(LED_pinMap[LED], LOW);
}



