/*******************************************************************************
** 硬件平台: 富士通MB9BF218s学习板
** 固件库  ：V1.0
** 文件名  : UART.c
** 实现功能: UART轮询收发
** 作者    : 微控事业部
** 创建日期: 2013/8/28
** 版权    : 利尔达科技集团
** -----------------------------------------------------------------------------
** 修改者:
** 修改日期:
** 修改内容:
*******************************************************************************/
#include    "all_header_file.h"


//uint8_t RX_flag = 0;
//uint8_t C_flag = 0;

/******************************************************************************/
/**                          以下是内部函数定义                              **/
/******************************************************************************/
int fputc(int Ch, FILE *p)
{
	Put_char(UartUSBCh, Ch);
	return Ch;
}

//int fgetc(FILE *_Str)
//{
//    
//}

/*******************************************************************************
* 函数名称: 
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
//void UART_setup(void)
//{
//    UART_Port_init();                        /* UART IO init */             
//    UARTConfigMode(&tUARTModeConfigT);       /* UART setup */  
//}

/*******************************************************************************
** 函数名  : InitPort
** 功能    : 端口初始化
** 输入参数: 无
** 输出参数: 无
** 作者    : 微控事业部
** 创建日期: 2013/8/28
--------------------------------------------------------------------------------
** 修改者  :
** 修改日期:
** 修改内容:
*******************************************************************************/
void UART_Port_init()
{
    //若有模拟输入复用功能引脚，则要禁用模拟输入功能
	/* Enable SOT and SIN */
    bFM3_GPIO_PFR3_P6 = 1;           //外设功能
    bFM3_GPIO_PFR3_P7 = 1;           
    bFM3_GPIO_EPFR08_SIN5S1 = 1;     //UART RX
    bFM3_GPIO_EPFR08_SIN5S0 = 1;
    bFM3_GPIO_EPFR08_SOT5B1 = 1;     
    bFM3_GPIO_EPFR08_SOT5B0 = 1;
    
    
    /*配置P48允许外设功能,EMU-L1-RXD*/
    IO_EnableFunc(IO_PORT2, IO_PINx1);
    /*配置P49允许外设功能,EMU-L1-TXD*/
    IO_EnableFunc(IO_PORT2, IO_PINx2);
    /*配置P48外设功能为SIN3_2,RXD*/
    IO_ConfigFuncMFSSINxPin(MFS_Ch0, IO_MFS_SINx_SINx_0);
    /*配置P0B外设功能为SOT3_2,TXD*/
    IO_ConfigFuncMFSSOTxPin(MFS_Ch0, IO_MFS_SOTx_SOTx_0);
}

/**************************************************
* 函数功能:  UART_config_mode  
* 输入参数:              
* 输出参数:          
* 函数说明:   UART 参数配置
*            4800,
*            UART_DATABITS_8,
*            UART_STOPBITS_1,
*            UART_PARITY_NONE,
*            UART_BITORDER_LSB,
*            UART_NRZ,
* 使用资源:
**************************************************/
//void UARTConfigMode(MFS_UARTModeConfigT *pModeConfig)
//{
//    
//    /* Config the MOD as UART */
//    MFS_UARTSetUARTMode(InUseCh);   // UART mode = 000 (SMR 5-7)
//    /* Enable Output */
//    MFS_UARTEnableSOE(InUseCh);     //SOE = 1 (SMR 0),enable UART IO output
//    /* LSB */
//    MFS_UARTConfigLSB(InUseCh);     //LSB = 0 (SMR 2)
//    /* Stop bit */
//    MFS_UARTConfigST(InUseCh, pModeConfig);  //SBL ESBL = 0 0，1 bit stop 
//    /* Parity NONE */   
//    MFS_UARTDisableParity(InUseCh);     // PEN = 0 (ESCR 4), none 
//    /* Data bits */
//    MFS_UARTConfigDT(InUseCh, pModeConfig);  // L = 000 (ESCR 012) 8 bit data
//    /* INV */
//    MFS_UARTConfigNoInv(InUseCh);       // INV = 0 (ESCR 5), 正向 
//    /* Baud rate */
//    MFS_UARTConfigBaudrate(InUseCh, pModeConfig->Baudrate);
//    
//    return;
//}
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
** 函数名  : UARTConfigMode
** 功能    : 配置UART模式
** 输入参数: 无
** 输出参数: 无
** 作者    : 微控事业部
** 创建日期: 2013/8/28
--------------------------------------------------------------------------------
** 修改者  :
** 修改日期:
** 修改内容:
    4800,
    UART_DATABITS_8,
    UART_STOPBITS_1,
    UART_PARITY_NONE,
    UART_BITORDER_LSB,
    UART_NRZ,
