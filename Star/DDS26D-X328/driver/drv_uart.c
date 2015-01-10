/*!
 ******************************************************************************
 **
 ** \file dr_uart.c
 **
 ** \brief UART drivers
 **
 ** \author 
 **
 ** \version V0.10
 **
 ** \date 2013-10-10
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */
 
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/

#include "Include.h"

void COMRxTimeOut(COM_TYPEDEF com);
void COMRxResetTmr(COM_TYPEDEF com);
void COMTxStart(COM_TYPEDEF com);
void COMTxEnd(COM_TYPEDEF com);
void COMTxTimeOut(COM_TYPEDEF com);
void COMRxMonitoringTask(void);
void COMTxMonitoringTask(void);
void ChannelInit(void);
void BaudRate_Config(MFS_UARTModeConfigT *pUARTModeConfig,COM_BaudeType baude);
void DataFormat_Config(MFS_UARTModeConfigT *pUARTModeConfig,COM_DataFormat type);
void UARTConfigMode(uint8_t Ch, MFS_UARTModeConfigT *pModeConfig);
static void Init_UartPort(COM_TYPEDEF com);
void LDRV_UsartOpen(COM_TYPEDEF com,COM_DataFormat type,COM_BaudeType baude,uint8 * rxfile,uint16 size, COM_DoubleMode doubleM);
void LDRV_UsartClose(COM_TYPEDEF com);
void LDRV_CommRxEnable(COM_TYPEDEF com);
void LDRV_CommTxEnable(COM_TYPEDEF com);
uint16 LDRV_UsartRecv(COM_TYPEDEF com);
void LDRV_UsartSend(COM_TYPEDEF com, uint8 * file, uint16 len);
void Uart_Send(COM_TYPEDEF com);
void Uart_Receive(COM_TYPEDEF com);

static void IRIrqTXCallback(void);
static void IRIrqRXCallback(void);
static void RS485IrqTXCallback(void);
static void RS485IrqRXCallback(void);
static void MASHIrqTXCallback(void);
static void MASHIrqRXCallback(void);
static void EMUL1IrqTXCallback(void);
static void EMUL1IrqRXCallback(void);
static void EMUL2IrqTXCallback(void);
static void EMUL2IrqRXCallback(void);

/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/

S_COM Comm[MAXCHNELNUMBER];             

uint8 Rxbuffer[250];
uint8 TXd[6];

//接收帧超时
void COMRxTimeOut(COM_TYPEDEF com)
{
	if(Comm[com].ucStR==ING)
	{
	    if(Comm[com].ucRecvTmr > 0)
	    {
	        Comm[com].ucRecvTmr--;
	    }
		else
		{
			Comm[com].ucStR=FINISH;
			//MFS_UARTDisableRXInt(Comm[com].channel);
		}
	}
}

//接收空闲定时重新打开接收
void COMRxResetTmr(COM_TYPEDEF com)
{
    if(Comm[com].ucStR ==IDLE)
    {
        if(Comm[com].ucResetTmr > 0)
        {
            Comm[com].ucResetTmr--;
        }
		else
		{
			LDRV_CommRxEnable(com);
			Comm[com].ucResetTmr = RX_IDLE_RST;
		}
    }
}

//接收->发送延时
void COMTxStart(COM_TYPEDEF com)
{
	if(Comm[com].doubleMode == HalfDouble)
	{
		if(Comm[com].ucStt==READY)
		{
		    if(Comm[com].ucOverTmr > 0)
		    {
		        Comm[com].ucOverTmr--;
		    }
			else 
			{
				Comm[com].ucStt=ING;
				Comm[com].ucTxTmr=TX_TIMEOUT_CNT;
				LDRV_CommTxEnable(com);
			}
		}
	}
}

//等待最后一个字节发送完成
void COMTxEnd(COM_TYPEDEF com)
{
    if(Comm[com].ucStt ==FINISH)
    {
        if((MFS_UARTGetTXBusIdleStatus(Comm[com].channel)) ==1)
        {
			//打开发送空闲
            Comm[com].ucStt =IDLE;
			//半双工发送完成后打开接收使能，关键是RS485_CS
			if(Comm[com].doubleMode==HalfDouble)
			{
				LDRV_CommRxEnable(com);
			}
        }
    }
}

//发送超时保护
void COMTxTimeOut(COM_TYPEDEF com)
{
    if((Comm[com].ucStt ==ING)||(Comm[com].ucStt ==FINISH))
    {
		if(Comm[com].ucTxTmr> 0)
		{
			Comm[com].ucTxTmr--;
		}		
		else
		{
			Comm[com].ucStt =IDLE;
			MFS_UARTDisableTXInt(Comm[com].channel);
			//半双工发送完成后打开接收使能，关键是RS485_CS
			if(Comm[com].doubleMode==HalfDouble)
			{
				LDRV_CommRxEnable(com);
			}
		}
    }
}

