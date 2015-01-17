/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：RF for DL645
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：
* 完成日期：2014-12-2
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include "Include.h"

/*** static function prototype declarations ***/
static void CC1101_ExtInt_init(void);
//static void CC1101_ExtInt_enable(void);
//static void CC1101_ExtInt_disable(void);
static void CC1101_pinMode(void);
//static void CC1101_sleepMode(void);
static void CC1101_RxMode(void);
static void CC1101_reset(void);
static void CC1101_regSetting(void);
static void CC1101_setPATable(void);
static void CC1101_setTxPower(uint8_t value);
static void CC1101_initStep(void);
//static void CC1101_waitSend(void);


#ifdef CC1101_DISPLAY_TX_COMM_SYMBOL_EN      
static void CC1101_displayTxCommSymbol(void);
#endif

#ifdef CC1101_DISPLAY_RX_COMM_SYMBOL_EN 
static void CC1101_displayRxCommSymbol(void);
#endif

/*** static variable declarations ***/



/*** extern variable declarations ***/
const u32 ONEDAYTIME = 24*60;    //
const u8 RF_shortPayloadSize = 13u; 
const u8 RF_shortBuffMaxSize = 15u; 
const u8 RF_payloadSize = 62u; 
const u8 RF_buffMaxSize = 64u;   // 62 payload + 2 CRC  {PKTLEN, 0x3C} payload = 60 

RF_TYPE  RF;      /* RF struct */
uint32_t oneDayCnt;   


//#define RF_SLAVE

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void RF_ExtIntCallbackFunc(void)
{
    RF.RX_FLG = 1u;
    EXTI_ClrIntFlag(EXTI_CH8);
} 

/*******************************************************************************
* Description :    failling edge
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void CC1101_ExtInt_init(void)
{
    IO_EnableFunc(IO_PORT3, IO_PINx5);  //GDO2
    IO_ConfigFuncINTxPin(IO_EXT_INT_CH8, IO_INTx_INTx_1);
    IO_ConfigGPIOPin(IO_PORT3, IO_PINx5, IO_DIR_INPUT, IO_PULLUP_ON);  //无外部上拉 需内部上拉
    
    EXTI_DisableInt(EXTI_CH8);
    EXTI_SetIntDetectMode(EXTI_CH8, EXTI_EDGE_FALLING);   //采用 RF IRQ 下降沿 (接收完成产生) 
    EXTI_ClrIntFlag(EXTI_CH8);
    EXTI_EnableInt(EXTI_CH8, RF_ExtIntCallbackFunc);
    NVIC_EnableIRQ(EXINT8_31_IRQn);  //
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void CC1101_ExtInt_enable(void)
{
    EXTI_ClrIntFlag(EXTI_CH8);//clear hardware flg
    EXTI_EnableInt(EXTI_CH8, RF_ExtIntCallbackFunc);//ext Int en
}

/*******************************************************************************
* Description : RF ExtINt
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void CC1101_ExtInt_disable(void)
{
    EXTI_DisableInt(EXTI_CH8);//ext Int dis
    EXTI_ClrIntFlag(EXTI_CH8);//clear hardware flg
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void CC1101_pinMode(void)
{  
    //普通IO
    IO_DisableFunc(IO_PORT3, IO_PINxD);  // CSN
    IO_DisableFunc(IO_PORT3, IO_PINx8);  // SCLK  
	IO_DisableFunc(IO_PORT3, IO_PINx7);  // SI
 	IO_DisableFunc(IO_PORT3, IO_PINx6);  // SO
    IO_DisableFunc(IO_PORT3, IO_PINx4);  // PA
    
    IO_ConfigGPIOPin(IO_PORT3, IO_PINxD, IO_DIR_INPUT, IO_PULLUP_OFF);
    IO_ConfigGPIOPin(IO_PORT3, IO_PINx8, IO_DIR_INPUT, IO_PULLUP_OFF);
    IO_ConfigGPIOPin(IO_PORT3, IO_PINx7, IO_DIR_INPUT, IO_PULLUP_OFF);// input
    IO_ConfigGPIOPin(IO_PORT3, IO_PINx4, IO_DIR_INPUT, IO_PULLUP_OFF); //PA
    
    IO_WriteGPIOPin(IO_PORT3, IO_PINxD, IO_BIT_CLR);   // CSN 
    IO_WriteGPIOPin(IO_PORT3, IO_PINx8, IO_BIT_CLR);   // SLCK 
    IO_WriteGPIOPin(IO_PORT3, IO_PINx7, IO_BIT_CLR);   // SI 
    IO_WriteGPIOPin(IO_PORT3, IO_PINx4, IO_BIT_CLR);   // PA 
    
    /* MCU RF 电平一致 */
