//MCU: SH99F01
#include  <SH99F01.h>
#include  "MAIN.h"
#include  "UART.h"
#include  "PLT.h"
#include  "string.h"


//bit  bBaudOption = 1;		// 1: 2.4k, 0: 9.6k
uchar uBaudOption = 0;		// 0: 2.4k, 1: 4.8k,  2: 9.6k
bit  bChanOption = 0;		// 1: vincom(290KHz), 0: vin(290KHz)
bit  bModeOption = 1;		// 1: DMZC, 0: NBSF
bit  bFreqOption = 1;		// 1: 50Hz, 0: 60Hz

uchar xdata meter_local_addr[6]={0};
uchar xdata meter_local_addr_bak[6]={0};  // dummy
  
uchar code meter_read_addr_645_1997[16]={0xFE,0xFE,0x68,0x99,0x99,0x99,0x99,0x99,0x99,0x68,0x01,0x02,0x65,0xF3,0xC1,0x16};
uchar code meter_read_addr_645_2007[14]={0xFE,0xFE,0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x13,0x00,0xDF,0x16};

bit bShakeHandsOk = 0;
uint xdata get_local_addr_timeout = 0xFFFF;
uint xdata uart_tx_idle_timeout = 0;
uint xdata uart_rx_idle_timeout = 0;