/*******************************************************************************
 * @function_name: COMMonitoring
 * @function_file: Dr_Uart.c
 * @描述：串口接受监控
 * @入口参数： 无
 * @出口参数： 无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
void COMRxMonitoringTask(void)
{
	COM_TYPEDEF com;

    for(com=(COM_TYPEDEF)1;com<(COM_TYPEDEF)MAXCHNELNUMBER;com++)
    {
       if(com !=UART_MASH) //当前是红外 RS485 需要处理 
       { 
          COMRxTimeOut(com);
          COMRxResetTmr(com);
       }
       else
       {                    //当前是mash判断 同时模块是RF_MESH 才进行超时和定时复位
          if(RFModel.Module == RF_MESH)  
          {
              COMRxTimeOut(com);
              COMRxResetTmr(com);                  
          } 
       
       }  
    }
}

/*******************************************************************************
 * @function_name: COMMonitoring
 * @function_file: Dr_Uart.c
 * @描述：串口接受监控
 * @入口参数： 无
 * @出口参数： 无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
void COMTxMonitoringTask(void)
{
	COM_TYPEDEF com;

    for(com=(COM_TYPEDEF)1;com<(COM_TYPEDEF)MAXCHNELNUMBER;com++)
    {
		//等待最后一个字节发送完成
		COMTxEnd(com);
		//发送超时保护
		COMTxTimeOut(com);
		//接收->发送延时
		COMTxStart(com);
    }
}

/*******************************************************************************
 * @function_name: ChannelInit
 * @function_file: Dr_Uart.c
 * @描述：将库函数所用的通道和通讯的通道结合一起
 * @入口参数： 无
 * @出口参数： 无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
void ChannelInit(void)
{
    Comm[UART_IR].channel = MFS_Ch4;
    Comm[UART_485].channel = MFS_Ch7;
    Comm[UART_MASH].channel = MFS_Ch5;
    Comm[UART_EMU_L1].channel = MFS_Ch3;
    Comm[UART_EMU_L2].channel = MFS_Ch6;
    return;
}

/*******************************************************************************
 * @function_name: BaudRate_Config
 * @function_file: Dr_Uart.c
 * @描述：配置UART的波特率
 * @入口参数： com-指串口号, 枚举的端口之一
              baude - 指选择波特率，枚举的波特率之一
 * @出口参数： 无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
void BaudRate_Config(MFS_UARTModeConfigT *pUARTModeConfig,COM_BaudeType baude)
{
    switch(baude)
    {
        case BAUDE_300BPS:
           pUARTModeConfig->Baudrate = 300;
           break;
        case BAUDE_600BPS:
			pUARTModeConfig->Baudrate = 600;
           break;
        case BAUDE_1200BPS:
			pUARTModeConfig->Baudrate = 1200;
           break;
        case BAUDE_2400BPS:
			pUARTModeConfig->Baudrate =2400;
           break;
        case BAUDE_4800BPS:
			pUARTModeConfig->Baudrate =4800;
           break;
        case BAUDE_9600BPS:
			pUARTModeConfig->Baudrate =9600;
           break;
        case BAUDE_19200BPS:
			pUARTModeConfig->Baudrate = 19200;
           break;
        case BAUDE_38400BPS:
			pUARTModeConfig->Baudrate = 38400;
           break;
        case BAUDE_57600BPS:
			pUARTModeConfig->Baudrate = 57600;
           break;
        case BAUDE_115200BPS:
			pUARTModeConfig->Baudrate = 115200;
           break;         
     }
}

/*******************************************************************************
 * @function_name: DataFormat_Config
 * @function_file: Dr_Uart.c
 * @描述：配置UART的波特率
 * @入口参数： com-指串口号, 枚举的端口之一
              baude - 指选择波特率，枚举的波特率之一
 * @出口参数： 无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
void DataFormat_Config(MFS_UARTModeConfigT *pUARTModeConfig,COM_DataFormat type)
{
     switch(type)
     {
          case USART_7E1:
            pUARTModeConfig->Databits = UART_DATABITS_7;
            pUARTModeConfig->Parity = UART_PARITY_EVEN;
            pUARTModeConfig->Stopbits = UART_STOPBITS_1;
            break;
          case USART_8E1:
            pUARTModeConfig->Databits = UART_DATABITS_8;
            pUARTModeConfig->Parity = UART_PARITY_EVEN;
            pUARTModeConfig->Stopbits = UART_STOPBITS_1;
            break;
          case USART_7O1:
            pUARTModeConfig->Databits = UART_DATABITS_7;
            pUARTModeConfig->Parity = UART_PARITY_ODD;
            pUARTModeConfig->Stopbits = UART_STOPBITS_1;
            break;
          case USART_8O1:
            pUARTModeConfig->Databits = UART_DATABITS_8;
            pUARTModeConfig->Parity = UART_PARITY_ODD;
            pUARTModeConfig->Stopbits = UART_STOPBITS_1;
            break;
          case USART_7N1:
            pUARTModeConfig->Databits = UART_DATABITS_7;
            pUARTModeConfig->Parity = UART_PARITY_NONE;
            pUARTModeConfig->Stopbits = UART_STOPBITS_1;
            break;
          case USART_8N1:
            pUARTModeConfig->Databits = UART_DATABITS_8;
            pUARTModeConfig->Parity = UART_PARITY_NONE;
            pUARTModeConfig->Stopbits = UART_STOPBITS_1;
            break;
     }
}


/*******************************************************************************
 * @function_name: UARTConfigMode
 * @function_file: dr_Uart.c
 * @描述：配置UART的工作模式
 * @参数： 
 * @param:ser  串口的参数
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
void UARTConfigMode(uint8_t Ch, MFS_UARTModeConfigT *pModeConfig)
{
    /*配置指定通道为工作模式O:UART模式*/
    MFS_UARTSetUARTMode(Ch);
    /*配置使能输出串行数据*/
    MFS_UARTEnableSOE(Ch); 
    if(pModeConfig->Bitorder == UART_BITORDER_MSB)
    {
        /*配置串行数据从最高位开始传输*/
        MFS_UARTConfigMSB(Ch);
    }
    if(pModeConfig->Bitorder == UART_BITORDER_LSB)
    {
        /*配置串行数据从最低位开始传输*/
        MFS_UARTConfigLSB(Ch);
    }
    /*配置停止位的长度*/
    MFS_UARTConfigST(Ch, pModeConfig);
    if(pModeConfig->Parity == UART_PARITY_NONE)
    {
        /*关闭奇偶校验功能*/
        MFS_UARTDisableParity(Ch);
    }
    else 
    {
        if(pModeConfig->Parity == UART_PARITY_ODD)
        {
            /*配置Uart数据校验模式为奇校验模式*/
            MFS_UARTConfigOddParity(Ch);
            /*开启奇偶校验功能*/
            MFS_UARTEnableParity(Ch);
        }
        if(pModeConfig->Parity == UART_PARITY_EVEN)
        {
            /*配置Uart数据校验模式为偶校验模式*/
            MFS_UARTConfigEvenParity(Ch);
            /*开启奇偶校验功能*/
            MFS_UARTEnableParity(Ch); 
        }
    }
    /*配置数据位的长度*/
    MFS_UARTConfigDT(Ch, pModeConfig);
    if(pModeConfig->NRZ == UART_NRZ)
    {
        /*配置Uart串行数据信号模式为NRZ模式*/
        MFS_UARTConfigNoInv(Ch);
    }
    if(pModeConfig->NRZ == UART_INV_NRZ)
    {
        /*配置Uart串行数据信号模式为反转NRZ模式*/
        MFS_UARTConfigInv(Ch);
    }
    /*配置Uart串行数据的波特率*/
    MFS_UARTConfigBaudrate(Ch, pModeConfig->Baudrate);
    return;
}

