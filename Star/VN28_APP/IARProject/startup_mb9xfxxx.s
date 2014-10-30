;/************************************************************************/
;/*               (C) Fujitsu Semiconductor Europe GmbH (FSEU)           */
;/*                                                                      */
;/* The following software deliverable is intended for and must only be  */
;/* used for reference and in an evaluation laboratory environment.      */
;/* It is provided on an as-is basis without charge and is subject to    */
;/* alterations.                                                         */
;/* It is the user's obligation to fully test the software in its        */
;/* environment and to ensure proper functionality, qualification and    */
;/* compliance with component specifications.                            */
;/*                                                                      */
;/* In the event the software deliverable includes the use of open       */
;/* source components, the provisions of the governing open source       */
;/* license agreement shall apply with respect to such software          */
;/* deliverable.                                                         */
;/* FSEU does not warrant that the deliverables do not infringe any      */
;/* third party intellectual property right (IPR). In the event that     */
;/* the deliverables infringe a third party IPR it is the sole           */
;/* responsibility of the customer to obtain necessary licenses to       */
;/* continue the usage of the deliverable.                               */
;/*                                                                      */
;/* To the maximum extent permitted by applicable law FSEU disclaims all */
;/* warranties, whether express or implied, in particular, but not       */
;/* limited to, warranties of merchantability and fitness for a          */
;/* particular purpose for which the deliverable is not designated.      */
;/*                                                                      */
;/* To the maximum extent permitted by applicable law, FSEU's liability  */
;/* is restricted to intentional misconduct and gross negligence.        */
;/* FSEU is not liable for consequential damages.                        */
;/*                                                                      */
;/* (V1.5)                                                               */
;/************************************************************************/
;/*  Startup for IAR                                                     */
;/*  Version     V0.01                                                   */
;/*  Date        2012-11-20                                              */
;/*  Target-mcu  MB9xfxxx                                                */
;/************************************************************************/


                MODULE  ?cstartup

                ;; Forward declaration of sections.
                SECTION CSTACK:DATA:NOROOT(3)

                SECTION .intvec:CODE:NOROOT(2)

                EXTERN  __iar_program_start
                EXTERN  SystemInit
                PUBLIC  __vector_table

                DATA
__vector_table  DCD     sfe(CSTACK)               ; Top of Stack
	        DCD     Reset_Handler             ; Reset
                DCD     NMI_Handler               ; NMI
                DCD     HardFault_Handler         ; Hard Fault
                DCD     MemManage_Handler         ; MPU Fault
                DCD     BusFault_Handler          ; Bus Fault
                DCD     UsageFault_Handler        ; Usage Fault
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall
                DCD     DebugMon_Handler          ; Debug Monitor
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV
                DCD     SysTick_Handler           ; SysTick

                DCD     IRQ0_Handler               ; 0: Clock Super Visor
                DCD     IRQ1_Handler               ; 1: Software Watchdog Timer
                DCD     IRQ2_Handler               ; 2: Low Voltage Detector
                DCD     IRQ3_Handler               ; 3: Wave Form Generator / DTIF
                DCD     IRQ4_Handler               ; 4: External Interrupt Request ch.0 to ch.7
                DCD     IRQ5_Handler               ; 5: External Interrupt Request ch.8 to ch.15
                DCD     IRQ6_Handler               ; 6: Dual Timer / Quad Decoder
                DCD     IRQ7_Handler         ; 7: MultiFunction Serial ch.0
                DCD     IRQ8_Handler         ; 8: MultiFunction Serial ch.0
                DCD     IRQ9_Handler         ; 9: MultiFunction Serial ch.1
                DCD     IRQ10_Handler         ; 10: MultiFunction Serial ch.1
                DCD     IRQ11_Handler         ; 11: MultiFunction Serial ch.2
                DCD     IRQ12_Handler         ; 12: MultiFunction Serial ch.2
                DCD     IRQ13_Handler         ; 13: MultiFunction Serial ch.3
                DCD     IRQ14_Handler         ; 14: MultiFunction Serial ch.3
                DCD     IRQ15_Handler         ; 15: MultiFunction Serial ch.4
                DCD     IRQ16_Handler         ; 16: MultiFunction Serial ch.4
                DCD     IRQ17_Handler         ; 17: MultiFunction Serial ch.5
                DCD     IRQ18_Handler         ; 18: MultiFunction Serial ch.5
                DCD     IRQ19_Handler         ; 19: MultiFunction Serial ch.6
                DCD     IRQ20_Handler         ; 20: MultiFunction Serial ch.6
                DCD     IRQ21_Handler         ; 21: MultiFunction Serial ch.7
                DCD     IRQ22_Handler         ; 22: MultiFunction Serial ch.7
                DCD     IRQ23_Handler         ; 23: PPG
                DCD     IRQ24_Handler         ; 24: OSC / PLL / Watch Counter
                DCD     IRQ25_Handler         ; 25: ADC0
                DCD     IRQ26_Handler         ; 26: ADC1
                DCD     IRQ27_Handler         ; 27: ADC2
                DCD     IRQ28_Handler         ; 28: Free-run Timer
                DCD     IRQ29_Handler         ; 29: Input Capture
                DCD     IRQ30_Handler         ; 30: Output Compare
                DCD     IRQ31_Handler         ; 31: Base Timer ch.0 to ch.7
                DCD     IRQ32_Handler         ; 32: CAN ch.0
                DCD     IRQ33_Handler         ; 33: CAN ch.1
                DCD     IRQ34_Handler         ; 34: USB Function
                DCD     IRQ35_Handler         ; 35: USB Function / USB HOST
                DCD     IRQ36_Handler         ; 36: Reserved
                DCD     IRQ37_Handler         ; 37: Reserved
                DCD     IRQ38_Handler             ; 38: DMAC ch.0
                DCD     IRQ39_Handler             ; 39: DMAC ch.1
                DCD     IRQ40_Handler             ; 40: DMAC ch.2
                DCD     IRQ41_Handler             ; 41: DMAC ch.3
                DCD     IRQ42_Handler             ; 42: DMAC ch.4
                DCD     IRQ43_Handler             ; 43: DMAC ch.5
                DCD     IRQ44_Handler             ; 44: DMAC ch.6
                DCD     IRQ45_Handler             ; 45: DMAC ch.7
                DCD     IRQ46_Handler             ; 46: Reserved
                DCD     IRQ47_Handler             ; 47: Reserved

                THUMB