void main(void)
{
	uchar i;
	uchar temp;

	EA = 0;		   //__disable interrupt
//	// power on delay,  wait 4*(65535/16000) = 16ms 
//	Timer_count = 0xFFFF;
//	while(Timer_count)
//		Timer_count--;			
//	Timer_count = 0xFFFF;
//	while(Timer_count)
//		Timer_count--;		
//	Timer_count = 0xFFFF;
//	while(Timer_count)
//		Timer_count--;	
//	Timer_count = 0xFFFF;
//	while(Timer_count)
//		Timer_count--;	
	///////////////////////////////////////////////////	

//  main_start:

    RSTSTAT = 0x00;                 //set watchdog duration¡A4096ms

	// power on delay,  wait 4*(65535/16000) = 16ms, 2014/3/21 
	Timer_count = 0xFFFF;
	while(Timer_count)
		Timer_count--;			
	Timer_count = 0xFFFF;
	while(Timer_count)
		Timer_count--;		
	Timer_count = 0xFFFF;
	while(Timer_count)
		Timer_count--;		
	Timer_count = 0xFFFF;
	while(Timer_count)
		Timer_count--;	

	InitVariable();		//Initial System

	uBaudOption	= 0;	// 2.4k
	bChanOption = 0;	// vin : 290KHz
	bModeOption = 1;	// DMZC
	bFreqOption = 1;	// 50Hz


	// Options for S1 switch
	#if 0   	
		if ((P0&0x10)==0x10)	  // P0.4 ~ bBaudOption	   Ðü¿Õ
			uBaudOption = 1;	  // 4.8K
		else	
			uBaudOption = 0;	  // 2.4K
	
		if ((P0&0x20)==0x20)	  // P0.5 ~ bChanOption
			bChanOption = 1;	  // 1: vincom(290KHz)
		else	
			bChanOption = 0;	  // 0: vin(290KHz)
			
		if ((P1&0x40)==0x40)	  // P1.6 ~ ucModeOption
			bModeOption = 1;	  // 1: DMZC
		else	
			bModeOption = 0;	  // 0: NBSF
							
		if ((P1&0x80)==0x80)	  // P1.7 ~ bACFreqDetect
			bACFreqDetect = 1;	
		else	
			bACFreqDetect = 0;					
	#endif
	
	InitPLT(); 			//Initial PLT  
	InitUART();			//Initial UART   

	bShakeHandsOk = 0;
	uart_tx_idle_timeout = 0;
	uart_rx_idle_timeout = 0;
	get_local_addr_timeout = 500;


//	uart_trans.uart_buf[0]=0xFE;
//	uart_trans.uart_buf[1]=0xFE;
//	uart_trans.uart_buf[2]=0xFE;
//	uart_trans.uart_buf[3]=0xFE;
//	uart_trans.uart_buf[4]=0x68;
//	uart_trans.uart_buf[5]=0x01;
//	uart_trans.uart_buf[6]=0x02;
//	uart_trans.uart_buf[7]=0x03;
//	uart_trans.uart_buf[8]=0x04;
//	uart_trans.uart_buf[9]=0x05;
//	uart_trans.uart_buf[10]=0x06;
//	uart_trans.uart_buf[11]=0x68;
//	uart_trans.uart_buf[12]=0x81;
//	uart_trans.uart_buf[13]=0x08;
//	uart_trans.uart_buf[14]=0x65;
//	uart_trans.uart_buf[15]=0xF3;
//	uart_trans.uart_buf[16]=0x34;
//	uart_trans.uart_buf[17]=0x35;
//	uart_trans.uart_buf[18]=0x36;
//	uart_trans.uart_buf[19]=0x37;
//	uart_trans.uart_buf[20]=0x38;
//	uart_trans.uart_buf[21]=0x39;
//	uart_trans.uart_buf[22]=0x0D;
//	uart_trans.uart_buf[23]=0x16;
//	uart_trans.uart_framelen = sizeof(meter_read_addr_645_1997);
//	for(i=0;i<(uart_trans.uart_framelen+4);i++)
//	{
//		if (uart_trans.uart_buf[i]==0x68)
//		  break;
//	}
//	temp = UartCheckSum(&uart_trans.uart_buf[i],18);
//	if ((temp==uart_trans.uart_buf[i+18])&&(uart_trans.uart_buf[i+8]==0x81))
//	{
//		bShakeHandsOk = 1;
//		memcpy(&meter_local_addr,&uart_trans.uart_buf[i+1],6);
//		memcpy(meter_local_addr_bak,meter_local_addr,6);
//	}

	EA = 1;
	
	while(1)
	{
		RSTSTAT = 0X00;      //watch dog

//		// 2014/3/21
//		if (uart_idle_timeout>90) 		// dummy
//		{
//			EA = 0;				
//			uart_idle_timeout = 0;
//			goto main_start;			
//		}

		// 2014/5/27, modify
		// 2014/6/5, modify new
		//if (uart_idle_timeout>500) 		// 500s  ~ 8 minute  timeout
		if (uart_tx_idle_timeout>600) 		// 600s ~ 10 minutes, 2014/6/9
		{
			EA = 0;				
			uart_tx_idle_timeout = 0;
			//goto main_start;						
			while(1);			  // 2014/6/5	  ?? software reset
		}

		if (uart_rx_idle_timeout>600) 		// 600s ~ 10 minutes, 2014/6/9
		{
			EA = 0;				
			uart_rx_idle_timeout = 0;
			//goto main_start;						
			while(1);			  // 2014/6/5	  ?? software reset
		}

		if (bShakeHandsOk)
		{
			App_Task();
			Uart_Task();
			PLTTask();
		}
		else
		{
			// 2014/3/21
			if (get_local_addr_timeout==0)
			{
				uart_trans.uart_framelen = sizeof(meter_read_addr_645_1997);
				ES=0;		// dummy		
				TI = 0;		// dummy
				RI = 0;
			  	for(i=0; i<uart_trans.uart_framelen; i++)	
				{
					UartReturn(meter_read_addr_645_1997[i]);	  //putchar();
				}
				ES = 1;		   //UART interrrupt enable
				uart_TI_ready = 0;
				uart_RI_ok = 0;
				uart_trans.uart_ptr = 0;
				uart_RI_start = 0;
				RSTSTAT = 0X00;      //watch dog			
				get_local_addr_timeout = 500;
				while((!uart_RI_ok)&&(get_local_addr_timeout))
				{
					Uart_Task();	
				}
	
				if (uart_RI_ok)
				{
					uart_RI_ok = 0;
					get_local_addr_timeout = 0xFFFF;
					for(i=0;i<(uart_trans.uart_framelen+4);i++)
					{
						if (uart_trans.uart_buf[i]==0x68)
						  break;
					}
					temp = UartCheckSum(&uart_trans.uart_buf[i],18);
					if ((temp==uart_trans.uart_buf[i+18])&&(uart_trans.uart_buf[i+8]==0x81))   // 1997  version
					{


						// 2014/6/4, quick restart
						get_local_addr_timeout = 100;
						PLC_TX=1;
						PLC_RX=1;		
						while(get_local_addr_timeout);
						get_local_addr_timeout = 100;
						PLC_TX=0;
						PLC_RX=0;		
						while(get_local_addr_timeout);
						get_local_addr_timeout = 100;
						PLC_TX=1;
						PLC_RX=1;		
						while(get_local_addr_timeout);
						get_local_addr_timeout = 100;
						PLC_TX=0;
						PLC_RX=0;		
						while(get_local_addr_timeout);
						////////////////////////////////

						bShakeHandsOk = 1;
						uart_tx_idle_timeout = 0;	// 2014/6/5
						uart_rx_idle_timeout = 0;	// 2014/6/5
						get_local_addr_timeout = 0xFFFF;	// dummy
						memcpy(meter_local_addr_bak,&uart_trans.uart_buf[i+12],6);		// 2014/3/21, UIU
						meter_local_addr[0] = meter_local_addr_bak[0]-0x33;
						meter_local_addr[1] = meter_local_addr_bak[1]-0x33;
						meter_local_addr[2] = meter_local_addr_bak[2]-0x33;
						meter_local_addr[3] = meter_local_addr_bak[3]-0x33;
						meter_local_addr[4] = meter_local_addr_bak[4]-0x33;
						meter_local_addr[5] = meter_local_addr_bak[5]-0x33;
						
						memcpy(meter_local_addr_bak,meter_local_addr,6);	// 2014/5/27

					}
					else
					{
						uart_RI_ok = 0;
						uart_trans.uart_ptr = 0;
						uart_RI_start = 0;			
						get_local_addr_timeout = 500;					
					}	
				}
				else
				{
					uart_trans.uart_framelen = sizeof(meter_read_addr_645_2007);
					ES=0;		// dummy		
					TI = 0;		// dummy, 2014/3/21
					RI = 0;
				  	for(i=0;i<uart_trans.uart_framelen;i++)	
					{
						UartReturn(meter_read_addr_645_2007[i]);
					}
					ES = 1;
					uart_TI_ready = 0;
					uart_RI_ok = 0;
					uart_trans.uart_ptr = 0;
					uart_RI_start = 0;	
					RSTSTAT = 0X00;      //watch dog		
					get_local_addr_timeout = 500;
					while((!uart_RI_ok)&&(get_local_addr_timeout))
					{
						Uart_Task();	
					}
		
					if (uart_RI_ok)
					{
						uart_RI_ok = 0;
						get_local_addr_timeout = 0xFFFF;
						for(i=0;i<(uart_trans.uart_framelen+4);i++)
						{
							if (uart_trans.uart_buf[i]==0x68)
							  break;
						}
						temp = UartCheckSum(&uart_trans.uart_buf[i],16);
						if ((temp==uart_trans.uart_buf[i+16])&&(uart_trans.uart_buf[i+8]==0x93))	// 2007	  version
						{


							// 2014/6/4, quick restart
							get_local_addr_timeout = 100;
							PLC_TX=1;
							PLC_RX=1;		
							while(get_local_addr_timeout);
							get_local_addr_timeout = 100;
							PLC_TX=0;
							PLC_RX=0;		
							while(get_local_addr_timeout);
							get_local_addr_timeout = 100;
							PLC_TX=1;
							PLC_RX=1;		
							while(get_local_addr_timeout);
							get_local_addr_timeout = 100;
							PLC_TX=0;
							PLC_RX=0;		
							while(get_local_addr_timeout);
							////////////////////////////////

							bShakeHandsOk = 1;
							uart_tx_idle_timeout = 0;	// debug
							uart_rx_idle_timeout = 0;	// debug
							get_local_addr_timeout = 0xFFFF;	// dummy
							memcpy(meter_local_addr_bak,&uart_trans.uart_buf[i+10],6);		// 2014/3/21, UIU
							meter_local_addr[0] = meter_local_addr_bak[0]-0x33;
							meter_local_addr[1] = meter_local_addr_bak[1]-0x33;
							meter_local_addr[2] = meter_local_addr_bak[2]-0x33;
							meter_local_addr[3] = meter_local_addr_bak[3]-0x33;
							meter_local_addr[4] = meter_local_addr_bak[4]-0x33;
							meter_local_addr[5] = meter_local_addr_bak[5]-0x33;

							memcpy(meter_local_addr_bak,meter_local_addr,6);	// 2014/5/27


						}
						else
						{
							uart_RI_ok = 0;
							uart_trans.uart_ptr = 0;
							uart_RI_start = 0;			
							get_local_addr_timeout = 500;					
						}
					}
					else
					{
						get_local_addr_timeout = 500;

						// change baud rate	 , 2014/3/21, Baud Rate
						uBaudOption++;
						if (uBaudOption>2)
							uBaudOption = 0;

						InitUART();						
					}
				}							
			}
//			if (uart_idle_timeout>90) 		// dummy
//				uart_idle_timeout = 0;
		}
	}
}


