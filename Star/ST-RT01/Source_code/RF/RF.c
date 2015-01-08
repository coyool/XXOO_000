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
* 作    者：F06553
* 完成日期：2014-12-2
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include "all_header_file.h"

/*** static function prototype declarations ***/
static void CC1101_ExtInt_init(void);
static void CC1101_ExtInt_enable(void);
static void CC1101_ExtInt_disable(void);
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
const u8 RF_payloadSize = 60u;
const u8 RF_buffMaxSize = 62;   // 60 payload + 2 CRC  {PKTLEN, 0x3C} payload = 60 

RF_TYPE  RF;      /* RF struct */
uint32_t oneDayCnt;   


//#define RF_SLAVE


/********************************************
* 函数功能:  在规定的时间持续没有接受到RF帧 RF 定时初始化函数
* 输入参数:  
* 输出参数:
* 函数说明:  1S调用一次     1天 初始化 一次 RF
*   RFReset(); //!!!  这个函数会造成 GDO2 有一个_|—|_ 电平变化 使程序进入 RX中断服务程序 
*   SPI_byte(SRES); //reset chip   注意!!!  这个语句会造成 GDO2 有一个_|—|_ 电平变化 使程序进入 RX中断服务程序     
* 使用的资源: 
********************************************/
//void TimeIntervalInitRF(void)
//{
//    if(oneDayCnt)
//    {
//        oneDayCnt--;            
//    }
//    else
//    {
//        if(!oneDayCnt)
//        {   
//            CC1101_init();            
//            oneDayCnt = ONEDAYTIME;
//        } 
//    }    
//}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
//static void RF_ExtIntCallbackFunc(void)
//{
//    RF.RX_FLG = 1u;
//    EXTI_ClrIntFlag(EXTI_CH11);
//} 

/*******************************************************************************
* Description :    failling edge
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void CC1101_ExtInt_init(void)
{
	GDO2_EXTI_EDGE_HL;	  
	GDO2_HARDWARE_FlAG_CLEAR;
	GDO2_EXTI_EN;
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void CC1101_ExtInt_enable(void)
{
    GDO2_EXTI_EDGE_HL;	  
    GDO2_HARDWARE_FlAG_CLEAR;    //clear hardware flg
    GDO2_EXTI_EN;                //ext Int en
}

/*******************************************************************************
* Description : RF ExtINt
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void CC1101_ExtInt_disable(void)
{
    GDO2_EXTI_DIS;               //ext Int dis
    GDO2_HARDWARE_FlAG_CLEAR;    //clear hardware flg
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
    
    /* MCU RF 电平一致 */
    CSN_DIR_OUT;  
    CSN_OUT_H;
    SCLK_DIR_OUT;
    SCLK_OUT_L;
    SI_DIR_OUT;
    SI_OUT_L;
    SO_DIR_IN;

	GDO2_DIR_IN;
    
    /* MCU RF 电平不一致 */
//    CSN_OUT_H;
//    SCLK_OUT_L;
//    SI_OUT_L;
//    SO_DIR_IN;
   
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
   
    CC1101_Recv();
    
    if (1u == RF.availableFlag)
    {
        RF.availableFlag = 0u; 
        return_val = 1u;
//        len = RF_payloadSize;  //60 byte
        memcpy(rxBuff, RF.RxBuff, RF_payloadSize);
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
        
#ifdef CC1101_DISPLAY_RX_COMM_SYMBOL_EN            
        CC1101_displayRxCommSymbol();
#endif
		CC1101_ExtInt_disable();  

        SpiWriteStrobe(SIDLE);    

        pktlen = SpiReadStatus(RXBYTES) & NUM_RXBYTES;  //62 byte return

        if (RF_buffMaxSize == pktlen)  //62 byte
        {
            SpiReadBurstReg(RXFIFO, RF.RxBuff, RF_buffMaxSize);
            
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
        CC1101_RxMode(); 
        
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
    memset(RF.TxBuff, 0xAA, RF_payloadSize);
    RF.TxBuff[0] = 0xFE;
    RF.TxBuff[1] = 0xFE;
    RF.TxBuff[2] = 0x68;
    RF.TxBuff[4] = 0x00;
    RF.TxBuff[5] = 0x00;
    RF.TxBuff[6] = 0x00;
    RF.TxBuff[7] = 0x00;
    RF.TxBuff[8] = 0x00;
    RF.TxBuff[9] = 0x22;
    RF.TxBuff[10] = 0x68;
    RF.TxBuff[11] = 0x11;
    RF.TxBuff[12] = 0x04;
    RF.TxBuff[13] = 0x3F;
    RF.TxBuff[14] = 0x34;
    RF.TxBuff[15] = 0x33;
    RF.TxBuff[16] = 0x37;
    RF.TxBuff[17] = 0xE4;
    RF.TxBuff[18] = 0x16;    
    
    while (1)
    {
        CC1101_Send(RF.TxBuff, RF_payloadSize);
        delayMs(20);
    }    
//    FE FE 68 00 00 00 00 00 22 68 11 04 3F 34 33 37 E4 16 //时间
    
#endif
    

}