; Dummy Exception Handlers (infinite loops which can be modified)

                PUBWEAK Reset_Handler
                SECTION .text:CODE:REORDER(2)
Reset_Handler
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__iar_program_start
                BX      R0

                PUBWEAK NMI_Handler
                SECTION .text:CODE:REORDER(1)
NMI_Handler
                B       NMI_Handler

                PUBWEAK HardFault_Handler
                SECTION .text:CODE:REORDER(1)
HardFault_Handler
                B       HardFault_Handler

                PUBWEAK MemManage_Handler
                SECTION .text:CODE:REORDER(1)
MemManage_Handler
                B       MemManage_Handler

                PUBWEAK BusFault_Handler
                SECTION .text:CODE:REORDER(1)
BusFault_Handler
                B       BusFault_Handler

                PUBWEAK UsageFault_Handler
                SECTION .text:CODE:REORDER(1)
UsageFault_Handler
                B       UsageFault_Handler

                PUBWEAK SVC_Handler
                SECTION .text:CODE:REORDER(1)
SVC_Handler
                B       SVC_Handler

                PUBWEAK DebugMon_Handler
                SECTION .text:CODE:REORDER(1)
DebugMon_Handler
                B       DebugMon_Handler

                PUBWEAK PendSV_Handler
                SECTION .text:CODE:REORDER(1)
PendSV_Handler
                B       PendSV_Handler

                PUBWEAK SysTick_Handler
                SECTION .text:CODE:REORDER(1)
SysTick_Handler
                B       SysTick_Handler



                PUBWEAK IRQ0_Handler
                SECTION .text:CODE:REORDER(1)
IRQ0_Handler
                B       IRQ0_Handler

                PUBWEAK IRQ1_Handler
                SECTION .text:CODE:REORDER(1)
IRQ1_Handler
                B       IRQ1_Handler

                PUBWEAK IRQ2_Handler
                SECTION .text:CODE:REORDER(1)
IRQ2_Handler
                B       IRQ2_Handler

                PUBWEAK IRQ3_Handler
                SECTION .text:CODE:REORDER(1)
IRQ3_Handler
                B       IRQ3_Handler
                
                PUBWEAK IRQ4_Handler
                SECTION .text:CODE:REORDER(1)
IRQ4_Handler
                B       IRQ4_Handler

                PUBWEAK IRQ5_Handler
                SECTION .text:CODE:REORDER(1)
IRQ5_Handler
                B       IRQ5_Handler
                
                PUBWEAK IRQ6_Handler
                SECTION .text:CODE:REORDER(1)
IRQ6_Handler
                B       IRQ6_Handler

                PUBWEAK IRQ7_Handler
                SECTION .text:CODE:REORDER(1)
IRQ7_Handler
                B       IRQ7_Handler

                PUBWEAK IRQ8_Handler
                SECTION .text:CODE:REORDER(1)
IRQ8_Handler
                B       IRQ8_Handler

                PUBWEAK IRQ9_Handler
                SECTION .text:CODE:REORDER(1)
IRQ9_Handler
                B       IRQ9_Handler

                PUBWEAK IRQ10_Handler
                SECTION .text:CODE:REORDER(1)
IRQ10_Handler
                B       IRQ10_Handler

                PUBWEAK IRQ11_Handler
                SECTION .text:CODE:REORDER(1)
IRQ11_Handler
                B       IRQ11_Handler

                PUBWEAK IRQ12_Handler
                SECTION .text:CODE:REORDER(1)
IRQ12_Handler
                B       IRQ12_Handler

                PUBWEAK IRQ13_Handler
                SECTION .text:CODE:REORDER(1)
IRQ13_Handler
                B       IRQ13_Handler

                PUBWEAK IRQ14_Handler
                SECTION .text:CODE:REORDER(1)