//    CSN_DIR_OUT;  
//    CSN_OUT_H;
//    SCLK_DIR_OUT;
//    SCLK_OUT_L;
//    SI_DIR_OUT;
//    SI_OUT_L;
//    SO_DIR_IN;
    
    /* MCU RF 电平不一致 */
    CSN_OUT_H;
    SCLK_OUT_L;
    SI_OUT_L;
    SO_DIR_IN;
   
    CC1101_ExtInt_init();
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
//static void CC1101_displayTxCommSymbol(void)
//{
//    SetSegment(SEG_M1, 0);  //display
//    SetSegment(SEG_M2, 0);  //display
//    SetSegment(SEG_M3, 0);  //display
//    SetSegment(SEG_M4, 0);  //display
//    SetSegment(SEG_M5, 0);  //display
//    SetSegment(SEG_M6, 0);  //display
//    SetSegment(SEG_M7, 0);  //display
//    SetSegment(SEG_M8, 0);  //display
//    
//    RefreshLCD();
//}

/*******************************************************************************
* Description :  comm -- communication
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
//static void CC1101_displayRxCommSymbol(void)
//{
//    SetSegment(SEG_sign, 0);  //display
//    SetSegment(SEG_s19, 0);   //display 
//    SetSegment(SEG_s20, 0);   //display 
//    SetSegment(SEG_s21, 0);   //display 
//    SetSegment(SEG_s22, 0);   //display 
//    
//    RefreshLCD();
//}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
//static void CC1101_sleepMode(void)
//{
//    CC1101_ExtInt_disable();  
//    
//    SpiWriteStrobe(SIDLE);
//    delayUs(500);
//    SpiWriteStrobe(SPWD);
//    delayMs(5);               //5ms 时间内不允许转到其他模式
//}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
//void CC1101_standbyMode(void)
//{
//    SpiWriteStrobe(SIDLE);	 
//}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void CC1101_RxMode(void)
{
    SpiWriteStrobe(SIDLE);	 
    delayUs(200);
    SpiWriteStrobe(SFRX);		// clear RF RX buff (buff index return 0)
    delayUs(200);
    SpiWriteStrobe(SRX); 		
    
    CC1101_ExtInt_enable();   
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void CC1101_reset(void)
{
    CSN_OUT_H;	    
    delayUs(200);
    CSN_OUT_L; 				
    delayUs(200);
    CSN_OUT_H; 				
    delayUs(200);
    CSN_OUT_L; 	
    
    if (TestSOMI())
    {    
        SPI_byte(SRES);	      // GDO2 _|-|_ 3.5us      
    }    
    else
    {
        RF.SOMI_false = 1u;       
    }          
    delayUs(200);
    if (TestSOMI())
    {
        NOP();
    }  
    else 
    {
         RF.SOMI_false = 1u;       
    }
    
    CSN_OUT_H; 	
}

/*******************************************************************************
* Description :   47 reg
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void CC1101_regSetting(void)
{
    uint8_t i = 0;
    
    for(i=0; i<47; i++)
    {
       SpiWriteReg(CC1101_regConfigTab[i][0],     // command
                   CC1101_regConfigTab[i][1]);    // value
    }
    
    /* read reg check */
    
    SpiWriteStrobe(SCAL);
    delayMs(30);    
	SpiWriteStrobe(SIDLE);
}

/*******************************************************************************
* Description : 8 level
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void CC1101_setPATable(void)
{
    SpiWriteBurstReg(PATABLE, (uint8_t *)PA_Tab, 8);    
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void CC1101_setTxPower(uint8_t value)
{
    SpiWriteReg(FREND0, value);   
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void CC1101_initStep(void)
{
    CC1101_ExtInt_disable();      

    CC1101_pinMode(); 
    CC1101_reset();        //GDO2 _|-|_  3.5us  
    CC1101_regSetting();       

    CC1101_setPATable();      //8 level 0~7
    CC1101_setTxPower(PA_LEVEL_7);  // choice max PA

#ifdef RF_PA_EN
    pinPA_Tx_DIS;      /* Tx don't through PA and Rx */
#endif	

	SpiWriteStrobe(SIDLE);	 
    delayUs(200);
    
#ifdef RF_SLAVE         
     CC1101_RxMode();    //default Rx mode enalbe ExtInt
#endif   
}

/*******************************************************************************
* Description : CC1101_init
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void CC1101_init(void)
{
    uint8_t i = 0u;  
    
    memset(&RF, 0, sizeof(RF));   /* init RF data struct */
    memset(ReadMeterSerialNumber, 0, sizeof(ReadMeterSerialNumber));
    //..
    oneDayCnt = ONEDAYTIME;