*******************************************************************************/
//void UARTConfigMode(MFS_UARTModeConfigT *pModeConfig)
//{
//    /* Config the MOD as UART */
//    MFS_UARTSetUARTMode(InUseCh);
//    /* Enable Output */
//    MFS_UARTEnableSOE(InUseCh);
//    /* LSB or MSB */
//    
//    if(pModeConfig->Bitorder == UART_BITORDER_MSB)
//    {
//        MFS_UARTConfigMSB(InUseCh);
//    }
//    if(pModeConfig->Bitorder == UART_BITORDER_LSB)
//    {
//       MFS_UARTConfigLSB(InUseCh);
//    }
//    /* Stop bit */
//    MFS_UARTConfigST(InUseCh, pModeConfig);
//    /* Parity */
//    if(pModeConfig->Parity == UART_PARITY_NONE)
//    {
//        MFS_UARTDisableParity(InUseCh);
//    }
//    else
//    {
//        if(pModeConfig->Parity == UART_PARITY_ODD)
//        {
//            MFS_UARTConfigOddParity(InUseCh);
//            MFS_UARTEnableParity(InUseCh);
//        }
//        if(pModeConfig->Parity == UART_PARITY_EVEN)
//        {
//            MFS_UARTConfigEvenParity(InUseCh);
//            MFS_UARTEnableParity(InUseCh);
//        }
//    }
//    /* Data bits */
//    MFS_UARTConfigDT(InUseCh, pModeConfig);
//    /* INV */
//    if(pModeConfig->NRZ == UART_NRZ)
//    {
//        MFS_UARTConfigNoInv(InUseCh);
//    }
//    if(pModeConfig->NRZ == UART_INV_NRZ)
//    {
//        MFS_UARTConfigInv(InUseCh);
//    }
//    MFS_UARTConfigBaudrate(InUseCh, pModeConfig->Baudrate);
//    return;
//}

/*******************************************************************************
** 函数名  : UARTPollTX
** 功能    : 轮询发送size个字符
** 输入参数: 无
** 输出参数: 无
** 作者    : 微控事业部
** 创建日期: 2013/8/28
--------------------------------------------------------------------------------
** 修改者  :
** 修改日期:
** 修改内容:
*******************************************************************************/
//void UARTPollTX(uint8_t Ch, uint8_t *data, uint16_t size)
//{
//    uint16_t count;
//
//    /* Check and send */
//    if((data != NULL) && (size != 0))
//    {
//        /* Init the count */
//        count = 0;
//        /* Enable the  TX*/
//        //MFS_UARTEnableTX(Ch);
//        while(count < size)
//        {
//            if(MFS_UARTGetTXRegEmptyStatus(Ch) && MFS_UARTGetTXBusIdleStatus(Ch))
//            {
//                /* TX one byte */
//                MFS_UARTTXOneData(Ch, *data++);
//                count++;
//            }
//        }
//        /* TX Finished? */
//        while(!MFS_UARTGetTXBusIdleStatus(Ch));
//        /* Disable the  TX after TX all bytes*/
//        //MFS_UARTDisableTX(Ch);
//    }
//    return;
//}

/*******************************************************************************
** 函数名  : UARTPollTX_string
** 功能    : 轮询发送一个字符串,不用考虑字符串大小
** 输入参数: 无
** 输出参数: 无
** 作者    : 微控事业部
** 创建日期: 2013/8/28
--------------------------------------------------------------------------------
** 修改者  :
** 修改日期:
** 修改内容:
*******************************************************************************/
void UARTPollTX_string(uint8_t *data)
{
    /* Check and send */
    if(data != NULL)
    {
        /* Enable the  TX*/
        //MFS_UARTEnableTX(UartUSBCh);
        while(*data != '\0')
        {
            if(MFS_UARTGetTXRegEmptyStatus(UartUSBCh) && MFS_UARTGetTXBusIdleStatus(UartUSBCh))
            {
                /* TX one byte */
                MFS_UARTTXOneData(UartUSBCh, *data++);
            }
        }
        /* TX Finished? */
        while(!MFS_UARTGetTXBusIdleStatus(UartUSBCh));  //等待发送完成
        /* Disable the  TX after TX all bytes*/
        //MFS_UARTDisableTX(UartUSBCh);
    }
    
    return;
}

/*******************************************************************************
** 函数名  : UARTPollRX
** 功能    : 轮询接收size个字符
** 输入参数: 无
** 输出参数: 无
** 作者    : 微控事业部
** 创建日期: 2013/8/28
--------------------------------------------------------------------------------
** 修改者  :
** 修改日期:
** 修改内容:
*******************************************************************************/
//void UARTPollRX(uint8_t *data, uint16_t size)
//{
//    uint16_t count = 0;
//    /* Check and receive */
//    if(data != NULL)
//    {
//        /* Enable the  RX*/
//        MFS_UARTEnableRX(InUseCh);
//        while(count < size)
//        {
//            if(MFS_UARTGetOEStatus(InUseCh) || MFS_UARTGetPEStatus(InUseCh) || MFS_UARTGetFEStatus(InUseCh))
//            {
//                break;//Error happen
//            }
//            if(MFS_UARTGetRXRegFullStatus(InUseCh))
//            {
//                *data++ = MFS_UARTRXOneData(InUseCh);
//                count++;
//            }
//        }
//        /* Disable the  RX*/
//        MFS_UARTDisableRX(InUseCh);
//    }
//    return;
//}

