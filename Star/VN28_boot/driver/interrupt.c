/*!
 ******************************************************************************
 **
 ** \file interrupt.c
 **
 ** \brief This file includes all IRQ handlers
 **
 ** \author FSLA AE Team 
 **
 ** \version V0.10
 **
 ** \date 2012-11-20
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************/

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "interrupt.h"

#if (PERIPHERAL_ENABLE_EXTINT == ON) || (PERIPHERAL_ENABLE_WD == ON)

/*!
 ******************************************************************************
 ** \brief NMI & HWG handler
 ******************************************************************************
 */
void NMI_Handler(void)
{
    uint32_t irq;
    irq = FM3_INTREQ->EXC02MON;
#if (PERIPHERAL_ENABLE_EXTINT == ON)    
    /* NMI int handler */
    if(( irq & 0x00000001) == 0x00000001) 
    {
        NMI_IntHandler();
    }
#endif
#if(PERIPHERAL_ENABLE_WD == ON)    
    /* Hardware watchdog handler */
    if((irq & 0x00000002) == 0x00000002) 
    {
        HWD_IntHandler();
    }
#endif    
}
#endif

#if (PERIPHERAL_ENABLE_CSV == ON) 
/*!
 ******************************************************************************
 ** \brief CSV IRQ handler
 ******************************************************************************
 */
void CSV_IRQHandler(void)
{
    uint32_t irq;
    irq = FM3_INTREQ->IRQ00MON;
    if((irq & 0x00000001) == 0x00000001)
    {
        CSV_IntHandler();
    }
}
#endif

#if (PERIPHERAL_ENABLE_WD == ON) 
/*!
 ******************************************************************************
 ** \brief Software watchdog IRQ handler
 ******************************************************************************
 */
void SWDT_IRQHandler(void)
{
    uint32_t irq;
    irq = FM3_INTREQ->IRQ01MON;
    if((irq & 0x00000001) == 0x00000001)
    {
        SWD_IntHandler();
    }
}
#endif
#if (PERIPHERAL_ENABLE_LVD == ON) 
/*!
 ******************************************************************************
 ** \brief LVD IRQ handler
 ******************************************************************************
 */
void LVD_IRQHandler(void)
{
    uint32_t irq;
    irq = FM3_INTREQ->IRQ02MON;
    if((irq & 0x00000001) == 0x00000001)
    {
        LVD_IntHandler();
    }
}
#endif
#if (PERIPHERAL_ENABLE_MFT_WFG == ON) 
/*!
 ******************************************************************************
 ** \brief MFT's waveform generator IRQ handler
 ******************************************************************************
 */
void MFT_WG_IRQHandler(void)
{
    uint32_t irq;
    uint8_t j;   
    irq = FM3_INTREQ->IRQ03MON;
    for(j=0; j<INT_MFT_AMOUNT; j++)
    {
        MFT_WFG_IntHandler(j, irq);
    }
}
#endif

#if (PERIPHERAL_ENABLE_EXTINT == ON) 
/*!
 ******************************************************************************
 ** \brief External interrupt IRQ handler
 ******************************************************************************
 */
void INT0_7_IRQHandler(void)
{
    uint8_t i;
    uint32_t irq;
    irq = FM3_INTREQ->IRQ04MON;
    for(i=0; i<8; i++)
    {
        if((irq & (1ul<<i)) == (1ul<<i))
        {
            ExtInt_IntHandler(i);
        }
    }
}

/*!
 ******************************************************************************
 ** \brief External interrupt IRQ handler
 ******************************************************************************
 */
void INT8_31_IRQHandler(void)
{
    uint8_t i;
    uint32_t irq;
    irq = FM3_INTREQ->IRQ05MON;
    for(i=0; i<24; i++)
    {
        if((irq & (1ul<<i)) == (1ul<<i))
        {
            ExtInt_IntHandler(i+8);
        }
    }
}
#endif    

