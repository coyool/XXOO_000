/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：A7139 config
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-9-29
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/




#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/
const u16 A7139_Reg_Config[]= //470MHz, 10kbps (IFBW = 50KHz, Fdev = 18.75KHz)
{
    0x0823,     /*0x0823(10K)-->0x3023(2K) 此时定时器时间加长*///SYSTEM CLOCK register,CSC=1,SDR=4 DataRate=10kbps(DMOS=1,so not use 0x0821)
//  0x1221,	    //SYSTEM CLOCK register,
	0x0A24,		//PLL1 register,
	0xB805,		//PLL2 register,	470.001MHz
	0x0000,		//PLL3 register,
	0x0A20,		//PLL4 register,
	0x0024,		//PLL5 register,
	0x0000,		//PLL6 register,
	0x0001,		//CRYSTAL register,
	0x0000,		//PAGEA,
	0x0000,		//PAGEB,	
    0x18D0,     //RX1 register, 	IFBW=50KHz, ETH=1	
	0x7009,		//RX2 register, 	by preamble
	0x4400,		//ADC register,	   	
	0x0800,		//PIN CONTROL register,		Use Strobe CMD
	0x4845,		//CALIBRATION register,
	0x20C0		//MODE CONTROL register, 	Use FIFO mode
};

const u16 A7139_Config_PageA[]=   //470MHz, 10kbps (IFBW = 50KHz, Fdev = 18.75KHz)
{
    0x1703,         /*0x1703--0x1260*///TX1 register, 	Fdev = 18.75kHz
//  0x1706,	S//TX1 register, 	Fdev = 37.5kHz
	0x0000,		//WOR1 register,
	0x0000,		//WOR2 register,
	0x1187,		//RFI register, 	Enable Tx Ramp up/down  
	0x8160,		//PM register,		CST=1
	0x0302,		//RTH register,
	0x400F,		//AGC1 register,	
	0x0AC0,		//AGC2 register, 
	0x0041,		//GIO register, 	GIO2=WTR, GIO1=WTR
//  0x0145,		//GIO register, 	GIO2=WTR, GIO1=WTR
	0xD281,		//CKO register
	0x0004,		//VCB register,
	0x0825,		//CHG1 register, 	480MHz
	0x0127,		//CHG2 register, 	500MHz
	0x003F,		//FIFO register, 	FEP=63+1=64bytes
	0x151F,		//CODE register, Preamble=4bytes, ID=4bytes, FEC+CRC
	0x0000		//WCAL register,
};

const u16 A7139_Config_PageB[]=   //470MHz, 10kbps (IFBW = 100KHz, Fdev = 18.75KHz)
{
	0x0B37,		/*0x0B7F-->0x0B37 733 721功率测试*///TX2 register, 	Tx power=20dBm
	0x8200,     //IF1 register, 	Enable Auto-IF, IF=100KHz     0x8400,IF=200KHz 
    0x0000,		//IF2 register,
	0x0000,		//ACK register,
	0x0000		//ART register,
};

//const u8 A7139_HopTab[] = {0,1,2,3,4}; //hopping channel

int A7139_Freq_Cal_Tab[]=
{
    0x0A24,	0xBA05,	//470.101MHz
	0x0A26, 0x4805,	//490.001MHz
	0x0A27, 0xD805,	//510.001MHz 
};

const u8 A7139_BitCnt_Tab[16] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
const u8 A7139_ID_Tab[8] = {0x34,0x75,0xC5,0x8C,0xC7,0x33,0x45,0xE7};  //ID code

/* PN9 random */
u8 PN9_Tab[]=    
{  
    0xFF,0x83,0xDF,0x17,0x32,0x09,0x4E,0xD1,
    0xE7,0xCD,0x8A,0x91,0xC6,0xD5,0xC4,0xC4,
    0x40,0x21,0x18,0x4E,0x55,0x86,0xF4,0xDC,
    0x8A,0x15,0xA7,0xEC,0x92,0xDF,0x93,0x53,
    0x30,0x18,0xCA,0x34,0xBF,0xA2,0xC7,0x59,
    0x67,0x8F,0xBA,0x0D,0x6D,0xD8,0x2D,0x7D,
    0x54,0x0A,0x57,0x97,0x70,0x39,0xD2,0x7A,
    0xEA,0x24,0x33,0x85,0xED,0x9A,0x1D,0xE0
};  

