/*******************************************************************
	版权声明:
	文件功能:LCD初始化配置头文件
	备注说明:
********************************************************************/

#ifndef _SCREEN_H_
#define _SCREEN_H_
//***************************************************************************

#define SEG_alarm		        (uint16)7*8+0
#define SEG_clock			(uint16)7*8+1	
#define SEG_cos			        (uint16)7*8+2
#define SEG_comm			(uint16)7*8+3
#define SEG_T			        (uint16)12*8+3
#define SEG_battery1		        (uint16)10*8+0
#define SEG_battery2			(uint16)10*8+1
#define SEG_cover			(uint16)10*8+2
#define SEG_term			(uint16)10*8+3
#define SEG_k			        (uint16)9*8+3
#define SEG_V			        (uint16)9*8+2
#define SEG_X3		                (uint16)9*8+1
#define SEG_X4			        (uint16)9*8+0
#define SEG_X5			        (uint16)8*8+0
#define SEG_X6			        (uint16)8*8+1
#define SEG_X7			        (uint16)8*8+2
#define SEG_Z			        (uint16)8*8+3

#define SEG_P1			        (uint16)16*8+7
#define SEG_P2 			        (uint16)14*8+7
#define SEG_P3				(uint16)10*8+7
#define SEG_P4				(uint16)8*8+7
#define SEG_P5				(uint16)20*8+3
#define SEG_P6				(uint16)18*8+3
#define SEG_P7				(uint16)16*8+3
#define SEG_P8				(uint16)14*8+3
#define SEG_P9				(uint16)18*8+7
#define SEG_P10				(uint16)12*8+7

		
#define SEG_Qseg1a     (uint16)21*8+4
#define SEG_Qseg1b     (uint16)21*8+5
#define SEG_Qseg1c     (uint16)21*8+6
#define SEG_Qseg1d     (uint16)21*8+7
#define SEG_Qseg1e     (uint16)22*8+6
#define SEG_Qseg1f     (uint16)22*8+4
#define SEG_Qseg1g     (uint16)22*8+5

#define SEG_Qseg2a     (uint16)19*8+4
#define SEG_Qseg2b     (uint16)19*8+5
#define SEG_Qseg2c     (uint16)19*8+6
#define SEG_Qseg2d     (uint16)19*8+7
#define SEG_Qseg2e     (uint16)20*8+6
#define SEG_Qseg2f     (uint16)20*8+4
#define SEG_Qseg2g     (uint16)20*8+5


#define SEG_Qseg3a     (uint16)17*8+4
#define SEG_Qseg3b     (uint16)17*8+5
#define SEG_Qseg3c     (uint16)17*8+6
#define SEG_Qseg3d     (uint16)17*8+7
#define SEG_Qseg3e     (uint16)18*8+6
#define SEG_Qseg3f     (uint16)18*8+4
#define SEG_Qseg3g     (uint16)18*8+5


#define SEG_Qseg4a      (uint16)15*8+4
#define SEG_Qseg4b      (uint16)15*8+5
#define SEG_Qseg4c      (uint16)15*8+6
#define SEG_Qseg4d      (uint16)15*8+7
#define SEG_Qseg4e      (uint16)16*8+6
#define SEG_Qseg4f      (uint16)16*8+4
#define SEG_Qseg4g      (uint16)16*8+5


#define SEG_Qseg5a      (uint16)13*8+4
#define SEG_Qseg5b      (uint16)13*8+5
#define SEG_Qseg5c      (uint16)13*8+6
#define SEG_Qseg5d      (uint16)13*8+7
#define SEG_Qseg5e      (uint16)14*8+6
#define SEG_Qseg5f      (uint16)14*8+4
#define SEG_Qseg5g      (uint16)14*8+5


#define SEG_Qseg6a      (uint16)11*8+4
#define SEG_Qseg6b      (uint16)11*8+5
#define SEG_Qseg6c      (uint16)11*8+6
#define SEG_Qseg6d      (uint16)11*8+7
#define SEG_Qseg6e      (uint16)12*8+6
#define SEG_Qseg6f      (uint16)12*8+4
#define SEG_Qseg6g      (uint16)12*8+5


#define SEG_Qseg7a      (uint16)9*8+4
#define SEG_Qseg7b      (uint16)9*8+5
#define SEG_Qseg7c      (uint16)9*8+6
#define SEG_Qseg7d      (uint16)9*8+7
#define SEG_Qseg7e      (uint16)10*8+6
#define SEG_Qseg7f      (uint16)10*8+4
#define SEG_Qseg7g      (uint16)10*8+5