#if (PERIPHERAL_ENABLE_DT == ON)  || (PERIPHERAL_ENABLE_QPRC == ON) 
/*!
 ******************************************************************************
 ** \brief Dual timer IRQ handler
 ******************************************************************************
 */
void DT_QPRC_IRQHandler(void)
{
    uint8_t i;
#if (PERIPHERAL_ENABLE_QPRC == ON)       
    uint32_t irq;
    irq = FM3_INTREQ->IRQ06MON;
#endif    
#if (PERIPHERAL_ENABLE_DT == ON)    
    for(i=0; i<2; i++)
    {
        DT_IntHandler(i+1);
    }
#endif
#if (PERIPHERAL_ENABLE_QPRC == ON)     
    for(i=0; i<3; i++)
    {
        QPRC_IntHandler(i, irq);
    }
#endif    
}
#endif

#if (PERIPHERAL_ENABLE_MFS_UART == ON) || \
    (PERIPHERAL_ENABLE_MFS_CSIO == ON) || \
    (PERIPHERAL_ENABLE_MFS_I2C == ON) || \
    (PERIPHERAL_ENABLE_MFS_LIN == ON)  
/*!
 ******************************************************************************
 ** \brief MFS ch.0 and ch.8 RX IRQ handler
 ******************************************************************************
 */
void MFS0_8_RX_IRQHandler(void)
{
    uint32_t irq;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ07MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ06MON;
#endif    
    if((irq & 0x00000001) == 0x00000001)
    {
        MFS_RX_IntHandler(0);            
    }
    if((irq & 0x00000002) == 0x00000002)
    {
        MFS_RX_IntHandler(8); 
    } 
}

/*!
 ******************************************************************************
 ** \brief MFS ch.0 and ch.8 TX IRQ handler
 ******************************************************************************
 */
void MFS0_8_TX_IRQHandler(void)
{
    uint32_t irq,inttype;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ08MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ07MON;
#endif  
    if((irq & 0x03) != 0)
    {
        inttype = irq & 0x03;
        MFS_TX_IntHandler(0, inttype);            
    }
    if((irq & 0x0C) != 0)
    {
        inttype = (irq & 0x0C)>>2;
        MFS_TX_IntHandler(8, inttype);    
    }  
    
}

/*!
 ******************************************************************************
 ** \brief MFS ch.1 and ch.9 RX IRQ handler
 ******************************************************************************
 */
void MFS1_9_RX_IRQHandler(void)
{
    uint32_t irq;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ09MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ08MON;
#endif    
    if((irq & 0x00000001) == 0x00000001)
    {
        MFS_RX_IntHandler(1);            
    }
    if((irq & 0x00000002) == 0x00000002)
    {
        MFS_RX_IntHandler(9); 
    } 
}

/*!
 ******************************************************************************
 ** \brief MFS ch.1 and ch.9 TX IRQ handler
 ******************************************************************************
 */
void MFS1_9_TX_IRQHandler(void)
{
    uint32_t irq, inttype;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ10MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ09MON;
#endif  
    if((irq & 0x03) != 0)
    {
        inttype = irq & 0x03;
        MFS_TX_IntHandler(1, inttype);            
    }
    if((irq & 0x0C) != 0)
    {
        inttype = (irq & 0x0C)>>2;
        MFS_TX_IntHandler(9, inttype);    
    }  
}

/*!
 ******************************************************************************
 ** \brief MFS ch.2 and ch.10 RX IRQ handler
 ******************************************************************************
 */
void MFS2_10_RX_IRQHandler(void)
{
    uint32_t irq;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ11MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ10MON;
#endif    
    if((irq & 0x00000001) == 0x00000001)
    {
        MFS_RX_IntHandler(2);            
    }
    if((irq & 0x00000002) == 0x00000002)
    {
        MFS_RX_IntHandler(10); 
    } 
}

/*!
 ******************************************************************************
 ** \brief MFS ch.2and ch.10 TX IRQ handler
 ******************************************************************************
 */
