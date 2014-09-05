#include  "SH99F01.h"
#include  "MAIN.h"
#include  "UART.h"
#include  "PLT.h"

/*********************************************************************
 *
 *                 PLT Access Routines 
 *
 *********************************************************************
 * FileName:        PLT.c
 * Dependencies:    
 * 					string.h
 * 					SH99F01.h
 *					MAIN.h
 *                  UART.h
 *                  PLT.h
 * Company:         Sinowealth Micro Electronics ltd
 *
 * All rights reserved.
 *
 * Author               Date           Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Robbie Zhang        2010/7/9	   	   Original  
 * joseph.zhou	   	2012.12.17	   Version 1.2
 ********************************************************************/

//#include "absacc.h"
#include "string.h"
//#include "stdlib.h"
#include  "SH99F01.h"
#include  "MAIN.h"
#include  "UART.h"
#include  "PLT.h"


uint xdata Timer_count=0;




PLT_TRANS	xdata plt_trans;

//////Input Variable
///Start transmit
bit 	transmit_enable = 0;

/////Output Variable
bit		rx_ok_flag = 0;
bit		tx_ok_flag = 0;
uchar 	rssi = 0;


/////Other Variable
bit		rx_flag = 0;
bit		tx_flag = 0;
bit		zc_flag = 0;
bit		pd_flag = 0;
bit		fa_flag = 0;
bit		nb_flag = 0;
bit		once_ok = 0;
bit		carrier_stage = 0;

uchar	rx_count = 0;
uchar   nb_repeat = 0;

bit 	en_zc_delay = 0;
bit 	en_delay_half = 0;
bit     en_delay_flag = 0;
bit     en_frame_delay = 0;
bit		transmit_continue = 0; 
bit 	frame_transmit_ready = 0;

uchar	zc_delay_cnt = 0;


uint   xdata  plt_trans_timeout = 0xFFFF;	
uint   xdata  plt_recev_timeout = 0xFFFF;


uchar xdata  plt_buf_trans[FRAME_LENGTH+2];	// UART data buffer / PLT data buffer / APP data buffer


uchar bdata bit_data;
sbit bit0 = bit_data^0;
sbit bit1 = bit_data^1;
sbit bit2 = bit_data^2;
sbit bit3 = bit_data^3;
sbit bit4 = bit_data^4;
sbit bit5 = bit_data^5;
sbit bit6 = bit_data^6;
sbit bit7 = bit_data^7;

void InitWB(); 
void InitNBZC(); 
void InitNB();


void Mode_SCSF();
void Mode_DMZC();


void PLTSetting();
void SetPLT(uchar adr,uchar dat);
void WriteTXBUF(uchar *frame);
void ReadRXBUF(uchar *frame); 
void Zc_Delay();
//uint CRC16(uchar n, uchar *buf);
uchar  CheckSum8(uchar n, uchar *buf);
uchar GetRSSI();


/*********************************************************************************************
 * Function:           void InitPLT(void)
 * Input Parameter:    None
 * OutPut Parameter:   None
 * Return Value:       None
 * Description:        This function  Initializes Registers and variables of PLT Module 
**********************************************************************************************/
void InitPLT()
{	
	memset(&plt_trans,0,sizeof(PLT_TRANS));	

	plt_trans.plt_timeout = 0xFFFF;
		
	transmit_enable = 0;
	tx_flag = 0;
	rx_flag = 0;
	tx_ok_flag = 0;
	rx_ok_flag = 0;
	pd_flag = 0;
	fa_flag = 0;	
	zc_flag = 0;
	transmit_continue = 0;
	en_zc_delay = 0;
	en_delay_half = 0;
	once_ok = 0;
	rx_count = 0;
	nb_flag=0;
	nb_repeat=0;
	carrier_stage=0;

	plt_trans_timeout = 0xFFFF;	
    plt_recev_timeout = 0xFFFF;

	PLTSetting();
						
	return;
}


void PLTSetting()
{
	if (bModeOption)
	{
		if(nb_flag)
			InitNBZC();
		else 
			InitWB();
	}
	else
		InitNB();
}

/***********************************************************************************************
 * Function:          void PLTTask(void)
 * Input Parameter:   None
 * OutPut Parameter:  None
 * Return Value:      None
 * Description:       This function acts as a task of PLT Module. Main application must call
 *                    this function repeatdly to ensure the PLT transmit and receive data on time.                          
************************************************************************************************/
void PLTTask()
{	
	if (bModeOption)
		Mode_DMZC();
	else
		Mode_SCSF();
}