#ifdef RF_PA_EN
    pinPA_Tx_DIS;      /* Tx don't through PA and Rx */
#endif

    CC1101_initStep();
    
    for (i=0; i<3; i++)
    { 
        if (RF.SOMI_false)
        {  
            RF.SOMI_false = 0;
            CC1101_initStep();
        } 
        else
        {
            break;     /* CC1101 init fail */  
        }    
    }//end for
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
//static void CC1101_waitSend(void)
//{
//    uint32_t i = 0;
//    
//    for (i=0; i<1000; i++)
//    {
//        delayMs(5u);     //SpiWriteStrobe(STX);  设置后出波形有延时   
//        if (GDO2_IN_H)
//        {     
//            delayMs(1u);
//            if (GDO2_IN_L)      // __|--|__
//            {
//                NOP();
//                break;
//            }
//            else
//            {
//                NOP();
//            }//end if
//        }
//        else
//        {
//            NOP();
//            break;
//        }    
//    }//end for
//}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void isSend(void)
{
    
}


/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
//void isSending(void)
//{
//    
//}

/*******************************************************************************
* Description : 
* Syntax      : CC1101(TxBuff, RF_payloadSize)
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void CC1101_Send(uint8_t *TxBuff, const uint8_t len)
{
#ifdef CC1101_DISPLAY_TX_COMM_SYMBOL_EN    
    CC1101_displayTxCommSymbol();
#endif    
    CC1101_ExtInt_disable();  
    
    SpiWriteStrobe(SIDLE);      /* IDL状态下进行配置 */
    delayUs(200);
    CC1101_setPATable();      //8 level 0~7
    CC1101_setTxPower(PA_LEVEL_7);  // choice max PA
    SpiWriteStrobe(SFTX);
    delayUs(200);
    SpiWriteBurstReg(TXFIFO, TxBuff, len); 
    delayUs(200);

#ifdef RF_PA_EN    
    pinPA_Tx_EN;   // Tx PA  (RF PA内部 三极管有延时,配置完PA需 delayUs(200);)
    delayMs(5);
#endif
        
    //delayMs(10);   // ARM 频率快  延时发送 20ms <= T <= 500ms
    SpiWriteStrobe(STX);             

    //CC1101_waitSend();  //待改
//    delayMs(55);   // 20K
    
    delayMs(5);      /* 预留间隔时间 */

#ifdef RF_PA_EN  	
    pinPA_Tx_DIS;  // Tx don't through PA and Rx 
#endif

//------------------------------------------   
#ifdef RF_SLAVE    
    SpiWriteStrobe(SIDLE);      /* IDL状态下进行配置 */
    delayUs(200);
    CC1101_setPATable();      //8 level 0~7
    CC1101_setTxPower(PA_LEVEL_1);  // choice max PA
    
    delayMs(10);   // ARM 频率快  延时发送
    SpiWriteStrobe(STX);            
    
    delayMs(55);
    //CC1101_waitSend();  //待改
    delayMs(5);   
#endif
    
    CC1101_RxMode();  //default Rx mode enalbe ExtInt
    
    //wzg test
    IO_WriteGPIOPin(IO_PORT4,IO_PINxC,IO_BIT_SET);   //M-RXD-LED
    IO_WriteGPIOPin(IO_PORT3,IO_PINxF,IO_BIT_CLR);   //M-TXD-LED 
    //---------------------
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
//void CC1101_readRSSI(void)
//{
//    
//}

/*******************************************************************************
* Description : 透传,不改变原结构，故新增一个查询函数
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : available = 1
*               invalid = 0
*******************************************************************************/
uint8_t CC1101_available(uint8_t *rxBuff, uint8_t len)
{
    uint8_t return_val = 0u;
   
    len = len;
    
    CC1101_Recv();
    
    if (1u == RF.availableFlag)
    {
        RF.availableFlag = 0u; 
        return_val = 1u;
        len = RF_shortPayloadSize;  
        memcpy(rxBuff, RF.RxBuff, RF_shortPayloadSize);
		
#ifdef  TEST_SEND
		RF.RxBuff[0]=0x66;
		RF.RxBuff[1]=0x33;  //00123456
		RF.RxBuff[2]=0x45;
		RF.RxBuff[3]=0x67;
		RF.RxBuff[4]=0x89;
		RF.RxBuff[5]=0x53;
		RF.RxBuff[6]=0x66;
		RF.RxBuff[7]=0x66;
		RF.RxBuff[8]=0x66;
		RF.RxBuff[9]=0x66;
		RF.RxBuff[10]=0x66;
		RF.RxBuff[11]=0x66;
		RF.RxBuff[12]=0x85;
		CC1101_Send(RF.RxBuff,13);
#endif 
        
    }
    else
    {
    
    }    
    
    return return_val;
}

