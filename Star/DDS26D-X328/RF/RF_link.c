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
* 完成日期：2015-1-11
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include "Include.h"

/*** static function prototype declarations ***/
void RFReceiveMessage(void);
void DataBatProcess(u8 *StartAddr, u8 SubData, u8 Length, u8 Flag);
void RF_getEnergy(u8 Command);
void RFSendACData(u8 DI);
void RFSendStealEnergyCount(u8 DI);
void RFSendPowerFactor(u8 DI);
void RFSendPower(u8 DI);
void Set_meter_address(u8 DI);
u8 CalcAddSum(u8 *StartAddr, u8 Length);
void HEX_TO_BCD(uint32 Data, u8 DataByte, u8 *Array, u8 Type);
u8 CheckSWH(void);
void UpdataSWH(void);
int over_deg_check(u8 *value, u8 length);



/*** static variable declarations ***/
static u8 RF_macRecvBuff[15] = {0};
static u8 RF_macSendBuff[64] = {0};
static u8 RF_linkBuff[64] = {0};



/*** extern variable declarations ***/






/*******************************************************************************
* Description : 10进制转BCD码,低字节在前，高字节在后  
*               Buff1大返回1；Buf2大返回－1；相等返回0
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void HEX_TO_BCD(uint32 Data, u8 DataByte, u8 *Array, u8 Type)
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

/*******************************************************************************
* Description : 十进制数检查
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
int over_deg_check(u8 *value, u8 length)
{
    int i;
    uint8_t digit;

    for (i=0;  i <length;  i++)
    {
      digit = *value++;
      if (((digit & 0x0F) < 0x0a) & (((digit & 0xf0)>>4)< 0x0a))
      {
         return TRUE;
      }          
    }

    return FALSE;  
}

/*******************************************************************************
* Description : 计算校验和
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
u8 CalcAddSum(u8 *StartAddr, u8 Length)
{
    u8 Sum = 0u;
    u8 i = 0u;
    
    for (i=0; i<Length; i++)
    {
        Sum += *(StartAddr+i);
    }
    
    return Sum;
}

/*******************************************************************************
* Description : meter address fields and data fields + 0x33 or - 0x33
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void DataBatProcess(u8 *StartAddr, u8 SubData, u8 Length, u8 Flag)
{
    u8 i;
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

/*******************************************************************************
* Description : 0x15 (21 十进制) --->  0x21 (BCD) 电能数据转换过程
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void RF_getEnergy(u8 Command)    //Command
{
	uint32 Energy;
	uint16 Status;
	u8  meter_num[6];
	uint16 Result;
    
    if ( MeterInfoIsStateBitSet(STATE_READ_ENERGY_ERR)
		 || MeterInfoIsStateBitSet(STATE_SAVE_ENERGY_ERR))  //save and read  Error
    {
        return;
    }
	
    //MeterInfoParaGet(0x01700002, (u8*)&Status);	  //取电表状态字
    
    if ((0x53 == Command) || (0x54 == Command) || (0x55 == Command))
    {    
        //EnergyParaGet(0x03140002, (u8*)&Energy);		  //有功
        RFGetDate(Command, RF_macRecvBuff);       //有功 
    }
    else if (0x77 == Command)
    {
         //Result = EnergyParaGet(0x03160002, (u8*)&Energy);		   //无功
         Result == RFGetDate(Command, RF_macRecvBuff);         //无功
         if (0x8000 == Result)
         {
             //RF_availableSendFlag = 0;
             RF_availableSendFlag = 0;
         }  
    }
	else if (0x76 == Command)    
    {    
        // Result = EnergyParaGet(0x03100002, (u8*)&Energy);		   //视在
         Result == RFGetDate(Command, RF_macRecvBuff);         //视在
         if (0x8000 == Result)
         {
             //RF_availableSendFlag = 0;
             RF_availableSendFlag = 0;
         }    
    }    
	//MeterInfoParaGet(0x01010002, (u8*)&meter_num);		 //取表地址6 byte

//    memcpy(RF_macSendBuff+1, meter_num+2, 4); //66 + address
//    RF_macSendBuff[5] = Command; //DI
//    HEX_TO_BCD(Energy, 4, RF_macSendBuff+6, BackOrder); //十进制 转换为 BCD码
//    RF_macSendBuff[10] = Status;      //增加电表状态字 只要低8 位， 高8位丢弃
    
    RF_macRecvBuff[5] = 0x00;
   

//    RF_macSendBuff[0] = 0x66;
//    RF_macSendBuff[5] = Command;    //DI
//    RF_macSendBuff[6] = 0x55;
//    RF_macSendBuff[7] = 0x66;
//    RF_macSendBuff[8] = 0x77;
//    RF_macSendBuff[9] = 0x88;
//    RF_macSendBuff[10] = 0xFF;
}

/*******************************************************************************
* Description :   电压电流
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void RF_SendACData(u8 DI)
{
	uint32 Cur;
	uint16 Vol;
     
	//memcpy(RF_macSendBuff, RF_macRecvBuff, 5);
	switch (DI)
	{
        case 0x59:   
//			EnergyParaGet(0x03800902, (u8*)&Vol); //L1 电压
//			EnergyParaGet(0x03800802, (u8*)&Cur); //L1 电流
            RFGetDate(Command, RF_macRecvBuff);  
			break;
		case 0x31:
//			EnergyParaGet(0x03801202, (u8*)&Vol); //L2 电压
//			EnergyParaGet(0x03801102, (u8*)&Cur); //L2 电流
            RFGetDate(Command, RF_macRecvBuff);
			break;
		case 0x32:
//			EnergyParaGet(0x03801B02, (u8*)&Vol); //L3 电压
//			EnergyParaGet(0x03801A02, (u8*)&Cur); //L3 电流
            RFGetDate(Command, RF_macRecvBuff);
			break;
		default:
			RF_availableSendFlag = 0;
			break;
	}//end switch (DI)

//	RF_macSendBuff[5] = DI;  //DI
//    RF_macSendBuff[6] = Vol & 0xff;;
//    RF_macSendBuff[7] = Vol >> 8;        //
//
//	Cur = Cur/10;
//    RF_macSendBuff[8] = Cur & 0xff;
//    RF_macSendBuff[9] = Cur >> 8; 
//    RF_macSendBuff[10] = Cur >> 16;	

#ifdef  TEST	
    RF_macSendBuff[5] = 0x59;  //DI
    RF_macSendBuff[6] = 0x00;
    RF_macSendBuff[7] = 0x22;
    RF_macSendBuff[8] = 0x00;
    RF_macSendBuff[9] = 0x05; 
    RF_macSendBuff[10] = 0x00;
#endif	
}

/*******************************************************************************
* Description : 反馈窃电次数 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void RF_SendStealEnergyCount(u8 DI)
{
    uint16 Data;
	
	//memcpy(RF_macSendBuff, RF_macRecvBuff, 5); //66 + address
    RFGetDate(Command, RF_macRecvBuff);
//	RF_macSendBuff[5] = DI;  //0x7a
//    RF_macSendBuff[6] = Data & 0xff;
//    RF_macSendBuff[7] = Data>>8;
	
#ifdef	 TEST
    RF_macSendBuff[5] = 0x7a;  //DI
    RF_macSendBuff[6] = 0x99;
    RF_macSendBuff[7] = 0x99;
#endif   
}

/*******************************************************************************
* Description : 反馈功率因数 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void RF_SendPowerFactor(u8 DI)
{
	uint16 Factor;
    
    //memcpy(RF_macSendBuff, RF_macRecvBuff, 5); //66 + address
    //EnergyParaGet(0x03800002, (u8*)&Factor);	
    RFGetDate(Command, RF_macRecvBuff);
//    RF_macSendBuff[5] = DI;  //0x7b
//    //RF_macSendBuff[6] = Factor/100;   // 待定语句 为了小数位能对应上  ybz
//    RF_macSendBuff[6] = Factor; 
    
#ifdef   TEST  
    RF_macSendBuff[5] = 0x7b;  //DI 
    RF_macSendBuff[6] = 100;
#endif	
}

/*******************************************************************************
* Description : 反馈当前功率
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void RF_SendPower(u8 DI)
{   
    uint32 Data;
   
//    memcpy(RF_macSendBuff, RF_macRecvBuff, 5); //66 + address	
//    EnergyParaGet(0x03810002, (u8*)&Data);
    RFGetDate(Command, RF_macRecvBuff);
    
//    RF_macSendBuff[5] = DI;  // 0x33 
//    RF_macSendBuff[6] = Data&0xff;
//    RF_macSendBuff[7] = (Data>>8)&0xff;
//    RF_macSendBuff[8] = (Data>>16)&0xff;    
    
//#define TEST_Power     
#ifdef  TEST_Power
    RF_macSendBuff[6] = 0x92;
    RF_macSendBuff[7] = 0x0B;
    RF_macSendBuff[8] = 0x05;       //1.10214 * 3 = 3.30642 kw/h (220V  5A)
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
/*******************************************************************************
* Description : set meter address
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void RF_setMeterAddress(u8 DI)
{
    u8 i;
	uint16 Status;
	u8 temp;
	u8 buff[6];
	
    if (!IO_ReadGPIOPin(IO_PORT_PRO_SWI,IO_PIN_PRO_SWI))   /*防止上位机没有设置权限下设表地址提示设表地址成功,外部主控部分也有相应的处理 */
	{
        if ((RF_macRecvBuff[6] != 0x99) || (RF_macRecvBuff[7] != 0x99)
            || (RF_macRecvBuff[8] != 0x99) || (RF_macRecvBuff[9] != 0x99)) //设置的表地址不能为通配符
        {
             temp = over_deg_check(&RF_macRecvBuff[6], 4);
             if (temp)    // 十进制数检查
             {
                 MeterInfoParaGet(0x01010002, buff); //取表地址6 byte
                 memcpy(&buff[2], &RF_macRecvBuff[6], 4);
                 MeterInfoParaSet(0x01010002, &buff[0], 6);   //新地址替换旧地址
 				 DisplayAlarm(DIS_METER_NO, TRUE); //显示 新 表地址	

				 memset(buff, 0, 6);  //清零
				 MeterInfoParaGet(0x01010002, buff); //取表地址6 byte
                 for (i=0; i<5; i++)  
                 {
                     RF_macSendBuff[i+1] = buff[i+2];// 填入更新后表地址     ??  检测地址存放顺序  
                 }
                 RF_macSendBuff[5] = DI; // RF_macSendBuff [5]   0x56   DI
                 MeterInfoParaGet(0x01700002, (u8*)&Status);	  //取电表状态字
                 RF_macSendBuff[10] = Status;      //增加电表状态字 只要低8 位， 高8位丢弃

	        }//end if  设表地址操作 

		}//end  if  不是通配符
	}//end if

}