void MFS2_10_TX_IRQHandler(void)
{
    uint32_t irq,inttype;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ12MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ11MON;
#endif  
    if((irq & 0x03) != 0)
    {
        inttype = irq & 0x03;
        MFS_TX_IntHandler(2, inttype);            
    }
    if((irq & 0x0C) != 0)
    {
        inttype = (irq & 0x0C)>>2;
        MFS_TX_IntHandler(10, inttype);    
    }  
}

/*!
 ******************************************************************************
 ** \brief MFS ch.3 and ch.11 RX IRQ handler
 ******************************************************************************
 */
void MFS3_11_RX_IRQHandler(void)
{
    uint32_t irq;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ13MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ12MON;
#endif    
    if((irq & 0x00000001) == 0x00000001)
    {
        MFS_RX_IntHandler(3);            
    }
    if((irq & 0x00000002) == 0x00000002)
    {
        MFS_RX_IntHandler(11); 
    } 
}

/*!
 ******************************************************************************
 ** \brief MFS ch.3 and ch.11 TX IRQ handler
 ******************************************************************************
 */
void MFS3_11_TX_IRQHandler(void)
{
    uint32_t irq,inttype;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ14MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ13MON;
#endif  
    if((irq & 0x03) != 0)
    {
        inttype = irq & 0x03;
        MFS_TX_IntHandler(3, inttype);            
    }
    if((irq & 0x0C) != 0)
    {
        inttype = (irq & 0x0C)>>2;
        MFS_TX_IntHandler(11, inttype);    
    }  
}   

/*!
 ******************************************************************************
 ** \brief MFS ch.4 and ch.12 RX IRQ handler
 ******************************************************************************
 */
void MFS4_12_RX_IRQHandler(void)
{
    uint32_t irq;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ15MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ14MON;
#endif    
    if((irq & 0x00000001) == 0x00000001)
    {
        MFS_RX_IntHandler(4);            
    }
    if((irq & 0x00000002) == 0x00000002)
    {
        MFS_RX_IntHandler(12); 
    }
}

/*!
 ******************************************************************************
 ** \brief MFS ch.4 and ch.12 TX IRQ handler
 ******************************************************************************
 */
void MFS4_12_TX_IRQHandler(void)
{
    uint32_t irq,inttype;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ16MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ15MON;
#endif  
    if((irq & 0x03) != 0)
    {
        inttype = irq & 0x03;
        MFS_TX_IntHandler(4, inttype);            
    }
    if((irq & 0x0C) != 0)
    {
        inttype = (irq & 0x0C)>>2;
        MFS_TX_IntHandler(12, inttype);    
    }  
}  

/*!
 ******************************************************************************
 ** \brief MFS ch.5 and ch.13 RX IRQ handler
 ******************************************************************************
 */
void MFS5_13_RX_IRQHandler(void)
{
    uint32_t irq;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ17MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ16MON;
#endif    
    if((irq & 0x00000001) == 0x00000001)
    {
        MFS_RX_IntHandler(5);            
    }
    if((irq & 0x00000002) == 0x00000002)
    {
        MFS_RX_IntHandler(13); 
    }
}

/*!
 ******************************************************************************
 ** \brief MFS ch.5 and ch.13 TX IRQ handler
 ******************************************************************************
 */
void MFS5_13_TX_IRQHandler(void)
{
    uint32_t irq,inttype;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ18MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ17MON;
#endif  
    if((irq & 0x03) != 0)
    {
        inttype = irq & 0x03;
        MFS_TX_IntHandler(5, inttype);            
    }
    if((irq & 0x0C) != 0)
    {
        inttype = (irq & 0x0C)>>2;
        MFS_TX_IntHandler(13, inttype);    
    }  
}

/*!
 ******************************************************************************
 ** \brief MFS ch.6 and ch.14 RX IRQ handler
 ******************************************************************************
 */
void MFS6_14_RX_IRQHandler(void)
{
    uint32_t irq;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ19MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ18MON;
#endif    
    if((irq & 0x00000001) == 0x00000001)
    {
        MFS_RX_IntHandler(6);            
    }
    if((irq & 0x00000002) == 0x00000002)
    {
        MFS_RX_IntHandler(14); 
    } 
}

