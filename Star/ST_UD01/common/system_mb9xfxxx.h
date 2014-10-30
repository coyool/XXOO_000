/************************************************************************/
/*               (C) Fujitsu Semiconductor Europe GmbH (FSEU)           */
/*                                                                      */
/* The following software deliverable is intended for and must only be  */
/* used for reference and in an evaluation laboratory environment.      */
/* It is provided on an as-is basis without charge and is subject to    */
/* alterations.                                                         */
/* It is the user's obligation to fully test the software in its        */
/* environment and to ensure proper functionality, qualification and    */
/* compliance with component specifications.                            */
/*                                                                      */
/* In the event the software deliverable includes the use of open       */
/* source components, the provisions of the governing open source       */
/* license agreement shall apply with respect to such software          */
/* deliverable.                                                         */
/* FSEU does not warrant that the deliverables do not infringe any      */
/* third party intellectual property right (IPR). In the event that     */
/* the deliverables infringe a third party IPR it is the sole           */
/* responsibility of the customer to obtain necessary licenses to       */
/* continue the usage of the deliverable.                               */
/*                                                                      */
/* To the maximum extent permitted by applicable law FSEU disclaims all */
/* warranties, whether express or implied, in particular, but not       */
/* limited to, warranties of merchantability and fitness for a          */
/* particular purpose for which the deliverable is not designated.      */
/*                                                                      */
/* To the maximum extent permitted by applicable law, FSEU's liability  */
/* is restricted to intentional misconduct and gross negligence.        */
/* FSEU is not liable for consequential damages.                        */
/*                                                                      */
/* (V1.5)                                                               */
/************************************************************************/
/** \file system_mb9abxxx.h
 **
 ** Headerfile for FM3 system parameters
 **
 ** History: 
 ** 2013-01-21  0.1  MWi  
 ** 2013-02-01  0.2  Edison Propose to unify the PLL setting
 ******************************************************************************/

#ifndef _SYSTEM_MB9ABXXX_H_
#define _SYSTEM_MB9ABXXX_H_

