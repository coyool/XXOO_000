/****************************************************************************
 *                  UART Task Module Header
 ***************************************************************************
 * FileName:  UART.h
 *
 * Company:   Sinowealth Micro Electronics ltd
 *
 * All rights reserved.
 *
 * Author              Date          Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * robbie.zhang       2010.4.13       Version 1.0
 ****************************************************************************/

#ifndef _UART_H_
#define _UART_H_

typedef struct{	
	uchar uart_framelen;
	uchar uart_ptr;	
	uchar uart_buf[FRAME_LIMIT];	
	uint  uart_timeout;

}UART_TRANS;


typedef struct{	
//	uchar plt_framelen;		// 2013-09-27
	uchar plt_ptr;	
	uchar plt_buf[FRAME_LIMIT];	
	uint  plt_timeout;			// backup	
}PLT_TRANS;




#define UART_DELAY 20	//10
#define UART_DURATION 30

void InitUART();
void Uart_Task();
void UartReturn(uchar dat);
uchar UartCheckSum(uchar *buf, uchar len);

extern bit	  uart_TI_ready;
extern bit	  uart_RI_start;
extern bit	  uart_TI_ok;
extern bit	  uart_RI_ok;

extern UART_TRANS	xdata uart_trans;


#endif