/*******************************************************************************
 * @function_name: Init_Uart_Port
 * @function_file: Dr_Uart.c
 * @描述：配置PLC,IR,485,NFC：外设端口
 * @入口参数： com-指串口号, 枚举的端口之一
 * @出口参数： 无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
static void Init_UartPort(COM_TYPEDEF com)
{
    switch(com)
    {
		case UART_IR:
          /*配置P0.A允许外设功能,IR-RXD*/
          IO_EnableFunc(IO_PORT0, IO_PINxA);
          /*配置P0.B允许外设功能,IR-TXD*/
          IO_EnableFunc(IO_PORT0, IO_PINxB);
          /*配置P0.B外设功能为SOT4_0*/
          IO_ConfigFuncMFSSOTxPin(MFS_Ch4, IO_MFS_SOTx_SOTx_0);
          /*配置P0.A外设功能为SIN4_0*/
          IO_ConfigFuncMFSSINxPin(MFS_Ch4, IO_MFS_SINx_SINx_0);
           break;
        case UART_485:
           /*配置P8.2为GPIO*/
           IO_DisableFunc(IO_PORT8, IO_PINx2); 
           /*配置P8.2 GPIO为输出*/
           IO_ConfigGPIOPin(IO_PORT8,IO_PINx2,IO_DIR_OUTPUT,IO_PULLUP_OFF);
           /*配置P8.1允许外设功能,485-TXD*/
           IO_EnableFunc(IO_PORT8, IO_PINx1);
           /*配置P8.0允许外设功能,485-RXD*/
           IO_EnableFunc(IO_PORT8, IO_PINx0);
           /*配置P8.1外设功能为SOT7_2*/
           IO_ConfigFuncMFSSOTxPin(MFS_Ch7, IO_MFS_SOTx_SOTx_2);
           /*配置P8.0外设功能为SIN7_2*/
           IO_ConfigFuncMFSSINxPin(MFS_Ch7, IO_MFS_SINx_SINx_2);
           break;
         case UART_MASH:
            /*配置P3.6允许外设功能,RF-SO*/
            IO_EnableFunc(IO_PORT3, IO_PINx6);
			IO_ConfigGPIOPin(IO_PORT3,IO_PINx6,IO_DIR_INPUT,IO_PULLUP_ON);
            /*配置P3.7允许外设功能,RF-SI*/
            //IO_EnableFunc(IO_PORT3, IO_PINx7);
            /*配置P3.7外设功能为SOT5_2*/
            //IO_ConfigFuncMFSSOTxPin(MFS_Ch5, IO_MFS_SOTx_SOTx_2);
            /*配置P3.6外设功能为SIN5_2*/
            IO_ConfigFuncMFSSINxPin(MFS_Ch5, IO_MFS_SINx_SINx_2);
            break;
	  case UART_EMU_L1:
		  /*配置P48允许外设功能,EMU-L1-RXD*/
		  IO_EnableFunc(IO_PORT4, IO_PINx8);
		  /*配置P49允许外设功能,EMU-L1-TXD*/
		  IO_EnableFunc(IO_PORT4, IO_PINx9);
		  /*配置P48外设功能为SIN3_2,RXD*/
		  IO_ConfigFuncMFSSINxPin(MFS_Ch3, IO_MFS_SINx_SINx_2);
		  /*配置P0B外设功能为SOT3_2,TXD*/
		  IO_ConfigFuncMFSSOTxPin(MFS_Ch3, IO_MFS_SOTx_SOTx_2);
		  break;
	  case UART_EMU_L2:
		  /*配置P53允许外设功能,EMU-L2-RXD*/
		   bFM3_LCDC_LCDCC3_VE1 = 0;
		  IO_EnableFunc(IO_PORT5, IO_PINx3);
		  /*配置P54允许外设功能,EMU-L2-TXD*/
		   bFM3_LCDC_LCDCC3_VE0 = 0;
		  IO_EnableFunc(IO_PORT5, IO_PINx4);
		  /*配置P53外设功能为SIN6_0,RXD*/
		  IO_ConfigFuncMFSSINxPin(MFS_Ch6, IO_MFS_SINx_SINx_0);
		  /*配置P54外设功能为SOT6_0,TXD*/
		  IO_ConfigFuncMFSSOTxPin(MFS_Ch6, IO_MFS_SOTx_SOTx_0);
		  break;
    }; 
}
/*******************************************************************************
 * @function_name: Init_Uart_Port
 * @function_file: Dr_Uart.c
 * @描述：配置PLC,IR,485,NFC：外设端口
 * @入口参数： com-指串口号, 枚举的端口之一
 * @出口参数： 无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
static void Init_UartPortPowerDown(COM_TYPEDEF com)
{
    switch(com)
    {
		case UART_IR:
		  //IR_RXD
		  IO_DisableFunc(IO_PORT0, IO_PINxA); 
		  IO_ConfigGPIOPin(IO_PORT0, IO_PINxA, IO_DIR_OUTPUT, IO_PULLUP_OFF);
		  IO_WriteGPIOPin(IO_PORT0,IO_PINxA,IO_BIT_CLR);
		  //IR_TXD
		  IO_DisableFunc(IO_PORT0, IO_PINxB); 
		  IO_ConfigGPIOPin(IO_PORT0, IO_PINxB, IO_DIR_OUTPUT, IO_PULLUP_OFF);
		  IO_WriteGPIOPin(IO_PORT0,IO_PINxB,IO_BIT_CLR);
          break;
        case UART_485:
          //RS485_TXD
		  IO_DisableFunc(IO_PORT8, IO_PINx1); 
		  IO_ConfigGPIOPin(IO_PORT8, IO_PINx1, IO_DIR_OUTPUT, IO_PULLUP_OFF);
          IO_WriteGPIOPin(IO_PORT8,IO_PINx1,IO_BIT_CLR);
		  //RS485_RXD
		  IO_DisableFunc(IO_PORT8, IO_PINx0); 
		  IO_ConfigGPIOPin(IO_PORT8, IO_PINx0, IO_DIR_OUTPUT, IO_PULLUP_OFF);
          IO_WriteGPIOPin(IO_PORT8,IO_PINx0,IO_BIT_CLR);
		  //RS485_EN
		  IO_DisableFunc(IO_PORT8, IO_PINx2); 
		  IO_ConfigGPIOPin(IO_PORT8, IO_PINx2, IO_DIR_OUTPUT, IO_PULLUP_OFF);
          IO_WriteGPIOPin(IO_PORT8,IO_PINx2,IO_BIT_CLR);
          break;
        case UART_MASH:		  
		  //SO
		  IO_DisableFunc(IO_PORT3, IO_PINx6);
		  IO_ConfigGPIOPin(IO_PORT3,IO_PINx6,IO_DIR_OUTPUT,IO_PULLUP_OFF); 
		  IO_WriteGPIOPin(IO_PORT3,IO_PINx6,IO_BIT_CLR);
		  
		  //SI
		  IO_DisableFunc(IO_PORT3, IO_PINx7);
		  IO_ConfigGPIOPin(IO_PORT3,IO_PINx7,IO_DIR_OUTPUT,IO_PULLUP_OFF); 
		  IO_WriteGPIOPin(IO_PORT3,IO_PINx7,IO_BIT_CLR);
          break;
	  	case UART_EMU_L1:
		  //RX1
		  IO_DisableFunc(IO_PORT4, IO_PINx8); 
		  IO_ConfigGPIOPin(IO_PORT4, IO_PINx8, IO_DIR_OUTPUT, IO_PULLUP_OFF);
		  IO_WriteGPIOPin(IO_PORT4,IO_PINx8,IO_BIT_CLR);
		  //TX1
		  IO_DisableFunc(IO_PORT4, IO_PINx9); 
		  IO_ConfigGPIOPin(IO_PORT4, IO_PINx9, IO_DIR_OUTPUT, IO_PULLUP_OFF);
		  IO_WriteGPIOPin(IO_PORT4,IO_PINx9,IO_BIT_CLR);
		  break;
	  	case UART_EMU_L2:
		  //RX2
		  bFM3_LCDC_LCDCC3_VE1 = 0;
		  IO_DisableFunc(IO_PORT5, IO_PINx3); 
		  IO_ConfigGPIOPin(IO_PORT5, IO_PINx3, IO_DIR_OUTPUT, IO_PULLUP_OFF);
		  IO_WriteGPIOPin(IO_PORT5,IO_PINx3,IO_BIT_CLR);
		  //TX2
		  bFM3_LCDC_LCDCC3_VE0 = 0;
		  IO_DisableFunc(IO_PORT5, IO_PINx4); 
		  IO_ConfigGPIOPin(IO_PORT5, IO_PINx4, IO_DIR_OUTPUT, IO_PULLUP_OFF);
		  IO_WriteGPIOPin(IO_PORT5,IO_PINx4,IO_BIT_CLR);
		  break;
    }; 
}


/*******************************************************************************
 * @function_name: LDRV_UsartOpen
 * @function_file: dr_Uart.c
 * @描述：配置UART的工作模式，打开UART接收
 * @入口参数： com-指串口号, 枚举的端口之一
              pModeConfig - 配置Uart的工作模式
              rxfile-文件接收初始地址
              size-文件大小
 * @出口参数:  无
* @Attention:  size一定要為sizeof(rxfile)
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
void LDRV_UsartOpen(COM_TYPEDEF com,COM_DataFormat type,COM_BaudeType baude,uint8 * rxfile,uint16 size, COM_DoubleMode doubleM)
{
	MFS_UARTModeConfigT tUARTModeConfig;

	ChannelInit();
	//电表通讯通道com和单片机通道channel映射
    Comm[com].ucStR=IDLE;
    Comm[com].ucStt=IDLE;
    Comm[com].ucRecvTmr=0;
    Comm[com].ucTxTmr=0;
    /*数据地址之间传递*/
    Comm[com].ucRxBuf = rxfile;
    /*接收数据长度大小*/
    Comm[com].ucRXLen = size;
    Comm[com].ucTXLen = 0;
    Comm[com].ucRxPos=0;	
    Comm[com].ucTxPos=0;	
    Comm[com].ucResetTmr = RX_IDLE_RST;   
	/*全双工/半双工*/
	Comm[com].doubleMode =doubleM;
	
    /*配置开启接受*/	
    /*配置关闭接受使能*/
    MFS_UARTDisableRX(Comm[com].channel);
    /*配置关闭发送总线空闲中断*/
    MFS_UARTDisableTXBusIdleInt(Comm[com].channel);
    /*配置关闭发送使能*/
    MFS_UARTDisableTX(Comm[com].channel);

    /*配置PLC端口*/
	Init_UartPort(com);

    /*选择串口模式*/
    /*选择PLC的波特率*/
    BaudRate_Config(&tUARTModeConfig,baude);
    /*选择PLC的数据类型*/
    DataFormat_Config(&tUARTModeConfig,type);
	/*配置串行数据从最低位开始传输*/
	tUARTModeConfig.Bitorder=UART_BITORDER_LSB;
	/*配置Uart串行数据信号模式为NRZ模式*/
	tUARTModeConfig.NRZ=UART_NRZ;	
    /*配置PLC的工作模式*/
    UARTConfigMode(Comm[com].channel,&tUARTModeConfig);

	//接收使能
    LDRV_CommRxEnable(com);
    return;
}

