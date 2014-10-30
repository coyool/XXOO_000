/*!
 ******************************************************************************
 **
 ** \file dac_fm3.c	
 **
 ** \brief dac drivers
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
 
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include"lcdc_fm3.h"

#if (PERIPHERAL_ENABLE_LCDC == ON) && (PERIPHERAL_AVAILABLE_LCDC == ON)
/*---------------------------------------------------------------------------*/
/* Bit definition                                                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/
static LCD_IntCallbackT *LCDIntCallback = NULL;
/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local functions prototypes                                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/

/*!
 ******************************************************************************
 ** \brief Enable LCD in timer mode
 **
 ** \param None
 ** \return None
 **
 ******************************************************************************
 */
void LCD_EnableTimerModeOpt(void)
{
  /* LCDCC1_LCDEN:LCDC timer mode open */
    bFM3_LCDC_LCDCC1_LCDEN = 1;
}

/*!
 ******************************************************************************
 ** \brief Disable LCD in timer mode
 **
 ** \param None
 ** \return None
 **
 ******************************************************************************
 */
void LCD_DisableTimerModeOpt(void)
{
  /* LCDCC1_LCDEN:LCDC timer mode close */
    bFM3_LCDC_LCDCC1_LCDEN = 0;
}
/*!
 ******************************************************************************
 ** \brief Configure LCD power driver mode
 **
 ** \param Mode
 ** \arg LCDC_EXT_RES
 ** \arg LCDC_INT_RES
 **
 ** \return None
 ******************************************************************************
 */
void LCD_ConfigDivResMode(LCD_DivResModeT Mode)
{
    ASSERT((Mode == LCDC_INT_RES)||(Mode == LCDC_EXT_RES));
  
    if(Mode == LCDC_EXT_RES)
        bFM3_LCDC_LCDCC1_VSEL = 0;              //external divider resister  
    else
        bFM3_LCDC_LCDCC1_VSEL = 1;              //internal divider resister     
}

/*!
 ******************************************************************************
 ** \brief set display mode 
 ** \param  Mode, display mode
 ** \arg    LCD_STOP: close display
 ** \arg    LCD_4_COM_1DIV2_DUTY: 4COM 1/2 bias
 ** \arg    LCD_4_COM_1DIV3_DUTY: 4COM 1/3 bias
 ** \arg    LCD_4_COM_1DIV4_DUTY: 4COM 1/4 bias
 ** \arg    LCD_8_COM_1DIV8_DUTY: 8COM 1/8 bias
 ** \return None
 **
 ******************************************************************************
 */
void LCD_SetDispMode(LCD_DispModeT Mode)
{
    ASSERT((Mode == LCD_4_COM_1DIV2_DUTY)||(Mode == LCD_4_COM_1DIV3_DUTY)||(Mode == LCD_4_COM_1DIV4_DUTY)
            ||(Mode == LCD_8_COM_1DIV8_DUTY)||(Mode == LCD_STOP));
    FM3_LCDC->LCDCC1 &= 0xe3;       //close display
    switch(Mode)
    {
        case LCD_STOP:
            break;
        case LCD_4_COM_1DIV2_DUTY:
            FM3_LCDC->LCDCC1 |= 0x04;       //4COM 1/2 bias
            break;
        case LCD_4_COM_1DIV3_DUTY:
            FM3_LCDC->LCDCC1 |= 0x08;       //4COM 1/3 bias 
            break;
        case LCD_4_COM_1DIV4_DUTY:
            FM3_LCDC->LCDCC1 |= 0x0c;       //4COM 1/4 bias 
            break;
        case LCD_8_COM_1DIV8_DUTY:
            FM3_LCDC->LCDCC1 |= 0x10;       //8COM 1/8 bias 
            break;
    }
}

/*!
 ******************************************************************************
 ** \brief select internal divide resistor
 ** \param  Val, resister value
 ** \arg   LCD_DIV_RES_100k: 100K 
 ** \arg   LCD_DIV_RES_10k: 10K
 ** \return None
 **
 ******************************************************************************
 */
