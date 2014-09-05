/*********************************************************************
 *
 *           Sector Self-Program Module Task
 *
 *********************************************************************
 * FileName:        SSP.c
 * Dependencies:    Compiler.h
 * Processor:       SH99F01
 * Complier:        Keil C51 v9.00
 * Company:         Sinowealth Electronic, Inc.
 *
 * Software License Agreement
 *
 * This software is owned by Sinowealth Electronic Inc. ("Sinowealth") 
 * and is supplied to you for use exclusively as described in the 
 * associated software agreement.  This software is protected by 
 * software and other intellectual property laws.  Any use in 
 * violation of the software license may subject the user to criminal 
 * sanctions as well as civil liability.  Copyright 2009 Sinowealth
 * Electronic Inc.  All rights reserved.
 *
 * This software is provided "AS IS."  SINOWEALTH DISCLAIMS ALL 
 * WARRANTIES, EXPRESS, IMPLIED, STATUTORY OR OTHERWISE, NOT LIMITED 
 * TO MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND 
 * INFRINGEMENT.  Sinowealth shall in no event be liable for special, 
 * incidental, or consequential damages.
 *
 *
 *
 * Author               Date           Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Ellie Fan            2010/7/9	   Original  
 * Joseph Zhou          2011/3/21	   Modify                                  
 ********************************************************************/

#include  <SH99F01.h>
#include  "MAIN.h"
#include  "absacc.h"
#include  "SSP.h"

/****************************************************************************
 * Function:            uchar SSP_Read(uint adr)
 * Input Parameter:     adr      - physical address of flash memeory
 * OutPut Parameter:    None
 * Return Value:        1 byte data
 * Description:         This function read a byte of data from Flash(EEPROM)
 ***************************************************************************/

 // 2014/6/5,  SSP function bypass

 #if 0
/////////////////////////////////////

uchar SSP_Read(uint adr)
{	 
	uchar datatemp;
	datatemp = CBYTE[adr];
	return datatemp;
}

/****************************************************************************
 * Function:            void SSP_Write(uint adr, uchar dat)
 * Input Parameter:     adr      - physical address of flash memeory
 *                      dat      - data to be wirte
 * OutPut Parameter:    None
 * Return Value:        None
 * Description:         This function writes a byte of data into Flash
 ***************************************************************************/
void SSP_Write(uint adr, uchar dat)
{
    EA = 0;      
    FLASHCON = 0x01;		   	// EEPROM block
	XPAGE = (uchar)(adr>>8);	//page and offset
	IB_OFFSET = (uchar)adr;
	IB_DATA = dat;

	IB_CON1 = 0x6E;	  //Program
	IB_CON2 = 0x05;	  // >1MHz	
	IB_CON3 = 0x0A;	  //
	IB_CON4 = 0x09;
	IB_CON5 = 0x06;	
	
	NOP
	NOP
	NOP
	NOP    	  
	EA = 1;
	FLASHCON = 0x00;	
}


/**************************************************************************
 * Function:          SSP_Erase(uchar page)
 * Input parameter:   page      - sector index of flash memory 
 * Output parameter:  None
 * Return Value:      None
 * Description:       This funcition erases a sector of EEPROM
 **************************************************************************/
void SSP_Erase(uchar page)
{  
    EA = 0;   
	FLASHCON = 0x01;	   	// EEPROM block
	XPAGE = page;	   // page for Erase

	IB_CON1 = 0xE6;	   //Erase
	IB_CON2 = 0x05;	   // >1MHz	
	IB_CON3 = 0x0A;	   //
	IB_CON4 = 0x09;
	IB_CON5 = 0x06;

    NOP
	NOP
	NOP
	NOP
	FLASHCON = 0x00;
    EA = 1;  	

}



/**************************************************************************
 * Function:          SSP_CopyPage(uchar from, uchar to, uchar start, uchar end)
 * Input parameter:   from       -  source sector index 
 					  to         -  destination sector index
     				  start      -  the start offset address in a sector   
					  end        -  the end offset address in a sector
 * Output parameter:  None       
 * Return Value:      None       
 * Description:       This funcition copy the data(between start offset and end offset) from the souce sector to the destination section 
 **************************************************************************/
void SSP_CopyPage(uchar from, uchar to, uchar start, uchar end)
{
	uint data addr_from, addr_to;
	uint data addr = start;
	EA = 0;
	FLASHCON = 0x01;		  	// EEPROM block

    addr_from = from*PAGE_SIZE+start;    // calc address
    addr_to = to*PAGE_SIZE + start;

	XPAGE = to;	         //page and offset
	IB_OFFSET = start;

	while(addr<=end)
	{
		RSTSTAT = 0x00;            //  eat dog		
        IB_DATA = SSP_Read(addr_from);	
	
		IB_CON1 = 0x6E;	//Program
		IB_CON2 = 0x5;	// >1MHz	
    	IB_CON3 = 0xA;	//
     	IB_CON4 = 0x9;
     	IB_CON5 = 0x6;	
	
	    NOP
		NOP
		NOP
		NOP

		++IB_OFFSET;
		if(IB_OFFSET==0)
			++XPAGE;

		++addr_from;
		++addr_to;
		++addr;
	}
	FLASHCON = 0x00;
    EA = 1;	
}




/**************************************************************************
 * Function:          void Write_FLASH(uint adr, uchar dat)
 * Input parameter:   addr- data address; dat 
 * Output parameter:  None
 * Return Value:      None
 * Description:       This funcition is to write one byte data to flash
 **************************************************************************/
void Write_FLASH(uint adr, uchar dat)
{
	EA = 0;     
    FLASHCON = 0X00;   		// CODE block
	XPAGE = (uchar)(adr>>8);	//page and offset
	IB_OFFSET = (uchar)adr;
	IB_DATA = dat;

	IB_CON1 = 0x6E;	  //Program
	IB_CON2 = 0x05;	  // >1MHz	
	IB_CON3 = 0x0A;	  //
	IB_CON4 = 0x09;
	IB_CON5 = 0x06;	
	
	NOP
	NOP
	NOP
	NOP    	  
	EA = 1;	
}

/**************************************************************************
 * Function:          void Erase_FLASH(uchar page)
 * Input parameter:   page      - sector index of flash memory 
 * Output parameter:  None
 * Return Value:      None
 * Description:       This funcition erases a sector of flash
 **************************************************************************/

void Erase_FLASH(uchar page)
{
	EA = 0;
	FLASHCON = 0X00;	 	// CODE block
	XPAGE = (page<<2);

	IB_CON1 = 0xE6;	   //Erase
	IB_CON2 = 0x05;	   // >1MHz	
	IB_CON3 = 0x0A;	   //
	IB_CON4 = 0x09;
	IB_CON5 = 0x06;

    NOP
	NOP
	NOP
	NOP
    EA = 1;  	
	XPAGE = 0;
}

/////////////////////////////////////
#endif