/*******************************************************************************
** 函数名  : Get_char
** 功能    : 查询方式接收一个字符
** 输入参数: 无
** 输出参数: 无
** 作者    : 微控事业部
** 创建日期: 2013/8/28
--------------------------------------------------------------------------------
** 修改者  :
** 修改日期:
** 修改内容:
*******************************************************************************/
uint32_t Get_One_char(uint8_t Ch,uint8_t *dat)
{
    if(MFS_UARTGetRXRegFullStatus(Ch))
    {
        *dat = MFS_UARTRXOneData(Ch);
        return 1u;                                  //接收成功返回1
    }
    else
    {
        return 0u;                                  //没有接收到 字符返回0
    }
}

//接收一个字符  第二个参数为超时参数
int8_t Receive_Byte(uint8_t Ch , uint8_t *c, uint32_t timeout)    //相比上面的函数,只是加了一个超时参数,就是在一定时间内只要收到一个数据就退出
{ 
   uint32_t i;
    
    for ( ; timeout; timeout--)     // 10 ms one time
    {
        for (i=SystemCoreClock/50000u; i; i--)  
        {
            if (Get_One_char(Ch, c) == 1u)
            {
                bFM3_GPIO_PDOR0_PC = ~bFM3_GPIO_PDIR0_PC;  /* LED 202 */
                return 0u;        //接收到一个字符后就返回0
            }
        }
    } 
    
    return -1;
}

/*******************************************************************************
** 函数名  : Put_char
** 功能    : 查询方式发送一个字符
** 输入参数: 无
** 输出参数: 无
** 作者    : 微控事业部
** 创建日期: 2013/8/28
--------------------------------------------------------------------------------
** 修改者  :
** 修改日期:
** 修改内容:
*******************************************************************************/
void Put_char(uint8_t Ch, uint8_t data)
{
//	MFS_UARTEnableTX(InUseCh);                   //使能TXE：将数据送入移位寄存器
	if(MFS_UARTGetTXRegEmptyStatus(Ch) && MFS_UARTGetTXBusIdleStatus(Ch))
	{
		MFS_UARTTXOneData(Ch, data);
	}
	while(!MFS_UARTGetTXBusIdleStatus(Ch));//等待发送完成
//	MFS_UARTDisableTX(InUseCh);                  //关闭发送功能
    //bFM3_GPIO_PDOR0_PD = ~bFM3_GPIO_PDIR0_PD; /* LED201 */
	return;
}

/**
  * @brief  Convert an Integer to a string
  * @param  str: The string
  * @param  intnum: The integer to be converted
  * @retval None
  */
void Int2Str(uint8_t* str, int32_t intnum)  //
{
  uint32_t i, Div = 1000000000, j = 0, Status = 0;

  for (i = 0; i < 10; i++)               
  {
    str[j++] = (intnum / Div) + 48;      

    intnum = intnum % Div;               
    Div /= 10;
    if ((str[j-1] == '0') & (Status == 0)) 
    {
      j = 0;                                
    }
    else
    {
      Status++;                             
    }
  }
}

/**
  * @brief  Convert a string to an integer
  * @param  inputstr: The string to be converted
  * @param  intnum: The integer value
  * @retval 1: Correct
  *         0: Error
  */
uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum)
{
  uint32_t i = 0, res = 0;
  uint32_t val = 0;

  if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X'))
  {
    if (inputstr[2] == '\0')
    {
        return 0;
    }
    for (i = 2; i < 11; i++)
    {
      if (inputstr[i] == '\0')
      {
        *intnum = val;
        /* return 1; */
        res = 1;
        break;
      }
      if (ISVALIDHEX(inputstr[i]))
      {
        val = (val << 4) + CONVERTHEX(inputstr[i]);
      }
      else
      {
        /* Return 0, Invalid input */
        res = 0;
        break;
      }
    }
    /* Over 8 digit hex --invalid */
    if (i >= 11)
    {
      res = 0;
    }
  }
  else /* max 10-digit decimal input */
  {
    for (i = 0;i < 11;i++)
    {
      if (inputstr[i] == '\0')
      {
        *intnum = val;
        /* return 1 */
        res = 1;
        break;
      }
      else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0))
      {
        val = val << 10;
        *intnum = val;
        res = 1;
        break;
      }
      else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0))
      {
        val = val << 20;
        *intnum = val;
        res = 1;
        break;
      }
      else if (ISVALIDDEC(inputstr[i]))
      {
        val = val * 10 + CONVERTDEC(inputstr[i]);
      }
      else
      {
        /* return 0, Invalid input */
        res = 0;
        break;
      }
    }
    /* Over 10 digit decimal --invalid */
    if (i >= 11)
    {
      res = 0;
    }
  }

  return res;
}