void LCD_SelDivResVal(LCD_DivResValT Val)
{
    ASSERT((Val == LCD_DIV_RES_100k)||(Val == LCD_DIV_RES_10k));
    if(Val == LCD_DIV_RES_100k)
        bFM3_LCDC_LCDCC2_RSEL = 0;              //divider resister 100K   
    else
        bFM3_LCDC_LCDCC2_RSEL = 1;              //divider resister 10K     
}

/*!
 ******************************************************************************
 ** \brief Select bias value in the 8COM mode
 ** \param Bias, 
 ** \arg   LCD_1DIV3_BIAS: 1/3 bias 
 ** \arg   LCD_1DIV4_BIAS: 1/4 bias
 ** \return None
 **
 ******************************************************************************
 */
void LCD_Sel8COMBias(LCD_8COMBiasT Bias)
{
    ASSERT((Bias == LCD_1DIV3_BIAS)||(Bias == LCD_1DIV4_BIAS));
  
    if(Bias == LCD_1DIV3_BIAS)
        bFM3_LCDC_LCDCC2_BLS8 = 0;              //8COM 1/3 bias
    else
        bFM3_LCDC_LCDCC2_BLS8 = 1;              //8COM 1/4 bias   
}

/*!
 ******************************************************************************
 ** \brief Enable display reverse
 ** \param  None
 ** \return None
 **
 ******************************************************************************
 */
void LCD_EnableDispRvs(void)
{
    bFM3_LCDC_LCDCC2_INV = 1;              //display is reversed
}

/*!
 ******************************************************************************
 ** \brief Disable display reverse
 ** \param  None
 ** \return None
 **
 ******************************************************************************
 */
void LCD_DisableDispRvs(void)
{
    bFM3_LCDC_LCDCC2_INV = 0;           //close display reverse
}

/*!
 ******************************************************************************
 ** \brief Enable blank display 
 ** \param  None
 ** \return None
 **
 ******************************************************************************
 */
void LCD_EnableBlankDisp(void)
{
    bFM3_LCDC_LCDCC2_BK = 1;            //display blank
}

/*!
 ******************************************************************************
 ** \brief Disable blank display 
 ** \param  None
 ** \return None
 **
 ******************************************************************************
 */
void LCD_DisableBlankDisp(void)
{
    bFM3_LCDC_LCDCC2_BK = 0;            //close blank display 
}

/*!
 ******************************************************************************
 ** \brief Enable LCD interrupt 
 ** \param  IntCallback, interrupt callback function address
 ** \return None
 **
 ******************************************************************************
 */
void LCD_EnableInt(LCD_IntCallbackT* IntCallback)
{
    LCDIntCallback = IntCallback;
    bFM3_LCDC_LCDCC2_LCDIEN = 1;    //enable interrupt
}

/*!
 ******************************************************************************
 ** \brief Disable LCD interrupt 
 ** \param  None
 ** \return None
 **
 ******************************************************************************
 */
void LCD_DisableInt(LCD_IntCallbackT* IntCallback)
{
    LCDIntCallback = IntCallback;
    bFM3_LCDC_LCDCC2_LCDIEN = 0;            //disable interrupt
}

/*!
 ******************************************************************************
 ** \brief Read LCD interrupt flag 
 ** \param  None
 ** \return None
 **
 ******************************************************************************
 */
FlagStatusT LCD_GetIntFlag(void)
{
    return (FlagStatusT)bFM3_LCDC_LCDCC2_LCDIF;
}
/*!
 ******************************************************************************
 ** \brief Select blink interval time
 ** \param Interval
 ** \arg   LCD_BLINK_HALF_SEC: blink interval is 0.5s
 ** \arg   LCD_BLINK_ONE_SEC:  blink interval is 1s
 ** \return None
 **
 ******************************************************************************
 */
void LCD_SetBinkInterval(LCD_BlinkIntervalT Interval)
{
    ASSERT((Interval == LCD_BLINK_HALF_SEC)||(Interval == LCD_BLINK_ONE_SEC));
  
    if(Interval == LCD_BLINK_HALF_SEC)
        bFM3_LCDC_LCDCC3_BLSEL = 0;                 //0.5s interval
    else
        bFM3_LCDC_LCDCC3_BLSEL = 1;                 //1s interval
}

