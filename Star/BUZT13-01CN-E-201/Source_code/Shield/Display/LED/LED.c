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
* 完成日期：2014-11-3
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/
//digitalValue_TYPE ledState = LOW;            // ledState used to set the LED
u32 previousMillis = 0;        // will store last time LED was updated

u32 interval = 1000;           // interval at which to blink (milliseconds)


/*** extern variable declarations ***/





/*******************************************************************************
* Description : LED_init
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void LED_init(void)
{
    pinMode_ALL(P2, 5, OUTPUT);
    pinMode_ALL(P2, 6, OUTPUT);
    pinMode_ALL(P3, 6, OUTPUT);
    digitalWrite(25, LOW);
    digitalWrite(26, LOW);
    digitalWrite(36, LOW);
}


/*******************************************************************************
* Description : Blink
* Syntax      : Blink(13, 10);
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void Blink(u32 Pin, u32 cnt)
{
    for (; cnt>0u; cnt--)
    {
        digitalWrite(Pin, HIGH);   
        delayMs(1000);              
        digitalWrite(Pin, LOW);    
        delayMs(1000);  
    }                 
}

/*******************************************************************************
* Description : BlinkWithoutDelay
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void BlinkWithoutDelay(u32 pin)
{
    u32 currentMillis;
    
    currentMillis = millis();

    if (currentMillis - previousMillis > interval) 
    {
        previousMillis = currentMillis;   

        GPIO_PIN_ADDR(pin/10, pin%10) ^= HIGH;  
    }
}

