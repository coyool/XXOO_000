/*!
 ******************************************************************************
 **
 ** \file lcdc_fm3.h
 **
 ** \brief DAC drivers head file
 **
 ** \author FSDC AE Team
 **
 ** \version V0.1
 **
 ** \date 2012-10-12
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(ChengDu) Co.,Ltd.
 **
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */

#ifndef _LCDC_FM3_H_
#define _LCDC_FM3_H_

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_LCDC == OFF)
    #if (PERIPHERAL_ENABLE_LCDC == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_LCDC == ON) && (PERIPHERAL_AVAILABLE_LCDC == ON)

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
/*! \brief LCD bit set */ 
#define    LCD_BIT_CLR      0
/*! \brief LCD bit clear */ 
#define    LCD_BIT_SET      1

/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief LCD Divier Resistor Usage enumeration.
 ******************************************************************************
 */
typedef enum
{
    LCDC_EXT_RES = 0,  //!< Use LCD external divider resistors
    LCDC_INT_RES,      //!< Use LCD internal divider resistors
  
}LCD_DivResModeT;


/*!
 ******************************************************************************
 ** \brief LCD Mode enumeration.
 ******************************************************************************
 */
typedef enum
{
    LCD_STOP = 0,            //!< LCD Display stop
    LCD_4_COM_1DIV2_DUTY,    //!< LCD 4COM, 1/2 duty
    LCD_4_COM_1DIV3_DUTY,    //!< LCD 4COM, 1/3 duty
    LCD_4_COM_1DIV4_DUTY,    //!< LCD 4COM, 1/4 duty
    LCD_8_COM_1DIV8_DUTY,    //!< LCD 4COM, 1/8 duty
  
}LCD_DispModeT;

/*!
 ******************************************************************************
 ** \brief LCD divider register value enumeration
 ******************************************************************************
 */
typedef enum
{
    LCD_DIV_RES_100k = 0,  //!< 100k divider register
    LCD_DIV_RES_10k,       //!< 10k divider register
  
}LCD_DivResValT;

/*!
 ******************************************************************************
 ** \brief LCD 8COM bias enumeration
 ******************************************************************************
 */
typedef enum
{
    LCD_1DIV3_BIAS = 0,  //!< 1/3 bias
    LCD_1DIV4_BIAS,      //!< 1/4 bias
  
}LCD_8COMBiasT;

/*!
 ******************************************************************************
 ** \brief LCD input IO status enumeration
 ******************************************************************************
 */
typedef enum
{
    LCD_INPUT_IO_CUTOFF = 0,   //!< Input IO work as GPIO
    LCD_INPUT_IO_CONNECT,      //!< Input IO work as COM/SEG pin
  
}LCD_InputIOModeT;

/*!
 ******************************************************************************
 ** \brief LCD Pin function
 ******************************************************************************
 */
typedef enum
{
    LCD_OFF = 0,    //!< write 0 to display RAM
    LCD_ON,         //!< write 1 to display RAM
  
}LCD_RAMDataT;
/*!
 ******************************************************************************
 ** \brief LCD blink interval
 ******************************************************************************
 */
typedef enum
{
    LCD_BLINK_HALF_SEC= 0,  //!< 0.5s blink interval
    LCD_BLINK_ONE_SEC,      //!< 1s blink interval
  
}LCD_BlinkIntervalT;

/*!
 ******************************************************************************
 ** \brief LCD blink interval enumeration
 ******************************************************************************
 */
typedef enum
{
    LCD_CLOCK_SEL_SUBCLK= 0, //!< select sub clock as LCD source clock
    LCD_CLOCK_SEL_PCLK,      //!< select peripheral clock as LCD source clock
  
}LCD_SrcClkT;

/*!
 ******************************************************************************
 ** \brief LCD blink function enumeration
 ******************************************************************************
 */
typedef enum
{   
    LCD_BLINK_OFF = 0,      //!< LCD blink on
    LCD_BLINK_ON,           //!< LCD blink off
}LCD_BlinkFuncT;

/*!
 ******************************************************************************
 ** \brief LCD interrrupt enumeration
 ******************************************************************************
 */
typedef enum
{   
    LCD_INT_DetectOFF = 0,      //!< LCD detected interrupt
    LCD_INT_DetectON,           //!< LCD did not detected interrupt
}FlagIntT;

/*!
 ******************************************************************************
 ** \brief LCD blink SEG COM dot enumeration
 ******************************************************************************
 */