/*******************************************************************************
 * @function_name: LDRV_UsartClose
 * @function_file: Dr_Uart.c
 * @描述：关闭UART接收
 * @入口参数： com-指串口号, 枚举的端口之一
 * @出口参数:  无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
void LDRV_UsartClose(COM_TYPEDEF com)
{
	Comm[com].ucStR=IDLE;
	Comm[com].ucStt=IDLE;
    MFS_UARTDisableRX(Comm[com].channel);
    MFS_UARTDisableRXInt(Comm[com].channel);
    MFS_UARTDisableTX(Comm[com].channel);
    MFS_UARTDisableTXInt(Comm[com].channel);
    NVIC_DisableIRQ((IRQn_Type)(MFS0_8_RX_IRQn + ((Comm[com].channel%8)*2)));
    NVIC_DisableIRQ((IRQn_Type)(MFS0_8_TX_IRQn + ((Comm[com].channel%8)*2)));
	Init_UartPortPowerDown(com);
    return;
}

/*******************************************************************************
 * @function_name: CommRxEnable
 * @function_file: Dr_Uart.c
 * @描述：开启UART接收
 * @入口参数： com-指串口号, 枚举的端口之一
 * @出口参数:  无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
void LDRV_CommRxEnable(COM_TYPEDEF com)
{
    if(com == UART_485)
    {
        IO_WriteGPIOPin(IO_PORT8,IO_PINx2,IO_BIT_CLR);
    }
    /*配置开启接受使能*/
    MFS_UARTEnableRX(Comm[com].channel);
    if(com == UART_IR)
    {
        /*配置使能接受中断*/
        MFS_UARTEnableRXInt(Comm[com].channel, IRIrqRXCallback);
    }
    if(com == UART_485)
    {
        /*配置使能接受中断*/
        MFS_UARTEnableRXInt(Comm[com].channel, RS485IrqRXCallback);
    }
    if(com == UART_MASH)
    {
    	//在接受模式下TXD配置为输入
		IO_DisableFunc(IO_PORT3,IO_PINx7);
      	IO_ConfigGPIOPin(IO_PORT3,IO_PINx7,IO_DIR_INPUT,IO_PULLUP_OFF);
		
        /*配置使能接受中断*/
        MFS_UARTEnableRXInt(Comm[com].channel, MASHIrqRXCallback);
    }
    if(com == UART_EMU_L1)
    {
        /*配置使能接受中断*/
        MFS_UARTEnableRXInt(Comm[com].channel, EMUL1IrqRXCallback);
    }
    if(com == UART_EMU_L2)
    {
        /*配置使能接受中断*/
        MFS_UARTEnableRXInt(Comm[com].channel, EMUL2IrqRXCallback);
    }
    NVIC_EnableIRQ((IRQn_Type)(MFS0_8_RX_IRQn + ((Comm[com].channel%8)*2)));
	//打开接收状态
	Comm[com].ucStR=IDLE;
	Comm[com].ucRxPos=0;	
	Comm[com].ucResetTmr = RX_IDLE_RST;   
    return;
}