/*******************************************************************************
* Description : uint8_t CC1101_Recv(void)
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void CC1101_Recv(void)
{
    uint8_t pktlen = 0u;
    uint8_t CRC_flag = 0u;
//    uint8_t return_val = 0u;
    
	if (RF.RX_FLG) //if (RF_RX_FLAG)
	{
		RF.RX_FLG = 0u;
		RF.availableFlag = 0u; // CC1101_available arg 
		memset(RF.TxBuff, 0u, sizeof(RF.TxBuff)); 
		memset(RF.RxBuff, 0u, sizeof(RF.RxBuff));
        
        RFModel.RFInitTimes =RF_INITTIME; 
        
#ifdef CC1101_DISPLAY_RX_COMM_SYMBOL_EN            
        CC1101_displayRxCommSymbol();
        IO_WriteGPIOPin(IO_PORT4,IO_PINxC,IO_BIT_CLR);   //M-RXD-LED
        IO_WriteGPIOPin(IO_PORT3,IO_PINxF,IO_BIT_SET);   //M-TXD-LED  
#endif
		CC1101_ExtInt_disable();  

        SpiWriteStrobe(SIDLE);    

        pktlen = SpiReadStatus(RXBYTES) & NUM_RXBYTES;  //62 byte return

        if (RF_shortBuffMaxSize == pktlen)  //62 byte
        {
            SpiReadBurstReg(RXFIFO, RF.RxBuff, RF_shortBuffMaxSize);
            
            CRC_flag = SpiReadStatus(LQI);
            if (CRC_flag & CRC_OK)	
            {
                //return_val = 1u;	
              /*----- CC1101_available arg ---------*/
                RF.availableFlag = 1u; 
              /*----- CC1101_available arg ---------*/  
            }
            else 
            {
//                CC1101_ExtInt_enable();
//                CC1101_RxMode();   /* CRC error */
//                return_val = 0u;
            }//end if CRC check	
        }
        else   
        {
//            CC1101_ExtInt_enable();
//            CC1101_RxMode();      
//            return_val = 0u;
        }//end if  length check 

//		SpiWriteStrobe(SIDLE);	 
//		delayUs(200);
        CC1101_RxMode();  // 恢复默认 Rx
        
#ifdef RF_SLAVE         
        CC1101_RxMode();    //default Rx mode enalbe ExtInt
#endif        
	}
	else     
	{
        //..
	}//end if (RF_RX_FLAG)    
    
    //return return_val;
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void CC1101_debug(void)
{
#ifdef CC1101_RX_Debug

    //..
    
    
#endif
    
#define CC1101_TX_Debug    
#ifdef CC1101_TX_Debug   

    //..
//    memset(RF.TxBuff, 0xAA, RF_payloadSize);
//    RF.TxBuff[0] = 0xFE;
//    RF.TxBuff[1] = 0xFE;
//    RF.TxBuff[2] = 0x68;
//    RF.TxBuff[4] = 0x00;
//    RF.TxBuff[5] = 0x00;
//    RF.TxBuff[6] = 0x00;
//    RF.TxBuff[7] = 0x00;
//    RF.TxBuff[8] = 0x00;
//    RF.TxBuff[9] = 0x22;
//    RF.TxBuff[10] = 0x68;
//    RF.TxBuff[11] = 0x11;
//    RF.TxBuff[12] = 0x04;
//    RF.TxBuff[13] = 0x3F;
//    RF.TxBuff[14] = 0x34;
//    RF.TxBuff[15] = 0x33;
//    RF.TxBuff[16] = 0x37;
//    RF.TxBuff[17] = 0xE4;
//    RF.TxBuff[18] = 0x16;    
    
    while (1)
    {
        CC1101_Send(RF.TxBuff, RF_shortPayloadSize);
        delayMs(20);
    }    
//    FE FE 68 00 00 00 00 00 22 68 11 04 3F 34 33 37 E4 16 //时间
    
//    TX_buff[0] = 0x66;             // test 测试数据帧  显示表地址
//    TX_buff[1] = 0xCC;
//    TX_buff[2] = 0xCC;
//    TX_buff[3] = 0xCC;
//    TX_buff[4] = 0xCC;
//    TX_buff[5] = 0xAF;
//    TX_buff[6] = 0x33;
//    TX_buff[7] = 0x33;
//    TX_buff[8] = 0x33;
//    TX_buff[9] = 0x33;
//    TX_buff[10] = 0x33;
//    TX_buff[11] = 0x33;
//    TX_buff[12] = 0x77;
    
#endif
    

}