IRQ14_Handler
                B       IRQ14_Handler

                PUBWEAK IRQ15_Handler
                SECTION .text:CODE:REORDER(1)
IRQ15_Handler
                B       IRQ15_Handler

                PUBWEAK IRQ16_Handler
                SECTION .text:CODE:REORDER(1)
IRQ16_Handler
                B       IRQ16_Handler

                PUBWEAK IRQ17_Handler
                SECTION .text:CODE:REORDER(1)
IRQ17_Handler
                B       IRQ17_Handler

                PUBWEAK IRQ18_Handler
                SECTION .text:CODE:REORDER(1)
IRQ18_Handler
                B       IRQ18_Handler

                PUBWEAK IRQ19_Handler
                SECTION .text:CODE:REORDER(1)
IRQ19_Handler
                B       IRQ19_Handler

                PUBWEAK IRQ20_Handler
                SECTION .text:CODE:REORDER(1)
IRQ20_Handler
                B       IRQ20_Handler

                PUBWEAK IRQ21_Handler
                SECTION .text:CODE:REORDER(1)
IRQ21_Handler
                B       IRQ21_Handler

                PUBWEAK IRQ22_Handler
                SECTION .text:CODE:REORDER(1)
IRQ22_Handler
                B       IRQ22_Handler

                PUBWEAK IRQ23_Handler
                SECTION .text:CODE:REORDER(1)
IRQ23_Handler
                B       IRQ23_Handler

                PUBWEAK IRQ24_Handler
                SECTION .text:CODE:REORDER(1)
IRQ24_Handler
                B       IRQ24_Handler

                PUBWEAK IRQ25_Handler
                SECTION .text:CODE:REORDER(1)
IRQ25_Handler
                B       IRQ25_Handler

                PUBWEAK IRQ26_Handler
                SECTION .text:CODE:REORDER(1)
IRQ26_Handler
                B       IRQ26_Handler

                PUBWEAK IRQ27_Handler
                SECTION .text:CODE:REORDER(1)
IRQ27_Handler
                B       IRQ27_Handler

                PUBWEAK IRQ28_Handler
                SECTION .text:CODE:REORDER(1)
IRQ28_Handler
                B       IRQ28_Handler

                PUBWEAK IRQ29_Handler
                SECTION .text:CODE:REORDER(1)
IRQ29_Handler
                B       IRQ29_Handler

                PUBWEAK IRQ30_Handler
                SECTION .text:CODE:REORDER(1)
IRQ30_Handler
                B       IRQ30_Handler

                PUBWEAK IRQ31_Handler
                SECTION .text:CODE:REORDER(1)
IRQ31_Handler
                B       IRQ31_Handler

                PUBWEAK IRQ32_Handler
                SECTION .text:CODE:REORDER(1)
IRQ32_Handler
                B       IRQ32_Handler

                PUBWEAK IRQ33_Handler
                SECTION .text:CODE:REORDER(1)
IRQ33_Handler
                B       IRQ33_Handler

                PUBWEAK IRQ34_Handler
                SECTION .text:CODE:REORDER(1)
IRQ34_Handler
                B       IRQ34_Handler

                PUBWEAK IRQ35_Handler
                SECTION .text:CODE:REORDER(1)
IRQ35_Handler
                B       IRQ35_Handler
                
                PUBWEAK IRQ36_Handler
                SECTION .text:CODE:REORDER(1)
IRQ36_Handler
                B       IRQ36_Handler

                PUBWEAK IRQ37_Handler
                SECTION .text:CODE:REORDER(1)
IRQ37_Handler
                B       IRQ37_Handler

                PUBWEAK IRQ38_Handler
                SECTION .text:CODE:REORDER(1)
IRQ38_Handler
                B       IRQ38_Handler

                PUBWEAK IRQ39_Handler
                SECTION .text:CODE:REORDER(1)
IRQ39_Handler
                B       IRQ39_Handler

                PUBWEAK IRQ40_Handler
                SECTION .text:CODE:REORDER(1)
IRQ40_Handler
                B       IRQ40_Handler

                PUBWEAK IRQ41_Handler
                SECTION .text:CODE:REORDER(1)
IRQ41_Handler
                B       IRQ41_Handler

                PUBWEAK IRQ42_Handler
                SECTION .text:CODE:REORDER(1)
IRQ42_Handler
                B       IRQ42_Handler

                PUBWEAK IRQ43_Handler
                SECTION .text:CODE:REORDER(1)
IRQ43_Handler
                B       IRQ43_Handler

                PUBWEAK IRQ44_Handler
                SECTION .text:CODE:REORDER(1)
IRQ44_Handler
                B       IRQ44_Handler

                PUBWEAK IRQ45_Handler
                SECTION .text:CODE:REORDER(1)
IRQ45_Handler
                B       IRQ45_Handler
                
                PUBWEAK IRQ46_Handler
                SECTION .text:CODE:REORDER(1)
IRQ46_Handler
                B       IRQ46_Handler
                
                PUBWEAK IRQ47_Handler
                SECTION .text:CODE:REORDER(1)
IRQ47_Handler
                B       IRQ47_Handler                
                END
