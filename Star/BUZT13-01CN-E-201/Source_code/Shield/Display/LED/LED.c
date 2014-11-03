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
const int ledPin =  13;         // the number of the LED pin


int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated

long interval = 1000;           // interval at which to blink (milliseconds)


/*** extern variable declarations ***/






/*******************************************************************************
* Description : Blink
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void Blink(__IO u32* ledPin)
{
    digitalWrite(ledPin, HIGH);   
    delay_ms(1000);              
    digitalWrite(ledPin, LOW);    
    delay_ms(1000);               
}

/*******************************************************************************
* Description : BlinkWithoutDelay
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void BlinkWithoutDelay(__IO u32* ledPin)
{
    u32 currentMillis = millis();

    if (currentMillis - previousMillis > interval) 
    {
        // save the last time you blinked the LED 
        previousMillis = currentMillis;   

        // if the LED is off turn it on and vice-versa:
        if (ledState == LOW)
          ledState = HIGH;
        else
          ledState = LOW;

        // set the LED with the ledState of the variable:
        digitalWrite(ledPin, ledState);
    }
}

