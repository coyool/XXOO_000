#ifndef __LSD_A7139_RF_H_
#define __LSD_A7139_RF_H_


/*** define and type ***/
/* SDIO */
#define LSD_SDIO_OH     digitalWrite(GPIOB, GPIO_Pin_14, HIGH)
#define LSD_SDIO_OL     digitalWrite(GPIOB, GPIO_Pin_14, LOW)
#define LSD_SDIO_IN     digitalRead(GPIOB, GPIO_Pin_14)  
#define LSD_SDIO_MI     pinMode(GPIOB, GPIO_Pin_14, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz) 
#define LSD_SDIO_MO     pinMode(GPIOB, GPIO_Pin_14, GPIO_Mode_Out_PP, GPIO_Speed_50MHz)

/* SCK */
#define LSD_SCK_MO      pinMode(GPIOB, GPIO_Pin_13, GPIO_Mode_Out_PP, GPIO_Speed_50MHz)	
#define LSD_SCK_H       digitalWrite(GPIOB, GPIO_Pin_13, HIGH)	
#define LSD_SCK_L       digitalWrite(GPIOB, GPIO_Pin_13, LOW)

/* SCS */
#define LSD_SCS_MO      pinMode(GPIOB, GPIO_Pin_12, GPIO_Mode_Out_PP, GPIO_Speed_50MHz)	
#define LSD_SCS_H       digitalWrite(GPIOB, GPIO_Pin_12, HIGH)	
#define LSD_SCS_L       digitalWrite(GPIOB, GPIO_Pin_12, LOW)

/* GIO1 */	
#define LSD_GIO1_MI     pinMode(GPIOB, GPIO_Pin_15, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz)
#define GIO1_IN         digitalRead(GPIOB, GPIO_Pin_15) 

/* GIO2 */
/* CKO */
/* WP */
/* SDA */

/* RF interrupt */
#define RF_A7139_IE_EN      EXTI_IE(EXTI9_5_IRQn, ENABLE)              
#define RF_A7139_IE_DIS     EXTI_IE(EXTI9_5_IRQn, DISABLE)  

#define RF_A7139_IFG_CLR    EXTI_ClearITPendingBit(EXTI_Line5) //CLR pending bit 




#define LSD_DELAY         _NOP()




/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern void LSD_RF_ByteSend(u8 src);
extern u8 LSD_RF_ByteRead(void);
extern void LSD_RF_StrobeCMD(u8 cmd);
extern void LSD_RF_WriteReg(u8 address, u16 dataWord);


extern u16 LSD_RF_ReadReg(u8 address);
extern void LSD_RF_WritePageA(u8 address, u16 dataWord);
extern u16 LSD_RF_ReadPageA(u8 address);
extern void LSD_RF_WritePageB(u8 address, u16 dataWord);
extern u16 LSD_RF_ReadPageB(u8 address);
extern u8 RF_A7139_setup(void);
extern u8 LSD_RF_Config(void);
extern u8 LSD_RF_WriteID(void);
extern void LSD_RF_FreqSet(u8 ch);
extern void LSD_RF_FreqCalSet(u8 ch);
extern u8 LSD_RF_Cal(void);
//extern void LSD_RF_WriteFIFO(void);
extern void LSD_RF_SendPacket(u8 *txBuffer, u8 size);
//extern void LSD_RF_RxPacket(void);
extern void LSD_RF_RxPacket(u8 *cRxBuf, u8 cLength);
extern void LSD_Err_State(void);
extern void LSD_RF_RCOSC_Cal(void);
extern void LSD_RF_RXmode(void);
extern void LSD_RF_Sleepmode(void);





#endif   /* end __A7139_RF_H_ */