/**********************************************************************************************
 * Function:          void InitWB()
 * Input parameter:   None 
 * OutPut parameter:  None
 * Return Value:      None 
 * Description:       This funcition configure the register of PLT Module(WideBand, Enable FEC)               
 ***********************************************************************************************/

void InitWB()
{
	PLOCK = 0xA5;	    //UnLock
	PLCON |= 0x4;		//SRST

	PLCON = 0xE8;		//Enable PLT/CRC/FEC/ RSSIEN, 

	SetPLT(UMR1,0x00); 

	SetPLT(UMR2,FRAME_LENGTH); // SetPLT()  special process

	if (bChanOption)
	{
		SetPLT(UMR3,0x43); // bypass ABPF,DBPF, vincom input
		SetPLT(TXFC,0x5C); // 290KHz
		SetPLT(RXFC,0x5C); 
	}
	else					  //ybz!!!
	{
		SetPLT(UMR3,0x41); // bypass ABPF,DBPF, vin input
//		SetPLT(TXFC,0x1F); // 100KHz
//		SetPLT(RXFC,0x1F); 
		SetPLT(TXFC,0x5C); // 290KHz, 2014/2/24
		SetPLT(RXFC,0x5C); 
	}

	SetPLT(SSCI,0x80); // 
	SetPLT(SSCQ,0x00); // 
	SetPLT(TACQ,0x10); // default
	SetPLT(TFA,0x0A);  // default	
    CLKCON = 0x00;      // PLTCLK = SYSCLK
	PLOCK = 0x5A;	//Lock	  

}





/**********************************************************************************************
 * Function:          void InitNB()
 * Input parameter:   None 
 * OutPut parameter:  None
 * Return Value:      None 
 * Description:       This funcition configure the register of PLT Module(NarrowBand Normal Speed)               
 ***********************************************************************************************/
void InitNB()
{
	PLOCK = 0xA5;	     //UnLock
	PLCON |= 0x4;		//SRST
	PLCON = 0xE8;		//Enable PLT/CRC/FEC/ RSSIEN, FACPR=0	
	
	SetPLT(UMR1,0x80);  //Narrow band, normal


	SetPLT(UMR2,FRAME_LENGTH); // 20 bytes, care SetPLT() special			

	if (bChanOption)
	{
		SetPLT(UMR3,0x43); // bypass ABPF,DBPF, vincom input
		SetPLT(TXFC,0x5C); // 290KHz
		SetPLT(RXFC,0x5C); 
	}
	else
	{
		SetPLT(UMR3,0x41); // bypass ABPF,DBPF, vin input
//		SetPLT(TXFC,0x1F); // 100KHz
//		SetPLT(RXFC,0x1F); 
		SetPLT(TXFC,0x5C); // 290KHz, 2014/2/24
		SetPLT(RXFC,0x5C); 
	}
	SetPLT(NACQCT,0x06); 

	// 2014/5/27
	SetPLT(NACQDT,0x90);		// central 160 (0xA0) 2013-12-17 modify
	SetPLT(NACQUT,0xB0);

	PLOCK = 0x5A;	//Lock  

	return;
}
/**********************************************************************************************
 * Function:          void InitZERO()
 * Input parameter:   None 
 * OutPut parameter:  None
 * Return Value:      None 
 * Description:       This funcition configure the register of PLT Module(SSLP)               
 ***********************************************************************************************/
void InitNBZC()
{
	PLOCK = 0xA5;	    //UnLock PLT REG
	PLCON |= 0x4;		//SRST   PLT模块软复位，自动解锁
	PLCON = 0x88;		//Enable PLT/RSSIEN, Disable CRC/FEC FACPR = 0	

	SetPLT(UMR1,0xC0); 	// Narrowband, Low Speed (超短帧调制 低速)

	SetPLT(UMR2,SSLP_LENGTH); 	// 4 bytes per fraction
	if (bChanOption)
	{
		SetPLT(UMR3,0x43); // bypass ABPF,DBPF, vincom input   0 dB
		SetPLT(TXFC,0x5C); // 290KHz
		SetPLT(RXFC,0x5C); 
	}
	else
	{
		SetPLT(UMR3,0x41); // bypass ABPF,DBPF, vin input	   0 dB	
//		SetPLT(TXFC,0x1F); // 100KHz
//		SetPLT(RXFC,0x1F); 
		SetPLT(TXFC,0x5C); // 290KHz, 2014/2/24
		SetPLT(RXFC,0x5C); 
	}
	SetPLT(NACQCT,0x06);   // 同步参数 默认值  ??

	// 2014/5/27
	SetPLT(NACQDT,0x90);		// central 160 (0xA0) 2013-12-17 modify	??
	SetPLT(NACQUT,0xB0);	    //  										??

	PLOCK = 0x5A;		//Lock	
	return;
}