/*!
 ******************************************************************************
 ** \brief MFS ch.6 and ch.14 TX IRQ handler
 ******************************************************************************
 */
void MFS6_14_TX_IRQHandler(void)
{
    uint32_t irq,inttype;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ20MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ19MON;
#endif  
    if((irq & 0x03) != 0)
    {
        inttype = irq & 0x03;
        MFS_TX_IntHandler(6, inttype);            
    }
    if((irq & 0x0C) != 0)
    {
        inttype = (irq & 0x0C)>>2;
        MFS_TX_IntHandler(14, inttype);    
    }  
}

/*!
 ******************************************************************************
 ** \brief MFS ch.7 and ch.15 RX IRQ handler
 ******************************************************************************
 */
void MFS7_15_RX_IRQHandler(void)
{
    uint32_t irq;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ21MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ20MON;
#endif    
    if((irq & 0x00000001) == 0x00000001)
    {
        MFS_RX_IntHandler(7);            
    }
    if((irq & 0x00000002) == 0x00000002)
    {
        MFS_RX_IntHandler(15); 
    } 
}

/*!
 ******************************************************************************
 ** \brief MFS ch.7 and ch.15 TX IRQ handler
 ******************************************************************************
 */
void MFS7_15_TX_IRQHandler(void)
{
    uint32_t irq,inttype;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ22MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ21MON;
#endif  
    if((irq & 0x03) != 0)
    {
        inttype = irq & 0x03;
        MFS_TX_IntHandler(7, inttype);            
    }
    if((irq & 0x0C) != 0)
    {
        inttype = (irq & 0x0C)>>2;
        MFS_TX_IntHandler(15, inttype);    
    }  
}
#endif

#if (PERIPHERAL_ENABLE_PPG == ON)
/*!
 ******************************************************************************
 ** \brief PPG Ch.0/2/4/6/8/10/12/16/18/20 IRQ handler
 ******************************************************************************
 */
void PPG_IRQHandler(void)
{
    uint32_t irq;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ23MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ22MON;
#endif
    if((irq & 0x1FF) != 0)
    {
        while(1);
    }
}
#endif

#if (PERIPHERAL_ENABLE_RTC == ON) || (PERIPHERAL_ENABLE_WC == ON)
/*!
 ******************************************************************************
 ** \brief Main osc, Sub osc, PLL, RTC IRQ handler
 ******************************************************************************
 */
void TIM_IRQHandler(void)
{
    uint32_t irq;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ24MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ23MON;
#endif
#if (PERIPHERAL_ENABLE_WC == ON) && (PERIPHERAL_AVAILABLE_WC == ON)   
    if((irq & 0x00000010) == 0x00000010)
    {
        WC_IntHandler();
    }
#endif
#if (PERIPHERAL_ENABLE_RTC == ON) && (PERIPHERAL_AVAILABLE_RTC == ON)      
    if((irq & 0x00000020) == 0x00000020)
    {
        RTC_IntHandler();
    }
#endif    
}
#endif

#if (PERIPHERAL_ENABLE_ADC == ON)
/*!
 ******************************************************************************
 ** \brief ADC0 IRQ handler
 ******************************************************************************
 */
void ADC0_IRQHandler(void)
{
    uint32_t irq;
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ25MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ24MON;
#endif
    
    ADC_IntHandler(0, irq);
}

/*!
 ******************************************************************************
 ** \brief ADC1 IRQ handler
 ******************************************************************************
 */
void ADC1_IRQHandler(void)
{
    uint32_t irq;
    irq = FM3_INTREQ->IRQ26MON; 
    
    ADC_IntHandler(1, irq);

}

#endif

#if (PERIPHERAL_ENABLE_ADC == ON) || (PERIPHERAL_ENABLE_LCD == ON)
/*!
 ******************************************************************************
 ** \brief ADC2,LCD IRQ handler
 ******************************************************************************
 */
