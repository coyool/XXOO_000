/*******************************************************************
Copyright (c) 2013,深圳市思达仪表有限公司 
All rights reserved. 
 
文件名称：IEC62056-21.h 
摘要：IEC62056-21通讯协议
作者：chy
当前版本：1.0.0 -20140701

历史记录：
********************************************************************/

#ifndef _IEC62056_21_H
#define _IEC62056_21_H

/*---------------------------------------------------------------------------*/
/*外部常数和宏定义                                                           */
/*---------------------------------------------------------------------------*/

#define		MAXBUFFER			 128

#define soh                      0x01
#define stx                      0x02
#define etx                      0x03
#define eot                      0x04
#define enq                      0x05
#define ack                      0x06
#define cr                       0x0a
#define er                       0x0d
#define nak                      0x15

#define SendFlag                 0x02
#define AddressCheckFlag         0x04

#define DataMode                 1
#define ProgramMode1             2
#define ProgramMode2             3
#define StopMode                 4

#define PredictOverFlag          0x01
#define SendFlag                 0x02
#define AddressCheckFlag         0x04
#define LongProcessFlag          0x08
#define BlockDataOK              0x10
#define BroadcastAddress         0x20
//#define BlockRead                0x40

#define  NormalRead			     1			//正常抄读单帧
#define  BlockRead               2         //数据块抄读

//定义OBIS代码
#define     OBIS_ADDR                 0x01
#define     OBIS_Time                 0x02
#define     OBIS_Date                 0x03
#define     OBIS_Voltage              0x04
#define     OBIS_Ib                   0x05
#define     OBIS_Frequency            0x06
#define     OBIS_Imax                 0x07
#define     OBIS_PConst               0x08
#define     OBIS_SoftwareVer          0x09
#define     OBIS_HardwareVer          0x0a
#define     OBIS_Disp_Add			  0x0b
#define     OBIS_CurrentCurrent       0x0c
#define     OBIS_CurrentVoltage       0x0d
#define     OBIS_CurrentPowerFactor1  0x0e
#define     OBIS_CurrentPowerFactor   0x0f
#define     OBIS_CurrentPower         0x10
#define     OBIS_CurrentFrequency     0x11
#define     OBIS_CurrentVAPower       0x12
#define     OBIS_Bat1Voltage          0x13
#define     OBIS_Bat2Voltage          0x14
#define     OBIS_EventStatus          0x15
#define     OBIS_ErrorStatus          0x16
#define     OBIS_DisplayTime          0x17
#define     OBIS_EngryDot             0x18
#define     OBIS_PowerDot             0x19
#define     OBIS_OverCurrent          0x1a
#define     OBIS_OverVoltage          0x1b
#define     OBIS_UnderVoltage         0x1c
#define     OBIS_PassWord1            0x1d
#define     OBIS_PassWord2            0x1e
#define     OBIS_PassWord3            0x1f
#define     OBIS_SetPassWord          0x20
#define     OBIS_PowerDownCounts      0x21
#define     OBIS_OverVoltageCounts    0x22
#define     OBIS_OverVoltageMinute    0x23
#define     OBIS_OverCurrentCounts    0x24
#define     OBIS_OverCurrentMinute    0x25
#define     OBIS_imbalanceCounts      0x26
#define     OBIS_imbalanceMinute      0x27
#define     OBIS_BatRunTime           0x28
#define     OBIS_MeterRunTime         0x29
#define     OBIS_AllEng               0x2a
#define     OBIS_EngPos               0x2b
#define     OBIS_EngNeg               0x2c
#define     OBIS_DayEng               0x2d

#define     OBIS_DemandPos            0x2e
#define     OBIS_QConst               0x2f
#define     OBIS_CurrentDemandPos     0x30
#define     OBIS_CurrentDemandNeg     0x31
#define     OBIS_IRBaud               0x32
#define     OBIS_RS485Baud            0x33
#define     OBIS_AutoCode             0x34
#define     OBIS_DemandDot            0x35
#define     OBIS_DemandCycle          0x36
#define     OBIS_RecordCycle          0x37
#define     OBIS_CellTime             0x38
#define     OBIS_BanlanceTime         0x39
#define     OBIS_HolidayTable         0x3a
#define     OBIS_DemandNeg            0x3b
#define     OBIS_OverVoltageTime      0x3c
#define     OBIS_BlockData            0x3d
#define     OBIS_STARUPGRADE          0x3e
#define     OBIS_JCSetData            0x3f
#define     OBIS_GeographicPos        0x40   

