/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：Drv_Rn8207.h
* 文件标识：
* 摘要：ATT7059计量驱动

* 当前版本：1.0.0 
* 作者：
* 完成日期：20131016
*******************************************************************/

#ifndef _DRV_ATT7059_H_
#define _DRV_ATT7059_H_

/****************************************************************/
//------------------------- IO口配置--------------------------
/****************************************************************/
#define	IO_PORT_LED_ACT	IO_PORT5//active pulse  P55
#define	IO_PIN_LED_ACT	IO_PINx5

#define	IO_PORT_LED_REA	IO_PORT5//reactive pulse  P56
#define	IO_PIN_LED_REA	IO_PINx6
/****************************************************************/
//--------------  计量芯片寄存器地址--------------------
/****************************************************************/
//ATT7059
//CHAN SHU JI CUN QI
#define Rms_I1_R		0X06//3BYTE
#define Rms_I2_R		0X07//3BYTE
#define Rms_U_R			0X08//3BYTE
#define Freq_U_R		0X09//2BYTE
#define PowerP1_R		0X0A//3BYTE
#define PowerQ1_R		0X0B//3BYTR
#define Power_S_R		0X0C
#define Energy_P_R		0X0D
#define Energy_Q_R		0X0E
#define Energy_S_R		0X0F
#define PowerP2_R		0X10
#define	PowerQ2_R		0X11
#define BackupData_R   	0x16
#define COMChecksum_R	0x17
#define SUMChecksum_R 	0X18
#define EMUSR_R			0X19
#define ChipIDr_R		0X1B//默认值7053b0

//ATT7059
//calibrate register
#define EMUIE_R 		0x30	//** 002Byte
#define EUMIF_R 		0X31	//** 00 2Byte
#define WPREG_R 		0X32	//** 00 1Byte
#define SRSTREG_W		0X33	//RESET 

#define EMUCFG_W        0x40
#define FreqCFG_W		0X41
#define ModuleEn_W		0X42
#define ANAEN_W			0X43
#define IOCFG_W			0X45

#define	GP1_W			0X50
#define	GQ1_W			0X51
#define	GS1_W			0X52

#define	GP2_W			0X54
#define	GQ2_W			0X55
#define GS2_W			0x56

#define QPhsCal_W		0x58
#define	ADCCON_W		0X59	

#define	I2Gain_W		0x5b
#define	I1Off_W			0x5c
#define	I2Off_W			0x5d
#define	UOff_W			0x5e
#define	PQStart_W		0x5f

#define	HFconst_W		0x61
#define	ICHK_W			0x62
#define	ITAMP_W			0X63

#define	P1OFFSETH_W		0X65
#define	P2OFFSETH_W		0X66
#define	Q1OFFSETH_W		0X67
#define	Q2OFFSETH_W		0X68

#define	I1RMSOFFSET_W 	0X69
#define	I2RMSOFFSET_W	0X6A

#define GPhs1_W		  	0x6d
#define GPhs2_W		  	0x6e	//WPREG==0XA6
#define	PFCnt_W			0x6F
#define QFCnt_W			0x70
#define SFCnt_W			0x71

#define ANACON_W		0X72
#define SUMCHECKL_W  	0X73
#define SUMCHECKH_W		0X74
#define MODECFG_W		0X75
#define P1OFFSETL_W		0X76
#define P2OFFSETL_W		0X77
#define Q1OFFSETL_W		0X78
#define Q2OFFSETL_W		0X79

#define W_ENABLE_1		0XA6	//可以写50h-7CH
#define W_ENABLE_2     	0xBC    //可以写40h-45h
#define W_DISABLE    	0xB2	//关写保护
#define EMURESET		0X55	//写入0x55可以复位EMU

//=================================================
//==============EMU通讯应用======================
//=================================================
#define  MAX_EMU_NUMBER		 	2		//最大计量芯片数量:0:EMU_L1 	1:EMU_L2		2:EMU_L3

#define  UART_ERR_MAX			16   	//  计量芯片通信联系通信错误次数20140313

#define	 EMU_Regisetr_NUM		14
#define  EMU_regSum			    41		//校表寄存器的个数 41个。用于算校验和




typedef struct
{
	COM_TYPEDEF ch;		//串口端口
	uint8 chipID;		//片选地址
}EMU_CS;

typedef enum
{
	EMU_L1=0,
	EMU_L2,
}EMU_NO;