void ADC2_LCD_IRQHandler(void)
{
#if (MCU_INT_TYPE == INT_TYPE_A)
    uint32_t irq;
    irq = FM3_INTREQ->IRQ27MON; 
    #if (PERIPHERAL_ENABLE_ADC == ON)
        ADC_IntHandler(2, irq);
    #endif
    #if (PERIPHERAL_ENABLE_LCD == ON)    
        if((irq & (1ul<<5)) == (1ul<<5))
        {
            LCD_IntHandler();
        }
    #endif    
#elif(MCU_INT_TYPE == INT_TYPE_C)  
    #if (PERIPHERAL_ENABLE_LCD == ON)  
        uint32_t irq;  
        irq = FM3_INTREQ->IRQ29MON;
        if((irq & (1ul<<4)) == (1ul<<4))
        {
            LCD_IntHandler();
        }
    #endif   
#endif        
}
#endif

#if (PERIPHERAL_ENABLE_MFT_FRT == ON)
/*!
 ******************************************************************************
 ** \brief MFT's free run timer handler
 ******************************************************************************
 */
void MFT_FRT_IRQHandler(void)
{
    uint8_t j;
    uint32_t irq;
    
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ28MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ25MON;
#endif
    
    for(j=0; j<INT_MFT_AMOUNT; j++)
    {
        MFT_FRT_IntHandler(j, irq);
    }
}
#endif

#if (PERIPHERAL_ENABLE_MFT_ICU == ON)
/*!
 ******************************************************************************
 ** \brief MFT's input capture unit handler
 ******************************************************************************
 */
void MFT_IPC_IRQHandler(void)
{
    uint8_t j;
    uint16_t irq,temp;
    
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = (uint16_t)FM3_INTREQ->IRQ29MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = (uint16_t)FM3_INTREQ->IRQ26MON;
#endif
    temp = 0x000F;
    for(j=0; j<INT_MFT_AMOUNT; j++)
    {
        if ((irq & temp) != 0)
        {
            MFT_ICU_IntHandler(j, irq);    
        }
        temp = temp<<4;
    }
}
#endif

#if (PERIPHERAL_ENABLE_MFT_OCU == ON)
/*!
 ******************************************************************************
 ** \brief MFT's output compare unit IRQ handler
 ******************************************************************************
 */
void MFT_OPC_IRQHandler(void)
{
    uint8_t j;
    uint32_t irq;
    
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ30MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ27MON;
#endif
    
    for(j=0; j<INT_MFT_AMOUNT; j++)
    {
        MFT_OCU_IntHandler(j, irq);
    }
}
#endif

#if (PERIPHERAL_ENABLE_BT == ON)
/*!
 ******************************************************************************
 ** \brief Base timer 0~7 IRQ handler
 ******************************************************************************
 */
void BT0_7_IRQHandler(void)
{
    uint8_t i;
    uint32_t irq, tempirq;
    
#if (MCU_INT_TYPE == INT_TYPE_A)
    irq = FM3_INTREQ->IRQ31MON; 
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ28MON;
#endif
    
    for(i=0; i<8; i++)
    {
        tempirq = irq>>(2*i);
        if((tempirq & 0x01) == 0x01)
        {
            BT_IntHandler(i, 0); /* IRQ 0 */
        }
        
        if((tempirq & 0x02) == 0x02)
        {
            BT_IntHandler(i, 1); /* IRQ 1 */
        }
    }
    
}
#endif

#if (PERIPHERAL_ENABLE_ETHER == ON) || (PERIPHERAL_ENABLE_CAN == ON)
/*!
 ******************************************************************************
 ** \brief Ethernet0, CAN0 IRQ handler
 ******************************************************************************
 */
void ETHER0_CAN0_IRQHandler(void)
{    
    uint32_t irq;
    irq = FM3_INTREQ->IRQ32MON;
#if (PERIPHERAL_ENABLE_CAN == ON) && (PERIPHERAL_AVAILABLE_CAN == ON)    
    if((irq & 0x01) == 0x01)
    {
        CAN_IntHandler(0);
    }
#endif
#if (PERIPHERAL_ENABLE_ETHER == ON) && (PERIPHERAL_AVAILABLE_ETHER == ON)  
    if((irq & 0xE0) != 0)
    {
        while(1);
    }
#endif    
}