/*******************************************************************************
* Description : 校验事务号 用于广播  本机存储的事务号 和 RX接收的事务号对比 不相同则响应
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : Transaction 事务
*******************************************************************************/
u8 RF_checkSWH(void) 
{
    u8 i;
    for (i=0; i<8; i++)
    {
        if (ReadMeterSerialNumber[i] == RF_DATA_COLLECT)      //  RF_macRecvBuff[10]
           return 0;
    }
    return 1;
}

/*******************************************************************************
* Description : 更新事务号 
*               用于广播  Huu接收到 15号表的电能数据后，下发一帧 15表地址的事务号更新命令
*               如果10号表 huu没有接收到电能数据，则不下发事务号更新命令。
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void RF_updataSWH(void)
{
    u8 i;
    u8 Temp[9];
    
    if (RF_MASK == 0)
    {    
        return;
    }    
    
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

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void RF_linkLayerProtocolStack(void)
{
    u8 checkSumFlag = 0u;
	u8 address[6] = {0};     
    u8 i;
    u8 RandomAddress;

    checkSumFlag = CalcAddSum(RF_macRecvBuff, 12); //计算 CS 校验和
    if (checkSumFlag != RF_macRecvBuff[12])
    {
        return;
    }

    // init 
    RF_availableSendFlag = 0u;
    memset(RF_macSendBuff, 0, sizeof(RF_macSendBuff));
    memset(RF_linkBuff, 0, sizeof(RF_macRecvBuff));
    
    DataBatProcess(RF_macRecvBuff+1, 0x33, 4, SubFlag); //地址域  - 33
    DataBatProcess(RF_macRecvBuff+6, 0x33, 6, SubFlag); //数据域  - 33

    /**********************  广播命令处理  ********************/
    if ((RF_macRecvBuff[1]==0x99) && (RF_macRecvBuff[2]==0x99) 
        && (RF_macRecvBuff[3]==0x99) && (RF_macRecvBuff[4]==0x99))
    {
        if (RF_DI == 0xaf) //显示表地址
        {
            //DisplayAlarm(DIS_METER_NO, TRUE); 显示表地址
            return;
        }
        else if (RF_DI == 0xa4)  //广播抄表
        {
            if (!CheckSWH())  
            {
                return;
            }    
            else
            {
            }
            
            if (RF_R_SEND_NUMBER < SET_R_ADDR)  //  RF_macRecvBuff[11] = RF_R_SEND_NUMBER
            {
               RandomAddress = BT_RTGet16bitCount(BT_CH_6) & 0xff;  // 调用库随机函数   种子 = BT_CH_6；                
            }
            RandomAddress &= RF_MASK;    // RF_macRecvBuff[8]   
            if (RF_R_SEND_NUMBER >= RandomAddress)   // RF_macRecvBuff[11]  >=
            {
                RF_availableSendFlag = 1u;
                RF_getEnergy(0x54);
                //..
                RF_macSendBuff[11] = RF_macRecvBuff[10]&0x3f; //此次更新事务号的时候,接收到掌机号,需要重新回传给掌机模块,掌机模块可以判断接收。
            }
            if (1u == RF_availableSendFlag)
            {
                DataBatProcess(RF_macSendBuff+1, 0x33, 4, AddFlag);
                DataBatProcess(RF_macSendBuff+6, 0x33, 6, AddFlag);
                RF_macSendBuff[0] = 0x66;
                RF_macSendBuff[12] = CalcAddSum(RF_macSendBuff, 12);
                
                CC1101_Send(RF_macSendBuff, RF_shortPayloadSize); // 13 byte
            }
            
            return;
        }
    }//end if 广播处理
     
    /******************** 验证RX_buff中的表地址 与 本机表地址是否一致 ********************/
    
    //MeterInfoParaGet(0x01010002, (u8*) address); //取表地址6 byte
    for (i=1; i<5; i++) 
    {
		if (RF_macRecvBuff[i] != address[i+1])
        {
            return;
        }    
        else
        {
        }    
    }//接收到的表地址 与 本机的表地址相对应 是否一致

    /************************** 除广播之外的 DI码 处理 **********************************/
	RF_availableSendFlag = 1u;   //RF 发送有效标志
     
	switch (RF_DI) // RF_DI
	{
		case 0xa3:
			RF_getEnergy(0x53);
			break;
		case 0xa5:
			UpdataSWH();   //
			RF_getEnergy(0x55);  //
			RF_macSendBuff[11] = RF_macRecvBuff[10]&0x3f; //此次更新事务号的时候,接收到掌机号,需要重新回传给掌机模块,掌机模块可以判断接收。
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
			RF_getEnergy(0x77);
			break;
		case 0x86:  //视在电能
			RF_getEnergy(0x76);
			break;            
		default:
			RF_availableSendFlag = 0u;
			break;		
	}//end switch (RF_DI)
	
    if (1u == RF_availableSendFlag)  
    {
        //处理协议通用 bytes
        
        /* frame head 0 */
        RF_macSendBuff[0] = 0x66;  
        /* address 1-4 */
        DataBatProcess(RF_macSendBuff+1, 0x33, 4, AddFlag);
        /* DI 5 */
        //..
        /* data fields 6-11 10 meter status 11 null */
        //DataBatProcess(RF_macRecvBuff+6, 0x33, 6, AddFlag);
        /* check sum 12 */
        RF_macSendBuff[12] = CalcAddSum(RF_macRecvBuff, 12);
        
        CC1101_Send(RF_macSendBuff, RF_shortPayloadSize); // 13 byte
    }
    else 
    {
    }    
}