/*!
 ******************************************************************************
 ** \brief Config LCD input IO mode
 ** \param Mode
 ** \arg   LCD_INPUT_IO_CUTOFF: Input IO of LCD is cut-off
 ** \arg   LCD_INPUT_IO_CONNECT:  Input IO of LCD is not cut-off
 ** \return None
 **
 ******************************************************************************
 */
void LCD_ConfigInputIOMode(LCD_InputIOModeT Mode)
{
    ASSERT((Mode == LCD_INPUT_IO_CUTOFF)||(Mode == LCD_INPUT_IO_CONNECT));
    
    if(Mode == LCD_INPUT_IO_CUTOFF)
    {
        bFM3_LCDC_LCDCC3_PICTL = 0;
    }
    else
    {
        bFM3_LCDC_LCDCC3_PICTL = 1;
    }
}

/*!
 ******************************************************************************
 ** \brief Configue VVx pin work status.default value is LCD pin
 **
 ** \param VV VVPin 00000~11111
 ** \arg    1 is LCD ppin, 
 ** \arg    0 is GPIO function 
 **         bit0 -- VV0 
 **         bit1 -- VV1
 **         bit2 -- VV2
 **         bit3 -- VV3
 **         bit4 -- VV4
 ** \return None
 **
 ******************************************************************************
 */
void LCD_ConfigVVPin(uint8_t VV)
{
    ASSERT(VV<=0x1f);
     
    FM3_LCDC->LCDCC3 &= ~(31ul<<1);       //clear VVpin status
    FM3_LCDC->LCDCC3 |= (VV<<1);         //according input value set VVpin function
}

/* Clock */
/*!
 ******************************************************************************
 ** \brief Select LCD clock
 ** \param Clk
 ** \arg   LCD_CLOCK_SEL_SUBCLK
 ** \arg   LCD_CLOCK_SEL_PCLK
 ** \return None
 **
 ******************************************************************************
 */
void LCD_SelSrcClk(LCD_SrcClkT Clk)
{
    ASSERT((Clk == LCD_CLOCK_SEL_SUBCLK)||(Clk == LCD_CLOCK_SEL_PCLK));
  
    if(Clk == LCD_CLOCK_SEL_SUBCLK)
        bFM3_LCDC_LCDC_PSR_CLKSEL = 0;      //LCD clock is sub clock 
    else
        bFM3_LCDC_LCDC_PSR_CLKSEL = 1;      //LCD clock is PCLK
  
}

/*!
 ******************************************************************************
 ** \brief set clock divide rate
 ** \param  Div
 ** \arg   divide rate is from 1 to 0x3fffff(2097153)
 ** \return None
 **
 ******************************************************************************
 */
void LCD_SetClkDiv(uint32_t Div)
{
    ASSERT(Div <= 0x3fffff);
  
    FM3_LCDC->LCDC_PSR &= 0xffc00000;     //clear rate register
    FM3_LCDC->LCDC_PSR += Div;            //set divide rate
}

/* COM&SEG Setting */
/*!
 ******************************************************************************
 ** \brief configure COM pin work status
 **
 ** \param  Com COM pin map
 ** \arg   bitx---COMx
 **        1 is LCD and 0 is GPIO function
 **
 ** \return None
 **
 ******************************************************************************
 */
void LCD_ConfigCOMPin(uint8_t Com)
{
    FM3_LCDC->LCDC_COMEN = Com;         //set COM pin function
}

/*!
 ******************************************************************************
 ** \brief Configure a SEG all pins work status
 ** \param Low32Seg SEG00~SEG31 pin map
 ** \arg   bitx---SEGx
 **        1 is LCD and 0 is GPIO function
 **
 ** \param  High8Seg SEG32~SEG39 map
 ** \arg   bitx---SEGx
 **        1 is LCD and 0 is GPIO function
 **
 ** \return None
 **
 ******************************************************************************
 */
void LCD_ConfigSEGPin(uint32_t Low32Seg, uint8_t High8Seg)
{
    FM3_LCDC->LCDC_SEGEN1 = Low32Seg;           //set low 32 SEG(SEG00~SEG31) function
    FM3_LCDC->LCDC_SEGEN2 = High8Seg;           //set high 8 SEG(SEG32~SEG39) function
}