/*!
 ******************************************************************************
 ** \brief Ethernet1, CAN1 IRQ handler
 ******************************************************************************
 */
void ETHER1_CAN1_IRQHandler(void)
{
    uint32_t irq;
    irq = FM3_INTREQ->IRQ33MON;
#if (PERIPHERAL_ENABLE_CAN == ON) && (PERIPHERAL_AVAILABLE_CAN == ON)        
    if((irq & 0x01) == 0x01)
    {
        CAN_IntHandler(1);
    }
#endif
#if (PERIPHERAL_ENABLE_ETHER == ON) && (PERIPHERAL_AVAILABLE_CAN == ON) 
    if((irq & 0x06) != 0)
    {
        while(1);
    }
#endif
}
#endif

#if (PERIPHERAL_ENABLE_USB == ON)
/*!
 ******************************************************************************
 ** \brief USB0 function IRQ handler
 ******************************************************************************
 */
void USB0F_IRQHandler(void)
{
    uint32_t irq;
    irq = FM3_INTREQ->IRQ34MON;
       
    if((irq & 0x1F) != 0)
    {
        while(1);
    }
}

/*!
 ******************************************************************************
 ** \brief USB0 function/host IRQ handler
 ******************************************************************************
 */
void USB0_IRQHandler(void)
{
    uint32_t irq;
    irq = FM3_INTREQ->IRQ35MON;
    if((irq & 0x3F) != 0)
    {
        while(1);
    }
}
#endif

#if (PERIPHERAL_ENABLE_USB == ON) || (PERIPHERAL_ENABLE_RC == ON)
/*!
 ******************************************************************************
 ** \brief USB1 function, HDMICEC0 IRQ handler
 ******************************************************************************
 */
void USB1F_HDMICEC0_IRQHandler(void)
{
    uint32_t irq;
    
#if (MCU_INT_TYPE == INT_TYPE_A)  
    irq = FM3_INTREQ->IRQ36MON;
    if((irq & 0x1F) != 0)
    {
        while(1);
    }
    if((irq & (1ul<<5)) == (1ul<<5))
    {
    #if (PERIPHERAL_ENABLE_RC == ON) && (PERIPHERAL_AVAILABLE_RC == ON)   
        HDMICEC_IntHandler(0);
    #endif    
    }
#elif(MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ30MON;
    if((irq & (1ul<<5)) == (1ul<<5))
    {
    #if (PERIPHERAL_ENABLE_RC == ON) && (PERIPHERAL_AVAILABLE_RC == ON)  
        HDMICEC_IntHandler(0);
    #endif    
    }
#endif    
}

/*!
 ******************************************************************************
 ** \brief USB1 function/host, HDMICEC1 IRQ handler
 ******************************************************************************
 */
void USB1_HDMICEC1_IRQHandler(void)
{
    uint32_t irq;
#if (MCU_INT_TYPE == INT_TYPE_A)     
    irq = FM3_INTREQ->IRQ37MON;
    if((irq & 0x1F) != 0)
    {
        while(1);
    }
    if((irq & (1ul<<6)) == (1ul<<6))
    {
    #if (PERIPHERAL_ENABLE_RC == ON) && (PERIPHERAL_AVAILABLE_RC == ON)   
        HDMICEC_IntHandler(1);
    #endif    
    }
    
#elif (MCU_INT_TYPE == INT_TYPE_C)
    irq = FM3_INTREQ->IRQ31MON;
    if((irq & (1ul<<6)) == (1ul<<6))
    {
    #if (PERIPHERAL_ENABLE_RC == ON) && (PERIPHERAL_AVAILABLE_RC == ON)         
        HDMICEC_IntHandler(1);
    #endif    
    }
#endif    
        
}
#endif

