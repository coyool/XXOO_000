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
static u8 RF_checkMeterAddress(void);
static void RF_getMeterAddress(void);

static u8 CalcAddSum(u8 *StartAddr, u8 Length);
static void DataBatProcess(u8 *StartAddr, u8 SubData, u8 Length, u8 Flag);
static u8 RF_checkSWH(void);
static void RF_updataSWH(void);



/*** static variable declarations ***/
static u8 RF_macSendBuff[64] = {0};
static u8 RF_linkBuff[64] = {0};




/*** extern variable declarations ***/
u8 RF_availableSendFlag = 0u;
u8 ReadMeterSerialNumber[8] = {0};
u8 RF_macRecvBuff[15] = {0};



/*******************************************************************************
* Description : 对比 RF接收数据帧中表地址 与 本机表地址 是否一致 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static u8 RF_checkMeterAddress(void)
{
    u16 temp = 0u;
    int temp1 = 0; 
    u8 meterAddress[6] = {0};
    u8 return_val = 0u;
    
    temp = API_GetDataValue(CurrentADDR_IDCode, meterAddress);
    if (6u == temp)
    {
        // meter read ok
        temp1 = memcmp(meterAddress+2, RF_macRecvBuff+1, 4u); // 00 00 00 12 34 56  
        if (0 == temp1)
        {
            return_val = 0u;
        }
        else
        {
            return_val = 1u;
        }    
            
    }
    else
    {
        // meter read error
        return_val = 1u;
    }    

    return return_val;
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static void RF_getMeterAddress(void)
{
    u16 temp = 0u;
//    u8 return_val = 0u;
    u8 meterAddress[6] = {0};
    
    temp = API_GetDataValue(CurrentADDR_IDCode, meterAddress);
    if (6u == temp)
    {
        memcpy(RF_macSendBuff+1, meterAddress+2, 4u); // 00 00 00 12 34 56  
//        return_val = 0u;
    }
    else
    {
        // meter read error
//        return_val = 1u;
    }    

//    return return_val;
}

/*******************************************************************************
* Description : 计算校验和
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static u8 CalcAddSum(u8 *StartAddr, u8 Length)
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
static void DataBatProcess(u8 *StartAddr, u8 SubData, u8 Length, u8 Flag)
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
* Description : 0x15 (21 十进制) --->  0x21 (BCD) 电能数据转换过程
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 0 -- invalid
*               len -- valid
*******************************************************************************/
static void RF_getMeterData(u8 DI)    //Command
{
	uint16 Result = 0u;
    
//    if ( MeterInfoIsStateBitSet(STATE_READ_ENERGY_ERR)
//		 || MeterInfoIsStateBitSet(STATE_SAVE_ENERGY_ERR))  //save and read  Error
//    {
//        return;
//    }
	
    RF_availableSendFlag = 1u;   //RF 发送有效标志默认为发送
    
    Result = RFGetData(DI, RF_linkBuff); //有功 
    if (0u == Result)   
    {
        RF_availableSendFlag = 0u;
    }  
    else
    {
        memcpy(RF_macSendBuff+6, RF_linkBuff, Result);
        switch (DI)
        {
            case 0xa3:  //电量
                RF_macSendBuff[5] = 0x53;
                break;
            case 0xa4:
                RF_macSendBuff[5] = 0x54;
                break;
            case 0xa5:
                RF_updataSWH(); 
                RF_macSendBuff[5] = 0x55;
                /*
                * 此次更新事务号的时候,接收到掌机号,需要重新回传给掌机模块,掌机模块可以判断接收
                */
                RF_macSendBuff[11] = RF_macRecvBuff[10]&0x3f; 
                break;
            case 0x87:
                RF_macSendBuff[5] = 0x77;
                break;
            case 0x86:
                RF_macSendBuff[5] = 076;
                break;
            case 0xa9:  //电流电压L1
                RF_macSendBuff[5] = 0x59;       
                break;
            case 0x81:  //电流电压L2
                RF_macSendBuff[5] = 0x31;
                break;	
            case 0x82:  //电流电压L3
                RF_macSendBuff[5] = 0x32;
                break;		
            case 0x83:  //功率
                RF_macSendBuff[5] = 0x33;
                break;
            case 0x8a:  //窃电次数
                RF_macSendBuff[5] = 0x7a;
                break;
            case 0x8b:  //功因
                RF_macSendBuff[5] = 0x7b;
                break;       
            //....      
            default:
                RF_availableSendFlag = 0u;
                break;      
        }//end switch(DI)  
    }//end if (0u == Result)       
    
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
//void RF_setMeterData(void)
//{
//    
//}


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
    u8 checkMeterAddFlag = 0u; 
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
            if (!RF_checkSWH())  
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
                RF_getMeterData(0xa4);
                //..
                RF_macSendBuff[11] = RF_macRecvBuff[10]&0x3f; //此次更新事务号的时候,接收到掌机号,需要重新回传给掌机模块,掌机模块可以判断接收。
            }
            if (1u == RF_availableSendFlag)
            {
                RF_macSendBuff[0] = 0x66;
                RF_getMeterAddress();
                DataBatProcess(RF_macSendBuff+1, 0x33, 4, AddFlag);
                //DI 5
                DataBatProcess(RF_macSendBuff+6, 0x33, 6, AddFlag);
                RF_macSendBuff[12] = CalcAddSum(RF_macSendBuff, 12);
                
                CC1101_Send(RF_macSendBuff, RF_shortPayloadSize); // 13 byte
            }
            
            return;
        }
        else
        {
        }    
    }//end if 广播处理
     
    /******************** 验证RX_buff中的表地址 与 本机表地址是否一致 ********************/
    checkMeterAddFlag = RF_checkMeterAddress();
    if (0u != checkMeterAddFlag)
    {
        checkMeterAddFlag = 0u;
        return;
    }    

    /************************** 除广播之外的 DI码 处理 **********************************/
	RF_getMeterData(RF_DI);
    //..  set data 
    
    
    if (1u == RF_availableSendFlag)  
    {
        //处理协议通用 bytes     
        /* frame head 0 */
        RF_macSendBuff[0] = 0x66;  
        /* address 1-4 */
        memcpy(RF_macSendBuff+1, RF_macRecvBuff+1, 4u);
        DataBatProcess(RF_macSendBuff+1, 0x33, 4, AddFlag);
        /* DI 5 */
        //..
        /* data fields 6-11 10 meter status 11 null */
        DataBatProcess(RF_macSendBuff+6, 0x33, 6, AddFlag);
        /* check sum 12 */
        RF_macSendBuff[12] = CalcAddSum(RF_macSendBuff, 12);
        
        CC1101_Send(RF_macSendBuff, RF_shortPayloadSize); // 13 byte
    }
    else 
    {
    }    
}
