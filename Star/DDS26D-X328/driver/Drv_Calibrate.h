/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：Drv_Calibrate
* 文件标识：
* 摘要：ATT7059校正及统一修调

* 当前版本：1.0.0 
* 作者：付学尧BrianYao
* 完成日期：20131016
*******************************************************************/

#ifndef _DRV_CALIBRATE_
#define _DRV_CALIBRATE_
#pragma pack (push,1) 
//-----------------统一修调与DLT645-2007接口-----------
typedef struct                    
{
        uint8 d[4];
        uint16  i;
}Calib_Comm_id;

typedef struct        //uart and display common data;
{
	flag   cmderr;  //0XFF ERROR 0X00 IS OK
	uint8 * addrr;
	uint8  addre;
	uint8  cmdlength;
	uint8  DI[4];  //ID[0-3]IDCODE ID4 is display 5th id --low
	uint8   data[192];
}Calibrate_Uart_type;

//-----------------统一修调使用结构体-----------
typedef struct   	  //参数设置命令(E0	00 01 00)
{
	uint8	Frequency;	//额定频率   
	uint16	VoltageB;	//额定电压
	uint16	CurrentB;		//额定电流
	uint16	CurrentMax;	//最大电流
	uint16	Constant_Kwh; 	//有功脉冲常数
	uint16	Constant_Kvah;	//无功脉冲常数
	uint16	Tamp_Rate;	//防窃电参量 
}BasicParameterType;

typedef struct  	  //功率法修调增益（E0	00	02	00）
{
	uint8	EmuNum;		//Phase Number:  0-tatol,1-A,2-B,3-C
	uint32	Power;	//active power	 XXXXX.xxx W
	uint32	Voltage;	//voltage XXX.xxx V
	uint32	Current;	//current XXX.xxx 	A
}PGainParameterType;

typedef struct  	  //误差法修调增益（E0	00	03	00）
{
	uint8	EmuNum;		//Phase Number:  0-tatol,1-A,2-B,3-C
	int16	Error;	//err%	XX.xxx%  
	uint32	Voltage;	//voltage XXX.xxx V
	uint32	Current;	//current XXX.xxx 	A
}EGainParameterType;

typedef struct   	  //功率法相角修调（E0	00	04	00）
{
	uint8	EmuNum;		//Phase Number:  0-tatol,1-A,2-B,3-C
	uint32	Power;	//active power		 XXXXX.xxx W
	uint16	PowerFactor;	//active power factor
}PAngleParameterType;

typedef struct   	  //误差法相角修调（E0	00	05	00）
{
	uint8	EmuNum;		//Phase Number:  0-tatol,1-A,2-B,3-C
	int16	Error;	//err%  XX.xxx%	
}EAngleParameterType;

typedef struct     //功率法偏置修调（E0	00	06	00）
{
	uint8	EmuNum;		//计量单元序号    0-tatol,1-A,2-B,3-C
	uint32	Power;	//active power		 XXXXX.xxx W	
	uint32	Current;	//current
}POffsetParameterType;

typedef struct     //误差法偏置修调（E0	00	07	00）
{
	uint8	EmuNum;		//Phase Number:  0-tatol,1-A,2-B,3-C 
	int16 	Error;	//err%  XX.xxx%	
}EOffsetParameterType;
#pragma pack (pop)
union CalibrateBasicDataType
{
	 BasicParameterType		BasicData;		//参数设置命令
	 PGainParameterType		PGainData;		//功率法修调增益
	 EGainParameterType		EGainData;		//误差法修调增益
	 PAngleParameterType	PAngleData;		//功率法相角修调
	 EAngleParameterType	EAngleData; 	//误差法相角修调
	 POffsetParameterType	POffsetData;	//功率法偏置修调
	 EOffsetParameterType	EOffsetData;	//误差法偏置修调
};
	
typedef struct  
{
	union	CalibrateBasicDataType	CalibrateBasicData;
	uint32	Irms;		//XXX.x xx  零线电流有效值修调 
	int16	Toff; 		//XXX.x ℃温度修调 
	int16	PPMOffset;	//XXX.x       PPM补偿fu add，20130716
	uint8	Type;		//通讯较表数据类型
}CalibrateComDataType;


#define  BigCurrentCounts        10// 10
#define  SmallCurrentCounts      50// 10


