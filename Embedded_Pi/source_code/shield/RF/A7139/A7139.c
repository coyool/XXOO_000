/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：A7139 笙科 RF
*
* 文件标识：
* 摘    要：
*
* 当前版本：V1.0
* 作    者：F06553
* 完成日期：2014-9-26
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：arduino SDK for STM32  
*                Embedded Pi    A7139 RF
*                  3V3 or 5V----VCC   (3.3V to 7V in)
*           SS pin D10----------CSN   (chip select in)
*          SCK pin D13----------SCK   (SPI clock in)
*         MOSI pin D11----------SDI   (SPI Data in)
*         MISO pin D12----------SDO   (SPI data out)
*                               IRQ   (Interrupt output, not connected)
*                  GND----------GND   (ground in)
*
*            Center Freq:
*               Freq dev:  
*               
* VCO -- Voltage Controlled Oscillator
* IF  -- Intermediate Frequency
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/
static u8 A7139_Config(void);
static void A7139_ByteSend(u8 src);
static u8 A7139_ByteRead(void);
static void A7139_StrobeCMD(const u8 cmd);
static void A7139_WriteReg(u8 address, u16 dat);
static u16 A7139_ReadReg(u8 address);
static void A7139_WritePageA(const u8 address, const u16 dat);
static u16 A7139_ReadPageA(const u8 address);
static void A7139_WritePageB(const u8 address, const u16 dat);
//static u16 A7139_ReadPageB(const u8 address);
static u8 A7139_Cal(void);
//static void A7139_RCOSC_Cal(void);

/*** static variable declarations ***/



/*** extern variable declarations ***/
FLAG_BIT_FIELD_TYPE     A7139_RF_FLG;
RF_STATUS_TYPE          A7139_status;






/*--- RF API -----------------------------------------------------------------*/

/*******************************************************************************
* Description : sleep Mode    disable IRQ
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void A7139_sleepMode(void)
{
    /* disable RF IRQ */
    A7139_IFG_CLR;
    A7139_IE_DIS;
    
	A7139_StrobeCMD(CMD_STBY);			
	//delay_us(600);
    delay_ms(5);
    A7139_StrobeCMD(CMD_SLEEP);	
    delay_ms(5);  /* at least delay 2ms */
}

/*******************************************************************************
* Description : only use for direct mode, don't use to TX mode of FIFO. 
*               disable IRQ  
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void A7139_TxMode(void)
{
    /* disable RF IRQ */
    A7139_IFG_CLR;
    A7139_IE_DIS;
    
    A7139_StrobeCMD(CMD_STBY);  
    delay_us(100u);
    A7139_StrobeCMD(CMD_TX);
}

/*******************************************************************************
* Description : Receive mode  enable IRQ
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      :  
*******************************************************************************/
void A7139_RxMode(void)
{ 
    A7139_StrobeCMD(CMD_STBY);  
    delay_us(100u);
    A7139_StrobeCMD(CMD_RX);            
    
    /* enable RF IRQ */
    A7139_IFG_CLR;
    A7139_IE_EN;
}

/*******************************************************************************
* Description : standby Mode  
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void A7139_standbyMode(void)
{
    /* disable RF IRQ */
    A7139_IFG_CLR;
    A7139_IE_DIS;
    
    A7139_StrobeCMD(CMD_STBY);  
    delay_us(100u);
}

/*******************************************************************************
* Description : Send data. 'data' should be A7139 payload bytes long.
*               send complete then this method will return the chip to 
*               receive mode.
* Syntax      : 
* Parameters I: send buff address   
*               send number
* Parameters O: 
* return      :  
*******************************************************************************/
void A7139_Send(u8 *txBuffer, const u8 size)
{
    A7139_standbyMode();
    A7139_flushTxFIFO();             // TX FIFO address pointer reset
    A7139_write(txBuffer, size);
    A7139_StrobeCMD(CMD_TX);         // TxMode
    delay_us(100u);                  // buffering check, see A7139 datasheet
    A7139_waitSend();
    
    A7139_RxMode();                  // afresh enter Recv mode
    
}

/*******************************************************************************
* Description : Indicates if the chip is in transmit mode and there is a packet 
*               currently being transmitted. (true)
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : ture    1 
*               false   0
*******************************************************************************/
void A7139_isSending(void)
{
    
}

/*******************************************************************************
* Description : true on success, false if the Max retries were exceeded, 
*               or if the chip is not in transmit mode.
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : ture  1
*               false 0
*******************************************************************************/
void A7139_waitSend(void)
{
    u32 i;
    
    for (i=0; i<1000; i++)
    {
        delay_ms(1u);
        if (A7139_GIO1_IN_HIGH)
        {         
        }
        else
        {
            delay_ms(1u);
            if (A7139_GIO1_IN_HIGH)
            {    
            }
            else
            {
                break;
            }//end if
        }    
    }//end for
}

