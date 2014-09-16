
/*********************************************************************
 *			Interrupt Service Routines 
 *********************************************************************
 * FileName:        ISR.c
 * Dependencies:    SH99F01.h
 *					MAIN.h
 *                  UART.h
 *                  PLT.h
 * Company:         Sinowealth Micro Electronics ltd
 *
 * All rights reserved.
 *
 * Author           Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * robbie.zhang    2010.4.13       Version 1.0
 ********************************************************************/

#include  <SH99F01.h>
#include  "MAIN.h"
#include  "UART.h"
#include  "PLT.h"

uint Timer_count1=0;

void ISR_OVL(void) interrupt 15        
{
	while(1);	// go watch dog
}


void ISR_TIMER0(void) interrupt 1        // 1ms   
{
	EA = 0;
	TL0	= -TIMER0;    //wait 1ms 	start const = 0xFFFF - 16000
	TH0	= (-TIMER0)>>8;	

	Timer_count++;
	PLT_TimerTask();

	if(get_local_addr_timeout && get_local_addr_timeout!=0xFFFF)                      //receive flow timeout
   		 --get_local_addr_timeout; 

	Timer_count1++;
	if(Timer_count1>=1000)	// 1s
	{ 
		Timer_count1 = 0;
		uart_tx_idle_timeout++;
		uart_rx_idle_timeout++;
	}

	EA = 1;
}


void ISR_INT1(void) interrupt 2
{
	EA = 0;
	EX1 = 0;
    zc_flag = 1;
	EA = 1;
}

void ISR_UART(void) interrupt 4		 
{
	EA = 0;    // disable all interrupt
	ES = 0;    // disable UART interrupt
	if(RI)	   // ART Rev_flag
	{		
		if (!uart_RI_start)
		{
			ACC = SBUF;
			if(P==RB8)		 //∆Ê≈º–£—È“ª÷¬
			{
			  uart_trans.uart_ptr = 0;
			  uart_trans.uart_buf[uart_trans.uart_ptr] = SBUF;
			  uart_trans.uart_ptr++;
			  uart_trans.uart_timeout = UART_DELAY;		 //UART_DELAY = 20	  			  
			  uart_RI_start = 1;	
			}
			else
			  ES = 1;	// enable again										
		}
		RI = 0;
	}
	//ES = 1; //if RX complete then wait for TX complete then enable ES = 1 again 	
	EA = 1;
}



void ISR_PLT(void) interrupt 7
{
	EA = 0;
	if(TXINT)						//Transmit interrupt
	{
		TXINT = 0;
		tx_flag = 1;
	}
	if(RXINT)	
	{
		RXINT = 0;
		rx_flag = 1;
	}
	EA = 1;
}