typedef struct 
{
	EMU_NO NO;			//片选
	uint8 cmd;			//命令
	uint8 len;			//数据长度
}EMU_CMD;

typedef enum
{
	HOLDE=0,
	GO_ON,
}EMU_HOLDE;

#pragma pack (push,1) // 1字节对齐，star
typedef struct 
{
	uint8 timeMark;		//时标
	uint8 commP;			//命令队列指针
	COM_TYPEDEF ch;		//当前端口
	uint8 bufferTx[10];	//当前发送缓存
	uint8 lenTx;			//当前发送长度
	uint8 bufferRx[10];	//当前接收缓存
	uint8 lenRx;			//当前接收长度
	EMU_HOLDE holdeState;//手动操作EMU时，自动读取挂起
}EMU_COMM;

/****************************************************************/
//-------------- 计量芯片寄存器值-----------------------
/****************************************************************/
typedef struct //variable of EMU Measure
{
	uint32	IRMS[3][3];//	0X22//3byte
	uint32	URMS[3][3];//	0X24//3byte
	uint16	UFreq[3][3];//	0X25//2byte
	int32	PowerP[3][3];//	0x26//4byte
	int32	PowerQ[3][3];//	0x28//4byte 
    int32	PowerS[3][3];//	0x28//4byte 
	uint32	EnergyP[3][3][2];//	0x29//3byte  //EnergyP[3][I]:  I=0:NEW DATA  I=1:OLD DATA  I=2:DAA DATA
	uint32	EnergyQ[3][3][2];//	0x2B//3byte
	uint32	EMUStatus[3][3];//	0x2D//3byte 00ee79h
	uint32	CheckSum[3][3];		//	0x18//3byte 00ee79h
	uint8	SysStatus[3];//	0x43//1byte
	uint32	DeviceID[3];//	0x7F//3byte  820700h};	
	uint16	ADCCON_7059[3];		//	0x00	//2byte
	uint16	HFConst_7059[3];	//	0x03	//2byte
	uint8   CheckSum_Err_Ct[3];	//
	uint8   EMU_Uart_Tx_Err_Time[2];	     //计量芯片在固定时间内未收到返回帧
	uint8   EMU_Uart_Err_Cnt[2];			//计量芯片多少次通讯异常计数
    uint8   EMU_Uart_Err_ing[2];	        //==1 计量芯片通讯当前正处于不通讯状态
    uint8   EMU_Uart_Ok_Recover[3];		    //通讯出现异常恢复
    uint8   EMU_err_ct[2];
}EMU_REGISTER;

/****************************************************************/
//--------------------  计量瞬时量--------------------------
/****************************************************************/
typedef struct 	//variable of EMU Measure
{
	uint32	Volt[3];	// XXXX.xxxx Phase A/B/C voltage rms:0.0001V     0:Phase A    1:Phase B   2:Phase C 
	uint32	Curr[3];
	uint32	Volt_UtRms;	// XXXX.xxxx Three phase voltage Rms :0.0001V     
	uint32	Curr_ItRms;	// XXXX.xxxx Three phase currents Rms :0.0001A
	int32	Act_Power[3];	
	int32	React_Power[3];  
	uint32	Apt_Power[4];	// XXXXX.xxx Total/Phase A/B/C  apparent power  :0.001VA   0:total     1:Phase A    2:Phase B   3:Phase C   
	uint32	Phase_Angle[3];// XXXX.xxx  power angles  , between  Phase A/B/C  voltages and currents    0:Phase A    1:Phase B   2:Phase C 
	uint32	angle[3];		// XXXX.xxx Voltage2Voltage Angles , between phase voltages  0:Yab    1:Yac   2:Ybc
	uint16	Factor[4];		// X.xxx Total/Phase A/B/C  power facter 0.001   0:total     1:Phase A    2:Phase B   3:Phase C   
	uint32	Freq[4];		// XX.xxxxxx  line frequency  0.000001HZ 0:total     1:Phase A    2:Phase B   3:Phase C   
	uint8	Quadrant[4];	// I II III IVQuadrant   Quadrant[i]=0:I ; Quadrant[i]=1:II ; Quadrant[i]=2:III ; Quadrant[i]=3:IV ; 0:total     1:Phase A    2:Phase B   3:Phase C   
	uint8	Temprature;	// XXXX.xxxx temprature register 
	uint8	EMU_Power_Direction[3];//power direction [0] 当前计量反向 [1]L1路通道计量功率方向  [2] L2路计量通道功率方向
	uint32	EMU_State_Register;//state register :lost phase ,phase err,
	uint8	Measure_Model;//Measure model     0x00: three-phase four-wire       0xff:  three-phase three-wire 
	uint8	Measure_Chnsel;//当前计量通道    0x00: 第一路       0x01:  第二路
	uint8   Current_Unbalanced; // 当前是否处于窃电状态  1: 处于  0: 不处于
}EMU_VARIABLE;
#pragma pack (pop) // 1字节对齐，end