typedef enum
{   
    LCD_BLINK_DOT_OFF = 0,      //!< LCD blink on
/***4COM*****************/
    LCDC_Blik4COMS3C3,            //!< lcdc blink for SEG3-COM3 
    LCDC_Blik4COMS3C2,           //!< lcdc blink for SEG3-COM2 
    LCDC_Blik4COMS3C1,            //!< lcdc blink for SEG3-COM1 
    LCDC_Blik4COMS3C0,            //!< lcdc blink for SEG3-COM0 
    LCDC_Blik4COMS2C3,            //!< lcdc blink for SEG2-COM3 
    LCDC_Blik4COMS2C2,            //!< lcdc blink for SEG2-COM2 
    LCDC_Blik4COMS2C1,            //!< lcdc blink for SEG2-COM1 
    LCDC_Blik4COMS2C0,            //!< lcdc blink for SEG2-COM0 
    LCDC_Blik4COMS1C3,            //!< lcdc blink for SEG1-COM3 
    LCDC_Blik4COMS1C2,            //!< lcdc blink for SEG1-COM2 
    LCDC_Blik4COMS1C1,             //!< lcdc blink for SEG1-COM1
    LCDC_Blik4COMS1C0,             //!< lcdc blink for SEG1-COM0
    LCDC_Blik4COMS0C3,             //!< lcdc blink for SEG0-COM3
    LCDC_Blik4COMS0C2,             //!< lcdc blink for SEG0-COM2
    LCDC_Blik4COMS0C1,             //!< lcdc blink for SEG0-COM1
    LCDC_Blik4COMS0C0,             //!< lcdc blink for SEG0-COM0
/***8COM*****************/    
    LCDC_Blik8COMS1C7,            //!< lcdc blink for SEG1-COM7 
    LCDC_Blik8COMS1C6,            //!< lcdc blink for SEG1-COM6 
    LCDC_Blik8COMS1C5,            //!< lcdc blink for SEG1-COM5 
    LCDC_Blik8COMS1C4,            //!< lcdc blink for SEG1-COM4 
    LCDC_Blik8COMS1C3,            //!< lcdc blink for SEG1-COM3 
    LCDC_Blik8COMS1C2,            //!< lcdc blink for SEG1-COM2 
    LCDC_Blik8COMS1C1,            //!< lcdc blink for SEG1-COM1 
    LCDC_Blik8COMS1C0,            //!< lcdc blink for SEG1-COM0 
    LCDC_Blik8COMS0C7,            //!< lcdc blink for SEG0-COM7 
    LCDC_Blik8COMS0C6,            //!< lcdc blink for SEG0-COM6 
    LCDC_Blik8COMS0C5,             //!< lcdc blink for SEG0-COM5
    LCDC_Blik8COMS0C4,             //!< lcdc blink for SEG0-COM4
    LCDC_Blik8COMS0C3,             //!< lcdc blink for SEG0-COM3
    LCDC_Blik8COMS0C2,             //!< lcdc blink for SEG0-COM2
    LCDC_Blik8COMS0C1,             //!< lcdc blink for SEG0-COM1
    LCDC_Blik8COMS0C0,             //!< lcdc blink for SEG0-COM0
}LCD_BlinkDotT;


/*!
 ******************************************************************************
 ** \brief LCD interrupt callback function type difinition
 ******************************************************************************/
typedef void LCD_IntCallbackT(void); 


/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {	
#endif
/* Control */
/* 
timer mode Setting */
void LCD_EnableTimerModeOpt(void);
void LCD_DisableTimerModeOpt(void);
/* 
display mode Setting */
void LCD_SetDispMode(LCD_DispModeT Mode);
void LCD_Sel8COMBias(LCD_8COMBiasT Bias);
/* 
divide resister Setting */
void LCD_ConfigDivResMode(LCD_DivResModeT Mode);
void LCD_SelDivResVal(LCD_DivResValT Val);
/* 
reverse Setting */
void LCD_EnableDispRvs(void);
void LCD_DisableDispRvs(void);
/* 
blank Setting */
void LCD_EnableBlankDisp(void);
void LCD_DisableBlankDisp(void);
/* Input IO mode */
void LCD_ConfigInputIOMode(LCD_InputIOModeT Mode);
/* 
interrupt Setting */
void LCD_LCDCInt(LCD_IntCallbackT* IntCallback );
void LCD_EnableInt(LCD_IntCallbackT* IntCallback);
void LCD_DisableInt(LCD_IntCallbackT* IntCallback);
FlagStatusT LCD_GetIntFlag(void);
/* 
Clock select*/
void LCD_SelSrcClk(LCD_SrcClkT Clk);
void LCD_SetClkDiv(uint32_t Div);
/* 
Pin VVX,COM&SEG Setting */
void LCD_ConfigVVPin(uint8_t VV);
void LCD_ConfigCOMPin(uint8_t Com);
void LCD_ConfigSEGPin(uint32_t Low32Seg, uint8_t High8Seg);
/* 
Blink setting */
void LCD_SetBinkInterval(LCD_BlinkIntervalT Interval);
void LCD_SetBlinkDot(LCD_BlinkDotT Dot, LCD_BlinkFuncT Func);
/* LCD Data */
void LCD_WriteRAMBits(uint8_t RAMIndex, uint8_t Bitsmap, uint8_t BitVal);
void LCD_WriteRAMByte(uint8_t RAMIndex,uint8_t DataByte);
void LCD_WriteRAMBit(uint8_t RAMIndex, uint8_t BitIndex, uint8_t BitVal);
void LCD_FillAllRAMArea(void);
void LCD_ClrAllRAMArea(void);
/* LCD interrupt handler */
void LCD_IntHandler(void);

#ifdef __cplusplus
}
#endif

#endif

#endif /* _LCDC_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */
