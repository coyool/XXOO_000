/*********************************************************************
* Copyright (c) 2013, star
* All rights reserved.
* 
* 文件名称：RF.libraly
* 文件标识：
* 摘    要：
* 
* 当前版本：V 1.0
* 作    者：F06553  vendetta
* 完成日期：2013年12月6日
*
* 取代版本：V 1.1 
* 原作者  ：输入原作者（或修改者）名字
* 完成日期：年 月 日
*
* 文件说明：RF 基本参数  408.925MHz    47.5KHz    325KHz   
*                           测试宏:  #define  TEST
*
***********************************************************************/

#include "Include.h"
/*
#include  "SPI.h"
#include  "delay.h"
#include  "IO.h"
*/

/************************* 内部函数声明 *******************************/
void RFReceiveMessage(void);
void DataBatProcess(uint8 *StartAddr, uint8 Length, uint8 SubData, uint8 Flag);
void RFSendEnergy(uint8 Command);
void RFSendACData(uint8 DI);
void RFSendStealEnergyCount(uint8 DI);
void RFSendPowerFactor(uint8 DI);
void RFSendPower(uint8 DI);
void Set_meter_address(uint8 DI);
uint8 CalcAddSum(uint8 *StartAddr, uint8 Length);
void HEX_TO_BCD(uint32 Data, uint8 DataByte, uint8 *Array, uint8 Type);
uint8 CheckSWH(void);
void UpdataSWH(void);
int over_deg_check(uint8 *value, uint8 length);
void SetUpCC1101PATABLE(void);
void SetCC1101TxPower(uint8 value);
void CC1101_Sleep(void);    //
void TimeIntervalInitRF(void);
void RF_init_config(void);  //


/***********************************************************/
uint8 TX_buff[15];
uint8 RX_buff[15];

const uint8_t PA_TABLE[8]= {0x00,0x12,0x0e,0x34,0x60,0xc5,0xc1,0xc0,};
uint8  ReadMeterSerialNumber[8];

uint8  RX_FIG;
uint8 SOMI_false;
//static uint8 Vol_low;     //初始化的状态 必须为 1
static uint8 RFWaitSendFlag;

//int32   Un;             //电压规格  
uint32  RF_OneHourcnt;    //1天 计时


/**************************************************
* 函数功能:  10进制转BCD码,低字节在前，高字节在后   
* 输入参数:  DI码            
* 输出参数:          
* 函数说明:  Buff1大返回1；Buf2大返回－1；相等返回0
* 使用资源:
**************************************************/
void HEX_TO_BCD(uint32 Data, uint8 DataByte, uint8 *Array, uint8 Type)
{
    if(Type == 0)
    {
        if(DataByte >= 1)
        {
            Array[DataByte-1] = (Data%10) | (((Data/10)%10)<<4);
        }
        if(DataByte >= 2)
        {
            Array[DataByte-2] = ((Data/100)%10) | (((Data/1000)%10)<<4);
        }
        if(DataByte >= 3)
        {
            Array[DataByte-3] = ((Data/10000)%10) | (((Data/100000)%10)<<4);
        }
        if(DataByte >= 4)
        {
            Array[DataByte-4] = ((Data/1000000)%10) | (((Data/10000000)%10)<<4);
        }
    }
    else
    {
        if(DataByte >= 1)
        {
            Array[0] = (Data%10) | (((Data/10)%10)<<4);
        }
        if(DataByte >= 2)
        {
            Array[1] = ((Data/100)%10) | (((Data/1000)%10)<<4);
        }
        if(DataByte >= 3)
        {
            Array[2] = ((Data/10000)%10) | (((Data/100000)%10)<<4);
        }
        if(DataByte >= 4)
        {
            Array[3] = ((Data/1000000)%10) | (((Data/10000000)%10)<<4);
        }
    }
}

/********************************************
* 函数功能:    十进制数检查  < = 10
* 输入参数:
* 输出参数:
* 函数说明:   
* 使用的资源：
********************************************/
int over_deg_check(uint8 *value, uint8 length)
{
    int i;
    uint8_t digit;

    for (i = 0;  i < length;  i++)
    {
      digit = *value++;
      if (((digit & 0x0F) < 0x0a) & (((digit & 0xf0)>>4)< 0x0a))
      return TRUE;
    }
      return FALSE;  
}

/********************************************
* 函数功能:  在规定的时间持续没有接受到RF帧 RF 定时初始化函数
* 输入参数:  
* 输出参数:
* 函数说明:  1S调用一次     1天 初始化 一次 RF
*   RFReset(); //!!!  这个函数会造成 GDO2 有一个_|—|_ 电平变化 使程序进入 RX中断服务程序 
*   SPI_byte(SRES); //reset chip   注意!!!  这个语句会造成 GDO2 有一个_|—|_ 电平变化 使程序进入 RX中断服务程序     
* 使用的资源: 
********************************************/
/*void TimeIntervalInitRF(void)
{
    if(RF_OneHourcnt)
    {
        RF_OneHourcnt--;
        if(!RF_OneHourcnt)
        {   
            RF_init();            
            RF_OneHourcnt = ONEDAYTIME;
        }             
    }  
}
*/
/********************************************
* 函数功能:    射频处理主程序
* 输入参数:
* 输出参数:
* 函数说明:   
* 使用的资源：
********************************************/
void RFProcess(void)
{
    uint8 Length;  //RF 帧长度
    
    if (!RX_FIG)   //判断接收标志 
        return;
        
    RX_FIG = 0;
    
    Disable_ExtInt();     //关闭 接收 IRQ中断
    memset(RX_buff, 0, sizeof(RX_buff));
    memset(TX_buff, 0, sizeof(TX_buff));
    RFWaitSendFlag = 0;

    SpiWriteStrobe(SIDLE); //手动切换待机
    Length = SpiReadStatus(RXBYTES) & NUM_RXBYTES;    
    if (RF_frame_length == Length)  
    {
         SpiReadBurstReg(RXFIFO, RX_buff, RF_frame_length);  //读取buff data
         if (RX_buff[CRC_POSITION] & RF_CRC_OK)
         {
              RFModel.RFInitTimes =RF_INITTIME;    //接收到完整一帧赋初值
             //DisplayCOM();  显示通信符  三相表无此功能
              RFReceiveMessage();
         } 
         
         if (RFWaitSendFlag)  
         {
              RFWaitSendFlag = 0;
			  //--------------------
			  //因为利尔达模块,控制PA上有个电流,导致控制的时候要延时一下
			  PA_OUT_L;
			  PublicDelayMs(5);  
			  //------------------------ 
			  
              RFSendData();
              PublicDelayMs(8);        //为了延时发送
              RFSendData();		   
              PublicDelayMs(3);              
#ifdef RF_MASH              
              SpiWriteReg(FREND0,0x12);//弱发送
              RFSendData();
              PublicDelayMs(3);
              SpiWriteReg(FREND0,0x17);//强发送
#endif      
			 //--------------------
			  //关闭
			  PA_OUT_H;
			  //------------------------   
         }   
		 
    }
    enable_RF_RX();   // 重启接收  
    RFExtInt_Init();  // 重启RF 接收中断
    
}

/********************************************
* 函数功能:    低功耗 RF 处理 
* 输入参数:
* 输出参数:
* 函数说明:  把 RF 用到的 SPI的IO 拉低  
* 使用的资源：
********************************************/
/*void SPI_POWER_DOWN_Init(void)
{
    IO_DisableFunc(IO_PORT4,IO_PINx9);
    IO_ConfigGPIOPin(IO_PORT4,IO_PINx9,IO_DIR_INPUT,IO_PULLUP_OFF);
    //==========================================================================
    IO_DisableFunc(IO_PORT4,IO_PINxA);
    IO_ConfigGPIOPin(IO_PORT4,IO_PINxA,IO_DIR_INPUT,IO_PULLUP_OFF);
    //==========================================================================
    IO_DisableFunc(IO_PORT4,IO_PINx8);
    IO_ConfigGPIOPin(IO_PORT4,IO_PINx8,IO_DIR_INPUT,IO_PULLUP_OFF);
    //==========================================================================
    IO_DisableFunc(IO_PORT4,IO_PINx1);
    IO_ConfigGPIOPin(IO_PORT4,IO_PINx1,IO_DIR_INPUT,IO_PULLUP_OFF);
    //==========================================================================
    IO_DisableFunc(IO_PORT4,IO_PINxB);
    IO_ConfigGPIOPin(IO_PORT4,IO_PINxB,IO_DIR_INPUT,IO_PULLUP_OFF);
    //==========================================================================   
}
*/

/********************************************
* 函数功能:    RF 开启接收状态
* 输入参数:
* 输出参数:
* 函数说明:   
* 使用的资源：
********************************************/
void enable_RF_RX(void)
{
    SpiWriteStrobe(SIDLE);	 
    SpiWriteStrobe(SFRX);				    // 清cc1100接收buffer 
    SpiWriteStrobe(SRX); 				    // 置为接收状态
    EXTI_ClrIntFlag(EXTI_CH8);
    RFExtInt_Init();  // 重启RF 接收中断
}

/********************************************
* 函数功能:    RF 睡眠模式
* 输入参数:
* 输出参数:
* 函数说明:   
* 使用的资源：
********************************************/
void CC1101_Sleep(void)
{
     Disable_ExtInt();     //关闭 接收 IRQ中断
     EXTI_ClrIntFlag(EXTI_CH8);
     SpiWriteStrobe(SIDLE);	
     SpiWriteStrobe(SPWD);
     SetUpCC1101PATABLE();     
     RX_FIG = 0;        //此句防止 写SPWD之前  RX_FIG标志被置位  
}


