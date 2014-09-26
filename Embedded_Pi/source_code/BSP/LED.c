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
GPIO_TypeDef* LED_GPIO_PORT[LEDn] = 
{
    LED1_GPIO_PORT
};

const uint16_t LED_GPIO_PIN[LEDn] = 
{
    LED1_PIN
};
//const uint32_t LED_GPIO_CLK[LEDn] = 
//{
//    LED1_GPIO_CLK
//};


/*** static variable declarations ***/



/*** extern variable declarations ***/







/*******************************************************************************
* Description : LED_setup
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void LED_setup(Led_TypeDef LED)
{
    pinMode_ALL(LED_GPIO_PORT[LED], LED_GPIO_PIN[LED], 
            GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
}

/*******************************************************************************
* Description : Turn an LED on and off   used blocking delay
* Syntax      : LED_Blink(LED1, 10, 100);
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void LED_Blink(Led_TypeDef LED, __IO u32 cnt, const u32 interval)
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
void LED_on(Led_TypeDef LED)
{
    digitalWrite_ALL(LED_GPIO_PORT[LED], LED_GPIO_PIN[LED], HIGH);
}

/*******************************************************************************
* Description : LED off
* Syntax      : 
* Parameters I: LED number
* Parameters O: 
* return      : 
*******************************************************************************/
void LED_off(Led_TypeDef LED)
{
    digitalWrite_ALL(LED_GPIO_PORT[LED], LED_GPIO_PIN[LED], LOW);
}