/*******************************************************************************
 * @function_name: LDRV_CommTxEnable
 * @function_file: Dr_Uart.c
 * @描述：开启UART发送
 * @入口参数： com-指串口号, 枚举的端口之一
 * @出口参数:  无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
void LDRV_CommTxEnable(COM_TYPEDEF com)
{
    if(com == UART_485)
    {
        IO_WriteGPIOPin(IO_PORT8,IO_PINx2,IO_BIT_SET);
    }
    /*配置开启发送使能*/
    MFS_UARTEnableTX(Comm[com].channel);
    if(com == UART_IR)
    {
        /*配置使能中断*/
        MFS_UARTEnableTXInt(Comm[com].channel, IRIrqTXCallback);   
    }
    if(com == UART_485)
    {
        /*配置使能中断*/
        MFS_UARTEnableTXInt(Comm[com].channel, RS485IrqTXCallback);
    }
    if(com == UART_MASH)
    {
    	//防止模块切换TXD高电平导致PTP模块SI电平过高，所有接收完后才配置发送 
    	
		/*配置P3.7允许外设功能,RF-SI*/
		IO_EnableFunc(IO_PORT3, IO_PINx7);
		/*配置P3.7外设功能为SOT5_2*/
		IO_ConfigFuncMFSSOTxPin(MFS_Ch5, IO_MFS_SOTx_SOTx_2);
		
        /*配置使能中断*/
        MFS_UARTEnableTXInt(Comm[com].channel, MASHIrqTXCallback);
    }
    if(com == UART_EMU_L1)
    {
        /*配置使能中断*/
        MFS_UARTEnableTXInt(Comm[com].channel, EMUL1IrqTXCallback);   
    }
    if(com == UART_EMU_L2)
    {
        /*配置使能中断*/
        MFS_UARTEnableTXInt(Comm[com].channel, EMUL2IrqTXCallback);   
    }
    /*配置开启发送总中断*/
    NVIC_EnableIRQ((IRQn_Type)(MFS0_8_TX_IRQn + ((Comm[com].channel%8)*2)));
    return;
}