//=================================================
//==============计量方式及防窃电应用=======================
//=================================================
typedef struct 	//variable of EMU Measure
{ 
  uint8    EnergyChnsel;    //当前选择的计量方式  0：防窃电,  1：I1  2： I2  3： I1+I2 
  uint8    TampMode;        //1:     常规设计,判断变比,0:   特殊设计 判断固定值
  uint8    Ichk;            //窃电的比例
  uint32   Itamp;
  uint32   CurrentHighthreshold;   //固定值
  uint32   CurrentLowthreshold;

}EMU_PARAMETER;

//=================================================
//==============电能量应用=======================
//=================================================
typedef struct  
{
//-------- used for Pulse ---------
	uint64	PowerActPerSecond;//每次读取的平均功率，通过能量计算active energy,used for Pulse
	uint64	AddedPowerAct;//累加的功率，上一次剩余的功率
	uint16	PulseWidthCountAct;//出脉冲脉宽计数
	uint64	PowerReaPerSecond;//每次读取的平均功率，通过能量计算reactive energy,used for Pulse
	uint64	AddedPowerRea;//累加的功率，上一次剩余的功率
	uint16	PulseWidthCountRea;//出脉冲脉宽计数
//-------- used for Energy ---------
	uint32	pulse_ct_act;//active pulse const
	uint32	pulse_ct_react;//reactive pulse const
	uint32	Rail_Plus_Active[4];//XXXXX.xxx wh	Sum/Phase A/Phase B/Phase C	positive active energy
	uint32	Rail_Nega_Active[4];//XXXXX.xxx wh	Sum/Phase A/Phase B/Phase C	negative Active energy
	uint32	Rail_Plus_Reactive[4];//XXXXX.xxx wh	Sum/Phase A/Phase B/Phase C	positive reactive energy
	uint32	Rail_Nega_Reactive[4];//XXXXX.xxx wh	Sum/Phase A/Phase B/Phase C	negative reactive energy 
	uint32	Rail_Quad_Reactive[4][4];//XXXXX.xxx wh	1st,2nd,3rd,4th quadrant	quadrant reactive energy   [PHASE:sum,A,B,C][QUAD:0,1,2,3]
	uint32	Active_Add_T[4];//XXXXX.xxx wh	0:total   1:PhaseL1  2:PhaseL2    3:PhaseL3   active energy
	uint32	Reative_Add_T[4];//XXXXX.xxx wh	0:total   1:PhaseL1  2:PhaseL2    3:PhaseL3     reactive energy;
	int32	Active_Sub_T[4];//XXXXX.xxx wh	0:total   1:PhaseL1  2:PhaseL2    3:PhaseL3      active energy
	int32	Reative_Sub_T[4];//XXXXX.xxx wh	0:total   1:PhaseL1  2:PhaseL2    3:PhaseL3     reactive energy;
	uint16	checksum;
}EMU_ENERGY;

