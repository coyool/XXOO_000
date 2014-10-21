#ifndef __A7139_H_
#define __A7139_H_


/*** define and type ***/
typedef enum
{
    Standby_mode = 0,
    TX_mode,
    RX_mode,
    Sleep_mode,
//    PLL_mode,
//    IDL_mode,
//    Deep_Sleep_mode
}RF_STATUS_TYPE;

/*RF flag*/
#define A7139_Recv_flag       A7139_RF_FLG.bits.bit0


/* buffer size */
#define A7139_FIFO_LIMIT_SIZE 64u   /* CC1101 buffer limit 62 byte + 2byte */
#define A7139_payload_len     62u   /* FIFO mode MAX 64, FEC MAX 508 byte */
#define A7139_onTheAir_len    ((((A7139_payload_len+2)/2)+1)*4) /* MAX 1024 byte */

/* SDIO */
#define A7139_SDIO_OUT_HIGH   digitalWrite_ALL(GPIOA, GPIO_Pin_6, HIGH)
#define A7139_SDIO_OUT_LOW    digitalWrite_ALL(GPIOA, GPIO_Pin_6, LOW)
#define A7139_SDIO_IN_HIGH    digitalRead_ALL(GPIOA, GPIO_Pin_6)     
#define A7139_SDIO_MI         pinMode_ALL(GPIOA, GPIO_Pin_6, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz) 
#define A7139_SDIO_MO         pinMode_ALL(GPIOA, GPIO_Pin_6, GPIO_Mode_Out_PP, GPIO_Speed_50MHz) 

/* SCK */
#define A7139_SCK_MO          pinMode_ALL(GPIOA, GPIO_Pin_5, GPIO_Mode_Out_PP, GPIO_Speed_50MHz)	
#define A7139_SCK_OUT_HIGH    digitalWrite_ALL(GPIOA, GPIO_Pin_5, HIGH)
#define A7139_SCK_OUT_LOW     digitalWrite_ALL(GPIOA, GPIO_Pin_5, LOW)

/* SCS */
#define A7139_SCS_MO          pinMode_ALL(GPIOA, GPIO_Pin_4, GPIO_Mode_Out_PP, GPIO_Speed_50MHz)	
#define A7139_SCS_OUT_HIGH    digitalWrite_ALL(GPIOA, GPIO_Pin_4, HIGH)
#define A7139_SCS_OUT_LOW     digitalWrite_ALL(GPIOA, GPIO_Pin_4, LOW)

/* GIO1 */	
#define A7139_GIO1_MI         pinMode_ALL(GPIOA, GPIO_Pin_7, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz)
#define A7139_GIO1_IN_HIGH    digitalRead_ALL(GPIOB, GPIO_Pin_7) 

/* GIO2 */
/* CKO */
/* WP */
/* SDA */

/* RF interrupt */
#define A7139_IE_EN      EXTI_IE(EXTI_Line7, ENABLE)              
#define A7139_IE_DIS     EXTI_IE(EXTI_Line7, DISABLE)  

#define A7139_IFG_CLR    EXTI_ClearITPendingBit(EXTI_Line7) //CLR pending bit 



/*** extern variable declarations ***/
extern FLAG_BIT_FIELD_TYPE  A7139_RF_FLG;



/*** extern function prototype declarations ***/
extern void A7139_sleepMode(void);
extern void A7139_TxMode(void);
extern void A7139_RxMode(void);
extern void A7139_standbyMode(void);
extern void A7139_Send(u8 *txBuffer, const u8 size);
extern void A7139_isSending(void);
extern void A7139_waitSend(void);
extern void A7139_Recv(u8 *RxBuf, const u8 size);
extern void A7139_available(void);
extern void A7139_flushTxFIFO(void);
extern void A7139_flushRxFIFO(void);
extern void A7139_write(u8 *txBuffer, const u8 size);
extern void A7139_read(u8 *RxBuf, const u8 size);
extern void A7139_setPower(void);
extern void A7139_setBaudRate(void);
extern u8 A7139_setup(void);
extern u8 A7139_WriteID(void);
extern void A7139_FreqSet(const u8 ch);



#endif   /* end __A7139_H_ */


