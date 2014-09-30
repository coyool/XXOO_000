#ifndef __A7139_H_
#define __A7139_H_


/*** define and type ***/



/*** extern variable declarations ***/
/* SDIO */
#define LSD_SDIO_OH     digitalWrite_ALL(GPIOB, GPIO_Pin_14, HIGH)
#define LSD_SDIO_OL     digitalWrite_ALL(GPIOB, GPIO_Pin_14, LOW)
#define LSD_SDIO_IN     digitalRead_ALL(GPIOB, GPIO_Pin_14)  
#define LSD_SDIO_MI     pinMode_ALL(GPIOB, GPIO_Pin_14, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz) 
#define LSD_SDIO_MO     pinMode_ALL(GPIOB, GPIO_Pin_14, GPIO_Mode_Out_PP, GPIO_Speed_50MHz)

/* SCK */
#define LSD_SCK_MO      pinMode_ALL(GPIOB, GPIO_Pin_13, GPIO_Mode_Out_PP, GPIO_Speed_50MHz)	
#define LSD_SCK_H       digitalWrite_ALL(GPIOB, GPIO_Pin_13, HIGH)	
#define LSD_SCK_L       digitalWrite_ALL(GPIOB, GPIO_Pin_13, LOW)

/* SCS */
#define LSD_SCS_MO      pinMode_ALL(GPIOB, GPIO_Pin_12, GPIO_Mode_Out_PP, GPIO_Speed_50MHz)	
#define LSD_SCS_H       digitalWrite_ALL(GPIOB, GPIO_Pin_12, HIGH)	
#define LSD_SCS_L       digitalWrite_ALL(GPIOB, GPIO_Pin_12, LOW)

/* GIO1 */	
#define LSD_GIO1_MI     pinMode_ALL(GPIOB, GPIO_Pin_15, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz)
#define GIO1_IN         digitalRead_ALL(GPIOB, GPIO_Pin_15) 

/* GIO2 */
/* CKO */
/* WP */
/* SDA */

/* RF interrupt */
#define A7139_IE_EN      EXTI_IE(EXTI9_5_IRQn, ENABLE)              
#define A7139_IE_DIS     EXTI_IE(EXTI9_5_IRQn, DISABLE)  

#define A7139_IFG_CLR    EXTI_ClearITPendingBit(EXTI_Line5) //CLR pending bit 


/*** extern function prototype declarations ***/
void A7139_ByteSend(u8 src);



#endif   /* end __A7139_H_ */


