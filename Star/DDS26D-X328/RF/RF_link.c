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
*           uint8 RFGetDate(uint8 RFDI,uint8 *pBuf)
***********************************************************************/

#include "Include.h"


/************************* 内部函数声明 *******************************/
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



/***********************************************************/
uint8  ReadMeterSerialNumber[8];




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