/**********************************************************************************************
 * Function:          void  Mode_NBSF()
 * Input parameter:   None
 * OutPut parameter:  None
 * Return Value:      None 
 * Description:       Single Frame Mode              
 ***********************************************************************************************/

void Mode_SCSF()
{
	if(transmit_enable)      
	{
		if(((PLCON&0x01)==0x00)&&(!tx_flag))
		{
					
			WriteTXBUF(plt_buf_trans);			

		   	if (RXINT||rx_flag)     // dummy
		   	{
		    	RXINT = 0;
		   	 	rx_flag = 0;
		   	}
			
		   	PLCON |= 0x01;  

		   	if (RXINT||rx_flag)     // dummy
		   	{
		    	RXINT = 0;
		    	rx_flag = 0;
		   	}
		   	plt_trans_timeout = FRAME_DURATION_NBSF_TX;

		}
        else if (tx_flag)
        {            			
			tx_flag = 0;
//			tx_ok_flag = 1;
			transmit_enable = 0;
//			plt_trans_timeout = 0xFFFF;	
//			memset(plt_buf_trans,0,FRAME_LENGTH);												

			plt_trans_timeout = 10;
        }	
	}
	else          
	{
		if(rx_flag)
		{   	            	   
 			rx_flag = 0;

			ReadRXBUF(plt_buf_trans);
			rx_ok_flag = 1;
			pd_flag = 0;
			fa_flag = 0;
			plt_recev_timeout = 0xFFFF;
		}	
		else if((!pd_flag)&&RXPD)
		{
			pd_flag = 1;
			plt_recev_timeout = FRAME_DURATION_NBSF_RX;
		}	
		 		
	    else if ((!fa_flag)&&RXFA)  
	    {       
	        fa_flag = 1;    
	 		rssi = GetRSSI();
	    }

	    if(!RXPD)
			pd_flag = 0;    
	    if (!RXFA)	
	    	fa_flag = 0;  				
	}

	if (!plt_recev_timeout)
	{		
		InitPLT();		// dummy
		plt_recev_timeout = 0xFFFF;
	}
	
	if (!plt_trans_timeout)
	{
			tx_ok_flag = 1;
			transmit_enable = 0;
			plt_trans_timeout = 0xFFFF;	
			//memset(plt_buf_trans,0,FRAME_LENGTH);	
	}

	return;
}






/**********************************************************************************************
 * Function:          void  Mode_DMZC()
 * Input parameter:   None
 * OutPut parameter:  None
 * Return Value:      None 
 * Description:       WideBand&SSLP           
 ***********************************************************************************************/