/*******************************************************************************
* Description : Receive data 
* Syntax      : 
* Parameters I: receive buff address 
*               receive number
* Parameters O: 
* return      :  
*******************************************************************************/
void A7139_Recv(u8 *RxBuf, const u8 size)
{
    A7139_flushRxFIFO();            //RX FIFO address pointer reset
    A7139_read(RxBuf, size);
    
#ifdef  PN9_CHECK    
    for(i=0; i<64; i++)
    {
        recv = tmpbuf[i];
        tmp = recv ^ PN9_Tab[i];
        if (tmp != 0)
        {
            Err_ByteCnt++;
            Err_BitCnt += (BitCount_Tab[tmp>>4] + BitCount_Tab[tmp & 0x0F]);
        }
    }
#endif    
}

/*******************************************************************************
* Description : Checks whether a received message is available. This can be 
*               called multiple times in a timeout loop.
*               true if a complete, valid message has been received and is able 
*               to be retrieved by A7139_Recv()
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : true 
*               false
*******************************************************************************/
void A7139_available(void)
{
    
}


/*******************************************************************************
* Description : TX FIFO address pointer reset 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void A7139_flushTxFIFO(void)
{
//    A7139_StrobeCMD(CMD_STBY);      // standby
//    delay_us(100u);
    A7139_StrobeCMD(CMD_TFR);		// TX FIFO address pointer reset
}

/*******************************************************************************
* Description : RX FIFO address pointer reset
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void A7139_flushRxFIFO(void)
{
//    A7139_StrobeCMD(CMD_STBY);      // standby
//    delay_us(100u);
    A7139_StrobeCMD(CMD_RFR);		// RX FIFO address pointer reset
}

/*******************************************************************************
* Description : Blocks until the current message (if any) has been transmitted
* Syntax      : 
* Parameters I: txbuffer -- payload 
*               size -- payload size   
* Parameters O: 
* return      : 
*******************************************************************************/
void A7139_write(u8 *txBuffer, const u8 size)
{
    u8 i;  
    
    /* Check the parameters */
    assert_param(NULL == txBuffer); 
    
    A7139_SCS_OUT_LOW;
    A7139_ByteSend(CMD_FIFO_W);	    // TX FIFO write command
    for (i=0; i<size; i++)
    {    
        A7139_ByteSend(txBuffer[i]);
    }
    A7139_SCS_OUT_HIGH;
}

/*******************************************************************************
* Description : A7139_read
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void A7139_read(u8 *RxBuf, const u8 size)
{
    u8 i;
    
    /* Check the parameters */
    assert_param(NULL == RxBuf); 
    
    A7139_SCS_OUT_LOW;
    A7139_ByteSend(CMD_FIFO_R);	    //RX FIFO read command
    for (i=0; i <size; i++)
    {
        RxBuf[i] = A7139_ByteRead();
    }
    A7139_SCS_OUT_HIGH;
}

/*******************************************************************************
* Description : A7139_setPower
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void A7139_setPower(void)
{
    
}

/*******************************************************************************
* Description : A7139_setBaudRate
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void A7139_setBaudRate(void)
{
    
}

/*******************************************************************************
* Description : RF setup 
* Syntax      : 
* Parameters I:    
* Parameters O: 
* return      : return 0 -- normal  
*               return 1 -- abnormal 
*******************************************************************************/
u8 A7139_setup(void)
{
    u8 return_val = 0u;
    /* init io pin */
    A7139_SCS_MO;
    A7139_SCK_MO;
    A7139_SDIO_MO;
    A7139_GIO1_MI;
    // AMICCOM_SPISetup();笙科初始化程序
    A7139_SCS_OUT_HIGH;
    A7139_SCK_OUT_LOW;              
    A7139_SDIO_OUT_HIGH;
    
    A7139_StrobeCMD(CMD_RF_RST); // reset  chip
    if(A7139_Config())               
    {    
        return_val = 1u;
    }
    else
    {
        return_val = 0u;
    }
    delay_ms(5);		    //for crystal stabilized 600us
    
    if(A7139_WriteID())    //write ID code
    {    
        return_val = 1u;
    }
    else if(A7139_Cal())		//IF and VCO calibration
    {
        return_val = 1u;
    }
    else
    {
        return_val = 0u;
    }
    delay_ms(5);
    
    return return_val;
}

