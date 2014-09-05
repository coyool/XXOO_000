#ifndef _MAIN_H_
#define _MAIN_H_

#define uchar unsigned char
#define uint unsigned int
extern void _nop_ (void);
#define NOP _nop_();


#define TIMER0 16000         
#define FRAME_LIMIT	90
		   					// 2013-07-31
#define METER  	1		// 1: meter, 0: target board

void InitVariable();
void App_Task();



extern uchar uBaudOption;
extern bit bChanOption;
extern bit	bFreqOption;
extern bit bModeOption;

extern uint xdata get_local_addr_timeout;
extern uint xdata uart_tx_idle_timeout;
extern uint xdata uart_rx_idle_timeout;
extern uchar xdata meter_local_addr[6];
extern uchar xdata meter_local_addr_bak[6];


//#if METER
//sbit PLC_RX = P0^7;			// LED_RXD
//sbit PLC_TX = P1^7;			// LED TXD
//
//#else
sbit PLC_TX = P0^6;		
sbit PLC_RX = P0^7;
//#endif

//sbit RXD_LED = P0^7;		
//sbit TXD_LED = P1^7;

#endif