#define     CurrentADDR_IDCode            0x01020002
#define     CurrentGeographicPos_IDCode   0x01020902    //这个要看看
#define     CurrentFrequency_IDCode       0x03800702
#define     CurrentCurrent_IDCode         0x03800802
#define     CurrentVoltage_IDCode         0x03800902
#define     CurrentPowerFactor_IDCode     0x03800A02
#define     CurrentPower_IDCode           0x03800B02
#define     CurrentVAPower_IDCode         0x03800F02
#define     AllEng_IDCode                 0x03340002
#define     EngPos_IDCode                 0x03200002
#define     NegPos_IDCode                 0x03220002
#define     UnderVoltage_IDCode           0x03B00102
#define     UnderVoltageMinute_IDCode     0x03B00202
#define     OverVoltage_IDCode            0x03B00302
#define     OverVoltageMinute_IDCode      0x03B00402
#define     BatRunTime_IDCode             0x03C00002
#define     MeterRunTime_IDCode           0x03C00102
#define     Bat1Voltage_IDCode            0x03C10002
#define     HisMonthEng_IDCode            0x07000002
#define     HisDayEng_IDCode              0x07010002
#define     Clock_IDCode                  0x08000002
#define     ClockBroadcast_IDCode         0x08000102
#define     PassWord3_IDCode              0x0F000207
#define     PassWord2_IDCode              0x0F000107
#define     PassWord1_IDCode              0x01A00002
#define     EngPoint_IDCode               0x01900002
#define     DisplayTime_IDCode            0x07100004   
#define     CheatEnergyNum_IDCode         0x0181A002
#define     SoftWare_IDCode               0x01510002
#define     HardWare_IDCode               0x01520002

#define     NonDot                        0xff
#define     CodeType                      0x01
#define     NumType                       0x02
#define     MaxSaveDays                   62
#define     MaxSaveMonths                 12

#define     HexType                      0
#define     ArrayType                    1
#define     DLMSDelayTime                120000 //2分钟 单位为5ms

#define     Level0             0         //无密码
#define     Level1             1         //一级密码
#define     Level2             2         //二级密码
#define     Level3             4         //三级密码
#define     Level4             8         //四级密码
#define     PassAESLength      48
#define     OBISREAD_NUMITEM   12
#define     HardWareLength     26
#define     SoftWareLength     23

/* main flash */
#define FLASH_STR_ADDR           (0x1000)  
#define FLASH_END_ADDR           (0x1FFFF) 
#define Flash_Read(addr)         *(volatile uint16_t*)((uint32_t)(addr))


/*---------------------------------------------------------------------------*/
/*外部结构体、枚举和类声明                                                   */
/*---------------------------------------------------------------------------*/
typedef struct
{
	uint8	comBuff[MAXBUFFER];
	uint8	rxPointer;
	uint8   nTx;
	uint8   DLMSMode;
	uint8   BCC;
	uint8   TxPointer; 
	uint16  OBISCodeCounts; 
    uint8   PasswordLevel;    //当前密码级别
    uint8   BlockDataCount;   //数据块指针
    uint8   BlockDataStatus;
    uint8   BlockDataBcc;
	flag  	Flag;
}IEC62056_Type;

/*---------------------------------------------------------------------------*/
/*外部变量声明                                                               */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*外部函数声明                                                               */
/*---------------------------------------------------------------------------*/
uint8 IEC62056ReceiveProcess(COM_TYPEDEF com);
void RS485DLMSStatusInit(void);
void BitSet(uint8 *Flag, uint8 Bit);
void BlockDataSend(COM_TYPEDEF com);
uint8 CalcWeek(uint8 *Date);

#endif