/********************************************
* 函数功能:    RF 开启发送状态
* 输入参数:
* 输出参数:
* 函数说明:    发送数据帧 长度 13 byte 
* 使用的资源：
********************************************/
void RFSendData(void)
{
    //EN_PA;       // 开启 外部PA
    SpiWriteStrobe(SIDLE);         //  重复进入IDLE

#ifdef  TEST    
    TX_buff[0] = 0x66;             // test 测试数据帧  显示表地址
    TX_buff[1] = 0xCC;
    TX_buff[2] = 0xCC;
    TX_buff[3] = 0xCC;
    TX_buff[4] = 0xCC;
    TX_buff[5] = 0xAF;
    TX_buff[6] = 0x33;
    TX_buff[7] = 0x33;
    TX_buff[8] = 0x33;
    TX_buff[9] = 0x33;
    TX_buff[10] = 0x33;
    TX_buff[11] = 0x33;
    TX_buff[12] = 0x77;
#endif
    
    SpiWriteBurstReg(TXFIFO, TX_buff, 13); 
    PublicDelayUs(200);                  // 将要发送数据写入cc1100buffer
    SpiWriteStrobe(STX);            // 发送数据(需等待发送完成) 
}

/**********************************************************************
* 函数功能: RF 上电、低功耗恢复 初始化
* 输入参数:
* 输出参数:
* 函数说明:  
* 使用的资源：
**********************************************************************/
void RF_init(void)
{
     uint8 i;  
    
     SOMI_false = 0;
     RF_init_config();
     for (i=0; i<3; i++)
     { 
          if (SOMI_false)
          {  
             SOMI_false = 0;
             RF_init_config();
          } 
          else
          {
               break;  //break for()
          }    
          
     }
}

/********************************************
* 函数功能:    RF功率表设置
* 输入参数:    
* 输出参数:
* 函数说明:    当设置功率表方式时,CC1101进入SPWD模式后,功率表会被清零,故需要重新赋值
* 使用的资源：
********************************************/
void SetUpCC1101PATABLE(void)
{
  SpiWriteBurstReg(PATABLE, (uint8_t *)PA_TABLE, 8); //设置功率表  
}

/********************************************
* 函数功能:    设置发送功率
* 输入参数:
* 输出参数:
* 函数说明:    value
* 使用的资源：
********************************************/
void  SetCC1101TxPower(uint8 value)
{
    SpiWriteReg(FREND0,value);     //选择最大功率，
}

/**********************************************************************
* 函数功能: RF 上电、低功耗恢复 初始化配置的个部分内容
* 输入参数:
* 输出参数:
* 函数说明: 上电配置为 待机状态 
*           Vol_low = 1 初始化默认值  RF 初始化状态为待机 
*           否则影响 RFModuleCheckProcess()函数的初始逻辑判断。
*           配置SPWD后 会清零发射功率reg的值，所以此处重新配置
* 使用的资源：
**********************************************************************/
void RF_init_config(void)
{
    //Vol_low = 1;   //上电 、 低功耗 初始化  
    RFModel.RFInitTimes =RF_INITTIME;   
    RX_FIG = 0; 
    memset(ReadMeterSerialNumber,0,8);
   
    Disable_ExtInt();   //关闭 RF IRQ中断    
    
    RF_IO_Init(); 
    RFReset();  
    RFSet();    //RF reg config
    
    //set TX power 
    SetUpCC1101PATABLE(); //设置功率表 
    SetCC1101TxPower(TXPOWERELEVEL7);   //选择最大功率
    
   // CC1101_Sleep();  //
    enable_RF_RX();    //上电直接进入接收模块, 此X328表的电源比较稳定,不需要考虑功耗大问题。
}

/********************************************
* 函数功能:    RF 复位
* 输入参数:
* 输出参数:
* 函数说明: SPI_byte(SRES); //reset chip   注意!!!  这个语句会造成 GDO2 有一个_|—|_ 电平变化 使程序进入 RX中断服务程序        
* 使用的资源：
********************************************/
void RFReset(void)
{    
    SCS_OUT_H;				//CSN=1;	
	PublicDelayUs(200);
	SCS_OUT_L; 				//CSN=0;  
	PublicDelayUs(200);
	SCS_OUT_H; 				//CSN=1;  
	PublicDelayUs(200);
	SCS_OUT_L; 				//CSN=0;	
	//while (1 == SO_IN);     //while (SOMI);SOMI=0表明晶体正在运作中，电压调制器已经稳定 
    if (TestSOMI())
    {    
	    SPI_byte(SRES);	        //reset chip   注意!!!  这个语句会造成 GDO2 有一个_|—|_ 电平变化 使程序进入 RX中断服务程序      
    }    
    else
    {
         SOMI_false = 1;        // TestSOMI 错误
    }    
        
	PublicDelayUs(50);  		
	//while (1 == SO_IN);     //while (SOMI);SOMI=0表明晶体正在运作中，电压调制器已经稳定  
    if (TestSOMI())
    {
        __NOP();
    }  
    else 
    {
         SOMI_false = 1;        // TestSOMI 错误
    }    
	SCS_OUT_H; 				//CSN=1;
}