//=================================================
//==============修调应用=========================
//=================================================
#pragma pack(push, 1)
typedef struct 		//修调数据
{
	uint32 EMUCFG_7059;		//01H 	EMUCON	2	0003h	计量控制寄存器
	//uint32 MODECFG_7059;		//00H	SYSCON	2	0003h	系统控制寄存器
	uint32 ADCCON_7059;		//00H	SYSCON	2	0003h	系统控制寄存器
	uint32 HFConst_7059;	//02H	HFConst	2	1000h	脉冲频率寄存器
	uint32 GP1_7059;		//05H	GPQ		2	0000h	功率增益校正寄存器
	uint32 GQ1_7059;		//05H	GPQ		2	0000h	功率增益校正寄存器
	uint32 GS1_7059;		//05H	GPQ		2	0000h	功率增益校正寄存器
	uint32 Phs_7059;		//07H	Phs		1	00h		相位校正寄存器
	uint32 QPhsCal_7059;		//09H	QPhsCal	2	0000h	无功相位补偿
	uint32 APOS_7059;		//0AH	APOS	2	0000h	有功功率Offset校正寄存器
	uint32 RPOS_7059;		//0CH	RPOS	2	0000h	无功功率Offset校正寄存器
	uint32 PQStart_7059;		//03H	PQStart	2	0060h	有功起动功率设置
	uint32 URMS_refer_7059;		//修调标定值
	uint32 IRMS_refer_7059;
	uint32 Power_refer_7059;		//
	uint32 EMU_checksum_7059;		//读自EMU的校验和寄存器的值
	uint16 ChkSum_7059;		//2DH	EMUstatus 4	Bit[15:0]校表参数校验和
}calibrate_ATT7059;
#pragma pack(pop)


typedef union 
{
	uint8	b[4];
	uint16	w[2];
	uint32	l;
}U32Type;

#pragma pack(push, 1)
typedef struct 		
{                               
	uint32	Ib;//XXX.xxx A
	uint32	Imax;//XXX.xxx A
	uint32	Meter_const_act;	//XXXX imp/kWh
	uint32	Meter_const_react;	//XXXX imp/kvarh
	uint16	Un;//XXX.x V
	uint8	Freq;//XX Hz
	uint16	checksum;
}calibrate_parameter; 
#pragma pack(pop)


typedef struct 
{
	uint32 EMU_Calu_Data_L1; 
	uint32 EMU_Calu_Data_L2;
}EMU_CALI_CHECKSUM;


typedef struct 		
{                               
	calibrate_ATT7059 Cali_EMU_L1;
	calibrate_ATT7059 Cali_EMU_L2;
	calibrate_parameter Cali_Par;
}tEE_CALIBRATE_METER; 


#define EE_CALIBRATE_ADDR_L1              EE_BAK_STRUCT_ADDR(EE_CalibrateBak.Cali_EMU_L1)		//L1修调存储地址
#define EE_CALIBRATE_ADDR_L2              EE_BAK_STRUCT_ADDR(EE_CalibrateBak.Cali_EMU_L2)		//L2修调存储地址
#define EE_CALIBRATE_ADDR_PAR			  EE_BAK_STRUCT_ADDR(EE_CalibrateBak.Cali_Par)			//Paramater修调存储地址


//=================================================
//============== 电表状态及标志=================
//=================================================