/*******************************************************************************
* Description : write ID 
* Syntax      : 
* Parameters I:    
* Parameters O: 
* return      : return 0 -- normal  
*               return 1 -- abnormal 
*******************************************************************************/
u8 A7139_WriteID(void)
{
    u8 i;
    u8 d1, d2, d3, d4;
    u8 return_val = 0u;

    A7139_SCS_OUT_LOW;
    A7139_ByteSend(CMD_ID_W);
    for(i=0; i<4; i++)
    {    
        A7139_ByteSend(ID_Tab[i]);
    }
    A7139_SCS_OUT_HIGH;
    delay_us(5u);
    
    A7139_SCS_OUT_LOW;
    A7139_ByteSend(CMD_ID_R);
    d1 = A7139_ByteRead();
    d2 = A7139_ByteRead();
    d3 = A7139_ByteRead();
    d4 = A7139_ByteRead();
    A7139_SCS_OUT_HIGH;
	
    if((d1!=ID_Tab[0]) || (d2!=ID_Tab[1])
       || (d3!=ID_Tab[2]) || (d4!=ID_Tab[3]))
    {
        printf(" A7139 write ID error! \r\n");
        return_val = 1u;
    }
    else
    {
        return_val = 0u;
    }
    
    return return_val;
}

/*******************************************************************************
* Description : Freq Cal Set 
* Syntax      : commend ch = 0 ~ 75
* Parameters I:    
* Parameters O: 
* return      :  
*******************************************************************************/
void A7139_FreqSet(u8 ch)
{
    u16 temp;
    
    temp = ch*16u;                      // 200k
    A7139_StrobeCMD(CMD_STBY);          // standby
    A7139_WritePageB(IF2_PAGEB, temp);  // setting PLL1 A7139_WritePageB   
}

/*------- RF driver ----------------------------------------------------------*/
/*******************************************************************************
* Description : RF config  
* Syntax      : 
* Parameters I:    
* Parameters O: 
* return      : return 0 -- normal  
*               return 1 -- abnormal 
*******************************************************************************/
static u8 A7139_Config(void)
{
    u8 i;
    u16 tmp = 0u;
    u8 return_val = 0u;

    for(i=0; i<8; i++)
    {    
        A7139_WriteReg(i, A7139_Reg_Config[i]);
    }
    /* PAGEA_REG PAGEB_REG don't config */
    for(i=10; i<16; i++)
    {
        A7139_WriteReg(i, A7139_Reg_Config[i]);
    }
    for(i=0; i<16; i++)
    {    
        A7139_WritePageA(i, A7139_Config_PageA[i]);
    }
    for(i=0; i<5; i++)
    {
        A7139_WritePageB(i, A7139_Config_PageB[i]);
    }
    //for check        
    tmp = A7139_ReadReg(SYSTEMCLOCK_REG);
    if(tmp != A7139_Reg_Config[SYSTEMCLOCK_REG])
    {
        //LSD_Err_State();
        printf(" A7139 config error! \r\n");
        return_val = 1u;
    }
    tmp = A7139_ReadReg(PLL3_REG);
    if(tmp != A7139_Reg_Config[PLL3_REG])
    {
        //LSD_Err_State();
        printf(" A7139 config error! \r\n");
        return_val = 1u;
    }
    
    return return_val;
}

/*******************************************************************************
* Description : write one byte data to A7139
* Syntax      : 
* Parameters I: data (8 bit)
* Parameters O: 
* return      : 
*******************************************************************************/
static void A7139_ByteSend(u8 src)
{
    u8 i;
    
    A7139_SDIO_MO;
    
    for (i=0; i<8; i++)
    {
        if (src & 0x80)
        {    
            A7139_SDIO_OUT_HIGH;  
        }
        else
        {
            A7139_SDIO_OUT_LOW; 
        }
        
        delay_us(5);
        A7139_SCK_OUT_HIGH;
        delay_us(5);
        A7139_SCK_OUT_LOW;
        src = src << 1;
    }
}

/*******************************************************************************
* Description : read one byte data from A7139
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : return data (8 bit)
*******************************************************************************/
static u8 A7139_ByteRead(void)
{
    u8 i,tmp;
    
    A7139_SDIO_MO;
    A7139_SDIO_OUT_HIGH;         
    A7139_SDIO_MI;               // change SDIO input 
    
    for (i=0; i<8; i++)          
    {
        if (A7139_SDIO_IN_HIGH)
        {
            tmp = (tmp << 1) | 0x01;
        }    
        else
        {    
            tmp = tmp << 1;
        }
        
        delay_us(5);
        A7139_SCK_OUT_HIGH;
        delay_us(5);
        A7139_SCK_OUT_LOW;
    }    
    
    return tmp;         // Return tmp value.
}

/*******************************************************************************
* Description : A7139_StrobeCMD
* Syntax      : 
* Parameters I: command
* Parameters O: 
* return      : 
*******************************************************************************/
static void A7139_StrobeCMD(const u8 cmd)
{
    A7139_SCS_OUT_LOW;
    A7139_ByteSend(cmd);
    A7139_SCS_OUT_HIGH;
}