/********************************************
* 函数功能:    RF 寄存器配置
* 输入参数:
* 输出参数:
* 函数说明:     
* 使用的资源：
********************************************/
void RFSet(void)
{
    SpiWriteReg(FSCTRL1,0x08);
    SpiWriteReg(FSCTRL0,0x00);
	
    SpiWriteReg(FREQ2,0x0f);
    SpiWriteReg(FREQ1,0xba);
    SpiWriteReg(FREQ0,0x56);
    
    SpiWriteReg(MDMCFG4,0x5b); //5BF8 100K    5983 20K  
    SpiWriteReg(MDMCFG3,0xf8);
    SpiWriteReg(MDMCFG2,0x03);  
    SpiWriteReg(MDMCFG1,0x22);
    SpiWriteReg(MDMCFG0,0xF8); 

    SpiWriteReg(DEVIATN,0x47); 
    
    SpiWriteReg(FREND1,0xb6); 
    SpiWriteReg(FREND0,0x10);   // 发射功率
     
    SpiWriteReg(MCSM1, 0x30);  
    SpiWriteReg(MCSM0, 0x18);  

    SpiWriteReg(FOCCFG,0x1D);
    SpiWriteReg(BSCFG, 0x1C); 
	
    SpiWriteReg(AGCCTRL2, 0xC7); 
    SpiWriteReg(AGCCTRL1, 0x00); 
    SpiWriteReg(AGCCTRL0, 0xB2); 
	
    SpiWriteReg(FSCAL3,   0xEA); 
    SpiWriteReg(FSCAL2,   0x2A); 
    SpiWriteReg(FSCAL1,   0x00);
    SpiWriteReg(FSCAL0,   0x1F); 
	
    //GDO设置  GDO2 output pin configuration.
    SpiWriteReg(IOCFG2,   0x06);
    SpiWriteReg(IOCFG0,   0x2E);  
    
    //同步字设置 Write custom sync word (avoiding default sync word)
    SpiWriteReg(SYNC1, 0x9B);
    SpiWriteReg(SYNC0, 0xAD);
    
    SpiWriteReg(PKTCTRL1, 0x04);
    SpiWriteReg(PKTCTRL0, 0x44);   

    SpiWriteReg(PKTLEN, 0x0d);
    SpiWriteReg(ADDR, 0x00);

    SpiWriteReg(CHANNR, 0x00);
    
    SpiWriteReg(FSTEST, 0x59);	
    SpiWriteReg(RF_TEST2,  0x88);
    SpiWriteReg(RF_TEST1,  0x31);  
    SpiWriteReg(RF_TEST0,  0x0b);
    
    //由于CC101是老片子，所以当发生掉电，外部缓冲电容容值大时，电平在后期下降缓慢，当在特定的电平时会进入死区，此时重新上电时RF片子复位不成功(有别于完全掉电在上电所进行的初始化)
    //以下没有赋值的寄存器的值有可能不是默认的值，需重新配置以解决此问题。
    SpiWriteReg(IOCFG1,  0x2e);  
    SpiWriteReg(FIFOTHR, 0x47);
    SpiWriteReg(MCSM2,   0x07);
    SpiWriteReg(WOREVT1, 0x87);
    SpiWriteReg(WOREVT0, 0X6B);
    SpiWriteReg(WORCTRL, 0xFB);
    SpiWriteReg(RCCTRL1, 0x41);
    SpiWriteReg(RCCTRL0, 0x00);
    SpiWriteReg(PTEST,   0x7F);
    SpiWriteReg(AGCTEST, 0x3F);
    //==========================================================================
	SpiWriteStrobe(SCAL);   
	PublicDelayMs(30);                       
	SpiWriteStrobe(SIDLE);
}


/**************************************************
* 函数功能:  计算校验和
* 输入参数:            
* 输出参数:          
* 函数说明:  Buff1大返回1；Buf2大返回－1；相等返回0
* 使用资源:
**************************************************/
uint8 CalcAddSum(uint8 *StartAddr, uint8 Length)
{
    uint8 Sum = 0;
    uint8 i = 0;
    for (i=0; i<Length; i++)
    {
        Sum += *(StartAddr+i);
    }
    return Sum;
}

/**************************************************
* 函数功能: 表地址 数据域  + 0x33 or - 0x33
* 输入参数:            
* 输出参数:          
* 函数说明:  
* 使用资源:
**************************************************/
void DataBatProcess(uint8 *StartAddr, uint8 Length, uint8 SubData, uint8 Flag)
{
    uint8 i;
    if (Flag == AddFlag)
    {
        for(i=0; i<Length; i++)
        {
            *(StartAddr+i) += SubData;     // + 0x33  
        }
    }
    else
    {
        for(i=0; i<Length; i++)
        {
            *(StartAddr+i) -= SubData;     // - 0x33
        }
    }
}

/**************************************************
* 函数功能:  反馈电能数据   
* 输入参数:  DI码            
* 输出参数:          
* 函数说明:  电能数据帧装帧
*            notice   EnergyDataErr  电表状态字 表地址 有功
*            0x15 (21 十进制) --->  0x21 (BCD) 电能数据转换过程
* 使用资源:
**************************************************/
void RFSendEnergy(uint8 Command)    //Command
{
	uint32 Energy;
	uint16 Status;
	uint8  meter_num[6];
	uint16 Result;
    
   /* if ( MeterInfoIsStateBitSet(STATE_READ_ENERGY_ERR)
		 || MeterInfoIsStateBitSet(STATE_SAVE_ENERGY_ERR))  //save and read  Error
    {
        return;
    }
	*/
	
    MeterInfoParaGet(0x01700002, (uint8*)&Status);	  //取电表状态字
    
    if ((Command == 0x53)||(Command == 0x54)||(Command == 0x55))
      EnergyParaGet(0x03140002, (uint8*)&Energy);		  //有功
    else if (Command ==0x77)
    {
         Result = EnergyParaGet(0x03160002, (uint8*)&Energy);		   //无功
         if (0x8000 == Result)
         {
             RFWaitSendFlag = 0;
         }  
    }
	else if (Command ==0x76)    
    {    
         Result = EnergyParaGet(0x03100002, (uint8*)&Energy);		   //视在
         if (0x8000 == Result)
         {
             RFWaitSendFlag = 0;
         }    
    }    
	MeterInfoParaGet(0x01010002, (uint8*)&meter_num);		 //取表地址6 byte

    memcpy(TX_buff+1, meter_num+2, 4); //66 + address
    TX_buff[5] = Command; //DI
    HEX_TO_BCD(Energy, 4, TX_buff+6, BackOrder); //十进制 转换为 BCD码
    TX_buff[10] = Status;      //增加电表状态字 只要低8 位， 高8位丢弃
   
#ifdef TEST_Energy	
    TX_buff[0] = 0x66;
    TX_buff[5] = Command;    //DI
    TX_buff[6] = 0x55;
    TX_buff[7] = 0x66;
    TX_buff[8] = 0x77;
    TX_buff[9] = 0x88;
    TX_buff[10] = 0xFF;
#endif 	
    /*************************
     uint16 Miss_A         ---- Tamper  
	 uint16 Miss_B
	 uint16 Miss_C		     
     uint16 Reverse_A	
	 uint16 Reverse_B
	 uint16 Reverse_C      ---- Tamper
	 uint16 BatteryLow     ---- Low batter
	 uint16 PowerDown      ---- power down 
	*************************/  
}

/**************************************************
* 函数功能:  反馈电压电流   
* 输入参数:  TX_buff   RX_buff    DI码              
* 输出参数:          
* 函数说明:  装帧
* 使用资源:  EnergyParaGet
**************************************************/
void RFSendACData(uint8 DI)
{
	uint32 Cur;
	uint16 Vol;

	memcpy(TX_buff, RX_buff, 5);
	switch (DI)
	{
        case 0x59:   
			EnergyParaGet(0x03800902, (uint8*)&Vol); //L1 电压
			EnergyParaGet(0x03800802, (uint8*)&Cur); //L1 电流
			break;
		case 0x31:
			EnergyParaGet(0x03801202, (uint8*)&Vol); //L2 电压
			EnergyParaGet(0x03801102, (uint8*)&Cur); //L2 电流
			break;
		case 0x32:
			EnergyParaGet(0x03801B02, (uint8*)&Vol); //L3 电压
			EnergyParaGet(0x03801A02, (uint8*)&Cur); //L3 电流
			break;
		default:
			RFWaitSendFlag = 0;
			break;
	}//end switch (DI)

	TX_buff[5] = DI;  //DI
    TX_buff[6] = Vol & 0xff;;
    TX_buff[7] = Vol >> 8;        //

	Cur = Cur/10;
    TX_buff[8] = Cur & 0xff;
    TX_buff[9] = Cur >> 8; 
    TX_buff[10] = Cur >> 16;	

#ifdef  TEST	
    TX_buff[5] = 0x59;  //DI
    TX_buff[6] = 0x00;
    TX_buff[7] = 0x22;
    TX_buff[8] = 0x00;
    TX_buff[9] = 0x05; 
    TX_buff[10] = 0x00;
#endif	
}

/**************************************************
* 函数功能:  反馈窃电次数   
* 输入参数:             
* 输出参数:          
* 函数说明:  装帧
* 使用资源:
**************************************************/
void RFSendStealEnergyCount(uint8 DI)
{
    uint16 Data;
	
	memcpy(TX_buff, RX_buff, 5); //66 + address
//wzg test    TamperParaGet(0x01860002, (uint8 *)&Data);
	TX_buff[5] = DI;  //0x7a
    TX_buff[6] = Data & 0xff;
    TX_buff[7] = Data>>8;
	
#ifdef	 TEST
    TX_buff[5] = 0x7a;  //DI
    TX_buff[6] = 0x99;
    TX_buff[7] = 0x99;
#endif   
}

/**************************************************
* 函数功能:  反馈功率因数 
* 输入参数:  DI码            
* 输出参数:          
* 函数说明:  装帧
* 使用资源:
**************************************************/
void RFSendPowerFactor(uint8 DI)
{
	uint16 Factor;
    
    memcpy(TX_buff, RX_buff, 5); //66 + address
    EnergyParaGet(0x03800002, (uint8*)&Factor);	
    TX_buff[5] = DI;  //0x7b
    //TX_buff[6] = Factor/100;   // 待定语句 为了小数位能对应上  ybz
    TX_buff[6] = Factor; 
    
#ifdef   TEST  
    TX_buff[5] = 0x7b;  //DI 
    TX_buff[6] = 100;
#endif	
}

/**************************************************
* 函数功能:  反馈当前功率 
* 输入参数:  DI码            
* 输出参数:          
* 函数说明:  装帧
* 使用资源:
**************************************************/
void RFSendPower(uint8 DI)
{   
    uint32 Data;
   
    memcpy(TX_buff, RX_buff, 5); //66 + address	
    EnergyParaGet(0x03810002, (uint8*)&Data);	
    
    TX_buff[5] = DI;  // 0x33 
    TX_buff[6] = Data&0xff;
    TX_buff[7] = (Data>>8)&0xff;
    TX_buff[8] = (Data>>16)&0xff;    
    
//#define TEST_Power     
#ifdef  TEST_Power
    TX_buff[6] = 0x92;
    TX_buff[7] = 0x0B;
    TX_buff[8] = 0x05;       //1.10214 * 3 = 3.30642 kw/h (220V  5A)
#endif 
    
}

/**************************************************
* 函数功能:  RF    设置表地址 
* 输入参数:  RX  新表地址            
* 输出参数:  TX  新表地址        
* 函数说明:  buff[6] = (头两个字节内部使用)  0x55 0x66 
*                      (实际表地址是后四个字节) 0x00 0x00 0x00 0x13 -- 13号表
*            设表地址需短接 J202
* 使用资源:
**************************************************/
void Set_meter_address(uint8 DI)
{
    uint8 i;
	uint16 Status;
	uint8 temp;
	uint8 buff[6];
	
    if (!IO_ReadGPIOPin(IO_PORT_PRO_SWI,IO_PIN_PRO_SWI))   /*防止上位机没有设置权限下设表地址提示设表地址成功,外部主控部分也有相应的处理 */
	{
        if ((RX_buff[6] != 0x99) || (RX_buff[7] != 0x99)
            || (RX_buff[8] != 0x99) || (RX_buff[9] != 0x99)) //设置的表地址不能为通配符
        {
             temp = over_deg_check(&RX_buff[6], 4);
             if (temp)    // 十进制数检查
             {
                 MeterInfoParaGet(0x01010002, buff); //取表地址6 byte
                 memcpy(&buff[2], &RX_buff[6], 4);
                 MeterInfoParaSet(0x01010002, &buff[0], 6);   //新地址替换旧地址
 				 DisplayAlarm(DIS_METER_NO, TRUE); //显示 新 表地址	

				 memset(buff, 0, 6);  //清零
				 MeterInfoParaGet(0x01010002, buff); //取表地址6 byte
                 for (i=0; i<5; i++)  
                 {
                     TX_buff[i+1] = buff[i+2];// 填入更新后表地址     ??  检测地址存放顺序  
                 }
                 TX_buff[5] = DI; // TX_buff [5]   0x56   DI
                 MeterInfoParaGet(0x01700002, (uint8*)&Status);	  //取电表状态字
                 TX_buff[10] = Status;      //增加电表状态字 只要低8 位， 高8位丢弃

	        }//end if  设表地址操作 

		}//end  if  不是通配符
	}//end if

}

/**************************************************
* 函数功能:  校验事务号 
* 输入参数:             
* 输出参数:          
* 函数说明:  用于广播  本机存储的事务号 和 RX接收的事务号对比 不相同则响应
* 使用资源:
**************************************************/
uint8 CheckSWH(void) 
{
    uint8 i;
    for (i=0; i<8; i++)
    {
        if (ReadMeterSerialNumber[i] == RF_DATA_COLLECT)      //  RX_buff[10]
           return 0;
    }
    return 1;
}

/**************************************************
* 函数功能:  更新事务号 
* 输入参数:             
* 输出参数:          
* 函数说明:  用于广播  Huu接收到 15号表的电能数据后，下发一帧 15表地址的事务号更新命令
*            如果10号表 huu没有接收到电能数据，则不下发事务号更新命令。
* 使用资源:
**************************************************/
void UpdataSWH(void)
{
    uint8 i;
    uint8 Temp[9];
    if(RF_MASK == 0)
        return;
    for(i=0; i<8; i++)
    {
        if((ReadMeterSerialNumber[i]&0x3f) == (RF_DATA_COLLECT&0x3f))
        {
            ReadMeterSerialNumber[i] = RF_DATA_COLLECT;
            return;
        }
    }
    memcpy(Temp+1, ReadMeterSerialNumber, 8);
    Temp[0] = RF_DATA_COLLECT;
    memcpy(ReadMeterSerialNumber, Temp, 8); 
}

/**************************************************
* 函数功能:  RF 模块检测    低压关闭
* 输入参数:             
* 输出参数:          
* 函数说明:  判定条件 三相都< 0.8Un && 任意两相 < 0.65Un  关RF  
*            相同条件门限 +5V  回差 恢复RF 
*            Note ：Vol_low 初始化必须为 1   上电RF初始化为待机 
* 使用资源:
**************************************************/
/*void RFModuleCheckProcess(void)
{
    
    uint16  Vol_A;
    uint16  Vol_B;
    uint16  Vol_C;
    int32   Un;  
 
    Un = Get_EMU_Measure_Data(SYS_PARAMETER_UN); //读取系统电压规格 兼容
    EnergyParaGet(0x03800902, (uint8*)&Vol_A); //L1 电压
    EnergyParaGet(0x03801202, (uint8*)&Vol_B); //L2 电压
    EnergyParaGet(0x03801B02, (uint8*)&Vol_C); //L3 电压
    Vol_A /= 10;
    Vol_B /= 10;
	Vol_C /= 10;
	
    if ( (Vol_A<(Un*8/10)) && (Vol_B<(Un*8/10)) &&(Vol_C<(Un*8/10)) ) 
    {
        if ( ((Vol_A<(Un*65/100)) &&(Vol_B<(Un*65/100))) 
             || ((Vol_A<(Un*65/100))&&(Vol_C<(Un*65/100)))
              || ((Vol_B<(Un*65/100))&&(Vol_C<(Un*65/100))) )    //判定条件 三相都 < 0.8Un && 任意两相 < 0.65Un  关RF   
        {
            if (!Vol_low)
            {
                 Vol_low = 1;       //低压条件
                 CC1101_Sleep();  
            } 
        }
    } // end if 低压  关 RF 接收
    
        
    if ( (Vol_A<(Un*8/10+50)) && (Vol_B<(Un*8/10+50)) &&(Vol_C<(Un*8/10+50)) )
    {
        if ( ((Vol_A<(Un*65/100+50)) && (Vol_B<(Un*65/100+50))) 
             || ((Vol_A<(Un*65/100+50))&&(Vol_C<(Un*65/100+50)))
              || ((Vol_B<(Un*65/100+50))&&(Vol_C<(Un*65/100+50))) )    //判定条件 三相都 < 0.8Un+5V && 任意两相 < 0.65Un+5V  关RF   
        {
           __NOP(); //两个门限中间的状态不对 RF 操作  
        }  //end if 还是继续关闭 RF 接收
        else 
        {
             if (Vol_low)
             {
                 Vol_low = 0; 
                 enable_RF_RX();
             }    
        }//end else 开启 RF 接收   
    }
    else
    {
         if (Vol_low)
         {
             Vol_low = 0; 
             enable_RF_RX();
         }    

    }// end if  恢复 RF 接收  回差开启  
   
    TimeIntervalInitRF();     //此句造成  进入关 RF后，120S 重新开启 RF后 无法再次关闭 RF
*/    
   /*  
      DDS26D - VN XX
      电压>300 or 电压<180, 关闭组网RF  。 
      组网模块抄表时功耗大，
      过压关闭防止烧坏元件；低压带不动，会造成掉电复位
    
      DTS27 - VN 2X
      低压 RF 待机      _|— (Vol_low=0 跳变为 1)
      电压恢复 RF RX模式  —|_ (Vol_low=1 跳变为 0) 
      Note ：Vol_low 初始化必须为 1,上电RF初始化为睡眠,以解决上电时 RF 动作问题    
    */   
