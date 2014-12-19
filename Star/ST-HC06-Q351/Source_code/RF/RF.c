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

#include "include.h"

/*** static function prototype declarations ***/
static void CC1101_ExtInt_init(void);
static void CC1101_ExtInt_enable(void);
static void CC1101_ExtInt_disable(void);
static void CC1101_pinMode(void);
static void CC1101_displayTxCommSymbol(void);
static void CC1101_displayRxCommSymbol(void);
//static void CC1101_sleepMode(void);
static void CC1101_RxMode(void);
static void CC1101_reset(void);
static void CC1101_regSetting(void);
static void CC1101_setPATable(void);
static void CC1101_setTxPower(uint8_t value);
static void CC1101_initStep(void);
static void CC1101_waitSend(void);

static void CC1101_Recv(void);


/*** static variable declarations ***/



/*** extern variable declarations ***/
RF_TYPE  RF;      /* RF struct */
uint32_t oneDayCnt;   





/********************************************
* 函数功能:  在规定的时间持续没有接受到RF帧 RF 定时初始化函数
* 输入参数:  
* 输出参数:
* 函数说明:  1S调用一次     1天 初始化 一次 RF
*   RFReset(); //!!!  这个函数会造成 GDO2 有一个_|—|_ 电平变化 使程序进入 RX中断服务程序 
*   SPI_byte(SRES); //reset chip   注意!!!  这个语句会造成 GDO2 有一个_|—|_ 电平变化 使程序进入 RX中断服务程序     
* 使用的资源: 
********************************************/
void TimeIntervalInitRF(void)
{
    if(oneDayCnt)
    {
        oneDayCnt--;            
    }
    else
    {
        if(!oneDayCnt)
        {   
            CC1101_init();            
            oneDayCnt = ONEDAYTIME;
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
static void RF_ExtIntCallbackFunc(void)
{
    RF.RX_FLG = 1u;
    EXTI_ClrIntFlag(EXTI_CH11);
} 

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void CC1101_ExtInt_init(void)
{
    IO_EnableFunc(IO_PORT3, IO_PINx8);
    IO_ConfigFuncINTxPin(IO_EXT_INT_CH11, IO_INTx_INTx_1);
    IO_ConfigGPIOPin(IO_PORT3, IO_PINx1, IO_DIR_INPUT, IO_PULLUP_OFF);
    
    EXTI_DisableInt(EXTI_CH11);
    EXTI_SetIntDetectMode(EXTI_CH11, EXTI_EDGE_FALLING);  //采用 RF IRQ 下降沿 (接收完成产生) ) 
    EXTI_ClrIntFlag(EXTI_CH11);
    EXTI_EnableInt(EXTI_CH11, RF_ExtIntCallbackFunc);
    NVIC_EnableIRQ(EXINT8_31_IRQn);  //
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
    EXTI_ClrIntFlag(EXTI_CH11);//clear hardware flg
    EXTI_EnableInt(EXTI_CH11, RF_ExtIntCallbackFunc);//ext Int en
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
    EXTI_DisableInt(EXTI_CH11);//ext Int dis
    EXTI_ClrIntFlag(EXTI_CH11);//clear hardware flg
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
    IO_DisableFunc(IO_PORT0, IO_PINx2);  // CSN
    IO_DisableFunc(IO_PORT0, IO_PINx0);  // SCLK  
	IO_DisableFunc(IO_PORT1, IO_PINx5);  // SI
 	IO_DisableFunc(IO_PORT1, IO_PINx4);  // SO
    IO_DisableFunc(IO_PORT3, IO_PINx4);  // PA
    
    IO_ConfigGPIOPin(IO_PORT0, IO_PINx2, IO_DIR_INPUT, IO_PULLUP_OFF);
    IO_ConfigGPIOPin(IO_PORT0, IO_PINx0, IO_DIR_INPUT, IO_PULLUP_OFF);
    IO_ConfigGPIOPin(IO_PORT1, IO_PINx5, IO_DIR_INPUT, IO_PULLUP_OFF);// input
    IO_ConfigGPIOPin(IO_PORT3, IO_PINx4, IO_DIR_INPUT, IO_PULLUP_OFF); //PA
    
    IO_WriteGPIOPin(IO_PORT0, IO_PINx2, IO_BIT_CLR);   // CSN 
    IO_WriteGPIOPin(IO_PORT0, IO_PINx0, IO_BIT_CLR);   // SLCK 
    IO_WriteGPIOPin(IO_PORT1, IO_PINx5, IO_BIT_CLR);   // SI 
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
static void CC1101_displayTxCommSymbol(void)
{
    SetSegment(SEG_M1, 0);  //display
    SetSegment(SEG_M2, 0);  //display
    SetSegment(SEG_M3, 0);  //display
    SetSegment(SEG_M4, 0);  //display
    SetSegment(SEG_M5, 0);  //display
    SetSegment(SEG_M6, 0);  //display
    SetSegment(SEG_M7, 0);  //display
    SetSegment(SEG_M8, 0);  //display
    
    RefreshLCD();
}

/*******************************************************************************
* Description :  comm -- communication
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void CC1101_displayRxCommSymbol(void)

{
    SetSegment(SEG_sign, 0);  //display
    SetSegment(SEG_s19, 0);   //display 
    SetSegment(SEG_s20, 0);   //display 
    SetSegment(SEG_s21, 0);   //display 
    SetSegment(SEG_s22, 0);   //display 
    
    RefreshLCD();
}
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

    pinPA_Tx_DIS;      /* Tx don't through PA and Rx */
    
    CC1101_RxMode();       //default Rx mode enalbe ExtInt
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
    
    pinPA_Tx_DIS;      /* Tx don't through PA and Rx */

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
    TaskAdd(CC1101_Recv, 300, 20);  //CSH
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void CC1101_waitSend(void)
{
    uint32_t i = 0;
    
    for (i=0; i<1000; i++)
    {
        delayMs(5u);     //SpiWriteStrobe(STX);  设置后出波形有延时   
        if (GDO2_IN_HIGH)
        {     
            delayMs(1u);
            if (GDO2_IN_LOW)      // __|--|__
            {
                __NOP();
                break;
            }
            else
            {
                __NOP();
            }//end if
        }
        else
        {
            __NOP();
            break;
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
    CC1101_displayTxCommSymbol();
    
    CC1101_ExtInt_disable();  
    
    SpiWriteStrobe(SIDLE);      /* IDL状态下进行配置 */
    delayUs(200);
    CC1101_setPATable();      //8 level 0~7
    CC1101_setTxPower(PA_LEVEL_7);  // choice max PA
    SpiWriteStrobe(SFTX);
    delayUs(200);
    SpiWriteBurstReg(TXFIFO, TxBuff, len); 
    delayUs(200);
    
    pinPA_Tx_EN;   // Tx PA  (RF PA内部 三极管有延时,配置完PA需 delayUs(200);)
        
    delayMs(10);   // ARM 频率快  延时发送
    SpiWriteStrobe(STX);            

    CC1101_waitSend();  //待改
    delayMs(5);        /* 预留间隔时间 */
    
    pinPA_Tx_DIS;  // Tx don't through PA and Rx 
    
//------------------------------------------    
    SpiWriteStrobe(SIDLE);      /* IDL状态下进行配置 */
    delayUs(200);
    CC1101_setPATable();      //8 level 0~7
    CC1101_setTxPower(PA_LEVEL_1);  // choice max PA
    
    delayMs(10);   // ARM 频率快  延时发送
    SpiWriteStrobe(STX);            

    CC1101_waitSend();  //待改
    delayMs(5);   
        
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
uint8_t CC1101_available(uint8_t *rxBuff, uint8_t *len)
{
    uint8_t return_val = 0u;
    
    if (1u == RF.availableFlag)
    {
        RF.availableFlag = 0u; 
        return_val = 1u;
        *len = RF_payloadSize;  //60 byte
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
static void CC1101_Recv(void)
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
        
        CC1101_displayRxCommSymbol();

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
        
        CC1101_RxMode();    //default Rx mode enalbe ExtInt
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
/*
void CC1101_debug(void)
{
#ifdef CC1101_RX_Debug

    //..
    
    
#endif
    
#ifdef CC1101_TX_Debug   

    //..
    
#endif
    
    //CC1101_pinMode();
    //SPI_byte(0xAA);
}
*/