/* Blink setting */
/*!
 ******************************************************************************
 ** \brief Set blink dot
 ** \param Dot
 ** \arg   dot define 4COMSEG0-COM0 ~ 4COMSEG3-COM3(8COMSEG0-COM0 ~ 8COMSEG1-COM7)
 ** \param Func
 ** \arg   LCD_BLINK_ON: blink enable
 ** \arg   LCD_BLINK_OFF: blink disable
 ** \return None
 ******************************************************************************
 */
void LCD_SetBlinkDot(LCD_BlinkDotT Dot, LCD_BlinkFuncT Func)
{
    ASSERT((Func == LCD_BLINK_OFF)||(Func == LCD_BLINK_ON));
  
    if(Dot == LCD_BLINK_DOT_OFF)
        FM3_LCDC->LCDC_BLINK = 0x0;               //close all blink function

    if((Dot == LCDC_Blik4COMS3C3)||(Dot == LCDC_Blik8COMS1C7))          //blink 4COM SEG3-SOM3 or 8COM SEG1-COM7
        bFM3_LCDC_LCDC_BLINK_BLD15 = Func;
    else if((Dot == LCDC_Blik4COMS3C2)||(Dot == LCDC_Blik8COMS1C6))     //blink 4COM SEG3-SOM2 or 8COM SEG1-COM6
        bFM3_LCDC_LCDC_BLINK_BLD14 = Func;
    else if((Dot == LCDC_Blik4COMS3C1)||(Dot == LCDC_Blik8COMS1C5))     //blink 4COM SEG3-SOM1 or 8COM SEG1-COM5
        bFM3_LCDC_LCDC_BLINK_BLD13 = Func;
    else if((Dot == LCDC_Blik4COMS3C0)||(Dot == LCDC_Blik8COMS1C4))     //blink 4COM SEG3-SOM0 or 8COM SEG1-COM4
        bFM3_LCDC_LCDC_BLINK_BLD12 = Func;
    else if((Dot == LCDC_Blik4COMS2C3)||(Dot == LCDC_Blik8COMS1C3))     //blink 4COM SEG2-SOM3 or 8COM SEG1-COM3
        bFM3_LCDC_LCDC_BLINK_BLD11 = Func;
    else if((Dot == LCDC_Blik4COMS2C2)||(Dot == LCDC_Blik8COMS1C2))     //blink 4COM SEG2-SOM2 or 8COM SEG1-COM2
        bFM3_LCDC_LCDC_BLINK_BLD10 = Func;
    else if((Dot == LCDC_Blik4COMS2C1)||(Dot == LCDC_Blik8COMS1C1))     //blink 4COM SEG2-SOM1 or 8COM SEG1-COM1
        bFM3_LCDC_LCDC_BLINK_BLD09 = Func;
    else if((Dot == LCDC_Blik4COMS2C0)||(Dot == LCDC_Blik8COMS1C0))     //blink 4COM SEG2-SOM0 or 8COM SEG1-COM0
        bFM3_LCDC_LCDC_BLINK_BLD08 = Func;
    else if((Dot == LCDC_Blik4COMS1C3)||(Dot == LCDC_Blik8COMS0C7))     //blink 4COM SEG1-SOM3 or 8COM SEG0-COM7
        bFM3_LCDC_LCDC_BLINK_BLD07 = Func;
    else if((Dot == LCDC_Blik4COMS1C2)||(Dot == LCDC_Blik8COMS0C6))     //blink 4COM SEG1-SOM2 or 8COM SEG0-COM6
        bFM3_LCDC_LCDC_BLINK_BLD06 = Func;
    else if((Dot == LCDC_Blik4COMS1C1)||(Dot == LCDC_Blik8COMS0C5))     //blink 4COM SEG1-SOM1 or 8COM SEG0-COM5
        bFM3_LCDC_LCDC_BLINK_BLD05 = Func;
    else if((Dot == LCDC_Blik4COMS1C0)||(Dot == LCDC_Blik8COMS0C4))     //blink 4COM SEG1-SOM0 or 8COM SEG0-COM4
        bFM3_LCDC_LCDC_BLINK_BLD04 = Func;
    else if((Dot == LCDC_Blik4COMS0C3)||(Dot == LCDC_Blik8COMS0C3))     //blink 4COM SEG0-SOM3 or 8COM SEG0-COM3
        bFM3_LCDC_LCDC_BLINK_BLD03 = Func;
    else if((Dot == LCDC_Blik4COMS0C2)||(Dot == LCDC_Blik8COMS0C2))     //blink 4COM SEG0-SOM2 or 8COM SEG0-COM2
        bFM3_LCDC_LCDC_BLINK_BLD02 = Func;
    else if((Dot == LCDC_Blik4COMS0C1)||(Dot == LCDC_Blik8COMS0C1))     //blink 4COM SEG0-SOM1 or 8COM SEG0-COM1
        bFM3_LCDC_LCDC_BLINK_BLD01 = Func;
    else if((Dot == LCDC_Blik4COMS0C0)||(Dot == LCDC_Blik8COMS0C0))     //blink 4COM SEG0-SOM0 or 8COM SEG0-COM0
        bFM3_LCDC_LCDC_BLINK_BLD00 = Func;

}
/* LCD Data */
/*!
 ******************************************************************************
 ** \brief Write the bits of LCD RAM
 **
 ** \param  RAMIndex RAM index (0~39)
 ** \param  Bitsmap which bits are operated
 ** \param  BitVal bit value (0,1)
 **
 ** \return None
 **
 ******************************************************************************
 */
