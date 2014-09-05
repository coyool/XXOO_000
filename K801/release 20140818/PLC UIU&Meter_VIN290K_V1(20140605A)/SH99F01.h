/*--------------------------------------------------------------------------
REG99F01.inc

Header file for generic SH99xxx series microcontroller.
Copyright (c) 1996-2007 Sionwealth Electronic and Sinowealth Software, Inc.
All rights reserved.
--------------------------------------------------------------------------*/

#ifndef REG99F01_H
#define REG99F01_H
/* ------------------- BYTE Register-------------------- */
/* CPU */
sfr PSW         = 0xD0;
sfr ACC         = 0xE0;
sfr B           = 0xF0;
sfr AUXC       	= 0xF1;
sfr XPAGE       = 0xF7;
sfr SP          = 0x81;
sfr DPL         = 0x82;
sfr DPH         = 0x83;
sfr DPL1        = 0x84;
sfr DPH1        = 0x85;
sfr INSCON      = 0x86;

/* FLASH */
sfr IB_CON1     = 0xF2;
sfr IB_CON2     = 0xF3;
sfr IB_CON3     = 0xF4;
sfr IB_CON4     = 0xF5;
sfr IB_CON5     = 0xF6;
sfr IB_OFFSET   = 0xFB;
sfr IB_DATA     = 0xFC;	
sfr FLASHCON	= 0xA7;

/* SYSTEM */
sfr PCON        = 0x87;
sfr SUSLO       = 0x8E;
sfr RSTSTAT     = 0xB1;
sfr CLKCON      = 0xB2;

/* PORT */
sfr P0          = 0x80;
sfr P1          = 0x90;
sfr P0CR        = 0xE1;
sfr P1CR        = 0xE2;
sfr P0PCR       = 0xE9;
sfr P1PCR       = 0xEA;

/* TIMER 0/1 */
sfr TCON        = 0x88;
sfr TMOD        = 0x89;
sfr TL0         = 0x8A;
sfr TL1         = 0x8B;
sfr TH0         = 0x8C;
sfr TH1         = 0x8D;
sfr TCON1       = 0xCE;

/* TIMER 2 */
sfr T2CON       = 0xC8;
sfr T2MOD       = 0xC9;
sfr RCAP2L      = 0xCA;
sfr RCAP2H      = 0xCB;
sfr TL2         = 0xCC;
sfr TH2         = 0xCD;

/* INTERRUPT */
sfr IEN0        = 0xA8;
sfr IEN1        = 0xA9;
sfr IPL0        = 0xB8;
sfr IPL1        = 0xB9;
sfr IPH0        = 0xB4;
sfr IPH1        = 0xB5;

/* EUART 0 */
sfr SCON        = 0x98;
sfr SBUF        = 0x99;
sfr SADDR       = 0x9A;
sfr SADEN       = 0x9B;
		
/* ADC */
sfr ADCON       = 0x93;
sfr ADT         = 0x94;		
sfr ADCH        = 0x95;
sfr ADDL        = 0x96;
sfr ADDH        = 0x97;


/* PLT */
sfr PLSTAT      = 0xD8;
sfr PLCON       = 0xD9;
sfr PLADR1      = 0xDA;
sfr PLBUF       = 0xDB;
sfr PLADR2      = 0xDC;
sfr PLREG       = 0xDD;
sfr PLOCK       = 0xDF;

/*--------------------------  BIT Register -------------------- */

/*  PSW   */
sbit CY         = PSW^7;
sbit AC         = PSW^6;
sbit F0         = PSW^5;
sbit RS1        = PSW^4;
sbit RS0        = PSW^3;
sbit OV         = PSW^2;
sbit F1         = PSW^1;
sbit P          = PSW^0;

/*  T2CON  */
sbit TF2 = T2CON^7;
sbit EXF2 = T2CON^6;
sbit RCLK = T2CON^5;
sbit TCLK = T2CON^4;
sbit EXEN2 = T2CON^3;
sbit TR2 = T2CON^2;
sbit C_T2 = T2CON^1;
sbit CP_RL2 = T2CON^0;

/*  IPL0   */ 
sbit PADCL = IPL0^6;
sbit PT2L = IPL0^5;
sbit PSL  = IPL0^4;
sbit PT1L = IPL0^3;
sbit PX1L = IPL0^2;
sbit PT0L = IPL0^1;
sbit PX0L = IPL0^0;

/*  IEN0   */
sbit EA = IEN0^7;
sbit EADC = IEN0^6;
sbit ET2 = IEN0^5;
sbit ES = IEN0^4;
sbit ET1 = IEN0^3;
sbit EX1 = IEN0^2;
sbit ET0 = IEN0^1;
sbit EX0 = IEN0^0;

/*  SCON  */
sbit SM0_FE = SCON^7;
sbit SM1_RXOV = SCON^6;
sbit SM2_TXCOL = SCON^5;
sbit REN = SCON^4;
sbit TB8 = SCON^3;
sbit RB8 = SCON^2;
sbit TI = SCON^1;
sbit RI = SCON^0;

/*  TCON  */
sbit TF1 = TCON^7;
sbit TR1 = TCON^6;
sbit TF0 = TCON^5;
sbit TR0 = TCON^4;
sbit IE1 = TCON^3;
sbit IT1 = TCON^2;
sbit IE0 = TCON^1;
sbit IT0 = TCON^0;


/*  PLSTAT  */ 			
sbit TXINT = PLSTAT^7;
sbit FEC_DONE = PLSTAT^6;
sbit CFD_DONE = PLSTAT^5;
sbit RXNFA = PLSTAT^3;
sbit RXFA = PLSTAT^2;
sbit RXPD = PLSTAT^1;
sbit RXINT = PLSTAT^0;

/* P0 */
sbit P0_7 = P0^7;
sbit P0_6 = P0^6;
sbit P0_5 = P0^5;
sbit P0_4 = P0^4;
sbit P0_3 = P0^3;
sbit P0_2 = P0^2;
sbit P0_1 = P0^1;
sbit P0_0 = P0^0;

/* P1 */
sbit P1_7 = P1^7;
sbit P1_6 = P1^6;
sbit P1_5 = P1^5;
sbit P1_4 = P1^4;
sbit P1_3 = P1^3;
sbit P1_2 = P1^2;
sbit P1_1 = P1^1;
sbit P1_0 = P1^0;

#endif