/****************************************************************************
 * Function:          void InitVariable() 
 * Input Parameter:   None
 * OutPut Parameter:  None
 * Return Value:      None
 * Description:       System Parameter setting
 *                                              
 ***************************************************************************/
void InitVariable()
{
		
	P0CR  = 0xC0;		  //P0.6,P0.7 output,other input	(P0.6 LED902)(P0.7 LED901) 
	P1CR  = 0x80;	      // P1.7output, other input         P1.7	

	P0PCR = 0xF8;	      // P0.0~P0.2 input no pull high, other pull high
	P1PCR = 0xFF;		  // input all pull high

	P0 = 0x00;
	P1 = 0x00;
    
	//Timer 1
	TMOD = 0x21;			//Timer0-mode1(16bit up/down timer),Timer1-mode2(8bit auto reloaded) 
	TR1 = 1;				//Start Timer1
	TF1 = 0;
	//Timer 0
    TH0	= (-TIMER0)>>8;		//wait 1ms  
	TL0	= -TIMER0;    
	TF0 = 0;
	TR0 = 1; 				//Start Timer0

    //ADC Initial
	ADCON = 0x80;
	ADT = 0xA8;		    	 //32*tsys, 4*Tad

    IPL0 = 0x02;             //T0 high priority, UART second priority
    IPH0 = 0x12;

	IEN0 = 0; 				 //Close  PLT interrupt
	IEN1 = 0X01;			 //enable PLT interrupt
	EA = 1;	 				 //Enable interrupt
    ET0 = 1;				 //Enable Timer0 interrupt

	return;
}