void Mode_DMZC()
{
	uchar i;	
	uint	crc_temp = 0;
	uchar	PltDataTemp0 = 0;
	uchar	PltDataTemp1 = 0;
	uint	crc_temp00 = 0;	

	if(transmit_enable)
	{		
		if(!nb_flag)
		{			
			if (tx_flag)		
			{
				tx_flag = 0;					
				transmit_continue = 0;	// dummy		
				nb_flag = 1;		  

				IE1 = 0;			 // drop  tx_timeout_count
				TCON |= 0x04;
				zc_flag = 0;
				EX1 = 1;	
			}

			else
			{
				if((PLCON&0x01)==0x00)    		
				{	
					nb_flag = 0;	// dummy				
					PLTSetting();					

					WriteTXBUF(plt_buf_trans);
					if (RXINT||rx_flag)		// dummy
					{
						RXINT = 0;
						rx_flag = 0;
					}
					PLCON |= 0x01; 	 
					if (RXINT||rx_flag)			// dummy
					{
						RXINT = 0;
						rx_flag = 0;
					}
					plt_trans_timeout = FRAME_DURATION_DMZC_TX;	
				}
			}
		}
		else 
		{			
			if(zc_flag)
			{
				zc_flag = 0;
				en_zc_delay = 1;
				Zc_Delay();
				nb_repeat = 0;		// dummy		
				//crc_temp = CRC16(FRAME_LENGTH,plt_buf_trans);

				//plt_buf_trans[FRAME_LENGTH] = crc_temp;
				//plt_buf_trans[FRAME_LENGTH+1] = crc_temp>>8;

				plt_buf_trans[FRAME_LENGTH] = CheckSum8(FRAME_LENGTH,plt_buf_trans);	// modify checksum

				PLTSetting();				
			}

		 	if(tx_flag)
			{
				tx_flag = 0;
				if(nb_repeat>=NB_CNT)
				{
					plt_trans_timeout = FRAME_DURATION_DMZC_TXTAIL;			// care, can't return directly
					transmit_enable = 0;
								
				}
			}
			else if(((PLCON&0x01)==0x00)&&transmit_continue)		// PLCON&0x01 dummy			
			{					
				for(i=0;i<SSLP_LENGTH;i++)
				{
					PLADR1= i;
					PLBUF = plt_buf_trans[i+SSLP_LENGTH*nb_repeat];
		    	}				
				nb_repeat++;

				if (RXINT||rx_flag)			
				{
					RXINT = 0;
					rx_flag = 0;
				}
				PLCON |= 0x01; 
				if (RXINT||rx_flag)				
				{
					RXINT = 0;
					rx_flag = 0;
				}
				transmit_continue = 0;	
			    en_delay_half = 1;
				Zc_Delay();
			}			
		 }
	}
	else          
	{
	    if(plt_recev_timeout==0)
        { 	
			rx_ok_flag = 0;			
            if (once_ok)
      	     	rx_ok_flag = 1;    							
            else
            {
        		if(rx_count>=ZERO_TOTAL_LEN)
        		{
        			//crc_temp = CRC16(FRAME_LENGTH,(uchar*)(&(plt_buf_trans[0])));
        			//PltDataTemp0 = plt_buf_trans[FRAME_LENGTH];        //CRCL
        		    //PltDataTemp1 = plt_buf_trans[FRAME_LENGTH+1];      //CRCH
    
        			//if(((PltDataTemp1<<8)|PltDataTemp0)==crc_temp)

        			PltDataTemp0 = plt_buf_trans[FRAME_LENGTH];        //CRCL
        		    PltDataTemp1 = CheckSum8(FRAME_LENGTH,(uchar*)(&(plt_buf_trans[0])));
					if (PltDataTemp0==PltDataTemp1)   
        				 rx_ok_flag = 1; 																
        		}                
            }				
            rx_count = 0;  // reset			
			once_ok = 0;	
			fa_flag = 0;	// modify
			pd_flag = 0;
   		    nb_flag = 0;				
			PLTSetting();	  // care, will modify frame_duration again
            carrier_stage = 0;
			plt_recev_timeout = 0xFFFF;												
		}

		if(rx_flag)
		{   
			if (!once_ok)
			{
			    if (!nb_flag)		// WB ok
		        {  	
					ReadRXBUF(plt_buf_trans);					// debug, bypass WB receive
				    once_ok = 1;  											
				}				// NB stage
				else
				{			              
					if(rx_count<ZERO_TOTAL_LEN)		
					{
	            		for(i=0;i<SSLP_LENGTH;i++)
	       				{
					   		PLADR1 = i;
							plt_buf_trans[rx_count] = PLBUF;
							rx_count++;
	       		    	}
					}               
	            } 
			}       
			rx_flag = 0; 
		}
	
		else if((!pd_flag)&&RXPD)
		{
			pd_flag = 1; 
				
			if(!carrier_stage)  
            {	
				carrier_stage = 1;	  // first entrance, WB stage

				plt_recev_timeout = FRAME_DURATION_DMZC_RX;					
				once_ok = 0;		// dummy
				nb_flag = 0;		// dummy
				rx_count = 0;		// dummy

				fa_flag = 0;
			}
						
		}

		else if ((!fa_flag)&&RXFA&&(!nb_flag))  
	    {       
	        fa_flag = 1;    // modify, only entrance once

			rssi = GetRSSI();
	    } 

 	    if((!RXPD)&&pd_flag)	
		{		
			pd_flag = 0;

			if (!nb_flag)
			{
				en_delay_flag = 1;
				Timer_count = 0;
			}	
		}

		if (frame_transmit_ready)
		{
			frame_transmit_ready = 0;
			nb_flag = 1;
			PLTSetting();			
		}
	}
	if (plt_trans_timeout==0)		
	{
		plt_trans_timeout = 0xFFFF;		
		nb_repeat = 0;
		nb_flag = 0;
		zc_flag = 0;	// dummy
		PLTSetting();
		transmit_continue = 0;
		transmit_enable = 0;
		en_delay_half = 0;
		en_zc_delay = 0;	//dummy
		tx_ok_flag = 1;		// ok, care, avoid flow halt	

		//memset(plt_buf_trans,0,FRAME_LENGTH+2);		// dummy						
	}
}