/*******************************************************************************
 * @function_name: LDRV_UsartRecv
 * @function_file: Dr_Uart.c
 * @描述：查询收到的数据，在主循环中查询，查询收到的字节数;在物理层中接收数据
 * @入口参数： com-指串口号, 枚举的端口之一
 * @出口参数:  接收到的数据长度,字节数
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
uint16 LDRV_UsartRecv(COM_TYPEDEF com)
{
    uint16 nRx;
    if(Comm[com].ucStR ==FINISH)
    {
        nRx = Comm[com].ucRxPos;
        Comm[com].ucRxPos = 0;
        return nRx;
    }
    else
    {
        return 0;
    }
}

/*******************************************************************************
 * @function_name: LDRV_UsartSend
 * @function_file: Dr_Uart.c
 * @描述：用通讯端口，开始发送数据
 * @入口参数： com-指串口号, 枚举的端口之一
 * @出口参数:  无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
void LDRV_UsartSend(COM_TYPEDEF com, uint8 * file, uint16 len)
{
	if(len)
	{
	    Comm[com].ucTxBuf = file;
	    Comm[com].ucTXLen = len;
	    Comm[com].ucTxPos = 0;
	    Comm[com].ucStt =READY;
		//全双工直接发送//半双工等到ucOverTmr延时后才能发
		if(Comm[com].doubleMode == FullDouble)	
		{
			Comm[com].ucStt=ING;
			Comm[com].ucTxTmr=TX_TIMEOUT_CNT;
			LDRV_CommTxEnable(com);
		}
	}
}

/*******************************************************************************
 * @function_name: LDRV_UsartSend
 * @function_file: Dr_Uart.c
 * @描述：用通讯端口，开始发送数据
 * @入口参数： com-指串口号, 枚举的端口之一
 * @出口参数:  无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
COMM_STATE LDRV_UsartGetStateRx(COM_TYPEDEF com)
{    
	return Comm[com].ucStR;
}

/*******************************************************************************
 * @function_name: LDRV_UsartSend
 * @function_file: Dr_Uart.c
 * @描述：用通讯端口，开始发送数据
 * @入口参数： com-指串口号, 枚举的端口之一
 * @出口参数:  无
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
COMM_STATE LDRV_UsartGetStateTx(COM_TYPEDEF com)
{    
	return Comm[com].ucStt;
}

void LDRV_UsartRestStateTx(COM_TYPEDEF com)
{    
	Comm[com].ucStt=IDLE;
}


//==============================================================================
//=========================中断回调函数==========================================
//==============================================================================
/*******************************************************************************
 * @function_name: Uart_Send
 * @function_file: dr_Uart.c
 * @描述：Uart发送函数
 * @参数： 
 * @param:ser  串口的参数
 *---------------------------------------------------------
 * @修改人：ffq     (2013-10-09)
 ******************************************************************************/