/*******************************************************************************
* Description : write A7139 reg 
* Syntax      : 
* Parameters I: address  
*               data 
* Parameters O: 
* return      : 
*******************************************************************************/
static void A7139_WriteReg(u8 address, u16 dat)
{
    u8 data_low_byte;
    u8 data_high_byte;
    
    data_low_byte = (dat) & 0x00FF;
    data_high_byte = (dat >> 8) & 0x00FF;
    
    A7139_SCS_OUT_LOW;
    
    address |= CMD_REG_W;     
    A7139_ByteSend(address); 
    delay_us(5);
    A7139_ByteSend(data_high_byte);
    A7139_ByteSend(data_low_byte);
    
    A7139_SCS_OUT_HIGH; 
}

/*******************************************************************************
* Description : read A7139 reg 
* Syntax      : 
* Parameters I: address   
* Parameters O: 
* return      : A7139 reg data 
*******************************************************************************/
static u16 A7139_ReadReg(u8 address)
{
    u16 data_low_byte;
    u16 data_high_byte;
    u16 tmp;
    
    A7139_SCS_OUT_LOW;
    
    address |= CMD_REG_R;
    A7139_ByteSend(address);
    delay_us(5);
    data_high_byte = A7139_ByteRead();
    data_low_byte = A7139_ByteRead();
    tmp = ((data_high_byte<<8)&0xFF00) | (data_low_byte&0x00FF); 
    
    A7139_SCS_OUT_HIGH; 
    
    return tmp;
}

/*******************************************************************************
* Description : write page_A reg 
* Syntax      : 
* Parameters I: address  
*               data
* Parameters O: 
* return      : 
*******************************************************************************/
static void A7139_WritePageA(const u8 address, const u16 dat)
{
    u16 tmp;

    tmp = address;
    tmp = (((tmp << 12)&0xF000) | A7139_Reg_Config[CRYSTAL_REG]);  
    A7139_WriteReg(CRYSTAL_REG, tmp);
    A7139_WriteReg(PAGEA_REG, dat);
}

/*******************************************************************************
* Description : read page_A reg  
* Syntax      : 
* Parameters I: address   
* Parameters O: 
* return      : reg data 
*******************************************************************************/
static u16 A7139_ReadPageA(const u8 address)
{
    u16 tmp;

    tmp = address;
    tmp = (((tmp << 12)&0xF000) | A7139_Reg_Config[CRYSTAL_REG]);
    A7139_WriteReg(CRYSTAL_REG, tmp);
    tmp = A7139_ReadReg(PAGEA_REG);
    /*here lost  A7108_WriteReg(CRYSTAL_REG, temp_Crystal_Reg);
    is it because never change CRYSTAL_REG?*/
    
    return tmp; 
}

/*******************************************************************************
* Description : write page_A reg  
* Syntax      : 
* Parameters I: address   
*               data
* Parameters O: 
* return      :  
*******************************************************************************/
static void A7139_WritePageB(const u8 address, const u16 dat)
{
    u16 tmp;

    tmp = address;
    tmp = (((tmp << 7)&0xFF80) | A7139_Reg_Config[CRYSTAL_REG]);
    A7139_WriteReg(CRYSTAL_REG, tmp);
    A7139_WriteReg(PAGEB_REG, dat);
}

/*******************************************************************************
* Description : read page_A reg  
* Syntax      : 
* Parameters I: address   
* Parameters O: 
* return      : reg data 
*******************************************************************************/
//static u16 A7139_ReadPageB(const u8 address)
//{
//    u16 tmp;
//
//    tmp = address;
//    tmp = (((tmp << 7)&0xFF80) | A7139_Reg_Config[CRYSTAL_REG]);
//    A7139_WriteReg(CRYSTAL_REG, tmp);
//    tmp = A7139_ReadReg(PAGEB_REG);
//    
//    return tmp;
//}