//=================================================
//==============对外应用=========================
//=================================================
typedef enum
{
	V_RMS_L1 =0, //XXXX.xxxx V
	V_RMS_L2 ,
	V_RMS_L3 ,
	I_RMS_L1 , //XXXX.xxxx A
	I_RMS_L2 ,
	I_RMS_L3 ,
	I_RMS_N ,
	V_RMS_T , //XXXX.xxxx V
	I_RMS_T , //XXXX.xxxx A
	
	ACTIVE_POWER_T ,//XXXXX.xxx W
	ACTIVE_POWER_L1 ,
	ACTIVE_POWER_L2 ,
	ACTIVE_POWER_L3 ,
	REACTIVE_POWER_T ,//XXXXX.xxx Var
	REACTIVE_POWER_L1 ,
	REACTIVE_POWER_L2 ,
	REACTIVE_POWER_L3 ,
	APPARENT_POWER_T ,//XXXXX.xxx VA
	APPARENT_POWER_L1 ,
	APPARENT_POWER_L2 ,
	APPARENT_POWER_L3 ,
	PHASE_ANGLE_L1 ,// XXXX.xxx  °
	PHASE_ANGLE_L2 ,
	PHASE_ANGLE_L3 ,
	ANGLE_L1L2 ,//Yab   XXXX.xxx  °
	ANGLE_L1L3 ,//Yac
	ANGLE_L2L3 ,//Ybc
	PF_T , //X.xxx
	PF_L1 ,
	PF_L2 ,
	PF_L3 ,
	FREQUENCY_T ,//XX.xxxxxx Hz
	FREQUENCY_L1 ,//XX.xxxxxx Hz
	FREQUENCY_L2 ,//XX.xxxxxx Hz
	FREQUENCY_L3 ,//XX.xxxxxx Hz
	QUADRANT_T ,//Quadrant[i]=0:I ;	1:II ;	2:III ;	3:IV
	QUADRANT_L1 ,
	QUADRANT_L2 ,
	QUADRANT_L3 ,
	TEMPERATURE  , //℃
	POWER_DIRECTION ,  //1:Negative  0:Positive     BIT7:rea_t;  BIT6:rea_c;  BIT5:rea_b;  BIT4:rea_a;  BIT3:act_t;  BIT2:act_c;  BIT1:act_b;  BIT0:act_a;
	CurrentImbalance,
	STATUS ,//1:Nor  0:Err  BIT04: Inverse Phase I    BIT03: Inverse Phase U    BIT02: Lost Phase L3      BIT01: Lost Phase L2   BIT00: Lost Phase L1
	MEASURRE_MODEL ,	//Measure model     0x00: three-phase four-wire       0xff:  three-phase three-wire 
	MEASURE_CHNSEL ,
	
	FUNDAMENTAL_VOLTAGE_L1 , //XXXX.xxxx V  Fundamental
	FUNDAMENTAL_VOLTAGE_L2 ,
	FUNDAMENTAL_VOLTAGE_L3 ,
	
	FUNDAMENTAL_CURRENT_L1 , //XXXX.xxxx A Fundamental
	FUNDAMENTAL_CURRENT_L2 ,
	FUNDAMENTAL_CURRENT_L3 ,
	
	FUNDAMENTAL_POWER_T ,//XXXXX.xxx W FUNDAMENTAL
	FUNDAMENTAL_POWER_L1 , 
	FUNDAMENTAL_POWER_L2 ,
	FUNDAMENTAL_POWER_L3 ,
	
	HARMONIC_VOLTAGE_L1 , //XXXX.xxxx V  Harmonic
	HARMONIC_VOLTAGE_L2 ,
	HARMONIC_VOLTAGE_L3 ,
	HARMONIC_CURRENT_L1 , //XXXX.xxxx A  Harmonic
	HARMONIC_CURRENT_L2 ,
	HARMONIC_CURRENT_L3 ,
	
	HARMONIC_POWER_T ,//XXXXX.xxx W  Harmonic
	HARMONIC_POWER_L1 ,
	HARMONIC_POWER_L2 ,
	HARMONIC_POWER_L3 ,
	
	HARMONIC_VOLTAGE_THD_L1 , //XXXX.xxxx 0.0001 %
	HARMONIC_VOLTAGE_THD_L2 ,
	HARMONIC_VOLTAGE_THD_L3 ,
	
	HARMONIC_CURRENT_THD_L1 , //XXXX.xxxx 0.0001 %
	HARMONIC_CURRENT_THD_L2 ,
	HARMONIC_CURRENT_THD_L3 ,

	SYS_PARAMETER_UN,//XXX.x V
	SYS_PARAMETER_IB,//XXX.xxx A
	SYS_PARAMETER_IMAX,//XXX.xxx A
	SYS_PARAMETER_CONST_ACT,	//XXXX imp/kWh
	SYS_PARAMETER_CONST_REA,	//XXXX imp/kvarh
	SYS_PARAMETER_FREQ,//XX Hz

} tMeasureID;