/***********************************************************************************************
 * Function:          void PLT_TimerTask(void)
 * Input Parameter:   None
 * OutPut Parameter:  None
 * Return Value:      None
 * Description:       This function deal with the timer Task of PLT Module. it is called in Timer0 
     				  interrupt routine to make sure the timer task of PLT is run on time.
************************************************************************************************/
void PLT_TimerTask()
{
	// reserve
	if(en_delay_flag&&(Timer_count>=FRAME_INTERVAL))  
	{
		en_delay_flag = 0;
		frame_transmit_ready = 1;
	}
		
	if(en_zc_delay&&(Timer_count>=zc_delay_cnt))			 
	{
		en_zc_delay = 0;
		transmit_continue = 1;		
	}
	if(en_delay_half&&(Timer_count>=zc_delay_cnt))		//half period of AC
	{
		en_delay_half = 0;
		transmit_continue = 1;
	}
	

	if(plt_trans_timeout && plt_trans_timeout!=0xFFFF)                      //receive flow timeout
   		 --plt_trans_timeout; 
	if(plt_recev_timeout && plt_recev_timeout!=0xFFFF)                      //receive flow timeout
   		 --plt_recev_timeout; 

	if(plt_trans.plt_timeout && plt_trans.plt_timeout!=0xFFFF)                      //receive flow timeout
   		 --plt_trans.plt_timeout; 
	if(uart_trans.uart_timeout && uart_trans.uart_timeout!=0xFFFF)                   
   		 --uart_trans.uart_timeout; 
}


/*******************************************************************************
 * Function:          bit  Transmit()
 * Input parameter:   None
 * OutPut parameter:  None
 * Return Value:      None
 * Description:       This funcition set the flag to start transmit              
 *******************************************************************************/
bit Transmit(void)
{
	if(RXINT||(PLCON&0x1)||TXINT)
	{
		RXINT = 0;   	// dummy
		TXINT = 0;
		return 0;       
	}   
	else 
	{
		transmit_enable = 1; 
		plt_trans_timeout = 0xFFFF;		
		tx_flag = 0;	
		tx_ok_flag = 0;		
		return 1;  
	}
}

/*******************************************************************************
 * Function:          void WriteTXBUF(uchar *frame)
 * Input parameter:   frame    -pointer to to the  begining of tramsmit data packet
 * OutPut parameter:  None
 * Return Value:      None
 * Description:       This funcition write the data to TXBUF              
 *******************************************************************************/
void WriteTXBUF(uchar *frame)
{
	uchar i; 
	for(i=0; i<FRAME_LENGTH; i++)
	{
		PLADR1 = i;        
		PLBUF = *(frame++);
	}
}

/*******************************************************************************
 * Function:          void ReadRXBUF(uchar *frame)
 * Input parameter:   frame    -pointer to to the  begining of tramsmit data packet
 * OutPut parameter:  None
 * Return Value:      None
 * Description:       This funcition read the data from RXBUF              
 *******************************************************************************/
void ReadRXBUF(uchar *frame)
{
	uchar i; 
	for(i=0; i<FRAME_LENGTH; i++)
	{
		PLADR1 = i;        
		*(frame++) = PLBUF;
	}
}

/*******************************************************************************
 * Function:          void SetPLT(uchar adr, uchar dat)
 * Input parameter:   adr, dat
 * OutPut parameter:  None
 * Return Value:      None
 * Description:       This funcition is to set one PLT register              
 *******************************************************************************/
