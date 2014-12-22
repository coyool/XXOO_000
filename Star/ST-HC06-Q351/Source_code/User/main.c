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
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：MCU -- M430FE427 
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/




/*** static variable declarations ***/



/*** extern variable declarations ***/
//__IO u32 systick_cnt = 0u;
//__IO u32 systick_ms = 0u;

const u32 systick_fixedTime = 1000u;


void SYS_Init(void)
{
    u32 i = 0u; 
//---------------------------------------------
    // Before we go to high speed we need to make sure the supply voltage is adequate. 
    SVSCTL |= (SVSON | 0x60);       // 0x60 检测电压为2.5V
    // Wait for adequate voltage to run at full speed 
    while ((SVSCTL & SVSOP))
    // dummy loop ;
    // The voltage should now be OK to run the CPU at full speed. Now it should
    // be OK to use the SVS as a reset source. 
     SVSCTL |= PORON;               // 电压低时会产生复位
//----------------------------------------------
    // Configure load caps 
	FLL_CTL0 |= XCAP14PF;
    // Loop until 32kHz crystal stabilizes
    do
    {
        IFG1 &= ~OFIFG;                 // Clear oscillator fault flag
        for (i = 50000; i>0; i--);        // Delay
    }
    while (IFG1 & OFIFG);               // Test osc fault flag
	
#define MAIN_FREQ  8
#if (8 == MAIN_FREQ)              //main frequency 8 MHz
    SCFI0 = FN_3 | FLLD_4;        //单指令 约等于 119 ns (实际略微大于)
    SCFQCTL = 64 - 1;               
#else
	SCFI0 = 0x00;  
    SCFQCTL |= 32-1;        
#endif    
    FLL_CTL0 |= DCOPLUS;  
    
    for (i=0; i<10000; i++)
    {
      _NOP();
    }      
}

/*******************************************************************************
* Description : main Frequency 8MHz, base timer clock = 32768Hz fixed time = 5ms, 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 待改 现在是1S
*******************************************************************************/
void BasicTimer_init(void)
{
    BTCTL = BT_fLCD_DIV64 | BT_fCLK2_DIV128 | BT_fCLK2_ACLK_DIV256;// ACLK/(256*128)
    IE2 |= BTIE;          //Basic Timer1 interrupt enable
    /* Enable the 1 second counter interrupt */
    // 采用32768时钟，256*128分频
    
}//end basic timer1 init

/*******************************************************************************
* Description : peripheral setup
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void setup(void)
{
    /* Unlock protected registers */


    /* Init System, IP clock and multi-function I/O */
    SYS_Init();
	
    /* Lock protected registers */

    
    /* global variable init */
//    systick_cnt = 0u;
    
    /* Peripheral and Sheild setup */
    BasicTimer_init();
    Serial_begin(bps_9600);
	CC1101_init();
    
//    LED_init();
    
    /*------------------------------------------------------------------------*/
    /* pwer on action                                                         */
    /*------------------------------------------------------------------------*/
    /* LED */
//    Blink(36, 10);


    /* printf log */
//    printf(" programing information: \r\n");
//    printf("\r\n ("__DATE__ "  " __TIME__ ") \r\n");
//    printf("version: BUZT13-01CN V1.0 \r\n");
//    printf("\r\nCPU @ %d Hz\r\n", SystemCoreClock);
//    printf("\r\n");
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void sizeofTest(void)
{
    u8 temp= 0u;
    temp = sizeof(char);
    temp = sizeof(int);
    temp = sizeof(long);
}

/*******************************************************************************
* 函数名称: main
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
void main(void)
{ 
	u8 taskFlag = 0u;
	
    /* power on setup */ 
    setup();

    __enable_irq();   //EA = 1

    while (1)
    {
        taskFlag = Serial_available(RF.RxBuff, Serial_fixed_TxRx_Len);
    	if (1u == taskFlag)
    	{
			CC1101_Send();
		}
		else
		{
		}

		taskFlag = CC1101_available(Serial.TxBuff, RF_payloadSize);
    	if (1u == taskFlag)
    	{
    		Serial_TxMode();
			Serial_send();
		}
		else
		{
		}	
		
  		
    }//end while
}