typedef enum
{
	PULSE_CT_ACT=0,//active pulse const
	PULSE_CT_REA,//reactive pulse const
	
	ACTIVE_Energy_Pos_T,//Sum positive active energy
	ACTIVE_Energy_Neg_T,//Sum/Phase L1/Phase L2/Phase L3 positive active energy
	REACTIVE_Energy_Pos_T,//Sum/Phase L1/Phase L2/Phase L3	positive reactive energy
	REACTIVE_Energy_Neg_T,//Sum/Phase L1/Phase L2/Phase L3	positive reactive energy
	
	ACTIVE_Energy_Pos_L1,//Phase L1 positive active energy
	ACTIVE_Energy_Neg_L1,
	REACTIVE_Energy_Pos_L1,
	REACTIVE_Energy_Neg_L1,
	
	ACTIVE_Energy_Pos_L2,//Phase L2 positive active energy
	ACTIVE_Energy_Neg_L2,
	REACTIVE_Energy_Pos_L2,
	REACTIVE_Energy_Neg_L2,
	
	ACTIVE_Energy_Pos_L3,//Phase L3 positive active energy
	ACTIVE_Energy_Neg_L3,
	REACTIVE_Energy_Pos_L3,
	REACTIVE_Energy_Neg_L3,
	
	REACTIVE_Energy_Q1_T,//Total	quadrant reactive energy
	REACTIVE_Energy_Q2_T,
	REACTIVE_Energy_Q3_T,
	REACTIVE_Energy_Q4_T,
				
	REACTIVE_Energy_Q1_L1,//Phase L1	quadrant reactive energy
	REACTIVE_Energy_Q2_L1,
	REACTIVE_Energy_Q3_L1,
	REACTIVE_Energy_Q4_L1,
	
	REACTIVE_Energy_Q1_L2,//Phase L2	quadrant reactive energy
	REACTIVE_Energy_Q2_L2,
	REACTIVE_Energy_Q3_L2,
	REACTIVE_Energy_Q4_L2,
	
	REACTIVE_Energy_Q1_L3,//Phase L3	quadrant reactive energy
	REACTIVE_Energy_Q2_L3,
	REACTIVE_Energy_Q3_L3,
	REACTIVE_Energy_Q4_L3,
	
	ACTIVE_ADD_T_T,
	ACTIVE_ADD_T_L1,
	ACTIVE_ADD_T_L2,
	ACTIVE_ADD_T_L3,
	REACTIVE_ADD_T_T,
	REACTIVE_ADD_T_L1,
	REACTIVE_ADD_T_L2,
	REACTIVE_ADD_T_L3,
	
	ACTIVE_SUB_T_T,
	ACTIVE_SUB_T_L1,
	ACTIVE_SUB_T_L2,
	ACTIVE_SUB_T_L3,
	REACTIVE_SUB_T_T,
	REACTIVE_SUB_T_L1,
	REACTIVE_SUB_T_L2,
	REACTIVE_SUB_T_L3,
	
	FUNDAMENTAL_Energy_T,//Total	Fundamental  energy
	FUNDAMENTAL_Energy_L1,
	FUNDAMENTAL_Energy_L2,
	FUNDAMENTAL_Energy_L3,

	APPARENT_Energy_T,//Total	APPARENT  energy
	APPARENT_Energy_L1,
	APPARENT_Energy_L2,
	APPARENT_Energy_L3,
} tEMUEnergyID;

extern EMU_REGISTER EMU_Register;
extern tEE_CALIBRATE_METER Calibrate_meter;
extern EMU_VARIABLE EMU_variable;
extern const EMU_CMD EMU_CMD_BUFF[EMU_Regisetr_NUM];
extern EMU_COMM EMUcommBuffer;	//EMU当前通讯缓存
extern EMU_PARAMETER  EMU_Parmeter; 
extern const uint32 EMU_REG_DATA[EMU_regSum];

//---------EMU UART函数-----------
uint8 EMUCommand(EMU_NO NO, uint8 command)  ;
uint8 EMUWrite(EMU_NO NO, uint8 command,uint8 L,uint8 *RAM);
uint8 EMURead(uint8 command,uint8 L,uint8 *RAM,EMU_NO NO);
void EMUUartAutoPause(void) ;
void EMUUartAutoGoOn(void) ;
void EMUWriteReadTest(void);
void EMU_Cali_CheckSum_L1(uint32 *RAM, uint8 len);
void EMU_Cali_CheckSum_L2(uint32 *RAM, uint8 len);
uint32 CalAppentPower(uint32 acPower,uint32 reacPower);

//---------主程序初始化函数-----------
void DRV_EMU_Initial_Calibrate(EMU_NO EMU_NUM);//just for test
void Get_Att7059_PUI(EMU_NO EMU_NUM,uint8 readPowerCount);
void Initial_Average_Calibrate_Data(EMU_NO EMU_NUM);
uint8 DRV_EMU_PowerUpInit(void);
void MeterPowerUPClearAll(void);

//---------修调接口函数-----------
uint8 Set_ATT7059(EMU_NO EMU_NUM);

//---------主程序5ms调用函数-----------
void DRV_EMU_Read_Energy(void);
//---------对外接口函数-----------
int32 Get_EMU_Measure_Data(tMeasureID id);
int32 Get_EMU_Energy_Data(tEMUEnergyID id);
//---------掉电初始化函数-----------
void DRV_EMU_PowerDownInit(void);
//---------测试函数-----------
void DRV_EMU_TEST_SEND(void);

#endif
