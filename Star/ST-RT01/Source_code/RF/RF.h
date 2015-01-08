#ifndef __RF_H_
#define __RF_H_

/*** define and type ***/ 
//#define CC1101_DISPLAY_TX_COMM_SYMBOL_EN    
//#define CC1101_DISPLAY_RX_COMM_SYMBOL_EN 

//#define NOP() __NOP()

#define delayMs(x)   delay_ms(x)  // uint32_t
#define delayUs(x)   delay_us(x)  // uint16_t


#define CSN_DIR_OUT     P2DIR |= BIT2   //P2.2
#define CSN_OUT_H       P2OUT |= BIT2
#define CSN_OUT_L       P2OUT &= ~BIT2

#define SCLK_DIR_OUT    P2DIR |= BIT1   //P2.1
#define SCLK_OUT_H      P2OUT |= BIT1
#define SCLK_OUT_L      P2OUT &= ~BIT1

#define SI_DIR_OUT      P1DIR |= BIT6   //P1.6
#define SI_OUT_H        P1OUT |= BIT6    
#define SI_OUT_L        P1OUT &= ~BIT6

#define SO_DIR_IN       P1DIR &= ~BIT7   //P1.7  
#define SO_IN           (P1IN & BIT7) 
#define SO_IN_H         (BIT7 == SO_IN)
#define SO_IN_L         (0u == SO_IN)
 
#define GDO2_DIR_IN               P1DIR &= ~BIT2  //P1.2
#define GDO2_IN                   (P1IN & BIT2)   
#define GDO2_IN_H                 (BIT2 == GDO2_IN)
#define GDO2_IN_L                 (0u == GDO2_IN) 
#define GDO2_EXTI_EN              P1IE |= BIT2    
#define GDO2_EXTI_DIS             P1IE &= ~BIT2
#define GDO2_EXTI_EDGE_HL         P1IES |= BIT2 
#define GDO2_EXTI_EDGE_LH         P1IES &= ~BIT2
#define GDO2_HARDWARE_FlAG_CLEAR  P1IFG &= ~BIT2

#ifdef RF_PA_EN
#define pinPA_Tx_EN   NOP() // L 使能
#define pinPA_Tx_DIS  NOP() // Tx 不经过PA RX
#endif

typedef struct 
{
    uint8_t    TxBuff[64];
    uint8_t    RxBuff[64];
    uint8_t    SOMI_false;
    uint8_t    RX_FLG;
    uint8_t    availableFlag;
//    uint8_t    sendFlag; 
    //..  
}RF_TYPE;

/*
//ARM 32 bit
typedef struct    
{
    unsigned int bit0:  1;
    unsigned int bit1:  1;
    unsigned int bit2:  1;
    unsigned int bit3:  1;
    unsigned int bit4:  1;
    unsigned int bit5:  1;
    unsigned int bit6:  1;
    unsigned int bit7:  1;
    unsigned int bit8:  1;
    unsigned int bit9:  1;
    unsigned int bit10: 1;
    unsigned int bit11: 1;
    unsigned int bit12: 1;
    unsigned int bit13: 1;
    unsigned int bit14: 1;
    unsigned int bit15: 1;
    unsigned int bit16: 1;
    unsigned int bit17: 1;
    unsigned int bit18: 1;
    unsigned int bit19: 1;
    unsigned int bit20: 1;
    unsigned int bit21: 1;
    unsigned int bit22: 1;
    unsigned int bit23: 1;
    unsigned int bit24: 1;
    unsigned int bit25: 1;
    unsigned int bit26: 1;
    unsigned int bit27: 1;
    unsigned int bit28: 1;
    unsigned int bit29: 1;
    unsigned int bit30: 1;
    unsigned int bit31: 1;
}BITS_TYPE;

typedef union
{
    uint32_t     bit32Field;
    BITS_TYPE    bits;
    //..
}RF_FLAG_TYPE;
*/

#define CC1101_standbyMode()   SpiWriteStrobe(SIDLE)

/*** extern variable declarations ***/
extern const u32 ONEDAYTIME;    //
extern const u8 RF_payloadSize;
extern const u8 RF_buffMaxSize;   // 60 payload + 2 CRC  {PKTLEN, 0x3C} payload = 60 

extern RF_TYPE  RF;
extern uint32_t oneDayCnt;   

/*** extern function prototype declarations ***/
extern void TimeIntervalInitRF(void);
extern void CC1101_init(void);
//extern void isSending(void);
extern void CC1101_Send(uint8_t *TxBuff, const uint8_t len);
extern uint8_t CC1101_available(uint8_t *rxBuff, uint8_t len);
extern void CC1101_Recv(void);
extern void CC1101_debug(void);

#endif   /* end __RF_H_ */