void SetPLT(uchar adr, uchar dat)
{
	
	PLADR2 = adr;
	if(PLADR2==UMR2)
	{
		PLREG = (dat/11)<<6;	 //FEC 有关
		PLREG = PLREG + dat;
	}
	else 
	 	PLREG = dat;
}
/*******************************************************************************
 * Function:          uchar GetRSSI()
 * Input parameter:   None
 * OutPut parameter:  None
 * Return Value:      None
 * Description:       This funcition is to Calculate RSSI of receive Signal              
 *******************************************************************************/
uchar GetRSSI()
{	
    uchar m_nRssiCount=0;
    uint  m_nRssiSum=0;
	uint  m_nRssiTemp=0;
	uchar m_nRssiH=0;
	uchar m_nRssiL=0;	
	
	ADCON = 0x80;		 // Open ADC
	ADT = 0xA8;		     //Tad=16*tsys, Tsamp=(8+1)*Tad, Ttotal=(12+9)*Tad=21*16*tsys= 0.021ms (47.6KHz)
    while(m_nRssiCount<8)        //the average of 8 samples
    {
        ADCON |= 0x01;
        while((ADCON&0x01)==0x01);
		PLADR2=PLRSSIH;
		m_nRssiH=PLREG;
		PLADR2=PLRSSIL;
		m_nRssiL=PLREG;
		m_nRssiTemp = 63+47*(ADDH*1.65-m_nRssiH)/(m_nRssiL-m_nRssiH);	 // correct
		m_nRssiSum+=m_nRssiTemp;

        ADCON&=0x0BF;              
        m_nRssiCount++;
    }
	ADCON &= 0x7F;		// Close ADC	 (for low power)

    return (255-(m_nRssiSum>>3)); 	 // reverse RSSI curve    
}

/*******************************************************************************
 * Function:          uint  CRC16(uchar n, uchar *buf)
 * Input parameter:   n---FrameLength, *buf--the point to the buginning of data packet 
 * OutPut parameter:  None
 * Return Value:      None
 * Description:       This funcition is to Calculate  CRC16 check code            
 *******************************************************************************/

//uint  CRC16(uchar n, uchar *buf)
//{
//    uchar i, j;
//	uint crc;
//	crc = 0x0A5A5;         
//	
//	if (n>30)	n %= 30;  	// n<=30
//	for(j=0;j<n;j++)
//	{
//	    bit_data = *(buf++);	
//		
//		for(i=0;i<8;i++)
//		{
//			crc=crc<<1;
//			if(bit0^CY)
//				crc=(crc^0x8004)|0x0001;
//			bit_data=bit_data>>1;
//		}
//	}
//	return crc;
//}

uchar  CheckSum8(uchar n, uchar *buf)
{
    uchar i;
	uchar checksum;
	   
	checksum = 0;
	if (n>31)	n = 31;		
	for(i=0;i<n;i++)
	{
	    checksum += *(buf++);			
	}
	return checksum;
}


/*******************************************************************************
 * Function:          void Zc_Delay()
 * Input parameter:   None
 * OutPut parameter:  None
 * Return Value:      None
 * Description:       This funcition is to set timeout parameter in 50Hz or in 60Hz            
 *******************************************************************************/
void Zc_Delay()
{			
	Timer_count = 0;	
	if(bFreqOption)						//AC: 50Hz
	{
		if(en_zc_delay)					//delay 7.5ms(10-2.5) after zero cross detect
		{
			TL0 = -MS_05;
			TH0 = (-MS_05)>>8;
			//zc_delay_cnt = 8;
			zc_delay_cnt = 18;		   // delay 17.5ms
		}
		else if(en_delay_half)			//delay 10ms(half period of AC)
	    {
			TL0	= -TIMER0;    
			TH0	= (-TIMER0)>>8;
			zc_delay_cnt = 10;
		
		}
	}
	else								// AC: 60Hz
	{
		if(en_zc_delay)					//delay 5.9ms(8.3-2.4) after zero cross detect
		{
			//TL0 = -MS_09;
			//TH0 = (-MS_09)>>8;
			//zc_delay_cnt = 6;

			TL0 = -MS_02;
			TH0 = (-MS_02)>>8;
			zc_delay_cnt = 15;		   // delay 14.2ms
		}
		else if(en_delay_half)			//delay 8.3ms(half period of AC)
		{
			TL0 = -MS_03;
			TH0 = (-MS_03)>>8;
			zc_delay_cnt = 9;
		}
	}	
	return;
}