#define SEG_Qseg8a	(uint16)7*8+4
#define SEG_Qseg8b	(uint16)7*8+5
#define SEG_Qseg8c	(uint16)7*8+6
#define SEG_Qseg8d	(uint16)7*8+7
#define SEG_Qseg8e	(uint16)8*8+6
#define SEG_Qseg8f	(uint16)8*8+4
#define SEG_Qseg8g	(uint16)8*8+5



#define SEG_Qseg9a      (uint16)21*8+0
#define SEG_Qseg9b      (uint16)21*8+1
#define SEG_Qseg9c      (uint16)21*8+2
#define SEG_Qseg9d      (uint16)21*8+3
#define SEG_Qseg9e      (uint16)22*8+2
#define SEG_Qseg9f      (uint16)22*8+0
#define SEG_Qseg9g      (uint16)22*8+1



#define SEG_Qseg10a     (uint16)19*8+0
#define SEG_Qseg10b     (uint16)19*8+1
#define SEG_Qseg10c     (uint16)19*8+2
#define SEG_Qseg10d     (uint16)19*8+3
#define SEG_Qseg10e     (uint16)20*8+2
#define SEG_Qseg10f     (uint16)20*8+0
#define SEG_Qseg10g     (uint16)20*8+1

#define SEG_Qseg11a     (uint16)17*8+0
#define SEG_Qseg11b     (uint16)17*8+1
#define SEG_Qseg11c     (uint16)17*8+2
#define SEG_Qseg11d     (uint16)17*8+3
#define SEG_Qseg11e     (uint16)18*8+2
#define SEG_Qseg11f     (uint16)18*8+0
#define SEG_Qseg11g     (uint16)18*8+1


#define SEG_Qseg12a     (uint16)15*8+0
#define SEG_Qseg12b     (uint16)15*8+1
#define SEG_Qseg12c     (uint16)15*8+2
#define SEG_Qseg12d     (uint16)15*8+3
#define SEG_Qseg12e     (uint16)16*8+2
#define SEG_Qseg12f     (uint16)16*8+0
#define SEG_Qseg12g     (uint16)16*8+1

#define SEG_Qseg13a     (uint16)13*8+0
#define SEG_Qseg13b     (uint16)13*8+1
#define SEG_Qseg13c     (uint16)13*8+2
#define SEG_Qseg13d     (uint16)13*8+3
#define SEG_Qseg13e     (uint16)14*8+2
#define SEG_Qseg13f     (uint16)14*8+0
#define SEG_Qseg13g     (uint16)14*8+1

#define SEG_Qseg14a     (uint16)11*8+0
#define SEG_Qseg14b     (uint16)11*8+1
#define SEG_Qseg14c     (uint16)11*8+2
#define SEG_Qseg14d     (uint16)11*8+3
#define SEG_Qseg14e     (uint16)12*8+2
#define SEG_Qseg14f     (uint16)12*8+0
#define SEG_Qseg14g     (uint16)12*8+1

//七段码字型定义
#define SEGD0    0
#define SEGD1    1
#define SEGD2    2
#define SEGD3    3
#define SEGD4    4
#define SEGD5    5
#define SEGD6    6
#define SEGD7    7  
#define SEGD8    8
#define SEGD9    9
#define SEGDA    10
#define SEGDb    11
#define SEGDC    12
#define SEGDd    13
#define SEGDE    14
#define SEGDF    15
#define SEGDg    16
#define SEGDH    17
#define SEGDi    18
#define SEGDJ    19				
#define SEGDL    20
#define SEGDM    21
#define SEGDn    22
#define SEGDo    23
#define SEGDP    24
#define SEGDq    25
#define SEGDr    26
#define SEGDS    27
#define SEGDt    28
#define SEGDU    29
#define SEGDv    30
#define SEGDW    31
#define SEGDX    32
#define SEGDy    33
#define SEGDZ 	 34 						
#define SEGD_0   35							
#define SEGDNULL 36
#define SEGD_    37




extern uint8 com_sign_hold_ct;

/*--------------------外部函数申明-------------------------*/
void SetLCD(void);
void ClrLCD(void);
void SetSegment(uint16 nSegment,uint8 B);
void SetDigit(uint8 nDigit,uint8 B);
void RefreshLCD(void);
void Display_OtherMessage(const uint16 *Buf);
void LCD_test(void);
void LCD_Dispdata_Clr(void);
void LCD_Flag_Driver_Task(void);
void SetallLCD(void);
void ClrallLCD(void);
void Flag_Flicker(uint16 Seg,uint8 i);
void Com_Sign_flag(void);
void DispdataNodisp(void);
#endif 