/*******************************************************************************
* Description : cal
* Syntax      : 
* Parameters I:    
* Parameters O: 
* return      :  
*******************************************************************************/
static u8 A7139_Cal(void)
{
    u8 i;
    u8 fbcf;	    //IF Filter
    u8 vbcf;		//VCO Current
    u8 vccf;	    //VCO Band
    u16 tmp;
    u8 return_val = 0u; 
    
    /* @STB state IF Filter & VCO Current Calibration */
    A7139_WriteReg(MODE_REG, A7139_Reg_Config[MODE_REG] | 0x0802);	 
    do
    {
        tmp = A7139_ReadReg(MODE_REG);
    }while(tmp & 0x0802);
	
    /* for check(IF Filter) */
    tmp = A7139_ReadReg(CALIBRATION_REG);
    fbcf = (tmp>>4) & 0x01;
    if (fbcf)
    {
        //LSD_Err_State();
        printf(" IF failed! ");
        return_val = 1u;
    }

	//for check(VCO Current)
    tmp = A7139_ReadPageA(VCB_PAGEA);
    vccf = (tmp>>4) & 0x01;
    if (vccf)
    {
        //LSD_Err_State();
        printf(" VCO Current failed! ");
        return_val = 1u;
    }
    
    //RSSI Calibration procedure @STB state
    A7139_WriteReg(ADC_REG, 0x4C00);	    //set ADC average=64
    A7139_WritePageA(WOR2_PAGEA, 0xF800);	//set RSSC_D=40us and RS_DLY=80us
    A7139_WritePageA(TX1_PAGEA, 
                     A7139_Config_PageA[TX1_PAGEA] | 0xE000); //set RC_DLY=1.5ms
    A7139_WriteReg(MODE_REG, 
                   A7139_Reg_Config[MODE_REG] | 0x1000);	  //RSSI Calibration
    do
    {
        tmp = A7139_ReadReg(MODE_REG);
    }while(tmp & 0x1000);
    A7139_WriteReg(ADC_REG, A7139_Reg_Config[ADC_REG]);
    A7139_WritePageA(WOR2_PAGEA, A7139_Config_PageA[WOR2_PAGEA]);
    A7139_WritePageA(TX1_PAGEA, A7139_Config_PageA[TX1_PAGEA]);

    //VCO calibration procedure @STB state
    for (i=0; i<3; i++)
    {
        A7139_WriteReg(PLL1_REG, Freq_Cal_Tab[i*2]);
        A7139_WriteReg(PLL2_REG, Freq_Cal_Tab[i*2+1]);
        //VCO Band Calibration
        A7139_WriteReg(MODE_REG, A7139_Reg_Config[MODE_REG] | 0x0004);
        
        do
        {
            tmp = A7139_ReadReg(MODE_REG);
        }while(tmp & 0x0004);

        //for check(VCO Band)
        tmp = A7139_ReadReg(CALIBRATION_REG);
        vbcf = (tmp >>8) & 0x01;
        if (vbcf)
        {
            //LSD_Err_State();
            printf(" VCO Band failed! ");
            return_val = 1u;
        }
    }
    
    A7139_WriteReg(PLL1_REG, Freq_Cal_Tab[0*2]);
    A7139_WriteReg(PLL2_REG, Freq_Cal_Tab[0*2+1]);
    //VCO Band Calibration
    A7139_WriteReg(MODE_REG, A7139_Reg_Config[MODE_REG] | 0x0004);	
    do
    {
        tmp = A7139_ReadReg(MODE_REG);
    }while(tmp & 0x0004);

    //for check(VCO Band)
    tmp = A7139_ReadReg(CALIBRATION_REG);
    vbcf = (tmp >>8) & 0x01;
    if(vbcf)
    {
        //LSD_Err_State();
        printf(" VCO Band failed! ");
        return_val = 1u;
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
//static void A7139_RCOSC_Cal(void)
//{
//    u16 tmp;
//
//    /* enable RC OSC */
//    A7139_WritePageA(WOR2_PAGEA, A7139_Config_PageA[WOR2_PAGEA] | 0x0010);		
//
//    while(1)
//    {
//        /* set ENCAL=1 to start RC OSC CAL */
//        A7139_WritePageA(WCAL_PAGEA, A7139_Config_PageA[WCAL_PAGEA] | 0x0001);	
//        do
//        {
//            tmp = A7139_ReadPageA(WCAL_PAGEA);
//        }while(tmp & 0x0001);
//          
//        /* read NUMLH[8:0] */
//        tmp = (A7139_ReadPageA(WCAL_PAGEA) & 0x03FF);		
//        tmp >>= 1;	
//        if ((tmp>186) && (tmp<198))	//NUMLH[8:0]~192
//        {
//            break;
//        }
//    }
//}






/*********************************************************************
** Err_State
*********************************************************************/
//void LSD_Err_State(void)
//{
//    //ERR display
//    //Error Proc...
//    //...
//    while(1);
//}

///*********************************************************************
//** WOR_enable_by_preamble
//*********************************************************************/
//void WOR_enable_by_preamble(void)
//{
//	StrobeCMD(CMD_STBY);
//	
//	A7139_WritePageA(GIO_PAGEA, 0x004D);	//GIO1=PMDO, GIO2=WTR
//
//	//Real WOR Active Period = (WOR_AC[5:0]+1) x 244us  X'TAL and Regulator Settling Time
//	//Note : Be aware that Xˇtal settling time requirement includes initial tolerance, 
//	//       temperature drift, aging and crystal loading.
//	A7139_WritePageA(WOR1_PAGEA, 0x8005);	//setup WOR Sleep time and Rx time
//	
//	RCOSC_Cal();     	//RC Oscillator Calibration
//	
//	A7139_WritePageA(WOR2_PAGEA, A7139Config_PageA[WOR2_PAGEA] | 0x0030);	//enable RC OSC & WOR by preamble
//	
//	A7139_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0200);		//WORE=1 to enable WOR function
//	
//	while(GIO1==0);		//Stay in WOR mode until receiving preamble(preamble ok)
//	
//}
//
///*********************************************************************
//** WOR_enable_by_sync
//*********************************************************************/
//void WOR_enable_by_sync(void)
//{
//	StrobeCMD(CMD_STBY);
//	
//	A7139_WritePageA(GIO_PAGEA, 0x0045);	//GIO1=FSYNC, GIO2=WTR
//
//	//Real WOR Active Period = (WOR_AC[5:0]+1) x 244us  X'TAL and Regulator Settling Time
//	//Note : Be aware that Xˇtal settling time requirement includes initial tolerance, 
//	//       temperature drift, aging and crystal loading.
//	A7139_WritePageA(WOR1_PAGEA, 0x8005);	//setup WOR Sleep time and Rx time
//	
//	RCOSC_Cal();     	//RC Oscillator Calibration
//	
//	A7139_WritePageA(WOR2_PAGEA, A7139Config_PageA[WOR2_PAGEA] | 0x0010);	//enable RC OSC & WOR by sync
//	
//	A7139_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0200);		//WORE=1 to enable WOR function
//	
//	while(GIO1==0);		//Stay in WOR mode until receiving ID code(sync ok)
//	
//}

///*********************************************************************
//** WOR_enable_by_carrier
//*********************************************************************/
//void WOR_enable_by_carrier(void)
//{
//	StrobeCMD(CMD_STBY);
//	
//	A7139_WritePageA(GIO_PAGEA, 0x0049);	//GIO1=CD, GIO2=WTR
//
//	//Real WOR Active Period = (WOR_AC[5:0]+1) x 244us  X'TAL and Regulator Settling Time
//	//Note : Be aware that Xˇtal settling time requirement includes initial tolerance, 
//	//       temperature drift, aging and crystal loading.
//	A7139_WritePageA(WOR1_PAGEA, 0x8005);	//setup WOR Sleep time and Rx time
//	
//	RCOSC_Cal();     	//RC Oscillator Calibration
//	
//	A7139_WritePageA(WOR2_PAGEA, A7139Config_PageA[WOR2_PAGEA] | 0x0410);	//enable RC OSC & WOR by carrier
//	A7139_WriteReg(ADC_REG, A7139Config[ADC_REG] | 0x8096);			//ARSSI=1, RTH=150
//	A7139_WritePageA(RFI_PAGEA, A7139Config_PageA[RFI_PAGEA] | 0x6000);	//RSSI Carrier Detect plus In-band Carrier Detect
//	A7139_WritePageB(ACK_PAGEB, A7139Config_PageB[ACK_PAGEB] | 0x0200);	//CDRS=[01]
//	A7139_WritePageA(VCB_PAGEA, A7139Config_PageA[VCB_PAGEA] | 0x4000);	//CDTM=[01]
//	
//	A7139_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0200);		//WORE=1 to enable WOR function
//	
//	while(GIO1==0);		//Stay in WOR mode until carrier signal strength is greater than the value set by RTH[7:0](carrier ok)
//	
//}
//
///*********************************************************************
//** WOT_enable
//*********************************************************************/
//void WOT_enable(void)
//{
//	StrobeCMD(CMD_STBY);
//	
//	A7139_WritePageA(GIO_PAGEA, 0x0045);	//GIO1=FSYNC, GIO2=WTR
//
//	//Real WOR Active Period = (WOR_AC[5:0]+1) x 244us  X'TAL and Regulator Settling Time
//	//Note : Be aware that Xˇtal settling time requirement includes initial tolerance, 
//	//       temperature drift, aging and crystal loading.
//	A7139_WritePageA(WOR1_PAGEA, 0x8005);	//setup WOR Sleep time and Rx time
//	
//	RCOSC_Cal();     		//RC Oscillator Calibration
//	
//	A7139_WriteFIFO();		//write data to TX FIFO
//
//	A7139_WriteReg(PIN_REG, A7139Config[PIN_REG] | 0x0400);		//WMODE=1 select WOT function
//	
//	A7139_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0200);	//WORE=1 to enable WOR function
//	
//	while(1);	
//	
//}
//
///*********************************************************************
//** TWOR_enable
//*********************************************************************/
//void TWOR_enable(void)
//{
//	StrobeCMD(CMD_STBY);
//	
//	A7139_WritePageA(GIO_PAGEA, 0x0051);	//GIO1=TWOR, GIO2=WTR
//
//	//Real WOR Active Period = (WOR_AC[5:0]+1) x 244us  X'TAL and Regulator Settling Time
//	//Note : Be aware that Xˇtal settling time requirement includes initial tolerance, 
//	//       temperature drift, aging and crystal loading.
//	A7139_WritePageA(WOR1_PAGEA, 0x8005);	//setup WOR Sleep time and Rx time
//	
//	RCOSC_Cal();     		//RC Oscillator Calibration
//	
//	A7139_WritePageA(WOR2_PAGEA, A7139Config_PageA[WOR2_PAGEA] | 0x0014);	//start TWOR by WOR_AC
//	//A7139_WritePageA(WOR2_PAGEA, A7139Config_PageA[WOR2_PAGEA] | 0x001C);	//start TWOR by WOR_SL
//	
//	StrobeCMD(CMD_SLEEP);	//entry sleep mode
//	
//	while(1);
//
//}
//
///*********************************************************************
//** RSSI_measurement
//*********************************************************************/
//void RSSI_measurement(void)
//{
//	Uint16 tmp;
//
//	StrobeCMD(CMD_STBY);
//	
//	A7139_WritePageA(GIO_PAGEA, 0x0045);			//GIO1=FSYNC, GIO2=WTR
//	
//	A7139_WriteReg(ADC_REG, A7139Config[ADC_REG] | 0x8000);	//ARSSI=1
//	
//	StrobeCMD(CMD_RX);	//entry RX mode
//	Delay10us(1);
//	
//	while(GIO1==0)		//Stay in WOR mode until receiving ID code(sync ok)
//	{
//		tmp = (A7139_ReadReg(ADC_REG) & 0x00FF);	//read RSSI value(environment RSSI)
//	}
//	tmp = (A7139_ReadReg(ADC_REG) & 0x00FF);		//read RSSI value(wanted signal RSSI)
//	
//}
//
///*********************************************************************
//** FIFO_extension_Infinite_TX
//*********************************************************************/
//void FIFO_extension_Infinite_TX(void)
//{
//	Uint8 i, n;
//	Uint16 j;	
//
//	//for Infinite : FEP[13:0]=remainder,  5 < FEP[13:0] < 63
//	A7139_WritePageA(VCB_PAGEA, 0x0004);
//	A7139_WritePageA(FIFO_PAGEA, 0x0006);
//
//	A7139_WritePageA(GIO_PAGEA, 0x0075);	//GIO1=FPF, GIO2=WTR
//	A7139_WritePageA(CKO_PAGEA, 0xD283);	//CKO=DCK
//	
//	A7139_WriteReg(PIN_REG, 0x0A00);	//set INFS=1, infinite length
//
//	n=0;
//
//	SCS=0;
//	ByteSend(CMD_FIFO_W);		        //write 1st TX FIFO 64bytes
//	for(i=0; i<64; i++)
//	{
//		ByteSend(n);
//		if(n==255)
//			n=0;
//		else
//			n++;
//	}
//	SCS=1;
//
//	StrobeCMD(CMD_TX);			//entry TX mode
//
//	for(j=0; j<500; j++)
//	{
//		while(GIO1==0);			//wait FPF go high
//		while(GIO1==1);			//wait FPF go low
//
//		SCS=0;
//		ByteSend(CMD_FIFO_W);	//write TX FIFO 64bytes
//		for(i=0; i <64; i++)
//		{
//			ByteSend(n);
//			if(n==255)
//				n=0;
//			else
//				n++;
//		}
//		SCS=1;
//	}
//
//	//remainder : FEP[13:0]=6, FIFO Length=6+1=7
//	while(GIO1==0);				//wait last FPF go high
//	while(GIO1==1);				//wait last FPF go low
//
//	SCS=0;
//	ByteSend(CMD_FIFO_W);		//write TX FIFO 7bytes
//	for(i=0; i<7; i++)
//	{
//		ByteSend(n);
//		if(n==255)
//			n=0;
//		else
//			n++;
//	}
//	SCS=1;
//
//	A7139_WriteReg(PIN_REG, 0x0800);	//disable INFS
//	
//	while(GIO2);	//wait transmit completed
//}
//
///*********************************************************************
//** FIFO_extension_Infinite_RX
//*********************************************************************/
//void FIFO_extension_Infinite_RX(void)
//{
//    Uint8 i, n;
//    Uint8 recv;
//    Uint8 tmp;
//	Uint16 j;
//
//	//for Infinite : FEP[13:0]=remainder,  5 < FEP[13:0] < 63
//	A7139_WritePageA(VCB_PAGEA, 0x0004);
//	A7139_WritePageA(FIFO_PAGEA, 0x0006);
//
//	A7139_WritePageA(GIO_PAGEA, 0x0075);	//GIO1=FPF, GIO2=WTR
//	A7139_WritePageA(CKO_PAGEA, 0xD28B);	//CKO=RCK
//
//	A7139_WriteReg(PIN_REG, 0x0A00);		//set INFS=1, infinite length
//
//	Err_ByteCnt=0;
//	Err_BitCnt=0;
//	n=0;
//	 
//	StrobeCMD(CMD_RX);			//entry RX mode
//
//	for(j=0; j<501 ; j++)
//	{
//		while(GIO1==0);			//wait FPF go high
//		while(GIO1==1);			//wait FPF go low
//
//		SCS=0;
//		ByteSend(CMD_FIFO_R);	//read RX FIFO 64bytes
//		for(i=0; i<64; i++)		
//			tmpbuf[i]=ByteRead();
//		SCS=1;
//
//		//for check
//		for(i=0; i<64; i++)
//		{
//			recv = tmpbuf[i];
//			tmp = recv ^ n;
//			if(tmp!=0)
//        	{
//				Err_ByteCnt++;
//            	Err_BitCnt += (BitCount_Tab[tmp>>4] + BitCount_Tab[tmp & 0x0F]);
//        	}
//
//			if(n==255)
//				n=0;
//			else
//				n++;
//		}
//	}
//
//	A7139_WriteReg(PIN_REG, 0x0800);	//disable INFS
//
//	while(GIO2);	//wait receive completed
//
//	//remainder : FEP[13:0]=6, FIFO Length=6+1=7
//	SCS=0;
//	ByteSend(CMD_FIFO_R);		//read RX FIFO 7bytes
//	for(i=0; i<7; i++)		
//		tmpbuf[i]=ByteRead();
//	SCS=1;
//
//	//for check
//	for(i=0; i<7; i++)
//	{
//		recv = tmpbuf[i];
//		tmp = recv ^ n;
//		if(tmp!=0)
//        {
//			Err_ByteCnt++;
//           	Err_BitCnt += (BitCount_Tab[tmp>>4] + BitCount_Tab[tmp & 0x0F]);
//        }
//
//		if(n==255)
//			n=0;
//		else
//			n++;
//	}
//}
//
///*********************************************************************
//** Auto_Resend
//*********************************************************************/
//void Auto_Resend(void)
//{
//	StrobeCMD(CMD_STBY);
//
//	A7139_WritePageA(GIO_PAGEA, 0x0071);	//GIO1=VPOAK, GIO2=WTR
//	A7139_WritePageA(CKO_PAGEA, 0xD283);	//CKO=DCK
//
//	A7139_WritePageB(ACK_PAGEB, 0x003F);	//enable Auto Resend, by event, fixed, ARC=15
//	A7139_WritePageB(ART_PAGEB, 0x001E);	//RND=0, ARD=30
//	//ACK format = Preamble + ID = 8bytes
//	//data rate=10kbps, Transmission Time=(1/10k)*8*8=6.4ms
//	//so set ARD=30, RX time=30*250us=7.75ms
//
//	while(1)
//	{
//		A7139_WriteFIFO();
//		StrobeCMD(CMD_TX);		//entry TX mode
//		Delay10us(1);
//		while(GIO2);			//wait transmit completed
//
//		//check VPOAK
//		if(GIO1)
//		{
//			//ACK ok
//		}
//		else
//		{
//			//NO ACK
//		}
//
//		StrobeCMD(CMD_STBY);	//clear VPOAK(GIO1 signal)
//
//		Delay10ms(1);
//	}
//}
//
///*********************************************************************
//** Auto_ACK
//*********************************************************************/
//void Auto_ACK(void)
//{
//	A7139_WritePageA(GIO_PAGEA, 0x0045);	//GIO1=FSYNC, GIO2=WTR
//	A7139_WritePageA(CKO_PAGEA, 0xD28B);	//CKO=RCK
//
//	A7139_WritePageB(ACK_PAGEB, 0x0001);	//enable Auto ACK
//
//	while(1)
//	{
//		StrobeCMD(CMD_RX);		//entry RX mode
//		Delay10us(1);			
//		while(GIO2);			//wait receive completed
//
//		RxPacket();
//	}
//}
//

//
///*********************************************************************
//** wake_up_from_deep_sleep_mode
//*********************************************************************/
//void wake_up_from_deep_sleep_mode(void)
//{
//	StrobeCMD(CMD_STBY);	//wake up (any Strobe CMD)
//	Delay100us(3);		//for VDD_D stabilized
//	InitRF();
//}