#if (PERIPHERAL_ENABLE_DMAC == ON)
/*!
 ******************************************************************************
 ** \brief DMAC ch.0 IRQ handler
 ******************************************************************************
 */
void DMAC0_IRQHandler(void)
{
    uint32_t irq;
    irq = FM3_INTREQ->IRQ38MON;
    if((irq & 0x00000001) == 0x00000001)
    {
        DMAC_IntHandler(0);
    }
}

/*!
 ******************************************************************************
 ** \brief DMAC ch.1 IRQ handler
 ******************************************************************************
 */
void DMAC1_IRQHandler(void)
{
    uint32_t irq;
    irq = FM3_INTREQ->IRQ39MON;
    if((irq & 0x00000001) == 0x00000001)
    {
        DMAC_IntHandler(1);
    }
}

/*!
 ******************************************************************************
 ** \brief DMAC ch.2 IRQ handler
 ******************************************************************************
 */
void DMAC2_IRQHandler(void)
{
    uint32_t irq;
    irq = FM3_INTREQ->IRQ40MON;
    if((irq & 0x00000001) == 0x00000001)
    {
        DMAC_IntHandler(2);
    }
}

/*!
 ******************************************************************************
 ** \brief DMAC ch.3 IRQ handler
 ******************************************************************************
 */
void DMAC3_IRQHandler(void)
{
    uint32_t irq;
    irq = FM3_INTREQ->IRQ41MON;
    if((irq & 0x00000001) == 0x00000001)
    {
        DMAC_IntHandler(3);
    }
}

/*!
 ******************************************************************************
 ** \brief DMAC ch.4 IRQ handler
 ******************************************************************************
 */
void DMAC4_IRQHandler(void)
{
    uint32_t irq;
    irq = FM3_INTREQ->IRQ42MON;
    if((irq & 0x00000001) == 0x00000001)
    {
        DMAC_IntHandler(4);
    }
}

/*!
 ******************************************************************************
 ** \brief DMAC ch.5 IRQ handler
 ******************************************************************************
 */
void DMAC5_IRQHandler(void)
{
    uint32_t irq;
    irq = FM3_INTREQ->IRQ43MON;
    if((irq & 0x00000001) == 0x00000001)
    {
        DMAC_IntHandler(5);
    }
}

/*!
 ******************************************************************************
 ** \brief DMAC ch.6 IRQ handler
 ******************************************************************************
 */
void DMAC6_IRQHandler(void)
{
    uint32_t irq;
    irq = FM3_INTREQ->IRQ44MON;
    if((irq & 0x00000001) == 0x00000001)
    {
        DMAC_IntHandler(6);
    }
}

/*!
 ******************************************************************************
 ** \brief DMAC ch.7 IRQ handler
 ******************************************************************************
 */
void DMAC7_IRQHandler(void)
{
    uint32_t irq;
    irq = FM3_INTREQ->IRQ45MON;
    if((irq & 0x00000001) == 0x00000001)
    {
        DMAC_IntHandler(7);
    }
}
#endif

#if (PDL_PERIPHERAL_ENABLE_BT == ON)
/*!
 ******************************************************************************
 ** \brief Base timer 8~15 IRQ handler
 ******************************************************************************
 */
void BT8_15_IRQHandler(void)
{
    uint8_t i;
    uint32_t irq, tempirq;
    irq = FM3_INTREQ->IRQ46MON; 
    for(i=0; i<8; i++)
    {
        tempirq = irq>>(2*i);
        if((tempirq & 0x01) == 0x01)
        {
            BT_IntHandler((i+8), 0); /* IRQ 0 */
        }
        
        if((tempirq & 0x02) == 0x02)
        {
            BT_IntHandler((i+8), 1); /* IRQ 1 */
        }
    }
}
#endif

#if (PERIPHERAL_ENABLE_WORKFLASH == ON)
/*!
 ******************************************************************************
 ** \brief Flash IRQ handler
 ******************************************************************************
 */
void FLASH_IRQHandler(void)
{
    while(1);
}
#endif

