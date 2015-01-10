/*******************************************************************
	版权声明:
	文件功能:LCD驱动文件
	备注说明:
********************************************************************/

#include "include.h"


/*--------------------全局常量定义---------------------*/
uint8 com_sign_hold_ct=0;


/*--------------------内部函数申明-------------------------*/

void Com_Sign_Driver(void);

const uint16 DigitSeg[14][7]={
{SEG_Qseg1a,SEG_Qseg1b,SEG_Qseg1c,SEG_Qseg1d,SEG_Qseg1e,SEG_Qseg1f,SEG_Qseg1g},
{SEG_Qseg2a,SEG_Qseg2b,SEG_Qseg2c,SEG_Qseg2d,SEG_Qseg2e,SEG_Qseg2f,SEG_Qseg2g},
{SEG_Qseg3a,SEG_Qseg3b,SEG_Qseg3c,SEG_Qseg3d,SEG_Qseg3e,SEG_Qseg3f,SEG_Qseg3g},
{SEG_Qseg4a,SEG_Qseg4b,SEG_Qseg4c,SEG_Qseg4d,SEG_Qseg4e,SEG_Qseg4f,SEG_Qseg4g},
{SEG_Qseg5a,SEG_Qseg5b,SEG_Qseg5c,SEG_Qseg5d,SEG_Qseg5e,SEG_Qseg5f,SEG_Qseg5g},
{SEG_Qseg6a,SEG_Qseg6b,SEG_Qseg6c,SEG_Qseg6d,SEG_Qseg6e,SEG_Qseg6f,SEG_Qseg6g},
{SEG_Qseg7a,SEG_Qseg7b,SEG_Qseg7c,SEG_Qseg7d,SEG_Qseg7e,SEG_Qseg7f,SEG_Qseg7g},
{SEG_Qseg8a,SEG_Qseg8b,SEG_Qseg8c,SEG_Qseg8d,SEG_Qseg8e,SEG_Qseg8f,SEG_Qseg8g},
{SEG_Qseg9a,SEG_Qseg9b,SEG_Qseg9c,SEG_Qseg9d,SEG_Qseg9e,SEG_Qseg9f,SEG_Qseg9g},
{SEG_Qseg10a,SEG_Qseg10b,SEG_Qseg10c,SEG_Qseg10d,SEG_Qseg10e,SEG_Qseg10f,SEG_Qseg10g},
{SEG_Qseg11a,SEG_Qseg11b,SEG_Qseg11c,SEG_Qseg11d,SEG_Qseg11e,SEG_Qseg11f,SEG_Qseg11g},
{SEG_Qseg12a,SEG_Qseg12b,SEG_Qseg12c,SEG_Qseg12d,SEG_Qseg12e,SEG_Qseg12f,SEG_Qseg12g},
{SEG_Qseg13a,SEG_Qseg13b,SEG_Qseg13c,SEG_Qseg13d,SEG_Qseg13e,SEG_Qseg13f,SEG_Qseg13g},
{SEG_Qseg14a,SEG_Qseg14b,SEG_Qseg14c,SEG_Qseg14d,SEG_Qseg14e,SEG_Qseg14f,SEG_Qseg14g},
};


const uint16 Uintdisp[19]={
SEG_P1,SEG_P2,SEG_P3,SEG_P4,SEG_P5,SEG_P6,SEG_P7,SEG_P8,SEG_P9,SEG_P10,
SEG_k,SEG_V,SEG_X3,SEG_X4,SEG_X5,SEG_X6,SEG_X7,SEG_Z,SEG_cos};


const uint8 DigitNum[38]={
     0xc0,//0 0
     0xf9,//1 1
     0xa4,//2 2
     0xb0,//3 3
     0x99,//4 4
     0x92,//5 5
     0x82,//6 6
     0xf8,//7 7
     0x80,//8 8
     0x90,//9 9
     0x88,//10 A
     0x83,//11 B
     0xc6,//12 C
     0xa1,//13 d
     0x86,//14 e
     0x8e,//15 f
     0x90,//16 g
     0x89,//17 h
     0xef,//18 i
     0xe1,//19 j
     0xc7,//20 L
     0xaa,//21 m
     0xC8,//22 n
     0xa3,//23 0
     0x8c,//24 p
     0x98,//25 q
     0xaf,//26 r
     0x92,//27 s
     0x87,//28 t
     0xc1,//29 u
     0xe3,//30 v
     0x95,//31 w
     0x9b,//32 x
     0x91,//33 y
     0xa4,//34 z
     0xbf,//35 -
     0xff,
     0xf7,
};

//变量定义-----------------------------------
uint8 LCDMEM[40];
/*--------------------全局变量定义---------------------*/


/*********************************************************** 
函数功能：全显
入口参数：
出口参数：
备    注：
***********************************************************/
void SetLCD(void)
{
   LCD_FillAllRAMArea();
}

/*********************************************************** 
函数功能：清屏
入口参数：
出口参数：
备    注：
***********************************************************/
void ClrLCD(void)
{
    LCD_ClrAllRAMArea();
}

