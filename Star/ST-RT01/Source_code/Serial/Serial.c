/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* �ļ����ƣ�
*
* �ļ���ʶ��
* ժ    Ҫ��
*
* ��ǰ�汾��
* ��    �ߣ�F06553
* ������ڣ�2014-12-18
* ���뻷����D:\Program Files\IAR_430\430
* 
* Դ����˵����
*******************************************************************************/
#include  "all_header_file.h"

/*** static function prototype declarations ***/




/*** static variable declarations ***/
static const u8 Serial_bps[10][3]= 
{
	{0x3A,0x6D,0x44},    //300 ������  		0   
	{0x9D,0x36,0x40},    //600 ������ 		1
	{0x4e,0x1b,0x02},    //1200 ������  	2  
	{0xa7,0x0d,0x11},    //2400 ������		3
	{0xd3,0x06,0x00},    //4800 ������		4
	{0x69,0x03,0x6b},    //9600 ������   	5
	{0xb4,0x01,0x2a},    //19200 ������      6
	{0xda,0x00,0xee},    //38400 ������      7
	{0x91,0x00,0x02},    //57600 ������      8
	{0x48,0x00,0xae},    //115200 ������     9
};

const u8 Serial_fixed_TxRx_Len = 60u;



/*** extern variable declarations ***/
SERIAL_TYPE Serial;
u32 Serial_timeoutCnt = 0u;
const u32 Serial_timeout_62ms = 4u;




/*******************************************************************************
* Description : UART0
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void Serial_begin(SERIAL_BPS_TYPE bps)
{
    P2SEL |= BIT4 | BIT5;        //����P2.4,P2.5�ڵڶ�����
	U0CTL = SWRST; 	             //SWRST ���ڸ�λ 	
    U0ME |= (UTXE0 + URXE0);     //Enable USART0 TXD/RXD 	
    U0CTL &= ~SWRST;             //SWRST ���ڸ�λ���� 
	U0IE |= URXIE0;              //�򿪽��� �ж�  
	U0CTL = CHAR | PENA | PEV;   //data length 0:7(8 bit), Parity enable, Even parity  
	U0TCTL = TXEPT;              //Transmitter empty flag
	U0TCTL |= SSEL1;             //ʱ��ԴΪSMCLK 8388608Mhz	
	if (bps >= bps_115200) 
    {
        bps = bps_9600;	
    }
    else
    {
        _NOP();
    }    
	U0BR0  = Serial_bps[bps][0];    // set bps
	U0BR1  = Serial_bps[bps][1]; 
	U0MCTL = Serial_bps[bps][2];    	
    
    memset(&Serial, 0, sizeof(Serial));  //clear Serial data 
    Serial_timeoutCnt = 0u;       //
    Serial_RxMode();              // default Rx mode
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
	U0IE |= UTXIE0;       // �򿪽��� �ж�     
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
	U0IE &= ~UTXIE0;      // �رշ��� �ж�                 
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
	U0IE |= URXIE0;      // �򿪽��� �ж�     
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
	U0IE &= ~URXIE0;     // �رս��� �ж�             
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
    U0TXBUF = Serial.TxBuff[0];  //��һ�� character ѹ�� U0TXBUF
    Serial.TxCnt++;
}

/*******************************************************************************
* Description : Writes binary data to the serial port. This data is sent as a byte or series of bytes
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void Serial_send(void)
{
    if (Serial.TxCnt >= Serial_fixed_TxRx_Len)
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
//void Serial_isSending(void)
//{
//    
//}

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
    
    Serial.RxFlag = 0u;     // Receiving   
    Serial.RxCnt = 0u;    
    Serial_timeoutCnt = 0u;
}

/*******************************************************************************
* Description : ȱ ��ʱ���� �������60 byte ���� ���� 60 byte
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void Serial_Recv(u8 RevByte)
{
    if (RXERR == (U0RCTL & RXERR))
    {
        //clear flag
        U0RCTL &= ~RXERR;  /* RX Error Error */
        U0RCTL &= ~FE;     /* Frame Error */
        U0RCTL &= ~PE;     /* Parity Error */
        U0RCTL &= ~OE;     /* Overrun Error */
        U0RCTL &= ~BRK;    /* Break detected */
    }    
    else
    {
        if (Serial.RxCnt >= Serial_fixed_TxRx_Len)  // ���� 60 byte ������ 
        {
            Serial.RxCnt = 0u;
            Serial.RxFlag = 1u;
//        Serial_RxInt_disable(); 
        }
        else
        {
            Serial.RxFlag = 2u;
            Serial_timeoutCnt = 0u;
            Serial.RxBuff[Serial.RxCnt] = U0RXBUF;
            Serial.RxCnt++;   
        }          
    }    
    
     
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
u8 Serial_available(u8 *rxBuff, const u8 len)
{
    u8 return_val = 0u;
    
    if (1u == Serial.RxFlag)
    {
        Serial.RxFlag = 0u;
        return_val = 1u;
//        len = Serial_fixed_TxRx_Len;                   
        memcpy(rxBuff, Serial.RxBuff, len); //60 byte
        memset(Serial.RxBuff, 0, Serial_fixed_TxRx_Len);
    }
    else
    {
        
    }  

	return return_val;
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
//void Serial_setTimeout(u32 time)
//{
//    Serial_timeoutCnt = 0u;
//    Serial_timeoutMax = time;
//}