//}

/********************************************************
* 函数功能:  RF 接收数据帧 解码处理
* 输入参数:            
* 输出参数:          
* 函数说明:  CS校验和; +/- 0x33; 判定DI码 分类处理 
*
* 使用资源:   
**********************************************************/
void RFReceiveMessage(void)
{
    uint8 Check_CS = 0;
	uint8 address[6];     //临时装载表地址 buff
    uint8 i;
    uint8 RandomAddress;

    Check_CS = CalcAddSum(RX_buff, 12); //计算 CS 校验和
    if (Check_CS != RX_buff[12])
    {
        return;
    }

    DataBatProcess(RX_buff+1, 4, 0x33, SubFlag); //地址   - 33
    DataBatProcess(RX_buff+6, 6, 0x33, SubFlag); //数据域 + 33

    /**********************  广播命令处理  ********************/
    if ((RX_buff[1]==0x99) && (RX_buff[2]==0x99) 
         && (RX_buff[3]==0x99) && (RX_buff[4]==0x99))
    {
        if (RF_DI == 0xaf) //显示表地址
        {
            DisplayAlarm(DIS_METER_NO, TRUE);
            return;
        }
        else if (RF_DI == 0xa4)  //广播抄表
        {
            if (!CheckSWH())    
                return;
            if (RF_R_SEND_NUMBER < SET_R_ADDR)  //  RX_buff[11] = RF_R_SEND_NUMBER
            {
                //BT_RTGet16bitCount(BT_CH_6);
               RandomAddress = BT_RTGet16bitCount(BT_CH_6) & 0xff;  // 调用库随机函数   种子 = BT_CH_6；
                
            }
            RandomAddress &= RF_MASK;    // RX_buff[8]   
            if (RF_R_SEND_NUMBER >= RandomAddress)   // RX_buff[11]  >=
            {
                RFWaitSendFlag = 1;
                RFSendEnergy(0x54);  
                TX_buff[11] = RX_buff[10]&0x3f; //此次更新事务号的时候,接收到掌机号,需要重新回传给掌机模块,掌机模块可以判断接收。
            }
            if (RFWaitSendFlag)
            {
                DataBatProcess(TX_buff+1, 4, 0x33, AddFlag);
                DataBatProcess(TX_buff+6, 6, 0x33, AddFlag);
                TX_buff[0] = 0x66;
                TX_buff[12] = CalcAddSum(TX_buff, 12);
            }
            
            return;
        }
    }//end if 广播处理
     
    /******************** 验证RX_buff中的表地址 与 本机表地址是否一致 ********************/
    
    MeterInfoParaGet(0x01010002, (uint8*) address); //取表地址6 byte
    for (i=1; i<5; i++) //本机的表地址 以 
    {
		if (RX_buff[i] != address[i+1])
            return;
    }//接收到的表地址 与 本机的表地址相对应 是否一致

    /************************** 除广播之外的 DI码 处理 **********************************/
    memcpy(TX_buff, RX_buff, 5); 
	RFWaitSendFlag = 1;
     
	switch (RF_DI) // RF_DI
	{
		case 0xa3:
			RFSendEnergy(0x53);
			break;
		case 0xa5:
			UpdataSWH();   //
			RFSendEnergy(0x55);  //
			TX_buff[11] = RX_buff[10]&0x3f; //此次更新事务号的时候,接收到掌机号,需要重新回传给掌机模块,掌机模块可以判断接收。
			break;
		case 0xa6:  //设表地址
			Set_meter_address(0x56);
			break;
		case 0xa9:  //电流电压L1
			RFSendACData(0x59);
			break;
		case 0x81:  //电流电压L2
			RFSendACData(0x31);
			break;	
		case 0x82:  //电流电压L3
			RFSendACData(0x32);
			break;		
		case 0x83:  //功率
			RFSendPower(0x33);
			break;
		case 0x8a:  //窃电次数
			RFSendStealEnergyCount(0x7A);
			break;
		case 0x8b:  //功因
			RFSendPowerFactor(0x7B);
			break;
		case 0x87:  //无功电能
			RFSendEnergy(0x77);
			break;
		case 0x86:  //视在电能
			RFSendEnergy(0x76);
			break;            
		default:
			RFWaitSendFlag = 0;
			break;		
	}//end switch (RF_DI)
	
    
    if (RFWaitSendFlag)  
    {
        DataBatProcess(TX_buff+1, 4, 0x33, AddFlag);
        DataBatProcess(TX_buff+6, 6, 0x33, AddFlag);
        TX_buff[0] = 0x66;
        TX_buff[12] = CalcAddSum(TX_buff, 12);
    }

}//end RF 接收处理