void Uart_Send(COM_TYPEDEF com)
{
    if(Comm[com].ucTxPos < Comm[com].ucTXLen)
    {   
        MFS_UARTTXOneData(Comm[com].channel, Comm[com].ucTxBuf[Comm[com].ucTxPos]);
        Comm[com].ucTxPos++;
    }
    else
    {
        Comm[com].ucStt =FINISH;	//实际还有一个字节正在发送，在COMTxMonitoring中判断结束
        MFS_UARTDisableTXInt(Comm[com].channel);
    }
	Comm[com].ucTxTmr=TX_TIMEOUT_CNT;
}

/*******************************************************************************
 * @function_name: Uart_Send
 * @function_file: dr_Uart.c
 * @描述：Uart接受函数
 * @参数： 
 * @param:ser  串口的参数
 *---------------------------------------------------------
 * @修改人：ffq     (2013-10-09)
 ******************************************************************************/
void Uart_Receive(COM_TYPEDEF com)
{
	uint8 TempData;

	TempData= MFS_UARTRXOneData(Comm[com].channel);
    if(MFS_UARTGetOEStatus(Comm[com].channel) || MFS_UARTGetPEStatus(Comm[com].channel) || MFS_UARTGetFEStatus(Comm[com].channel))
    {
        MFS_UARTErrorClr(Comm[com].channel);
        return;
    }
	if((Comm[com].doubleMode==FullDouble) || (Comm[com].ucStt ==IDLE))//半双工只在发送空闲时接收
	{
	 	if((Comm[com].ucStR==ING) || (Comm[com].ucStR ==IDLE))//接收完成，命令处理过程中不接收
	 	{
		    if(Comm[com].ucRxPos < Comm[com].ucRXLen)
			{
				Comm[com].ucRxBuf[Comm[com].ucRxPos++] = TempData;
				if(Comm[com].ucRxPos < Comm[com].ucRXLen)
				{
		        	Comm[com].ucStR =ING;
				}
				else
				{
					Comm[com].ucStR=FINISH;
					//MFS_UARTDisableRXInt(Comm[com].channel);
				}
		        Comm[com].ucRecvTmr = RX_TIMEOUT_CNT;
		        Comm[com].ucOverTmr = RT_INTERVAL_MIN;
				Comm[com].ucResetTmr = RX_IDLE_RST; 
                                if(com <= 3)
                                {
                                    //lcd_comm_hold_ct= 0x0A;
                                }
                                
			}
	 	}
	}
}


