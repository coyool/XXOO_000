#ifndef __RF_H_
#define __RF_H_

/*** define and type ***/ 
//#define CC1101_DISPLAY_TX_COMM_SYMBOL_EN    
//#define CC1101_DISPLAY_RX_COMM_SYMBOL_EN
#define RF_PA_EN
//..

#define NOP() __NOP()

#define CC1101_standbyMode()   SpiWriteStrobe(SIDLE)

#define delayMs(x)   PublicDelayMs(x)  // uint_t
#define delayUs(x)   PublicDelayUs(x)  // uint_t

#define CSN_OUT_H    IO_ConfigGPIOPin(IO_PORT3,IO_PINxD,IO_DIR_INPUT,IO_PULLUP_OFF)    //CSN output HIGH
#define CSN_OUT_L    IO_ConfigGPIOPin(IO_PORT3,IO_PINxD,IO_DIR_OUTPUT,IO_PULLUP_OFF)   //CSN output LOW

#define SCLK_OUT_H   IO_ConfigGPIOPin(IO_PORT3,IO_PINx8,IO_DIR_INPUT,IO_PULLUP_OFF)    //SCLK output HIGH
#define SCLK_OUT_L   IO_ConfigGPIOPin(IO_PORT3,IO_PINx8,IO_DIR_OUTPUT,IO_PULLUP_OFF)   //SCLK output LOW

#define SI_OUT_H     IO_ConfigGPIOPin(IO_PORT3,IO_PINx7,IO_DIR_INPUT,IO_PULLUP_OFF)   //SI output HIGH
#define SI_OUT_L     IO_ConfigGPIOPin(IO_PORT3,IO_PINx7,IO_DIR_OUTPUT,IO_PULLUP_OFF)   //SI output LOW

#define SO_DIR_IN    {FM3_GPIO->PFR3&= ~IO_PINx6;FM3_GPIO->PCR3|= IO_PINx6;FM3_GPIO->DDR3&= ~IO_PINx6;}  //IO_ConfigGPIOPin(IO_PORT3, IO_PINx6, IO_DIR_INPUT, IO_PULLUP_ON); //SO input 
#define SO_IN_H      (1u == IO_ReadGPIOPin(IO_PORT3, IO_PINx6)) 
#define SO_IN_L      (0u == IO_ReadGPIOPin(IO_PORT3, IO_PINx6)) 

#define GDO2_IN_HIGH (1u == IO_ReadGPIOPin(IO_PORT3, IO_PINx5))  
#define GDO2_IN_LOW  (0u == IO_ReadGPIOPin(IO_PORT3, IO_PINx5))  

#ifdef RF_PA_EN
#define pinPA_Tx_EN  IO_ConfigGPIOPin(IO_PORT3, IO_PINx4, IO_DIR_OUTPUT, IO_PULLUP_OFF); // L 使能
#define pinPA_Tx_DIS IO_ConfigGPIOPin(IO_PORT3, IO_PINx4, IO_DIR_INPUT, IO_PULLUP_OFF);  // Tx 不经过PA RX
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



/*** extern variable declarations ***/
extern const u32 ONEDAYTIME;    //
extern const u8 RF_shortPayloadSize; 
extern const u8 RF_shortBuffMaxSize; 
extern const u8 RF_payloadSize;
extern const u8 RF_buffMaxSize;   // 60 payload + 2 CRC  {PKTLEN, 0x3C} payload = 60 

extern RF_TYPE  RF;
extern uint32_t oneDayCnt;   

/*** extern function prototype declarations ***/
extern void CC1101_ExtInt_enable(void);
extern void CC1101_ExtInt_disable(void);

extern void CC1101_init(void);
//extern void isSending(void);
extern void CC1101_Send(uint8_t *TxBuff, const uint8_t len);
extern uint8_t CC1101_available(uint8_t *rxBuff, uint8_t len);
extern void CC1101_Recv(void);
extern void CC1101_debug(void);

#endif   /* end __RF_H_ */


