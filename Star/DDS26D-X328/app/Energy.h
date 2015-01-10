/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：Energy.h
* 文件标识：见配置管理计划书 
* 摘要：能量处理模块(含瞬时量处理)

* 当前版本：1.0.0 
* 作者：曾明谦
* 完成日期：20131028
*******************************************************************/
#ifndef __ENERGY_H
#define __ENERGY_H

/*External functions-----------------------------------------*/
uint16 EnergyDataInit(void);
void EnergyDataInitBattery(void);
void EnergyCalcProcess(void);
void EnergyPowerOffProcess(void);
void EnergyClear(void);
void SaveRundata(void);
uint16 EnergyParaGet(uint32 DataID, uint8 * pBuf);
uint16 VariableParaGet(uint32 DataID, uint8 * pBuf);
uint64 PowerRatioNumerator();
uint64 PowerRatioDenominator();

void MeterClearRecordInit(void);
void MeterClearRecord(void);

/*Export define-----------------------------------------*/
#define MAX_TARIFF   0
#define ENERGY_LENGTH    4
//#define RATE_OF_UNITS    10    // Meter precision/EMU minimum unit = 1Wh/0.1Wh 
#define METER_PRECISION    10    //Wh
#define IMPULSE_INC    16     //Meter precision * Impulse constant = 0.01KWh * 1600imp/KWh
//#define ACTIVE_IMPULSE_INC    4
//#define REACTIVE_IMPULSE_INC    4
#define ENERGY_SCALER         1
#define ACTIVE_ENERGY_UNIT       30
#define REACTIVE_ENERGY_UNIT   32
//#define APPARENT_ENERGY_UNIT   31
//#define FLIP_VALUE_ONE        100
//#define FLIP_VALUE_TWO       10000
//#define FLIP_VALUE_THREE    1000000
//#define FLIP_VALUE_FOUR      100000000
#define  MAX15MINENERGY  1500  //220 *100 *1.9*1.2*1.2 /4    //15.00KWH

/*Private define-----------------------------------------*/
#define   ENERGY_TYPE_NUMBER      (ACTIVE_Energy_Neg_L1-ACTIVE_Energy_Pos_L1+1+1) // +1 是因为包括组合有功共1个

#define    POWER_SCALER          (-1)
#define    VOLTAGE_SCALER       (-1)
#define    CURRENT_SCALER       (-3)
#define    FREQUENCY_SCALER   (-2)
#define    FACTOR_SCALER         (-3)

#define    ACTIVE_UNIT             27
#define    REACTIVE_UNIT         29
#define    APPARENT_UNIT         28
#define    VOLTAGE_UNIT           35
#define    CURRENT_UNIT           33
#define    FREQUENCY_UNIT       44
#define    FACTOR_UNIT            255

/*Private typedef-----------------------------------------*/
typedef struct
{
	uint8 Id;
	uint8 Len;
	uint8 Vari_type;
	uint8 scaler;
	uint8 unit;
}tVariableInfo;

typedef struct
{
	uint8 Id;//bit16 to bit23 of DataID
	uint8 Len;//data length
	uint8 scaler;//attribute 3
	uint8 unit;//attribute 3
	uint32 *RAM_Addr;//data address in ram
}tEnergyInfo;

typedef struct
{
	uint32 vol_numerator;     //电压分子变比
	uint32 vol_denominator; //电压分母变比
	uint32 cur_numerator;    //电流分子变比
	uint32 cur_denominator;//电流分母变比
	uint16 checksum;
}tRatio;

typedef struct
{
	uint8 remain_t[MAX_TARIFF+1];  //0: total      1~MAXTARIFF: tariff 1 ~ tariff MAXTARIFF
}tRemainder;

typedef struct
{
	tRemainder active_pos;      //正向有功电能余数
	tRemainder active_neg;      //反向有功电能余数
	tRemainder reactive_pos;    
	//tRemainder reactive_neg; 
	//tRemainder reactive_q1;  
	//tRemainder reactive_q2;   
	//tRemainder reactive_q3;      
	//tRemainder reactive_q4;    
	//tRemainder apparent_pos;    
	//tRemainder apparent_pos;     
	tRemainder act_mixing_add;   
	//tRemainder act_mixing_sub; 
	uint16 checksum;
}tEnergyRemain;

typedef struct
{
	//tEnergyRemain remain_total;
	tEnergyRemain remain_l1;
	//tEnergyRemain remain_l2;
	//tEnergyRemain remain_l3;
}tRemainderAll;

