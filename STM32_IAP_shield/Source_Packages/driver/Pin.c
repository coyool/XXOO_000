/*******************************************************************************
* Copyright 2014      ����˼���Ǳ�  �̼���  
* All right reserved
*
* �ļ����ƣ�pin interface
*
* �ļ���ʶ��pin.c
* ժ    Ҫ��pin API   digital I/O  Analog I/O
*
* ��ǰ�汾��V1.0
* ��    �ߣ�F06553
* ������ڣ�2014-8-4
* ���뻷����IAR_for_ARM  6.50
* 
* Դ����˵����
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/



/***  
Digital I/O   
***/

/*******************************************************************************
* ��������: pin_mode   pinMode(pin, mode)
* �������: pin: the number of the pin whose mode you wish to set
*           mode: INPUT, OUTPUT, or INPUT_PULLUP. 
* �������: 
* --����ֵ: 
* ��������: Configures the specified pin to behave either as an input or an output.
*******************************************************************************/
void pin_mode(void)
{
    _NOP();
    
}

/*******************************************************************************
* ��������: pin_write   digitalWrite(pin, value)
* �������: pin:   the pin number   
*           value: HIGH or LOW
* �������: 
* --����ֵ: 
* ��������: Write a HIGH or a LOW value to a digital pin 
*******************************************************************************/
void pin_write(GPIO_TypeDef* Port, u16 Pin, BitAction value)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(Port));
    assert_param(IS_GET_GPIO_PIN(Pin));
    assert_param(IS_GPIO_BIT_ACTION(value)); 
    
    /* Set Value the GPIOx port pin xx */ 
    GPIO_WriteBit(Port, Pin, value);
}

/*******************************************************************************
* ��������: pin_read   digitalRead(pin)
* �������: pin: the number of the digital pin you want to read (int)
* �������: 
* --����ֵ: HIGH or LOW
* ��������: Reads the value from a specified digital pin, either HIGH or LOW.
*******************************************************************************/
//void pin_read(GPIO_TypeDef* Port, uint16_t Pin)
//{
//    /* Reads the seventh pin of the GPIOB and store it in ReadValue 
//    variable */ 
//    u8 ReadValue = 0x00;
//    
//    /* Check the parameters */
//    assert_param(IS_GPIO_ALL_PERIPH(Port));
//    assert_param(IS_GET_GPIO_PIN(Pin)); 
//  
//    ReadValue = GPIO_ReadInputDataBit(Port, Pin);
//}


/***
Analog I/O
***/