typedef struct  
{
	uint8	Level;
	uint8	IbCalibration;
	uint8	Number;
	uint8	Counts;
	uint8	TempPointer;
    uint8   TempPointer1;
	int32	Power_P_Temp;
	int32	Power_Q_Temp;
	uint32	IrmsTemp;
	uint32	VrmsTemp;
	int32	Power_P[3];	//average active power
	int32	Power_Q[3];	//average reactive power
	uint32	Irms_Reg[3];	//average current register data
	uint32	Vrms_Reg[3];//average voltage register data
	flag		Status;             
}AutomaticCalibrationType;

//宏定义-----------------------------------
//------------------
#define  WRITE_COMMAD						0x14		//写命令
#define  READ_COMMAD						0x11		//读命令
#define  WRITE_METER_COMMAD				0x15       //写表地址
#define  READ_METER_COMMAD					0x13       //读表地址
#define  ReadBasicLength					0x0D		//回读基本参数长度
#define  Calibrate_BasicParameterSet		0x01		//参数设置命令
#define  Calibrate_PGainSet				0x02		//功率法修调增益
#define  Calibrate_EGainSet				0x03		//误差法修调增益
#define  Calibrate_PPhaseAngleSet			0x04	    //功率法相角修调
#define  Calibrate_EPhaseAngleSet			0x05	    //误差法相角修调
#define  Calibrate_POffsetSet				0x06		//功率法偏置修调
#define  Calibrate_EOffsetSet				0x07		//误差法偏置修调
#define  Calibrate_Gain_NCurrentSet		0x08		//零线电流有效值修调
#define  Calibrate_TemperatureSet			0x09		//温度修调
#define  Calibrate_ResetSet				0x0a		//修调寄存器初始化
#define  Calibrate_PPMSet					0x0b	    //PPM值
#define  Calibrate_Time					0x0C       //校时
#define  Calibrate_ReadBack				0x0e		//参数回读

#define  Calibrate_BasicParameterType	0x01		//参数设置命令类型
#define  Calibrate_PGainSetType			0x02		//功率法修调增益类型
#define  Calibrate_EGainSetType			0x03		//误差法修调增益类型
#define  Calibrate_PPhaseAngleType		0x04	    //功率法相角修调类型
#define  Calibrate_EPhaseAngleType		0x05	    //误差法相角修调类型
#define  Calibrate_POffsetType			0x06		//功率法偏置修调类型
#define  Calibrate_EOffsetType			0x07		//误差法偏置修调类型
#define  Calibrate_Gain_NCurrentType		0x08		//零线电流有效值修调类型
#define  Calibrate_TemperatureType		0x09		//温度修调类型
#define  Calibrate_ResetType				0x0A		//修调寄存器初始化类型
#define  Calibrate_PPMSetType				0x0b	    //PPM值修调类型

#define  NoCalibrateRequest				0x02		//无请求数据
#define  NoCalibratePermit					0x04		//无较表权限
#define  NoCalibrateVersion				0x08		//修调软件版本不兼容

//-------------------------
//extern flag CalibrationStatusType;
#define CalibrationStartPress			CalibrationStatusType.bits.bit0           //校表开始按键
#define CalibrationInitPress			CalibrationStatusType.bits.bit1           //校表初始化按键
#define CalibrationStartFlag			CalibrationStatusType.bits.bit2           //触发按键修调标志
#define CalibrationInitFlag			CalibrationStatusType.bits.bit3           //按键清零修调参数
//#define								CalibrationStatusType.bits.bit4	//
//#define								CalibrationStatusType.bits.bit5	//
//#define								CalibrationStatusType.bits.bit6	//
//#define								CalibrationStatusType.bits.bit7	//

//外部常量申明-----------------------------------
extern uint8 I0_PRO_SWI;
extern AutomaticCalibrationType  AutomaticCalibrationData; 

//---------初始化调用--------------------------------
void DRV_Calibrate_PRO_SWI_INIT(void);
void DRV_Calibrate_PRO_SWI_POWER_DOWN_INIT(void);

//---------外部函数调用--------------------------------
void Drv_Calibrate_DLT6452007(uint8 mode);
void Drv_Calibrate_Init(void);
//---------main.c函数调用--------------------------------
void AutomaticCalibrationProcess(void);
#endif