typedef struct 						
{
	uint32 t[MAX_TARIFF+1]; //xxxxxx.xx     0: total      1~MAXTARIFFNUM: tariff 1~tariff MAXTARIFFNUM
}tENG;

typedef struct
{
	tENG act_pos;		//当前正向有功电能
	tENG act_neg;		//当前反向有功电能
	tENG react_pos;     // 正向   无功 电量             
	//tENG react_neg;     // 反向   无功 电量    
	//tENG react_q1;
	//tENG react_q2;
	//tENG react_q3;
	//tENG react_q4;
	//tENG apparent_pos;
	//tENG apparent_neg;
	tENG act_mix_add;		//当前组合有功电能+
	//tENG act_mix_sub;		//当前组合有功电能-
	uint16 checksum;
}tEnergy;

typedef struct
{
	//tEnergy energy_total;
	tEnergy energy_l1;
	//tEnergy energy_l2;
	//tEnergy energy_l3;
}tEnergyAll;

typedef struct
{
       tEnergyAll energy_all;
       tRemainderAll remainder_all;
	tRatio ratio;
}tRunData;

typedef struct
{
	uint32 	meterClearNum;
	tDateTime Time;
	tRunData  meterClearEnergyData;
}tEE_MeterClearRecord;

typedef struct
{
	tEE_MeterClearRecord EE_MeterClearRecord; 
}tEE_MeterClearRecordInfo;


/*
typedef struct
{
	uint8 scaler;
	uint8 unit;
}tSU;

typedef struct
{
	tSU act_pos[MAX_TARIFF+1];		
	tSU act_neg[MAX_TARIFF+1];		
	tSU react_pos[MAX_TARIFF+1];                  
	tSU react_neg[MAX_TARIFF+1];       
	tSU react_q1[MAX_TARIFF+1];
	tSU react_q2[MAX_TARIFF+1];
	tSU react_q3[MAX_TARIFF+1];
	tSU react_q4[MAX_TARIFF+1];
	//tSU apparent_pos[MAX_TARIFF+1];
	//tSU apparent_neg[MAX_TARIFF+1];
	tSU act_mix_add[MAX_TARIFF+1];		
	tSU act_mix_sub[MAX_TARIFF+1];		
}tScalerUnit;

typedef struct
{
       tScalerUnit su_total;
	tScalerUnit su_l1;
	tScalerUnit su_l2;
	tScalerUnit su_l3;
}tEnergyPara;
*/

/*Export define-----------------------------------------*/
#define EE_ENERGY_TOTAL_ADDR              EE_BAK_STRUCT_ADDR(EE_EnergyBak.energy_all.energy_total)
#define EE_ENERGY_L1_ADDR                     EE_BAK_STRUCT_ADDR(EE_EnergyBak.energy_all.energy_l1)
#define EE_ENERGY_L2_ADDR                     EE_BAK_STRUCT_ADDR(EE_EnergyBak.energy_all.energy_l2)
#define EE_ENERGY_L3_ADDR                     EE_BAK_STRUCT_ADDR(EE_EnergyBak.energy_all.energy_l3)
#define EE_REMAINDER_TOTAL_ADDR        EE_BAK_STRUCT_ADDR(EE_EnergyBak.remainder_all.remain_total)
#define EE_REMAINDER_L1_ADDR               EE_BAK_STRUCT_ADDR(EE_EnergyBak.remainder_all.remain_l1)
#define EE_REMAINDER_L2_ADDR               EE_BAK_STRUCT_ADDR(EE_EnergyBak.remainder_all.remain_l2)
#define EE_REMAINDER_L3_ADDR               EE_BAK_STRUCT_ADDR(EE_EnergyBak.remainder_all.remain_l3)
#define EE_RATIO_ADDR                             EE_BAK_STRUCT_ADDR(EE_EnergyBak.ratio)

//电表清零记录信息
#define EE_METER_CLAER_RECORD_SAVE_ADDR				EE_STRUCT_ADDR(EE_MeterClearRecordInfo.EE_MeterClearRecord)
//#define EE_SCALER_UNIT_TOTAL_ADDR     EE_STRUCT_ADDR(EE_Energy_Para.su_total)
//#define EE_SCALER_UNIT_L1_ADDR           EE_STRUCT_ADDR(EE_Energy_Para.su_l1)
//#define EE_SCALER_UNIT_L2_ADDR           EE_STRUCT_ADDR(EE_Energy_Para.su_l2)
//#define EE_SCALER_UNIT_L3_ADDR           EE_STRUCT_ADDR(EE_Energy_Para.su_l3)

#endif