#ifdef __cplusplus
 extern "C" {
#endif 

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
   
#include <stdint.h>

/******************************************************************************/
/* Global pre-processor symbols/macros ('define')                             */
/******************************************************************************/
 
#define CLOCK_SETUP           1   // clock setup
#define HWWD_DISABLE          1   //  HW Watach dog enable  
#define CR_TRIM_SETUP         1   //  CR trimming at startup enable    
   
/******************************************************************************/
/* Device dependent System Clock absolute maximum ranges                      */
/******************************************************************************/
/* Type 0:    80MHz   */  
/* Type 1 & 5:40MHz   */   
/* Type 2 & 4:144MHz  */   
/* Type 3 & 7:20MHz   */   
/* Type 6 & 8:40MHz   */   
/* Type 9:    72MHz   */       
#if (MCU_TYPE == DEVICE_TYPE0)
  #define __CLKMO        ( 4000000UL)         // External   4MHz Crystal
  #define __CLKSO        (   32768UL)         // External  32KHz Crystal
  #define __CLKHC        ( 4000000UL)         // Internal   4MHz CR Oscillator 
  #define __CLKLC        (  100000UL)         // Internal 100KHz CR Oscillator 
  #define SCM_CTL_Val           0x0000005A    // SCM_CTL   
  #define BSC_PSR_Val           0x00000000    // BSC_PSR  
  #define APBC0_PSR_Val         0x00000001    // APBC0_PSR    
  #define APBC1_PSR_Val         0x00000081    // APBC1_PSR   
  #define APBC2_PSR_Val         0x00000081    // APBC2_PSR 
  #define SWC_PSR_Val           0x00000003    // SWC_PSR  
  #define TTC_PSR_Val           0x00000000    // TTC_PSR    
  #define CSW_TMR_Val           0x0000005C    // CSW_TMR     
  #define PSW_TMR_Val           0x00000000    // PSW_TMR   
  #define PLL_CTL1_Val          0x00000000    // K=1, M=1    
  #define PLL_CTL2_Val          0x00000013    // N=19
  
  #define __PLLK         (((PLL_CTL1_Val >> 4) & 0x0F) + 1)
  #define __PLLN         (((PLL_CTL2_Val     ) & 0x1F) + 1)

#elif (MCU_TYPE == DEVICE_TYPE1) || (MCU_TYPE == DEVICE_TYPE5)

  #define __CLKMO        ( 4000000UL)         // External   4MHz Crystal
  #define __CLKSO        (   32768UL)         // External  32KHz Crystal
  #define __CLKHC        ( 4000000UL)         // Internal   4MHz CR Oscillator 
  #define __CLKLC        (  100000UL)         // Internal 100KHz CR Oscillator 
  #define SCM_CTL_Val           0x0000005A    // SCM_CTL   
  #define BSC_PSR_Val           0x00000000    // BSC_PSR  
  #define APBC0_PSR_Val         0x00000001    // APBC0_PSR    
  #define APBC1_PSR_Val         0x00000081    // APBC1_PSR   
  #define APBC2_PSR_Val         0x00000081    // APBC2_PSR 
  #define SWC_PSR_Val           0x00000003    // SWC_PSR  
  #define TTC_PSR_Val           0x00000000    // TTC_PSR    
  #define CSW_TMR_Val           0x0000005C    // CSW_TMR     
  #define PSW_TMR_Val           0x00000000    // PSW_TMR   
  #define PLL_CTL1_Val          0x00000004    // K=1, M=5    
  #define PLL_CTL2_Val          0x00000009    // N=10
  
  #define __PLLK         (((PLL_CTL1_Val >> 4) & 0x0F) + 1)
  #define __PLLN         (((PLL_CTL2_Val     ) & 0x3F) + 1)
   
#elif (MCU_TYPE == DEVICE_TYPE2) || (MCU_TYPE == DEVICE_TYPE4)

  #define __CLKMO        ( 4000000UL)         // External   4MHz Crystal
  #define __CLKSO        (   32768UL)         // External  32KHz Crystal
  #define __CLKHC        ( 4000000UL)         // Internal   4MHz CR Oscillator 
  #define __CLKLC        (  100000UL)         // Internal 100KHz CR Oscillator 
  #define SCM_CTL_Val           0x0000005A    // SCM_CTL   
  #define BSC_PSR_Val           0x00000000    // BSC_PSR  
  #define APBC0_PSR_Val         0x00000001    // APBC0_PSR    
  #define APBC1_PSR_Val         0x00000082    // APBC1_PSR   
  #define APBC2_PSR_Val         0x00000081    // APBC2_PSR 
  #define SWC_PSR_Val           0x00000003    // SWC_PSR  
  #define TTC_PSR_Val           0x00000000    // TTC_PSR    
  #define CSW_TMR_Val           0x0000005C    // CSW_TMR     
  #define PSW_TMR_Val           0x00000000    // PSW_TMR   
  #define PLL_CTL1_Val          0x00000001    // K=1, M=2    
  #define PLL_CTL2_Val          0x00000023    // N=36
  
  #define __PLLK         (((PLL_CTL1_Val >> 4) & 0x0F) + 1)
  #define __PLLN         (((PLL_CTL2_Val     ) & 0x3F) + 1)

#elif (MCU_TYPE == DEVICE_TYPE3) || (MCU_TYPE == DEVICE_TYPE7)

  #define __CLKMO        ( 8000000UL)         // External   4MHz Crystal
  #define __CLKSO        (   32768UL)         // External  32KHz Crystal
  #define __CLKHC        ( 4000000UL)         // Internal   4MHz CR Oscillator 
  #define __CLKLC        (  100000UL)         // Internal 100KHz CR Oscillator 
#define SCM_CTL_Val 		  0x00000020	// SCM_CTL	0x00000088 
  #define BSC_PSR_Val           0x00000000    // BSC_PSR  
  #define APBC0_PSR_Val         0x00000000    // APBC0_PSR    
  #define APBC1_PSR_Val         0x00000080    // APBC1_PSR   
  #define APBC2_PSR_Val         0x00000081    // APBC2_PSR 
  #define SWC_PSR_Val           0x00000003    // SWC_PSR  
  #define TTC_PSR_Val           0x00000000    // TTC_PSR    
  #define CSW_TMR_Val           0x0000005C    // CSW_TMR     
  #define PSW_TMR_Val           0x00000000    // PSW_TMR   
  #define PLL_CTL1_Val          0x00000000    // K=1, M=1    
  #define PLL_CTL2_Val          0x00000003    // N=4
  
  #define __PLLK         (((PLL_CTL1_Val >> 4) & 0x0F) + 1)
  #define __PLLN         (((PLL_CTL2_Val     ) & 0x3F) + 1)

#elif (MCU_TYPE == DEVICE_TYPE6) || (MCU_TYPE == DEVICE_TYPE8)

  #define __CLKMO        ( 4000000UL)         // External   4MHz Crystal
  #define __CLKSO        (   32768UL)         // External  32KHz Crystal
  #define __CLKHC        ( 4000000UL)         // Internal   4MHz CR Oscillator 
  #define __CLKLC        (  100000UL)         // Internal 100KHz CR Oscillator 
  #define SCM_CTL_Val           0x0000005A    // SCM_CTL   
  #define BSC_PSR_Val           0x00000000    // BSC_PSR  
  #define APBC0_PSR_Val         0x00000001    // APBC0_PSR    
  #define APBC1_PSR_Val         0x00000081    // APBC1_PSR   
  #define APBC2_PSR_Val         0x00000081    // APBC2_PSR 
  #define SWC_PSR_Val           0x00000003    // SWC_PSR  
  #define TTC_PSR_Val           0x00000000    // TTC_PSR    
  #define CSW_TMR_Val           0x0000005C    // CSW_TMR     
  #define PSW_TMR_Val           0x00000000    // PSW_TMR   
  #define PLL_CTL1_Val          0x00000001    // K=1, M=2    
  #define PLL_CTL2_Val          0x00000009    // N=10
  
  #define __PLLK         (((PLL_CTL1_Val >> 4) & 0x0F) + 1)
  #define __PLLN         (((PLL_CTL2_Val     ) & 0x3F) + 1)
   
#elif (MCU_TYPE == DEVICE_TYPE9)

  #define __CLKMO        ( 4000000UL)         // External   4MHz Crystal
  #define __CLKSO        (   32768UL)         // External  32KHz Crystal
  #define __CLKHC        ( 4000000UL)         // Internal   4MHz CR Oscillator 
  #define __CLKLC        (  100000UL)         // Internal 100KHz CR Oscillator 
  #define SCM_CTL_Val           0x0000005A    // SCM_CTL   
  #define BSC_PSR_Val           0x00000000    // BSC_PSR  
  #define APBC0_PSR_Val         0x00000001    // APBC0_PSR    
  #define APBC1_PSR_Val         0x00000081    // APBC1_PSR   
  #define APBC2_PSR_Val         0x00000081    // APBC2_PSR 
  #define SWC_PSR_Val           0x00000003    // SWC_PSR  
  #define TTC_PSR_Val           0x00000000    // TTC_PSR    
  #define CSW_TMR_Val           0x0000005C    // CSW_TMR     
  #define PSW_TMR_Val           0x00000000    // PSW_TMR   
  #define PLL_CTL1_Val          0x00000001    // K=1, M=2    
  #define PLL_CTL2_Val          0x000000011   // N=18
  
  #define __PLLK         (((PLL_CTL1_Val >> 4) & 0x0F) + 1)
  #define __PLLN         (((PLL_CTL2_Val     ) & 0x3F) + 1)   
   
#endif   
   
   
#define __PLLCLK       ((__CLKMO  * __PLLN) / __PLLK)

/**
 ******************************************************************************
 ** \brief Define Master Clock from settings
 ******************************************************************************/
#if   (((SCM_CTL_Val >> 5) & 0x07) == 0)
  #define __MASTERCLK     (__CLKHC)
#elif (((SCM_CTL_Val >> 5) & 0x07) == 1)
  #define __MASTERCLK     (__CLKMO)
#elif (((SCM_CTL_Val >> 5) & 0x07) == 2)
  #define __MASTERCLK     (__PLLCLK)
#elif (((SCM_CTL_Val >> 5) & 0x07) == 4)
  #define __MASTERCLK     (__CLKLC)
#elif (((SCM_CTL_Val >> 5) & 0x07) == 5)
  #define __MASTERCLK     (__CLKSO)
#else
  #define __MASTERCLK     (0UL)
#endif

/**
 ******************************************************************************
 ** \brief Define System Clock Frequency (Core Clock) from settings
 ******************************************************************************/
#if   ((BSC_PSR_Val & 0x07) == 0)
  #define __HCLK         (__MASTERCLK / 1)
#elif ((BSC_PSR_Val & 0x07) == 1)
  #define __HCLK         (__MASTERCLK / 2)
#elif ((BSC_PSR_Val & 0x07) == 2)
  #define __HCLK         (__MASTERCLK / 3)
#elif ((BSC_PSR_Val & 0x07) == 3)
  #define __HCLK         (__MASTERCLK / 4)
#elif ((BSC_PSR_Val & 0x07) == 4)
  #define __HCLK         (__MASTERCLK / 6)
#elif ((BSC_PSR_Val & 0x07) == 5)
  #define __HCLK         (__MASTERCLK / 8)
#elif ((BSC_PSR_Val & 0x07) == 6)
  #define __HCLK         (__MASTERCLK /16)
#else
  #define __HCLK         (0UL)
#endif
   
/******************************************************************************/
/* Global function prototypes ('extern', definition in C source)              */
/******************************************************************************/
   
extern uint32_t SystemCoreClock;          // System Clock Frequency (Core Clock)
   
extern void SystemInit (void);            // Initialize the system

extern void SystemCoreClockUpdate (void); // Update SystemCoreClock variable

#ifdef __cplusplus
}
#endif

#endif /* _SYSTEM_MB9ABXXX_H_ */