/*********************************************************** 
函数功能：LCD显示一个字符
入口参数：
出口参数：
备	          
***********************************************************/
void SetSegment(uint16 nSegment,uint8 B)
{
	uint8 dot;
	if(nSegment>=320)
  	return;

  	dot=0x01<<(nSegment%8);
  	if (!B)
  	{
  		LCDMEM[nSegment>>3] |=dot;
  	}
  	else 
  	{
  		LCDMEM[nSegment>>3] &=(~dot);
  	}
  return;
}


/*********************************************************** 
函数功能：LCD数据区显示一个数字
入口参数：
出口参数：
备	         
***********************************************************/
void SetDigit(uint8 nDigit,uint8 B)
{
	uint8 i,Temp0;
	nDigit--;
	if (nDigit>14)return;  //数值范围1---14
	Temp0=DigitNum[B];
	for(i=0;i<0x7;i++)
	{
		SetSegment(DigitSeg[nDigit][i],Temp0&0x1);
		Temp0=Temp0>>1;
	}
	return;
}
/*********************************************************** 
函数功能：LCD显示刷新
入口参数：
出口参数：
备	         
***********************************************************/
void RefreshLCD(void)
{
    uint8 i;
    for(i=0;i<40;i++)
    {
        *((volatile uint8*)(&FM3_LCDC->LCDRAM00 + i)) = LCDMEM[i];
    }

}

/*********************************************************** 
函数功能：显示单位函数
入口参数：
出口参数：
备	         
***********************************************************/
void Display_OtherMessage(const uint16 *Buf)
{
  while(*Buf!=0xff)
  {
	SetSegment(*Buf,0);
	Buf++;
  }
  return;
}

/*********************************************************** 
函数功能：缓存清除函数
入口参数：
出口参数：
备	         
***********************************************************/
void LCD_Dispdata_Clr(void)
{
    uint8 i,j;
    for(i=0;i<19;i++)
    {  
        SetSegment(Uintdisp[i],1);
    } 
    for(j=0;j<13;j++)
    {
        SetDigit(j+1,36); 
    }
}

void DispdataNodisp(void)
{
  SetSegment(SEG_alarm,1);
  SetSegment(SEG_cover,1);
  SetSegment(SEG_term,1);
  SetSegment(SEG_battery1,1);
  //=============================
  //ceshi看门狗复位显示电池符号，发生产要删除掉
  //if(RTCResetRecord[1] != 0)
  //{
  //  SetSegment(SEG_battery1,0);
  //}
  //=============================
  SetSegment(SEG_T,1);
  SetDigit(14,36);
}   


void SetallLCD(void)
{
    uint8 i;
    for(i=0; i<sizeof(LCDMEM); i++)
    {
        LCDMEM[i] = 0xff;
    }
    
}
void ClrallLCD(void)
{
    uint8 i;
    for(i=0; i<sizeof(LCDMEM); i++)
    {
        LCDMEM[i] = 0x00;
    }
    
}

/*********************************************************** 
函数功能：符号闪烁函数
入口参数：
出口参数：
备	         
***********************************************************/
void LCD_Flag_Driver_Task(void) 
{
    Com_Sign_Driver();
}


void Flag_Flicker(uint16 Seg,uint8 i)
{
    if (i)
    {
        SetSegment(Seg,0);
    }
    else
    {
        SetSegment(Seg,1);
    }
    return;
}



/*********************************************************** 
函数功能：通讯符合驱动	1000ms
入口参数：
出口参数：
备 	  注：1 通讯符合常亮
              2 通讯符合定宽点亮
              3 通讯符合熄灭
***********************************************************/
void Com_Sign_Driver(void)
{
    if(com_sign_hold_ct)		
    {
        SetSegment(SEG_comm,0);
        if(PowerDownDetect()==1)
        {
            com_sign_hold_ct=0;
            SetSegment(SEG_comm,1);
        }
        else
        {
            com_sign_hold_ct--;
        }
    }
    else				
    {
        SetSegment(SEG_comm,1);
    } 
}


/*********************************************************** 
函数功能：获取通讯状态函数	
入口参数：
出口参数：
备 	  注：
***********************************************************/
void Com_Sign_flag(void)
{
    uint8 i;
    for(i=UART_VT; i<UART_EMU_L1; i++)
    {
        if(IDLE != LDRV_UsartGetStateRx((COM_TYPEDEF)i))
        {
            com_sign_hold_ct=3;
            break;
        }
    } 
}



/*void LCD_test(void)
{
    uint16 i,j,k=8;
    DRV_LCD_Init(0);
 for(i=0;i<160;i++)
     {  
           SetSegment(fuhao[i],0);
           RefreshLCD();
           DelayMs(1000);
           SetSegment(fuhao[i],1);
           DelayMs(500);
     }     
      for(j=1;j<14;j++)
           {
               //for(k=0;k<10;k++)
               //{   
                SetDigit(j,k);
                //RefreshLCD();
                //DelayMs(1000);
                //SetDigit(j,36);
               //}
       
     } 
      //Display_OtherMessage(&KVarh[0]);
      //DelayMs(1000);
      RefreshLCD();     
}*/ 


