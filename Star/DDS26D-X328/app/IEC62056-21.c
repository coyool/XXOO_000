/*******************************************************************
Copyright (c) 2013,深圳市思达仪表有限公司 
All rights reserved. 
 
文件名称：IEC62056-21.h 
摘要：IEC62056-21通讯协议
作者：chy
当前版本：1.0.0 -20140701

历史记录：
********************************************************************/

#include "Include.h"
const uint16 IbCode = 5000;
const uint32 ImaxCode = 80000;
const uint16 PulseConst = 1600;
const uint16 UnCode = 2200;
const uint16 FrequencyCode = 5000;
const uint8 ConstCode[] = {'*', 'i', 'm', 'p', '/', 'k', 'W', 'h'};
const uint8 MinuteCode[] = {'*', 'M', 'i', 'n', 'u', 't', 'e', 's'};
const uint8 kWhCode[] = {'*', 'k','W','h'};
const uint8 kWCode[] = {'*', 'k','W'};
const uint8 AES128KEY[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
/*---------------------------------------------------------------------------*/
/*内部常数和宏定义                                                           */
/*---------------------------------------------------------------------------*/
const uint8 IdentificationCode[]=
{
    '/',             //起始符
    'S','T','R',     //XXX 公司专用商标
    '4',             //4800bps
    'T','i','p',':','C','M','1','3','1','D','0','1','4','A',
    er,
    cr
};

const uint8 OBISCode[] =							   
{   
    0x00, 0x00, 0x00, OBIS_ADDR,     //地址
	0x00, 0x09, 0x01, OBIS_Time,	 //时间
    0x00, 0x09, 0x02, OBIS_Date,     //日历     
    0x00, 0x06, 0x00, OBIS_Voltage,  // 220.0V
    0x00, 0x06, 0x01, OBIS_Ib,       //5.0A
    0x00, 0x06, 0x02, OBIS_Frequency,//50.0HZ
    0x00, 0x06, 0x03, OBIS_Imax,     //80.0A
    0x00, 0x03, 0x00, OBIS_PConst,   //NNNNNN imp/kWh
    0x00, 0x02, 0x00, OBIS_SoftwareVer, //程序版本号 DDS26D-VN21-2012.06.01
    0x00, 0x02, 0x01, OBIS_HardwareVer, //硬件版本号 DDS26D-VN21 VER:
    0x00, 0x02, 0x03, OBIS_Disp_Add,    //显示表地址
    0x11, 0x07, 0x00, OBIS_CurrentCurrent,     //当前电流
    0x12, 0x07, 0x00, OBIS_CurrentVoltage,     //当前电压
    0x13, 0x04, 0x00, OBIS_CurrentPowerFactor1, //功率因素
    0x13, 0x07, 0x00, OBIS_CurrentPowerFactor, //功率因素 //兼容
    0x01, 0x07, 0x00, OBIS_CurrentPower,       //实时功率
    0x14, 0x07, 0x00, OBIS_CurrentFrequency,   //实时频率
    0x09, 0x07, 0x00, OBIS_CurrentVAPower,     //视在功率
    0x96, 0x06, 0x03, OBIS_Bat1Voltage, //电池1电压值 x.xxV
    0x96, 0x06, 0x04, OBIS_Bat2Voltage, //电池2电压值 x.xxV
    0x96, 0x05, 0x00, OBIS_EventStatus, //事件状态
    0x96, 0x05, 0x05, OBIS_ErrorStatus, //错误状态
    0x96, 0x50, 0x00, OBIS_DisplayTime, //自动显示时间 XXS
    0x96, 0x50, 0x02, OBIS_EngryDot,    //电能显示小数位数 X 0--2
    0x96, 0x50, 0x03, OBIS_PowerDot,    //功率显示小数位数 X 0--4
    0x31, 0x35, 0x00, OBIS_OverCurrent, //过流门限值  100.000A
    0x32, 0x35, 0x00, OBIS_OverVoltage, //过压门限值  264.0V
    0x32, 0x31, 0x00, OBIS_UnderVoltage,//欠压门限值  132.0V
    0x96, 0x52, 0x00, OBIS_PassWord1,   //1级密码
    0x96, 0x52, 0x01, OBIS_PassWord2,   //2级密码
    0x96, 0x52, 0x02, OBIS_PassWord3,   //3级密码
    0x96, 0x96, 0x00, OBIS_SetPassWord, //设置密码
    0x96, 0x60, 0x00, OBIS_PowerDownCounts,//掉电次数 
    0x96, 0x62, 0x00, OBIS_OverVoltageCounts, //过压次数
    0x96, 0x62, 0x01, OBIS_OverVoltageMinute, //过压累计时间
    0x96, 0x64, 0x00, OBIS_OverCurrentCounts,//过流次数
    0x96, 0x64, 0x01, OBIS_OverCurrentMinute,//过流累计时间
    0x96, 0x66, 0x00, OBIS_imbalanceCounts,  //电流不平衡次数
    0x96, 0x66, 0x01, OBIS_imbalanceMinute,  //电流不平衡累计时间
    0x96, 0x06, 0x00, OBIS_BatRunTime,       //电池运行时间
    0x96, 0x08, 0x00, OBIS_MeterRunTime,     //电表运行时间   
    0x01, 0x09, 0xff, OBIS_AllEng,           //总有功
    0x01, 0x08, 0xff, OBIS_EngPos,   //正向有功电能
    0x02, 0x08, 0xff, OBIS_EngNeg,   //反向有功电能
    0x99, 0x02, 0x01, OBIS_DayEng,
    0x99, 0x01, 0x00, OBIS_BlockData,       //读数据块  
    0x00, 0x09, 0x05, OBIS_JCSetData,     //集抄设置广播时间
    0x96, 0x50, 0x06, OBIS_GeographicPos, // 地理信息
};

// OBIS 格式----块抄时使用
#define  PATTERN1   1
#define  PATTERN2   2
#define  PATTERN3   3
#define  PATTERN4   4
#define  PATTERN5   5
#define  PATTERN6   6
#define  PATTERN7   7
#define  PATTERN8   8

#define  PATTERNUM  8  //增加OBIS格式需要修改此值
#define  PATTERLEN  8  //修改OBIS格式内容需要修改此值

const uint8 OBISPattern[PATTERNUM*PATTERLEN]=
{
	 0x01,'.' ,0x01 ,'.'  ,0x01 ,0xFF ,0xff ,0xff,    //N.N.N
	 0x02,'.' ,0x02 ,0xFF ,0xFF ,0xFF ,0xff ,0xff,    //NN.NN 
	 0x02,'.' ,0x01 ,0xFF ,0xFF ,0xFF ,0xff ,0xff,    //NN.N     
	 0x0A,'.' ,0x01 ,'.'  ,0x01 ,0xFF ,0xff ,0xff,    //A.N.N   //A 是ASCLL 如C D P L 	
	 0x0A,'.' ,0x02 ,0xFF ,0xFF ,0xFF ,0xff ,0xff,    //A.NN    //A 是ASCLL 如C D P L       
     0x0A,'.' ,0x0A ,0xFF ,0xFF ,0xFF ,0xff ,0xff,    //A.A    
     0x01,'.' ,0x01 ,'.'  ,0x01 ,'*'  ,0x2  ,0xff,	  //N.N.N*N
	 0x02,'.' ,0x01 ,'.'  ,0x01 ,0xFF ,0xff ,0xff,    //NN.N.N  
};

const uint8 CMDOBISCode[] =
{
    0x44, 0x0,  0x0 , OBIS_STARUPGRADE,         //升级密码
};

const uint8 OBISCodeBlockRead[] =
{
	OBIS_ADDR 	             ,0,0,PATTERN1,
	OBIS_Time                ,0,0,PATTERN1,
	OBIS_Date                ,0,0,PATTERN1,    
    OBIS_CurrentCurrent      ,0,0,PATTERN8,
    OBIS_CurrentVoltage      ,0,0,PATTERN8,    
    OBIS_CurrentPowerFactor  ,0,0,PATTERN8, 
    OBIS_CurrentPower        ,0,0,PATTERN1,
    OBIS_CurrentFrequency    ,0,0,PATTERN8,
    OBIS_CurrentVAPower      ,0,0,PATTERN1,
    OBIS_AllEng              ,0,0,PATTERN1,
    OBIS_EngPos              ,0,0,PATTERN1,
    OBIS_EngNeg              ,0,0,PATTERN1,    
};	 

const uint8 ProgramAnswerCode[]=
{
    0x01,
    'P','0',0x02,'(',
    '7','5','4','1','0','2','3','1',
    ')',0x03
};

// OBIS 格式----块抄时使用
#define  PATTERN1   1
#define  PATTERN2   2
#define  PATTERN3   3
#define  PATTERN4   4
#define  PATTERN5   5
#define  PATTERN6   6
#define  PATTERN7   7

#define  PATTERNUM  8  //增加OBIS格式需要修改此值
#define  PATTERLEN  8  //修改OBIS格式内容需要修改此值

/*---------------------------------------------------------------------------*/
/*内部结构体、枚举和类声明                                                   */
/*---------------------------------------------------------------------------*/
IEC62056_Type	*IEC62056[4];
tDateTime  AdjustTime; //HighByteYear、LowByteYear、Month、Day、Week、Hour、Minute、Second

/*---------------------------------------------------------------------------*/
/*变量定义                                                                   */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*内部函数声明                                                               */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*函数定义                                                                   */
/*---------------------------------------------------------------------------*/


/*****************************************************************************************
*   Action: 置位一个标志位
*   Input:  
*   Output:   
*****************************************************************************************/
void BitSet(uint8 *Flag, uint8 Bit)
{
	*Flag |= Bit;
}
    
/*****************************************************************************************
*   Action: 清一个标志位
*   Input:  
*   Output:   
*****************************************************************************************/
void BitClear(uint8 *Flag, uint8 Bit)
{
	*Flag &= (~Bit);
}
    
/*****************************************************************************************
*   Action: 单字节数据存入串口缓存
*   Input:  
*   Output:   
*****************************************************************************************/    
uint8 DataToBuff(uint8 Data, COM_TYPEDEF com)
{
    if(IEC62056[com]->nTx >= MAXBUFFER-1)
        return 0;
    IEC62056[com]->comBuff[IEC62056[com]->nTx++] = Data;
    return 1;
}

/*****************************************************************************************
*   Action:   485通讯初始化
*   Input:    
*   Output:   
*****************************************************************************************/
void RS485DLMSStatusInit(void)
{
     ProtocolUartInit(UART_485,USART_7E1,BAUDE_300BPS,commRxBffer[UART_485],sizeof(commRxBffer[UART_485]),HalfDouble);      
     memset((uint8 *)commRxBffer[UART_485], 0, sizeof(commRxBffer[UART_485]));
}

/*****************************************************************************************
*   Action:   485通讯初始化
*   Input:    
*   Output:   
*****************************************************************************************/
void MESHDLMSStatusInit(void)
{
     ProtocolUartInit(UART_MASH,USART_7E1,BAUDE_300BPS,commRxBffer[UART_MASH],sizeof(commRxBffer[UART_MASH]),HalfDouble); 
     memset((uint8 *)commRxBffer[UART_MASH], 0, sizeof(commRxBffer[UART_MASH]));
}

/*****************************************************************************************
*   Action:   485通讯初始化
*   Input:    
*   Output:   
*****************************************************************************************/
void IRDLMSStatusInit(void)
{
     ProtocolUartInit(UART_IR,USART_7E1,BAUDE_300BPS,commRxBffer[UART_IR],sizeof(commRxBffer[UART_IR]),HalfDouble);    
     memset((uint8 *)commRxBffer[UART_IR], 0, sizeof(commRxBffer[UART_IR]));
}

/*****************************************************************************************
*   Action:   VT虚拟串口通讯初始化
*   Input:    
*   Output:   
*****************************************************************************************/
void VTDLMSStatusInit(void)
{
     memset((uint8 *)commRxBffer[UART_VT], 0, sizeof(commRxBffer[UART_VT]));
}

/*****************************************************************************************
*   Action:  获取数据长度
*   Input:   起始位，结束位的前一位
*   Output:  数据长度
*****************************************************************************************/
uint8 GetDataLength(uint8 *StartAddr, uint8 EndChar)
{
    uint8 i = 0;
    while(*StartAddr != EndChar)
    {
        if(i < MAXBUFFER)
            i++;
        else
            break;
        StartAddr++;
    }
    return i;
}

/*****************************************************************************************
*   Action:  ASCII码数据转换
*   Input:   
*   Output:  
*****************************************************************************************/
uint8 ASCIIToHex(uint8 *Buff, uint8 *Src, uint8 Length)
{
   uint8 i;
   uint8 DataTemp;
   for(i=0; i<Length; i++)	  //Length为接收到的数据长度
   {
       DataTemp = *(Src+i);
       if((DataTemp>='0') && (DataTemp<='9'))	//为数字
       {
          DataTemp &=0x0f; 
       }
       else if((DataTemp>='A') && (DataTemp<='F'))
       {
          DataTemp -=55;      
       } 
       else
       {
          return 0;
       }  
       if(i%2)
       {   
          *Buff <<= 4;
          *Buff +=DataTemp;        
           Buff ++; 
       }  
       else
       {
         *Buff = (DataTemp&0x0f);        
       }              
   }
   return 1;   
}

/*****************************************************************************************
*   Action:  将数组数据或4字节数据转成ASCII发送
*   Input:   数组数据，4字节数据，待转换数据位数，ASCII数据位数，类型
*   Output:  
*****************************************************************************************/
uint8 DataOrganize(uint8 *buff, uint32 data, uint8 *DataTemp, uint8 DataLength, uint8 ASCIILength, uint8 Dot, uint8 Type)
{
    uint8 i,j = 0;
    uint8 Flag = 0;
    uint32 LevelTemp = 1;
    uint8 Temp;
    uint32 Data=0;
    uint32 TempData;
    
    if(Type == ArrayType)       
    {
        for(i=0;i<DataLength/2;i++)  
        {
            TempData = *buff;
            for(j=0;j<i;j++)
                TempData=TempData<<8;
            Data += TempData;
            buff++;
        }        
        i=0;
        j=0;
    }
    else if(Type == NumType)
    {
        Data = data;   
    }
    for(i=0; i<DataLength-ASCIILength; i++)
    {
        Data /= 10;
    }
    for(i=0; i<ASCIILength-1; i++)
    {
        LevelTemp *= 10;
    }
    ASCIILength++;
    if(Dot > 4)
    {
        Dot = 0;
    }
    for(i=0; i<ASCIILength; i++)
    {
        if((i==ASCIILength-1-Dot))
        {
            if(Dot == 0)
                continue;
            *(DataTemp + j) = '.';
            j++;
            continue;
        }
        Temp = (Data/LevelTemp)%10;
        if((Temp!=0) || (Flag==1) || (i>=ASCIILength-2-Dot))
        {
            Flag = 1;
            *(DataTemp + j) = Temp;
            j++;
            if(!LevelTemp)
                return j;
        }
        LevelTemp /= 10;
    }
    return j;
}

/*****************************************************************************************
*   Action:  根据日期算星期
*   Input:   
*   Output:  
*****************************************************************************************/ 
uint8 CalcWeek(uint8 *Date)
{
    uint8 d;
   	uint8 m;
    uint16 y;
    uint8 w;
    y = 2000 + *Date;
	Date++;
    m = *Date;
	Date++;
   	d = *Date;	
    d++;
	if(m < 3)   
	{
	    m += 12;   //把一月、二月看成上一年的十三月、十四月
	    y--;
   	}
   	w = (d + 2*m + 3*(m+1)/5 + y + y/4 - y/100 + y/400)%7;    //标准公式 0－6（0为星期天）
    if(w==0)
        w = 7;
    return w;
}

/*****************************************************************************************
*   Action:  多字节数据存入串口缓存
*   Input:   
*   Output:  
*****************************************************************************************/ 
void MultiDataToBuff(uint8 *StartAddr, uint8 Length, COM_TYPEDEF com)
{
    uint8 i;
    uint8 Temp;
    for(i=0; i<Length; i++)
    {
        Temp = *(StartAddr+i);
        if(Temp<=9)
        {
            Temp += '0';
        }
        DataToBuff(Temp, com);
    }
}

/*****************************************************************************************
*   Action:  获取日期
*   Input:   
*   Output:  
*****************************************************************************************/ 
void GetOBISDate(uint8 YY, uint8 MM, uint8 DD, COM_TYPEDEF com)
{
    uint8 DataTemp[10];
    DataTemp[0] = 2;
    DataTemp[1] = 0;
    DataTemp[2] = YY/10;
    DataTemp[3] = YY%10;
    DataTemp[4] = '-';
    DataTemp[5] = MM/10;
    DataTemp[6] = MM%10;
    DataTemp[7] = '-';
    DataTemp[8] = DD/10;
    DataTemp[9] = DD%10;
    MultiDataToBuff(DataTemp, 10, com);
}

/*****************************************************************************************
*   Action:  获取时间
*   Input:   
*   Output:  
*****************************************************************************************/ 
void GetOBISTime(uint8 hh, uint8 mm, uint8 ss, COM_TYPEDEF com)
{
    uint8 DataTemp[8];
    DataTemp[0] = hh/10;
    DataTemp[1] = hh%10;
    DataTemp[2] = ':';
    DataTemp[3] = mm/10;
    DataTemp[4] = mm%10;
    DataTemp[5] = ':';
    DataTemp[6] = ss/10;
    DataTemp[7] = ss%10;
    MultiDataToBuff(DataTemp, 8, com);    
}

/*****************************************************************************************
*   Action:  ASCII码转换为数据
*   Input:   
*   Output:  
*****************************************************************************************/ 
uint8 ASCIIToData(uint8 *Buff, uint8 *Src, uint8 Length, uint8 Type)
{
    uint8 i,j = 0;
    uint8 DataLength = 0;
    uint8 DataTemp;
    for(i=0; i<4; i++)
    {
        if(*(Src+i) != 'F')
            break;
    }
    if(i == 4)
    {
        *Buff = 0xff;
        *(Buff+1) = 0xff;
        DataLength = 2;
        return DataLength;       //数据为全F
    }
    if(Type == HexType)
    {
        for(i=0; i<Length; i++)	  //Length为接收到的数据长度
        {
            j++;
            DataTemp = *(Src+i);
            if(((DataTemp<'0')||(DataTemp>'9')) && ((DataTemp<'A')||(DataTemp>'F')))
            {
                j = 0;
                Buff++;
                DataLength++;
            }
            else if(j == 1)//第一位数值
            {
                if((DataTemp>='0') && (DataTemp<='9'))	//为数字
                    *Buff = DataTemp&0x0f;  
                else
                    *Buff = DataTemp - 55;				//A为65 减去55为10
            }
            else if(j == 2)//第二位数值
            {
                if((DataTemp>='0') && (DataTemp<='9'))
                {
                    *Buff = (*Buff)*10;
                    *Buff += (DataTemp&0x0f);
                }
                else if((DataTemp>='A') && (DataTemp<='F'))
                {
                    *Buff <<= 4;
                    *Buff |= DataTemp - 55;
                }
            }
            else if(j >= 3)
            {
                j = 0;
                Buff++;
                DataLength++;
                i--;
            }
        }
        return ++DataLength;
    }

    if(Type == ArrayType)
    {
        for(i=0; i<Length; i++)
        {
            DataTemp = *(Src+i);
            if((DataTemp>='0') && (DataTemp<='9'))
            {
                *Buff = DataTemp - '0';
                Buff++;
                DataLength++;
            }
        }
        return DataLength;
    }
    return DataLength;	 
}

/*****************************************************************************************
*   Action: 表地址验证
*   Input:   
*   Output: 通过返回1，失败返回0  
*****************************************************************************************/ 
uint8 AddressCompare(uint8 *Src, const uint8 EndChar, COM_TYPEDEF com)
{
    uint8 i;
    uint8 Temp;
	uint8 MeterNOTemp[2*METER_ADDR_LEN];
    uint8 ReadMeter[METER_ADDR_LEN];
    API_GetDataValue(CurrentADDR_IDCode,ReadMeter);
	for(i=0; i<METER_ADDR_LEN; i++)
	{
	    MeterNOTemp[2*i] = ReadMeter[i]/0x10;
		MeterNOTemp[2*i+1] = ReadMeter[i]%0x10;
	}																
    for(i=0; i<=2*METER_ADDR_LEN; i++)														 
    {
        if(*(Src+i) == EndChar) 
            break;     
    }    
    if(!i)
    {
        BitSet((uint8 *)&IEC62056[com]->Flag, BroadcastAddress);
        return 1;           //i为0表示读表地址
    }
    BitClear((uint8 *)&IEC62056[com]->Flag, BroadcastAddress);
    if(i == (2*METER_ADDR_LEN + 1))  //输入的表地址长度超出
        return 0;
    Temp = i;               //Temp表示有效表地址（不包括前端的0）长度
    for(i=0; i<Temp; i++)
    {
        if((*(Src+i)-'0') != MeterNOTemp[i+2*METER_ADDR_LEN-Temp])   //有效表地址比较
            return 0;
    }
    for(i=0; i<2*METER_ADDR_LEN-Temp; i++)    //无效表地址要全为0
    {
        if(MeterNOTemp[i] != 0)
        {
            return 0;
        }
    }
    return 1;
}

/********************************************/
//功能: 把四个字节的OBIS信息按照格式写入到发送缓存中
//输入参数:
//输出参数:
//函数返回值说明:
//使用的资源:
//作者:
//日期:
//备注: 
/********************************************/
void LoadOBIS(uint8 *OBIS, uint8 *pattern, COM_TYPEDEF com)
{
    uint8 i;
	uint8 j=0;
	for(i=0;i<PATTERLEN;i++)  //格式长度固定 此3 固定
	{
	    if(pattern[i]==0x0A)
		{
			IEC62056[com]->comBuff[IEC62056[com]->nTx++] =*(OBIS+j);  // 首字节是字母类型
		}		
	    else if(pattern[i]==0x01)
		{		
			IEC62056[com]->comBuff[IEC62056[com]->nTx++] = '0' + ((*(OBIS+j))&0x0f);
		}
	    else if(pattern[i]==0x02)
		{	
		    IEC62056[com]->comBuff[IEC62056[com]->nTx++] = '0' + ((*(OBIS+j))>>4);
			IEC62056[com]->comBuff[IEC62056[com]->nTx++] = '0' + ((*(OBIS+j))&0x0f);
		}
		else
		{
			return ;  //数据也为0xFF
		}
		i++;
		j++;
		if(pattern[i]==0xff)
		{
			return;   //分隔符为0xFF
		}
		else
		{
		    IEC62056[com]->comBuff[IEC62056[com]->nTx++] = pattern[i];
		}
	}
}

/*****************************************************************************************
*   Action:  获取OBIS
*   Input:   
*   Output:  
*****************************************************************************************/ 
uint8 GetOBIS(uint8 *OBIS, uint8 *Buff)
{
    uint8 i,j = 0;
    for(i=4; i<15; i++)	       //前四个字节为SOH R2 STX
    {
        j++;
        if(*(Buff+i) == '(')   //遇到'('返回
            break;
        else if((*(Buff+i)=='.') || (*(Buff+i)=='*'))  //中间的符号'.'、'*'判断
        {
            j = 0;			   //j表示一个OBIS码含有的数字个数
            OBIS++;			   //符号区分一个OBIS码
        }
        else if(j==3)		   //一个OBIS码含有的数字个数最多两个 
        {
            j = 0;
            OBIS++;
            i--;
        }
        else if((*(Buff+i)>='0') && (*(Buff+i)<='9'))
        {
            if(j == 1)
            {
                *OBIS = (*(Buff+i))&0x0f;   
            }
            else if(j == 2)
            {
                *OBIS = *OBIS << 4;
                *OBIS |= (*(Buff+i))&0x0f;	 //含两个数字码
            }
        }
        else
            return 0;
    }
    if((i>14) || (i==4))
        return 0;		  //OBIS读取有误
    return 1;	          //OBIS读取正常
    
}

/*****************************************************************************************
*   Action:  校验BCC
*   Input:   数据缓存，串口名
*   Output:  
*****************************************************************************************/ 
uint8 CheckBCC(uint8 *Buff, COM_TYPEDEF com)
{
    uint8 i;
    uint8 StatusTemp = 0;
    uint8 NorTemp = 0;
    for(i=1; i<IEC62056[com]->rxPointer-1; i++)
    {
        NorTemp ^= *(Buff+i);
        if((*(Buff+i) == '(') && (StatusTemp==0))
            StatusTemp = 1;
        if((StatusTemp==1) && (*(Buff+i)==')') && (*(Buff+i+1)==etx))
        {
            if((i+3) == IEC62056[com]->rxPointer)
                StatusTemp = 2;
        }
    }
    if(2 == StatusTemp)
    {
        if(*(Buff+i) == NorTemp)
            return 1;
    }
    return 0;
}

/*****************************************************************************************
*   Action:  正常应答
*   Input:   数据缓存
*   Output:  
*****************************************************************************************/ 
void DLMSACK(uint8 Data, COM_TYPEDEF com)
{
	BitSet((uint8 *)&IEC62056[com]->Flag.byte, SendFlag);
    IEC62056[com]->nTx = 0;
    DataToBuff(Data, com);
}

/*****************************************************************************************
*   Action:  计算BCC校验和
*   Input:   起始位，数据长度
*   Output:  
*****************************************************************************************/
uint8 CalcBCC(uint8 *StartPoint, uint8 Counts)
{
    uint8 Temp = 0;
    uint8 i;
    if(!Counts)
        return 0;
    for(i=0; i<Counts; i++)
    {
        Temp ^= *(StartPoint+i);
    }
    *(StartPoint + Counts) = Temp;
    return 1;
}

/*****************************************************************************************
*   Action:  获取OBIS类型
*   Input:   数据缓存
*   Output:  
*****************************************************************************************/
uint8 GetOBISType(uint8 *OBIS)
{
    uint16 i;
    uint8 Type = 0;
    for(i=0; i<(sizeof(OBISCode)>>2); i++)
    {
        if((*OBIS==OBISCode[i<<2]) && (*(OBIS+1)==OBISCode[(i<<2)+1]) && ((*(OBIS+2)==OBISCode[(i<<2)+2]) || (OBISCode[(i<<2)+2]==0xff)))
        {
            Type = OBISCode[(i<<2)+3];
            return Type;
        }
    }
    return Type;
}

/*****************************************************************************************
*   Action:  获取命令操作中的OBIS类型
*   Input:   数据缓存
*   Output:  
*****************************************************************************************/
uint8 GetCMDOBISType(uint8 *OBIS)
{
    uint16 i;
    uint8 Type = 0;
    for(i=0; i<(sizeof(CMDOBISCode)>>2); i++)
    {
        if((*OBIS==CMDOBISCode[i<<2]) && (*(OBIS+1)==CMDOBISCode[(i<<2)+1]) && ((*(OBIS+2)==CMDOBISCode[(i<<2)+2]) || (CMDOBISCode[(i<<2)+2]==0xff)))
        {
            Type = CMDOBISCode[(i<<2)+3];
            return Type;
        }
    }
    return Type; 
}

/*****************************************************************************************
*   Action:  获取表地址
*   Input:   
*   Output:  
*****************************************************************************************/
void GetAddress(COM_TYPEDEF com)
{
    uint8 i;
    uint8 j = 0;
    uint8 Temp[METER_ADDR_LEN];
    uint8 MeterNOTemp[2*METER_ADDR_LEN];
    API_GetDataValue(CurrentADDR_IDCode,Temp);
	for(i=0; i<METER_ADDR_LEN; i++)
	{
	    MeterNOTemp[2*i] = Temp[i]/0x10;
		MeterNOTemp[2*i+1] = Temp[i]%0x10;
	}				
    for(i=0; i<2*METER_ADDR_LEN; i++)
    {
        if(MeterNOTemp[i] != 0)
            j=1;
        if(j||(i==(2*METER_ADDR_LEN-1)))
        {
            DataToBuff(MeterNOTemp[i]+'0', com);
        }
    }    
}

/*****************************************************************************************
*   Action:  数据抄读
*   Input:   OBIS码，数据缓存，串口名
*   Output:  
*****************************************************************************************/
uint8 OBISRead(uint8 *OBIS, uint8 *TempBuff, COM_TYPEDEF com, uint8 ReadType)
{
	uint8 OBISType = 0;        //OBIS数据类型
    uint8 Temp[MAXBUFFER];	   
    uint8 FlagTemp = 0;	   
    uint8 LengthTemp;
    uint8 TempData;
    uint8 Addr;
    uint8 i;
    uint8  BlockFirstFlame = 0;   //读数据块第一帧
	uint8  BlockLastFlame = 0;    //读数据块最后一帧
    /*------------------------------------*/
	if((ReadType!=NormalRead)&&(ReadType!=BlockRead))   //暂时只抄普通数据
		return 0;
	if(ReadType==NormalRead)
    {        
	    OBISType = GetOBISType(OBIS);
        if(!OBISType)
            return 0;
        DataToBuff(stx, com);          
        TempData = GetDataLength(TempBuff, stx);
        TempData +=1; 
	    LengthTemp =GetDataLength(TempBuff, '(');			
		memcpy((uint8 *)&IEC62056[com]->comBuff[IEC62056[com]->nTx],TempBuff+TempData, LengthTemp-TempData); //载入刚下发的OBIS,原路返回 
		IEC62056[com]->nTx+=LengthTemp-TempData;
        DataToBuff('(', com);
        memset(Temp, 0, sizeof(Temp));  //清缓存数据       
    }
    else
    {
        Addr=IEC62056[com]->BlockDataCount;//Addr
		Addr =Addr*4;
		OBISType=OBISCodeBlockRead[Addr];
		i=OBISCodeBlockRead[Addr+3]; //OBIS格式
		i =(i-1)*PATTERLEN;
		memcpy(OBIS, &OBISCode[4*(OBISType-1)], 3);
		if(OBIS[2]==0xff)
		{
			OBIS[2]=OBISCodeBlockRead[Addr+1];	//费率或者次数
			OBIS[3]=OBISCodeBlockRead[Addr+2];	//月
		}		
		if(IEC62056[com]->BlockDataCount==0)
		{  
		    BlockFirstFlame = 1;
			DataToBuff(stx, com);			
		}
		else if(IEC62056[com]->BlockDataCount==OBISREAD_NUMITEM-1)
		{
			BlockLastFlame=1;
		}
		memcpy(Temp,OBISPattern+i,PATTERLEN);
		LoadOBIS(OBIS,Temp,com);
        DataToBuff('(', com);
    }
    if(OBISType == OBIS_ADDR)  //表地址
    {
        GetAddress(com);
        DataToBuff(')', com);
    }
    else if(OBISType == OBIS_Voltage)//电压
    {
        LengthTemp = DataOrganize(Temp, UnCode, Temp, 4, 4, 1, NumType);
        Temp[LengthTemp++] = '*';
        Temp[LengthTemp++] = 'V';
        FlagTemp = 1;
    }
    else if(OBISType == OBIS_Ib)//Ib
    {
        LengthTemp = DataOrganize(Temp, IbCode, Temp, 8, 8, 3, NumType);
        Temp[LengthTemp++] = '*';
        Temp[LengthTemp++] = 'A';
        FlagTemp = 1;
    }
    else if(OBISType == OBIS_Frequency)//工频
    {
        LengthTemp = DataOrganize(Temp, FrequencyCode, Temp, 4, 4, 2, NumType);
        Temp[LengthTemp++] = '*';
        Temp[LengthTemp++] = 'H';
        Temp[LengthTemp++] = 'Z';
        FlagTemp = 1;
    }
   else if(OBISType == OBIS_Date)//日期
   {
       uint8 Year;
       API_GetDataValue(Clock_IDCode,(uint8 *)&AdjustTime);
       Year = (uint8)(AdjustTime.HighByteYear*0x100 + AdjustTime.LowByteYear - 2000);
       GetOBISDate(Year, AdjustTime.Month, AdjustTime.Day, com);
       DataToBuff(' ', com);
       DataToBuff(AdjustTime.Week+'0', com);
       DataToBuff(')', com);
   }
   else if(OBISType == OBIS_Time)//时间
    {
        API_GetDataValue(Clock_IDCode,(uint8 *)&AdjustTime);
        GetOBISTime(AdjustTime.Hour, AdjustTime.Minute, AdjustTime.Second, com);
        DataToBuff(')', com);
    }    
    else if(OBISType == OBIS_Imax)//最大电流
    {
        LengthTemp = DataOrganize(Temp, ImaxCode, Temp, 8, 8, 3, NumType);
        Temp[LengthTemp++] = '*';
        Temp[LengthTemp++] = 'A';
        FlagTemp = 1;
    }
    else if(OBISType == OBIS_PConst)//有功常数
    {
        LengthTemp = DataOrganize(Temp, PulseConst, Temp, 4, 4, NonDot, NumType);
        memcpy(Temp + LengthTemp, ConstCode, sizeof(ConstCode));
        LengthTemp += sizeof(ConstCode);
        FlagTemp = 1;
    } 
    else if(OBISType == OBIS_HardwareVer)//硬件版本号
    {
        API_GetDataValue(HardWare_IDCode,Temp);
        LengthTemp = HardWareLength;
        FlagTemp = 1;
    }    
    else if(OBISType == OBIS_SoftwareVer)//软件版本号   
    {
        API_GetDataValue(SoftWare_IDCode,Temp);
        LengthTemp = SoftWareLength;
        FlagTemp = 1;
    } 
    else if(OBISType == OBIS_CurrentCurrent) //电流
    {
        API_GetDataValue(CurrentCurrent_IDCode,Temp);
        LengthTemp = DataOrganize(Temp, 0, Temp, 8, 8, 3, CodeType);  //8表示最多4个字节
        Temp[LengthTemp++] = '*';
        Temp[LengthTemp++] = 'A';
        FlagTemp = 1;        
    }
    else if(OBISType == OBIS_CurrentVoltage) //当前电压
    {
        API_GetDataValue(CurrentVoltage_IDCode,Temp);
        LengthTemp = DataOrganize(Temp, 0, Temp, 4, 4, 1, CodeType);
        Temp[LengthTemp++] = '*';
        Temp[LengthTemp++] = 'V';
        FlagTemp = 1;
    }
    else if((OBISType == OBIS_CurrentPowerFactor)||(OBISType == OBIS_CurrentPowerFactor1))//功率因数
    {
        API_GetDataValue(CurrentPowerFactor_IDCode,Temp);
        LengthTemp = DataOrganize(Temp, 0, Temp, 4, 4, 3, CodeType);
        FlagTemp = 1;
    }
    else if(OBISType == OBIS_CurrentPower)//功率  
    {
        API_GetDataValue(CurrentPower_IDCode,Temp);
        LengthTemp = DataOrganize(Temp, 0, Temp, 8, 8, 4, CodeType);
        memcpy(Temp+LengthTemp, kWCode, sizeof(kWCode));
        LengthTemp += sizeof(kWCode);
        FlagTemp = 1;
    }
    else if(OBISType == OBIS_CurrentFrequency) //频率
    {
        API_GetDataValue(CurrentFrequency_IDCode,Temp);
        LengthTemp = DataOrganize(Temp, 0, Temp, 4, 4, 2, CodeType);
        Temp[LengthTemp++] = '*';
        Temp[LengthTemp++] = 'H';
        Temp[LengthTemp++] = 'Z';
        FlagTemp = 1;
    }
    else if(OBISType == OBIS_CurrentVAPower)//视在
    {
        API_GetDataValue(CurrentVAPower_IDCode,Temp);
        LengthTemp = DataOrganize(Temp, 0, Temp, 8, 8, 4, CodeType);
        Temp[LengthTemp++] = '*';
        Temp[LengthTemp++] = 'k';
        Temp[LengthTemp++] = 'V';
        Temp[LengthTemp++] = 'A';
        FlagTemp = 1;
    }
    else if(OBISType == OBIS_DisplayTime)//每屏显示时间
    {
        API_GetDataValue(DisplayTime_IDCode,(uint8 *)&Temp);
        if(Temp[0]/10)
            DataToBuff(Temp[0]/10+'0', com);
        DataToBuff(Temp[0]%10+'0', com);
        DataToBuff('*', com);
        DataToBuff('S', com);        
        DataToBuff(')', com);
    }    
    else if(OBISType == OBIS_EngryDot)//电能显示小数位数
    {
        API_GetDataValue(EngPoint_IDCode,(uint8 *)&Temp);
        DataToBuff(Temp[0]+'0', com);
        DataToBuff(')',com);
    }	
    else if(OBISType == OBIS_GeographicPos) // 地理信息  回抄40个字节
    {
      API_GetDataValue(CurrentGeographicPos_IDCode,(uint8 *)&Temp); 
      LengthTemp = METER_GEOGRPINIF_LEN;
      FlagTemp = 1;
    }
   /*-----------------------------------------------------------------------------------------------------*/
    else if(OBISType == OBIS_AllEng) //累计有功电能
    {
        uint8 TempMonth;
        TempMonth = ((*(OBIS+3))>>4)*10 + (*(OBIS+3)&0x0f);
        if(TempMonth > MaxSaveMonths)
            return 0;        
        if(TempMonth==0)
            API_GetDataValue(AllEng_IDCode,Temp);
        else                         //12个字节时间+4个字节正向电能+4个字节反向电能+4个字节组合电能
        {
            Temp[0] = TempMonth;
            API_GetDataValue(HisMonthEng_IDCode,Temp);
            memcpy(Temp, Temp+20, 4);
        }          
        LengthTemp = DataOrganize(Temp, 0, Temp, 8, 8, 2, CodeType);
        memcpy(Temp+LengthTemp, kWhCode, sizeof(kWhCode));
        LengthTemp += sizeof(kWhCode);
        FlagTemp = 1;
    }
   /*-----------------------------------------------------------------------------------------------------*/
    else if(OBISType == OBIS_EngPos)
    { 
        uint8 TempMonth;
        TempMonth = ((*(OBIS+3))>>4)*10 + (*(OBIS+3)&0x0f);
        if(TempMonth > MaxSaveMonths)
            return 0;        
        if(TempMonth==0)
            API_GetDataValue(EngPos_IDCode,Temp);
        else                         //12个字节时间+4个字节正向电能+4个字节反向电能+4个字节组合电能
        {
            Temp[0] = TempMonth;
            API_GetDataValue(HisMonthEng_IDCode,Temp);
            memcpy(Temp, Temp+12, 4);
        }          
        LengthTemp = DataOrganize(Temp, 0, Temp, 8, 8, 2, CodeType);
        memcpy(Temp+LengthTemp, kWhCode, sizeof(kWhCode));
        LengthTemp += sizeof(kWhCode);
        FlagTemp = 1;
    }
    /*-------------------------------------------------------------------------------------------------------------------------------------*/
    else if(OBISType == OBIS_EngNeg)
    { 
        uint8 TempMonth;
        TempMonth = ((*(OBIS+3))>>4)*10 + (*(OBIS+3)&0x0f);
        if(TempMonth > MaxSaveMonths)
            return 0;
        if(TempMonth==0)
            API_GetDataValue(NegPos_IDCode,Temp);
        else                         //12个字节时间+4个字节正向电能+4个字节反向电能+4个字节组合电能
        {
            Temp[0] = TempMonth;
            API_GetDataValue(HisMonthEng_IDCode,Temp);
            memcpy(Temp, Temp+16, 4);
        }          
        LengthTemp = DataOrganize(Temp, 0, Temp, 8, 8, 2, CodeType);                
        memcpy(Temp+LengthTemp, kWhCode, sizeof(kWhCode));
        LengthTemp += sizeof(kWhCode);
        FlagTemp = 1;
    }
    else if(OBISType == OBIS_DayEng)
    {
        uint8 TempDay;
        uint8 Temp1[4];
        TempDay = ((*(OBIS+3))>>4)*10 + (*(OBIS+3)&0x0f);
        if(TempDay > MaxSaveDays)
            return 0;
        if(TempDay==0)
        {
            return 0;
        }
        else                         //12个字节时间+4个字节正向电能+4个字节反向电能+4个字节组合电能
        {
            Temp[0] = TempDay;
            API_GetDataValue(HisDayEng_IDCode,Temp);
        }
        
        memcpy(Temp1, Temp+12, 4);  //正向能量
        LengthTemp = DataOrganize(Temp1, 0, Temp1, 8, 8, 2, CodeType);                
        memcpy(Temp1+LengthTemp, kWhCode, sizeof(kWhCode));
        LengthTemp += sizeof(kWhCode);
        MultiDataToBuff(Temp1, LengthTemp, com);
        DataToBuff(')', com);
        
        DataToBuff('(', com);
        memcpy(Temp1, Temp+16, 4);  //反向能量
        LengthTemp = DataOrganize(Temp1, 0, Temp1, 8, 8, 2, CodeType);                
        memcpy(Temp1+LengthTemp, kWhCode, sizeof(kWhCode));
        LengthTemp += sizeof(kWhCode);
        MultiDataToBuff(Temp1, LengthTemp, com); 
        DataToBuff(')', com);
        
        DataToBuff('(', com);       
        memcpy(Temp1, Temp+20, 4); //组合能量
        LengthTemp = DataOrganize(Temp1, 0, Temp1, 8, 8, 2, CodeType);                
        memcpy(Temp1+LengthTemp, kWhCode, sizeof(kWhCode));
        LengthTemp += sizeof(kWhCode);
        MultiDataToBuff(Temp1, LengthTemp, com);
        
        DataToBuff(')', com);
    }
    else if(OBISType == OBIS_BatRunTime)//电池运行时间
    {
        API_GetDataValue(BatRunTime_IDCode,Temp);
        LengthTemp = DataOrganize(Temp, 0, Temp, 8, 8, NonDot, CodeType);
        memcpy(Temp + LengthTemp, MinuteCode, sizeof(MinuteCode));
        LengthTemp += sizeof(MinuteCode);
        FlagTemp = 1;
    }   
    else if(OBISType == OBIS_MeterRunTime)//电表工作时间
    {
        API_GetDataValue(MeterRunTime_IDCode,Temp);
        LengthTemp = DataOrganize(Temp, 0, Temp, 8, 8, NonDot, CodeType);
        memcpy(Temp + LengthTemp, MinuteCode, sizeof(MinuteCode));
        LengthTemp += sizeof(MinuteCode);
        FlagTemp = 1;
    }    
    else if(OBISType == OBIS_Bat1Voltage)//电池1电压
    {
        API_GetDataValue(Bat1Voltage_IDCode,Temp);
        LengthTemp = DataOrganize(Temp, 0, Temp, 4, 4, 2, CodeType);
        Temp[LengthTemp++] = '*';
        Temp[LengthTemp++] = 'V';
        FlagTemp = 1;
    } 
    else if(OBISType == OBIS_EventStatus)//事件状态字
    {        
 /*       uint8 StatusTemp = 0;
        if(CURRENTIMBALANCEFLAG)//电流不平衡
            StatusTemp |= 1<<4;
        if(REVERSEFLAG)//有功方向
            StatusTemp |= 1<<5; 
        if(CURRENTMETERCHANNEL)      //当前计量通道
            StatusTemp |= 1<<6; 
        if(MODELSELECTFLAG)//当前射频模块
            StatusTemp |= 1<<7;
        LengthTemp = DataOrganize(Temp, StatusTemp, Temp, 3, 3, NonDot, NumType);        
        FlagTemp = 1;
     */ 
    }
    else if(OBISType == OBIS_ErrorStatus) //错误状态字
    {
  /*      uint8 TempErrStatus = 0;
        if(BATTERYLOWFLAG)
            TempErrStatus |= 0x01;
        if(CLOCKFAULTFLAG)
            TempErrStatus |= 0x04;
        LengthTemp = DataOrganize(Temp,TempErrStatus, Temp, 3, 3, NonDot, NumType);
        FlagTemp = 1;
     */ 
    }       
    else if(OBISType == OBIS_imbalanceCounts) //窃电次数
    {
        API_GetDataValue(CheatEnergyNum_IDCode,Temp);
        LengthTemp = DataOrganize(Temp, 0, Temp, 8, 8, NonDot, CodeType);
        FlagTemp = 1;
    }
    else if(OBISType == OBIS_Disp_Add)//事件状态字 
    {
        DataToBuff('O', com);
        DataToBuff('K', com);   
        DataToBuff(')', com);
        DisplayAlarm(DIS_METER_NO,TRUE);        
    }
    else if(OBISType == OBIS_BlockData)//日期
    {
        uint8 Year;
        API_GetDataValue(Clock_IDCode,(uint8 *)&AdjustTime);
        Year = (uint8)(AdjustTime.HighByteYear*0x100 + AdjustTime.LowByteYear - 2000);
        GetOBISDate(Year, AdjustTime.Month, AdjustTime.Day, com);
        DataToBuff(' ', com);
        DataToBuff(AdjustTime.Week+'0', com);
        DataToBuff(')', com);        //日期
       
        DataToBuff('(', com);
        API_GetDataValue(Clock_IDCode,(uint8 *)&AdjustTime);
        GetOBISTime(AdjustTime.Hour, AdjustTime.Minute, AdjustTime.Second, com);
        DataToBuff(')', com);        //时间

        DataToBuff('(', com);       
        API_GetDataValue(AllEng_IDCode,Temp);  
        LengthTemp = DataOrganize(Temp, 0, Temp, 8, 8, 2, CodeType);
        memcpy(Temp+LengthTemp, kWhCode, sizeof(kWhCode));
        LengthTemp += sizeof(kWhCode);
        MultiDataToBuff(Temp, LengthTemp, com); 
        DataToBuff(')', com);        //总能量 
       
        DataToBuff('(', com);
        API_GetDataValue(CurrentVoltage_IDCode,Temp);
        LengthTemp = DataOrganize(Temp, 0, Temp, 4, 4, 1, CodeType);
        Temp[LengthTemp++] = '*';    
        Temp[LengthTemp++] = 'V';    
        MultiDataToBuff(Temp, LengthTemp, com); 
        DataToBuff(')', com);        //电压
       
        DataToBuff('(', com);
        API_GetDataValue(CurrentCurrent_IDCode,Temp);
        LengthTemp = DataOrganize(Temp, 0, Temp, 8, 8, 3, CodeType);  //8表示最多4个字节
        Temp[LengthTemp++] = '*';
        Temp[LengthTemp++] = 'A';       
        MultiDataToBuff(Temp, LengthTemp, com); 
        DataToBuff(')', com);        //电流
       
        DataToBuff('(', com);       
        API_GetDataValue(CurrentPower_IDCode,Temp);
        LengthTemp = DataOrganize(Temp, 0, Temp, 8, 8, 4, CodeType);
        memcpy(Temp+LengthTemp, kWCode, sizeof(kWCode));
        LengthTemp += sizeof(kWCode);    
        MultiDataToBuff(Temp, LengthTemp, com); 
        DataToBuff(')', com);        //功率      

        DataToBuff('(', com);       
        API_GetDataValue(CurrentPowerFactor_IDCode,Temp);
        LengthTemp = DataOrganize(Temp, 0, Temp, 4, 4, 3, CodeType);
        MultiDataToBuff(Temp, LengthTemp, com); 
        DataToBuff(')', com);        //功率因素          

        DataToBuff('(', com);       
        API_GetDataValue(CheatEnergyNum_IDCode,Temp);
        LengthTemp = DataOrganize(Temp, 0, Temp, 8, 8, NonDot, CodeType); 
        MultiDataToBuff(Temp, LengthTemp, com); 
        DataToBuff(')', com);        //窃电次数
    }    
    else 
        return 0;
    /*--------------------------------------------------------------------------------------------*/
    if(FlagTemp)
    {
        MultiDataToBuff(Temp, LengthTemp, com); 
        DataToBuff(')', com);
    } 
    
    if(ReadType == BlockRead)  //数据块抄读处理
    {
		IEC62056[com]->BlockDataStatus = ING;
        DataToBuff(er, com);
        DataToBuff(cr, com);              
	    if(BlockFirstFlame)									     //计算BCC校验和 不计算第一帧的02H
	    {    
			memcpy(TempBuff, IEC62056[com]->comBuff, IEC62056[com]->nTx);
            CalcBCC(TempBuff + 1, IEC62056[com]->nTx-1);
		    IEC62056[com]->BlockDataBcc ^= TempBuff[IEC62056[com]->nTx];
	    }
		else if(BlockLastFlame)
		{
	        DataToBuff(etx, com);
            memcpy(TempBuff, IEC62056[com]->comBuff, IEC62056[com]->nTx);
	        CalcBCC(TempBuff, IEC62056[com]->nTx);
		    IEC62056[com]->BlockDataBcc ^= TempBuff[IEC62056[com]->nTx];
			DataToBuff(IEC62056[com]->BlockDataBcc, com);
		}
	    else
	    {
			memcpy(TempBuff, IEC62056[com]->comBuff, IEC62056[com]->nTx);
            CalcBCC(TempBuff, IEC62056[com]->nTx);
		    IEC62056[com]->BlockDataBcc ^= TempBuff[IEC62056[com]->nTx];
	    }
        
        if(++IEC62056[com]->BlockDataCount>= OBISREAD_NUMITEM)
        {
            IEC62056[com]->BlockDataStatus = IDLE;
            IEC62056[com]->BlockDataCount = 0;
            IEC62056[com]->BlockDataBcc = 0;
            
            if(com == UART_485)
            {
				TaskReset(RS485DLMSStatusInit, 100, 0, 1); //初始化
			}           
            else if(com == UART_IR)
            {
				TaskReset(IRDLMSStatusInit, 100, 0, 1);
			} 
			else if(com == UART_MASH)
			{
                TaskReset(MESHDLMSStatusInit, 100, 0, 1);
			}
			else if(com==UART_VT)
		    {
	            TaskReset(VTDLMSStatusInit, 100, 0, 1);
		    }	
        }   
	}    
    return 1;
}   
    
/*****************************************************************************************
*   Action:  设置数据
*   Input:   OBIS码，串口名
*   Output:  1表示设置成功，0表示失败
*****************************************************************************************/
uint8 WriteData(uint8 *OBIS, uint8 *Buff, COM_TYPEDEF com)
{
    uint8 DataTemp1;
    uint8 DataTemp2;
    uint8 OBISType;
    uint8 DataLength;
	uint8 Result;
    
    DataTemp1 = GetDataLength(Buff, '(');
    DataTemp1++;
    DataTemp2 = GetDataLength(Buff + DataTemp1, ')');
    OBISType = GetOBISType(OBIS);
    /*---------------------------------------------------------------------*/
    if(OBISType == OBIS_ADDR)     
    {
        uint8 Temp[2*METER_ADDR_LEN];
		uint8 Temp1[2*METER_ADDR_LEN];
        uint8 i,j=0;
        uint8 HighVoltageFlag = 0;  //电平为高
        tMeterImportInfo MeterImportInfo;        
        if((IEC62056[com]->PasswordLevel&(Level1|Level2|Level3)) == 0)
            return 0;        
        if(DataTemp2 > 2*METER_ADDR_LEN)
            return 0;
	    for(i=0;i<=50;i++)    //判断IO口电平
	    {
		    if(IO_ReadGPIOPin(IO_PORT_PRO_SWI,IO_PIN_PRO_SWI))
            {
                j++;
            }
            else
            {
                if(j)
                {
                    j--;
                }
            }
            PublicDelayMs(1);
	    }  
        if(j>25)
             HighVoltageFlag = 1;  //电平为高     
        memset(Temp, 0, sizeof(Temp));
        API_GetDataValue(CurrentADDR_IDCode,(uint8 *)&MeterImportInfo.MeterAddr);
        if(HighVoltageFlag&&(memcmp(MeterImportInfo.MeterAddr, Temp, sizeof(MeterImportInfo.MeterAddr))))  //相等为0
            return 0;      

        memset(Temp1, 0, sizeof(Temp));
        memcpy(Temp, Buff+DataTemp1, DataTemp2);
        for(i=0; i<DataTemp2; i++)
        {
            if(Temp[i]>'9' || Temp[i]<'0')
            {
                 return 0;
             }
             Temp[i] -= '0';
        }

        if(DataTemp2%2) //为奇数个
        {
            memcpy(Temp1+1, Temp, DataTemp2);
            DataTemp2 = DataTemp2 + 1;
        }
        else
            memcpy(Temp1, Temp, DataTemp2);
                  
        memset((uint8 *)MeterImportInfo.MeterAddr, 0, sizeof(MeterImportInfo.MeterAddr));  
        for(i=0;i<DataTemp2/2;i++)
        {
            MeterImportInfo.MeterAddr[METER_ADDR_LEN-1-i] = Temp1[DataTemp2-2-2*i]*0x10 + Temp1[DataTemp2-1-2*i];
        }
        API_SetDataValue(CurrentADDR_IDCode,(uint8 *)&MeterImportInfo.MeterAddr,METER_ADDR_LEN);
		//=======================================
		//清电能及密码
		BillingParaSet(0x09050081, NULL, 0);		
		PassWordSetDefault();
		DisplayAlarm(DIS_ALARM_CLEAR_OK,TRUE);//显示OK
		//=======================================
        IEC62056[com]->PasswordLevel = 0;
    }//修改密码
    else if(OBISType == OBIS_Time)           //写时间
    {    
        if((IEC62056[com]->PasswordLevel&(Level1|Level2|Level3)) == 0)
            return 0;                
        memcpy(IEC62056[com]->comBuff, Buff+DataTemp1, DataTemp2);
        ASCIIToData(IEC62056[com]->comBuff, IEC62056[com]->comBuff, DataTemp2, HexType); //数据转换
        AdjustTime.Hour =  *(IEC62056[com]->comBuff); 
        AdjustTime.Minute =  *(IEC62056[com]->comBuff + 1);
        AdjustTime.Second =  *(IEC62056[com]->comBuff + 2); 
        
        Result=API_SetDataValue(Clock_IDCode,(uint8 *)&AdjustTime,sizeof(AdjustTime));
        if(Result !=DA_SUCCESS)  //20140923 wzg 
		{
		   return 0; 
		}
        IEC62056[com]->PasswordLevel = 0;  //VN27发送完数据不会清整个缓存，设置好后要清密码等级（设置日期不用）
        DisplayAlarm(DIS_TIME_NO,TRUE); 
	}
	else if(OBISType == OBIS_Date)           //写日期
    {
        if((IEC62056[com]->PasswordLevel&(Level1|Level2|Level3)) == 0)
            return 0;                
        memcpy(IEC62056[com]->comBuff, Buff+DataTemp1, DataTemp2);
        memset((uint8 *)&AdjustTime, 0, sizeof(AdjustTime));
        ASCIIToData(IEC62056[com]->comBuff, IEC62056[com]->comBuff, DataTemp2, HexType); //数据转换
        AdjustTime.HighByteYear = (*(IEC62056[com]->comBuff) + 2000)/0x100; 
        AdjustTime.LowByteYear = (*(IEC62056[com]->comBuff) + 2000)%0x100;
        AdjustTime.Month =  *(IEC62056[com]->comBuff + 1); 
        AdjustTime.Day =  *(IEC62056[com]->comBuff + 2);
		AdjustTime.Week =  CalcWeek(IEC62056[com]->comBuff);
        DisplayAlarm(DIS_DATA_NO,TRUE);
    } 
	else if(OBISType ==OBIS_JCSetData) //集抄写日期时间
	{	//(2014-07-28,10:08:56,20)  //日期,时间,修正秒
       if((IEC62056[com]->PasswordLevel&(Level1|Level2|Level3)) == 0)
            return 0;  
        memcpy(IEC62056[com]->comBuff, Buff+DataTemp1, DataTemp2);
        memset((uint8 *)&AdjustTime, 0, sizeof(AdjustTime));
        ASCIIToData(IEC62056[com]->comBuff, IEC62056[com]->comBuff, DataTemp2, HexType); //数据转换
        AdjustTime.HighByteYear = (*(IEC62056[com]->comBuff) + 2000)/0x100; 
        AdjustTime.LowByteYear = (*(IEC62056[com]->comBuff) + 2000)%0x100;
        AdjustTime.Month =  *(IEC62056[com]->comBuff + 1); 
        AdjustTime.Day =  *(IEC62056[com]->comBuff + 2);
		AdjustTime.Week =  CalcWeek(IEC62056[com]->comBuff); //3
		AdjustTime.Hour =  *(IEC62056[com]->comBuff+ 3); 
		AdjustTime.Minute =  *(IEC62056[com]->comBuff + 4); 
		AdjustTime.Second =  *(IEC62056[com]->comBuff + 5); 
                
		uint32 Seconds;			
             
		Seconds = DateTimeToSeconds(&AdjustTime) + *(IEC62056[com]->comBuff + 6);	//下一天
		SecondsToDateTime(&AdjustTime, Seconds);
		
		if((IEC62056[com]->Flag.byte) & BroadcastAddress) 
		{
			Result=API_SetDataValue(ClockBroadcast_IDCode,(uint8 *)&AdjustTime,sizeof(AdjustTime));	 //广播设时间
		}
		else
		{
			Result=API_SetDataValue(Clock_IDCode,(uint8 *)&AdjustTime,sizeof(AdjustTime));		//普通校时	
		}
		if(Result !=DA_SUCCESS)
		{
		   return 0; 
		}
        DisplayAlarm(DIS_TIME_NO,TRUE);    //显示时间
		IEC62056[com]->PasswordLevel = 0;
				
	}
    else if(OBISType == OBIS_GeographicPos) //设置地理信息
    {
        if((IEC62056[com]->PasswordLevel&(Level1|Level2|Level3)) == 0)
        {   
            return 0;  
        }
        Result=API_SetDataValue(CurrentGeographicPos_IDCode,Buff+DataTemp1, DataTemp2);      
        if(Result !=DA_SUCCESS)
        {
           return 0; 
        }
        IEC62056[com]->PasswordLevel = 0;
    }  
    else if(OBISType == OBIS_EngryDot)      //电能小数位
    {
        uint8 TempBuff[4];
        if((IEC62056[com]->PasswordLevel&(Level1|Level2|Level3)) == 0)
            return 0;                
        memset(TempBuff, 0, sizeof(TempBuff));
        if(DataTemp2!=0x01)
            return 0;
        memcpy(IEC62056[com]->comBuff, Buff+DataTemp1, DataTemp2);
        TempBuff[0] = *(IEC62056[com]->comBuff) - '0';
        API_SetDataValue(EngPoint_IDCode,TempBuff,1);
        IEC62056[com]->PasswordLevel = 0;
    }   
    else if(OBISType == OBIS_DisplayTime)   //轮显时间
    {
        uint8 DataLength = 0;
        uint8 TempBuff[4];
        if((IEC62056[com]->PasswordLevel&(Level1|Level2|Level3)) == 0)
            return 0;                
        memset((uint8 *)TempBuff, 0, sizeof(TempBuff));
        memcpy(TempBuff, Buff+DataTemp1, DataTemp2);
        while(TempBuff[DataLength]!= '*')
        {
            DataLength++;    
        }
        if((DataLength!=0x01)&&(DataLength!=0x02))
            return 0;
        if(DataLength==0x01)
            TempBuff[0] = TempBuff[0] - '0';
        else if(DataLength==0x02)
        {
            TempBuff[0] = (TempBuff[0] - '0')*10 + (TempBuff[1] - '0');  
            if((TempBuff[0]>60)||(TempBuff[0]<3))
                return 0;
        }
        memset((uint8 *)TempBuff+1, 0, 3);
        API_SetDataValue(DisplayTime_IDCode,TempBuff,4);
        IEC62056[com]->PasswordLevel = 0;
    }        
    else if(OBISType == OBIS_SetPassWord)    //设置密码
    {
        if((IEC62056[com]->PasswordLevel&(Level1|Level2|Level3)) == 0)
            return 0;                
        memcpy(IEC62056[com]->comBuff, Buff+DataTemp1, DataTemp2);
        DataLength = ASCIIToData(Buff, IEC62056[com]->comBuff, DataTemp2, ArrayType);
        if(DataLength != KEY_LEN)
            return 0;
        if(IEC62056[com]->PasswordLevel == Level1)  
        { 
            API_SetDataValue(PassWord1_IDCode,Buff,KEY_LEN);
            IEC62056[com]->PasswordLevel = 0;
        }
        else if(IEC62056[com]->PasswordLevel == Level2)
        {
            API_SetDataValue(PassWord2_IDCode,Buff,KEY_LEN);
            IEC62056[com]->PasswordLevel = 0;
        }
        else if(IEC62056[com]->PasswordLevel == Level3)
        {
            API_SetDataValue(PassWord3_IDCode,Buff,KEY_LEN);
            IEC62056[com]->PasswordLevel = 0;
        }        
    }
	else
        return 0;
    return 1;
}

/*****************************************************************************************
*   Action:  密码验证
*   Input:   OBIS码，串口名
*   Output:  1表示设置成功，0表示失败
*****************************************************************************************/
uint8 PasswordConfirm(uint8 *Temp, uint8 Level)
{
    uint8 i;
    uint8 PassBuff[KEY_LEN];
	uint8 Passkey[16],PassExpKey[176]; 
    uint32 IDCode;
    if(*(Temp+4) != '(')
        return 0;
    i = GetDataLength(Temp+5, ')');
    if(Level !=Level4)
	{	
		if(i != KEY_LEN)        //8个字节
	    return 0;
    }
	else
	{
		if(i != PassAESLength)  //32个字节
	    return 0;	
	}
    
    if(Level == Level4)
    {
        /*通讯帧 16个字节ASCII明文+32个ASCII字节密文 */
		/*上位机是把16个字节ASCII明文加密成16个字节的HEX密文,所以通讯帧里16个字节密文需要转换成32字节*/
        /*16个字节明文加密后与密文比较看是否一致  */
        memcpy(Passkey,AES128KEY,16);
        AES_Expandkey(Passkey,PassExpKey);
        AES_Encrypt(Temp+5,PassExpKey,Passkey);   //Passkey临时存放明文加密后的文件
        ASCIIToHex(PassExpKey,(Temp+5+16),32);
        for(i=0;i<16;i++)
        {
            if(Passkey[i] !=PassExpKey[i])
                return 0;
        }                                      
    }
    else
    {
        if(Level == Level1)
        {
            IDCode = PassWord1_IDCode;    
        }
        else if(Level == Level2)
        {
            IDCode = PassWord2_IDCode;
        }
        else if(Level == Level3)
        {
           IDCode = PassWord3_IDCode;  
        }    
        API_GetDataValue(IDCode,PassBuff);
         
        for(i=0; i<KEY_LEN; i++)
        {
            if(PassBuff[i] != (*(Temp+5+i)-'0'))
                return 0;
        }
    }
    return 1;
}

/*******************************************************************************
* 函数名称: 
* 输入参数:  read Flash address
* 输出参数:  
* --返回值:  read appoint location of data
* 函数功能:  read IAP flag
*******************************************************************************/
uint16_t MFlash_Read16bit(uint16_t* pReadAddr)
{    
    uint8_t  *Addr;
    uint16_t  ReadData;  
    Addr = (uint8_t *)pReadAddr;
    
    if ((Addr>=(uint8_t *)FLASH_STR_ADDR)&&(Addr<=(uint8_t *)(FLASH_END_ADDR-2)))
    {
        if (((uint32_t)Addr%2) == 1)
        {
            Addr--;
            ReadData = (uint16_t)(Flash_Read(Addr));
        }
        else
        {
            ReadData = (uint16_t)(Flash_Read(Addr));
        } 
        return ReadData;
    }

    return 0u;
}


/*****************************************************************************************
*   Action: 清数据
*   Input: OBIS、缓存、串口名 
*   Output:  
*****************************************************************************************/
uint8 MeterControlCMD(uint8 *OBIS,uint8 *buff,COM_TYPEDEF com)
{
    uint8 OBISType;
    uint16 updata_flag; 
	
    OBISType = GetCMDOBISType(OBIS);      
    if((OBISType == OBIS_STARUPGRADE)&&(IEC62056[com]->PasswordLevel==Level4))
	{	      
         EnergyPowerOffProcess(); //升级之前先存一下电量
         //此处增加跳转升级
		 //write updata OK flag = 0, 0x0001FF84 flag write only one time !!!
         MFlash_Write((uint16_t*) 0x0001FF84,(uint32_t) 0x00000000); /* 必须强制转换 写 32bit */
         updata_flag = MFlash_Read16bit ((uint16_t*)0x0001FF84);  

		 if (0 == updata_flag)
		 {
             *(volatile unsigned long*)(0xE000ED0C)=0x05FA0004;
			 //NVIC_SystemReset();  /* system reset, also soft reset */
			 while (1);  
             //return 1;
		 }
		 else
		 {
				
			 return 0;
		 }
		
	    //return 1;
	}
    return 0;
}

/*****************************************************************************************
*   Action:  编程模式下处理
*   Input:   数据缓存，串口名
*   Output:  
*****************************************************************************************/
void ProgramMode1Process(uint8 *Buff, COM_TYPEDEF com)
{
    uint8 OBIS[4];
    memset(OBIS, 0, sizeof(OBIS));
    if(*Buff != soh)
    {
        DLMSACK(nak, com);
        return;
    }
	/********************************************/
	if((*Buff == soh) && (*(Buff+1)=='B')) //通讯端口回到初始状态
    {
        if((*(Buff+2)=='0') && (*(Buff+3)==etx) && (*(Buff+4)==0x71))
        {
		    IEC62056[com]->DLMSMode = StopMode;
            DLMSACK(ack, com);
            if(com == UART_485)
            {
				TaskReset(RS485DLMSStatusInit, 30, 0, 1); //初始化
			}           
            else if(com == UART_IR)
            {
				TaskReset(IRDLMSStatusInit, 30, 0, 1);
			} 
			else if(com == UART_MASH)
			{
                TaskReset(MESHDLMSStatusInit, 30, 0, 1);
			}
			else if(com==UART_VT)
		    {
	            TaskReset(VTDLMSStatusInit, 30, 0, 1);
		    }		
        }
        else
        {
            DLMSACK(nak, com);
        }
		return;
    }
	/********************************************/
    if(!CheckBCC(Buff, com)) //异或验证
    {
        DLMSACK(nak, com);
		return;
    }
	/********************************************/
    if(*(Buff+1) == 'R')
    {
        if(((*(Buff+2)=='1')||(*(Buff+2)=='2')) && (*(Buff+3)==stx))//读数据
        {			
            if(GetOBIS(OBIS, Buff))	            //OBIS初值为4个0数组 从BUFF中读出OBIS号放入OBIS中
            {
                if(!OBISRead(OBIS, Buff, com, NormalRead))
                {
                    DLMSACK(nak,com);
					BitSet((uint8 *)&IEC62056[com]->Flag, SendFlag);
                    return;
                }
                DataToBuff(etx, com);
			    BitSet((uint8 *)&IEC62056[com]->Flag, SendFlag);
                CalcBCC(IEC62056[com]->comBuff+1, IEC62056[com]->nTx-1);  //计算READOBIS的BCC校验和  
                IEC62056[com]->nTx++;            	
			}
        }
        if((*(Buff+2)=='5') && (*(Buff+3)==stx))
        {
            if(GetOBIS(OBIS, Buff))	            //OBIS初值为4个0数组 从BUFF中读出OBIS号放入OBIS中
            {
                if(!OBISRead(OBIS, Buff, com, NormalRead))  //读负荷曲线
                {
                    DLMSACK(nak,com);
                    return;
                }
                DLMSACK(nak, com);
			}
        }
    }
    else if(*(Buff+1) == 'W')//写数据
    {
        if(((*(Buff+2)=='1')||(*(Buff+2)=='2'))&& (*(Buff+3)==stx))
        {
            if(GetOBIS(OBIS, Buff))
            {
                if(WriteData(OBIS, Buff, com))
                {
                    DLMSACK(ack, com);
                }
                else
                {
                    DLMSACK(nak, com);
                }
                return ;
            }
        }
    }
    else if((*(Buff+1)=='P') && (*(Buff+3)==stx))    //密码验证
    {   
        if(*(Buff+2) == '1')						 //1级密码验证
        {
            if(PasswordConfirm(Buff, Level1))
            {
                IEC62056[com]->PasswordLevel = Level1;
                DLMSACK(ack, com);
            }
            else
                DLMSACK(nak, com);
		}
        else if(*(Buff+2) == '2')					 //2级密码验证
        {
            if(PasswordConfirm(Buff, Level2))
            {
                IEC62056[com]->PasswordLevel = Level2;
                DLMSACK(ack, com);
            }
            else
                DLMSACK(nak, com);
        }
        else if(*(Buff+2) == '3')					 //2级密码验证
        {
            if(PasswordConfirm(Buff, Level3))
            {
                IEC62056[com]->PasswordLevel = Level3;
                DLMSACK(ack, com);
            }
            else
                DLMSACK(nak, com);
        }
        else if(*(Buff+2) == '4')					 //2级密码验证
        {
            if(PasswordConfirm(Buff, Level4))
            {
                IEC62056[com]->PasswordLevel = Level4;
                DLMSACK(ack, com);
            }
            else
                DLMSACK(nak, com);
        }        
        else
            DLMSACK(nak, com);    
    }
    else if((*(Buff+1)=='E') && (*(Buff+3)==stx) && (*(Buff+2)=='2'))//控制命令
    {
        if(GetOBIS(OBIS, Buff))
        {
            if(MeterControlCMD(OBIS,Buff,com)) //删除数据
            {
                DLMSACK(ack, com);
            }
            else
            {
                DLMSACK(nak, com);
            }
        }
        else
        {
            DLMSACK(nak, com);
        }
    }
}

/*****************************************************************************************
*   Action:  发送数据块
*   Input:   串口名
*   Output:  
*****************************************************************************************/
void BlockDataSend(COM_TYPEDEF com)
{										  
	uint8 TempBuff[MAXBUFFER];
	uint8 OBIS[4];
    uint8 *Uart;
	IEC62056_Type *IEC62056Block[4];
    
	GetCommRxPoint(com,&Uart);
    IEC62056Block[com] = (IEC62056_Type *)Uart;
    
    if(IEC62056Block[com]->BlockDataCount==0)
    {
        return;
    }
    if(Comm[com].ucStt == IDLE)
    {
        IEC62056Block[com]->nTx = 0;         
        memset(OBIS, 0, 4);
        memset(TempBuff, 0, MAXBUFFER);
        OBISRead(OBIS, TempBuff, com, BlockRead);
        if(com==UART_VT)
        {
			//VirtualUartTx(com,(uint8 *)&IEC62056[com]->comBuff, IEC62056[com]->nTx);
		}
		else
		{
			LDRV_UsartSend(com,(uint8 *)&IEC62056[com]->comBuff, IEC62056[com]->nTx);    //启动发送数据  
		}
    }
}

/*****************************************************************************************
*   Action:  接收数据处理
*   Input:   串口名
*   Output:  1表示有接收到收据，0表示没接收到数据
*****************************************************************************************/
uint8 IEC62056ReceiveProcess(COM_TYPEDEF com)
{
	uint8 *pBuf;
	uint8 tempBuff[MAXBUFFER];         //临时数据缓存
	uint8 OBIS[4];
    memset(OBIS, 0, 4);    
	uint16 len = 0;    
    len = ProtocolDataRec(com, &pBuf); //中断中接收数据放入pBuf
	if(len != 0)
	{
        if(com==UART_IR)
	    {
		    TaskReset(IRDLMSStatusInit, DLMSDelayTime, 0, 1);//超时处理 120S 只会处理一次
	    }
        else if(com==UART_485)
	    {
	        TaskReset(RS485DLMSStatusInit, DLMSDelayTime, 0, 1);
        }
		else if(com==UART_MASH)
		{
			TaskReset(MESHDLMSStatusInit, DLMSDelayTime, 0, 1);
		}
		else if((com==UART_VT))
		{
			TaskReset(VTDLMSStatusInit, DLMSDelayTime, 0, 1);   //虚拟串口不切换波特率，只清除Buffer;
		}
        IEC62056[com] = (IEC62056_Type *)(pBuf);                //地址传递，IEC62056[com]与缓存commRxBffer[com]数据一样
        IEC62056[com]->rxPointer  = len;                
		memset((uint8 *)tempBuff, 0, sizeof(tempBuff));
		memcpy(tempBuff, IEC62056[com]->comBuff, IEC62056[com]->rxPointer);
		memset(IEC62056[com]->comBuff, 0, sizeof(IEC62056[com]->comBuff));
        IEC62056[com]->nTx = 0;
		if((*tempBuff=='/') && (*(tempBuff+1)=='?'))
		{
	        if(AddressCompare(tempBuff+2, '!', com))
            {
                 if((*(tempBuff+IEC62056[com]->rxPointer-2)==er) && (*(tempBuff+IEC62056[com]->rxPointer-1)==cr))
                 {
                     memcpy(IEC62056[com]->comBuff, IdentificationCode, sizeof(IdentificationCode));
                     IEC62056[com]->nTx += sizeof(IdentificationCode);
                     BitSet((uint8 *)&IEC62056[com]->Flag.byte, AddressCheckFlag);
                     BitSet((uint8 *)&IEC62056[com]->Flag.byte, SendFlag);
                 }
            }
		}   
		else if(IEC62056[com]->Flag.byte&AddressCheckFlag)
    	{
            if((*tempBuff==ack) && (*(tempBuff+1)=='0') && (*(tempBuff+4)==er) && (*(tempBuff+5)==cr))
        	{
            	if(*(tempBuff+2) == IdentificationCode[4])
            	{
				   if(com==UART_VT)
				   {
						//虚拟串口不做处理
				   }
				   else
				   {
						if(com==UART_IR)
	               		{
		                	MFS_UARTConfigBaudrate(MFS_Ch4, 4800);//超时处理 120S 只会处理一次
	               		}
                   		else if(com==UART_485)
	               		{
	                    	MFS_UARTConfigBaudrate(MFS_Ch7, 4800);
                    	}
		                else if(com==UART_MASH)
		                {
		                    MFS_UARTConfigBaudrate(MFS_Ch5, 4800);
		                }
		                PublicDelayMs(50);
					}			   
            	}
            	if(*(tempBuff+3) == '0')    
            	{
                    IEC62056[com]->DLMSMode = DataMode;//数据模式
                    IEC62056[com]->BlockDataCount = 0;
                    IEC62056[com]->BlockDataStatus = IDLE;
                    IEC62056[com]->BlockDataBcc = 0;
                    OBISRead(OBIS, tempBuff, com, BlockRead);
                    BitSet((uint8 *)&IEC62056[com]->Flag, SendFlag);
            	}
            	else if(*(tempBuff+3) == '1')
            	{
                	IEC62056[com]->DLMSMode = ProgramMode1;
                	memcpy(IEC62056[com]->comBuff, ProgramAnswerCode, sizeof(ProgramAnswerCode));
                	CalcBCC((uint8 *)&IEC62056[com]->comBuff+1, sizeof(ProgramAnswerCode)-1);
                	IEC62056[com]->nTx = sizeof(ProgramAnswerCode) + 1;
                	BitSet((uint8 *)&IEC62056[com]->Flag, SendFlag);
            	}
        	}
       	 	else if(IEC62056[com]->DLMSMode == ProgramMode1)
        	{
            	ProgramMode1Process(tempBuff, com);
        	}
    	}

		IEC62056[com]->rxPointer = 0;    //清接收指针
    	if(IEC62056[com]->Flag.byte&SendFlag)
    	{	
    		if(com==UART_VT)
    		{
				//VirtualUartTx(com,(uint8 *)pBuf, IEC62056[com]->nTx);
			}
			else
			{
				LDRV_UsartSend(com,(uint8 *)pBuf, IEC62056[com]->nTx);    //发送数据  
			}	     
        	return 1;
    	}
	}
    return 0;
}