void LCD_WriteRAMBits(uint8_t RAMIndex, uint8_t Bitsmap, uint8_t BitVal)
{    
    if(BitVal == LCD_BIT_CLR)
    {
        *((volatile uint8_t*)(&FM3_LCDC->LCDRAM00 + RAMIndex)) &= ~Bitsmap;
    }
    else
    {
        *((volatile uint8_t*)(&FM3_LCDC->LCDRAM00 + RAMIndex)) |= Bitsmap;
    }   
}
/*!
 ******************************************************************************
 ** \brief Write the byte of LCD RAM
 **
 ** \param  RAMIndex RAM index (0~39)
 ** \param  DataByte Byte value
 **
 ** \return None
 **
 ******************************************************************************
 */
void LCD_WriteRAMByte(uint8_t RAMIndex,uint8_t DataByte)
{
  
    *((volatile uint8_t*)(&FM3_LCDC->LCDRAM00 + RAMIndex)) = DataByte;
              
}

/*!
 ******************************************************************************
 ** \brief Write the bit of LCD RAM
 **
 ** \param  RAMIndex RAM index (0~39)
 ** \param  DataByte Byte value
 **
 ** \return None
 **
 ******************************************************************************
 */
void LCD_WriteRAMBit(uint8_t RAMIndex, uint8_t BitIndex, uint8_t BitVal) 
{
    if(BitVal == LCD_BIT_CLR)
    {
        *((volatile uint8_t*)(&FM3_LCDC->LCDRAM00 + RAMIndex)) &= ~(1ul<<BitIndex);
    }
    else
    {
        *((volatile uint8_t*)(&FM3_LCDC->LCDRAM00 + RAMIndex)) |= (1ul<<BitIndex);
    }
}

/*!
 ******************************************************************************
 ** \brief Fill all RAM area of LCD with 1
 **
 ** \return None
 **
 ******************************************************************************
 */
void LCD_FillAllRAMArea(void)
{
    uint8_t i;
    for(i=0;i<40;i++)
    {
        *((volatile uint8_t*)(&FM3_LCDC->LCDRAM00 + i)) = 0xFF;
    }
}

/*!
 ******************************************************************************
 ** \brief Clear all RAM area of LCD
 **
 ** \return None
 **
 ******************************************************************************
 */
void LCD_ClrAllRAMArea(void)
{
    uint8_t i;
    for(i=0;i<40;i++)
    {
        *((volatile uint8_t*)(&FM3_LCDC->LCDRAM00 + i)) = 0x00;
    }
}

/*!
 ******************************************************************************
 ** \brief LCD IRQ function
 ** \param  None
 ** \return None
 **
 ******************************************************************************
 */
void LCD_IntHandler(void)
{
    if(bFM3_LCDC_LCDCC2_LCDIF)
    {
        bFM3_LCDC_LCDCC2_LCDIF = 0;   //clear flag
        if(LCDIntCallback != NULL)
        {
            LCDIntCallback();
        }
    }
}
/*****************************************************************************/

#endif

/* END OF FILE */