/*******************************************************************************
 * @function_name: IRIrqTXCallback
 * @function_file: Dr_Uart_IR.c
 * @描述：IR发送中断回调函数
 * @入口参数： 无
 * @出口参数:
 *---------------------------------------------------------
 * @修改人：ffq     (2013-10-09)
 ******************************************************************************/
static void IRIrqTXCallback(void)
{  
    Uart_Send(UART_IR);
}

/*******************************************************************************
 * @function_name: IRIrqRXCallback
 * @function_file: Dr_Uart_IR.c
 * @描述：IR接受中断回调函数
 * @入口参数：  无 
 * @出口参数:
 *---------------------------------------------------------
 * @修改人：ffq     (2013-10-09)
 ****************************************************************************/
static void IRIrqRXCallback(void)
{    
    Uart_Receive(UART_IR);
}

/*******************************************************************************
 * @function_name: RS485IrqTXCallback
 * @function_file: Dr_Uart_RS485.c
 * @描述：RS485发送中断回调函数
 * @入口参数： 
 * @:ser  串口的参数
 *---------------------------------------------------------
 * @修改人：ffq     (2013-10-09)
 ******************************************************************************/
static void RS485IrqTXCallback(void)
{  
    Uart_Send(UART_485);
}

/*******************************************************************************
 * @function_name: RS485IrqRXCallback
 * @function_file: Dr_Uart_RS485.c
 * @描述：RS485接受中断回调函数
 * @参数： 
 * @param:ser  串口的参数
 *---------------------------------------------------------
 * @修改人：ffq     (2013-10-09)
 ****************************************************************************/
static void RS485IrqRXCallback(void)
{    
    Uart_Receive(UART_485);
}

/*******************************************************************************
 * @function_name: MASHIrqTXCallback
 * @function_file: Dr_Uart_MASH.c
 * @描述：RS485发送中断回调函数
 * @入口参数： 
 * @:ser  串口的参数
 *---------------------------------------------------------
 * @修改人：ffq     (2013-10-09)
 ******************************************************************************/
static void MASHIrqTXCallback(void)
{  
    Uart_Send(UART_MASH);
}

/*******************************************************************************
 * @function_name: MASHIrqRXCallback
 * @function_file: Dr_Uart_MASH.c
 * @描述：RS485接受中断回调函数
 * @参数： 
 * @param:ser  串口的参数
 *---------------------------------------------------------
 * @修改人：ffq     (2013-10-09)
 ****************************************************************************/
static void MASHIrqRXCallback(void)
{    
    Uart_Receive(UART_MASH);
}

/*******************************************************************************
 * @function_name: NFCIrqTXCallback
 * @function_file: Dr_Uart_NFC.c
 * @描述：计量L1路发送中断回调函数
 * @参数： 
 * @param:ser  串口的参数
 *---------------------------------------------------------
 * @修改人：ffq     (2013-10-09)
 ******************************************************************************/
static void EMUL1IrqTXCallback(void)
{  
   	Uart_Send(UART_EMU_L1);
}

/*******************************************************************************
 * @function_name: NFCIrqRXCallback
 * @function_file: Dr_Uart_NFC.c
 * @描述：计量L1路接受中断回调函数
 * @参数： 
 * @param:ser  串口的参数
 *---------------------------------------------------------
 * @修改人：ffq     (2013-10-09)
 ****************************************************************************/
static void EMUL1IrqRXCallback(void)
{    
   	Uart_Receive(UART_EMU_L1);
}

/*******************************************************************************
 * @function_name: NFCIrqTXCallback
 * @function_file: Dr_Uart_NFC.c
 * @描述：计量L2路发送中断回调函数
 * @参数： 
 * @param:ser  串口的参数
 *---------------------------------------------------------
 * @修改人：ffq     (2013-10-09)
 ******************************************************************************/
static void EMUL2IrqTXCallback(void)
{  
   	Uart_Send(UART_EMU_L2);
}

/*******************************************************************************
 * @function_name: NFCIrqRXCallback
 * @function_file: Dr_Uart_NFC.c
 * @描述：计量L1路接受中断回调函数
 * @参数： 
 * @param:ser  串口的参数
 *---------------------------------------------------------
 * @修改人：ffq     (2013-10-09)
 ****************************************************************************/
static void EMUL2IrqRXCallback(void)
{    
   	Uart_Receive(UART_EMU_L2);
}


/*********************************************************** 
函数功能：测试
入口参数：
出口参数：
备注说明：当准备好发送后定时发送
***********************************************************/
void LDRV_Test(void)
{
	uint16 i,temp;
	COM_TYPEDEF com;

	for(com=(COM_TYPEDEF)1; com<(COM_TYPEDEF)(MAXCHNELNUMBER-2);com++)
	{
		if(temp=LDRV_UsartRecv(com))
		{
			//处理
			for(i=0;i<temp;i++)
			{
				Rxbuffer[i]=Rxbuffer[i]+0x33;
			}
			//需要应答则应答
			if(temp)
			{
				LDRV_UsartSend(com, Rxbuffer, temp);
			}
			//发送完成，驱动会自动打开接收使能
		}
	}
}

