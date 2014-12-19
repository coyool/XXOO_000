/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-12-18
* 编译环境：D:\Program Files\IAR_430\430
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/
static void Serial_send(void);



/*** static variable declarations ***/
static const u8 Serial_bps[10][3]= 
{
	{0x3A,0x6D,0x44},    //300 波特率  		0   
	{0x9D,0x36,0x40},    //600 波特率 		1
	{0x4e,0x1b,0x02},    //1200 波特率  	2  
	{0xa7,0x0d,0x11},    //2400 波特率		3
	{0xd3,0x06,0x00},    //4800 波特率		4
	{0x69,0x03,0x6b},    //9600 波特率   	5
	{0xb4,0x01,0x2a},    //19200 波特率      6
	{0xda,0x00,0xee},    //38400 波特率      7
	{0x91,0x00,0x02},    //57600 波特率      8
	{0x48,0x00,0xae},    //115200 波特率     9
};

static const u8 fixed_TxRx_len = 60u;



/*** extern variable declarations ***/
SERIAL_TYPE Serial;





/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void Serial_begin(SERIAL_BPS_TYPE bps)
{
    P2SEL |= BIT4 | BIT5;        //开启P2.4,P2.5口第二功能
	U0CTL = SWRST; 	             //SWRST 串口复位 	
    U0ME |= (UTXE0 + URXE0);     //Enable USART0 TXD/RXD 	
    U0CTL &= ~SWRST;             //SWRST 串口复位结束 
	U0IE |= URXIE0;              //打开接收 中断  
	U0CTL = CHAR;                //data length  0:7 （8 bit)   
	U0TCTL = TXEPT;              //
	U0TCTL |= SSEL1;             //时钟源为SMCLK 8388608Mhz	
	if (bps >= 9) 
    {
        bps = 5;	
    }
    else
    {
        _NOP();
    }    
	U0BR0  = Serial_bps[bps][0];    // set bps
	U0BR1  = Serial_bps[bps][1]; 
	U0MCTL = Serial_bps[bps][2];    	
    
    memset(&Serial, 0, sizeof(Serial));  //clear Serial data 
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void Serial_end(void)
{
    
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void Serial_TxInt_enable(void)
{
    U0IFG &= ~UTXIFG0;    
	U0IE |= UTXIE0;       // 打开接收 中断     
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void Serial_TxInt_disable(void)
{
    U0IFG &= ~UTXIFG0;
	U0IE &= ~UTXIE0;      // 关闭发送 中断                 
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void Serial_RxInt_enable(void)
{
    U0IFG &=~URXIFG0;
	U0IE |= URXIE0;      // 打开接收 中断     
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void Serial_RxInt_disable(void)
{
    U0IFG &= ~URXIFG0;
	U0IE &= ~URXIE0;     // 关闭接收 中断             
}

/*******************************************************************************
* Description : send init and send first byte
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void Serial_TxMode(void)
{
    Serial_TxInt_enable(); 
    
    Serial.TxFlag = 0u;     // sending   
    Serial.TxCnt = 0u;     
    U0TXBUF = *Serial.TxBuff[0];  //第一个 character 压入 U0TXBUF
    Serial.TxCnt++;
}

/*******************************************************************************
* Description : Writes binary data to the serial port. This data is sent as a byte or series of bytes
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void Serial_send(void)
{
    if (Serial.TxCnt >= fixed_TxRx_len)
    {
    	 //send finish
        Serial.TxCnt = 0u;  
        Serial.TxFlag = 1u;  
        Serial_TxInt_disable();    
    } 
    else
    {    
    	//sending
        U0TXBUF = Serial.TxBuff[Serial.TxCnt]; 
        Serial.TxCnt++;
    }// end if      
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void Serial_RxMode(void)
{
    Serial_RxInt_enable(); 
    
    Serial.TxFlag = 0u;     // sending   
    Serial.TxCnt = 0u;      
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void Serial_Recv(u8 RevByte)
{
    Serial.RxBuff = 
}





