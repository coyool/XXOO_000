/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：Energy.c
* 文件标识：见配置管理计划书 
* 摘要：能量处理模块(含瞬时量处理)

* 当前版本：1.0.0 
* 作者：曾明谦
* 完成日期：20131028
*******************************************************************/
#include "Include.h"

/*--------------------全局变量定义---------------------*/

__no_init static tRunData run_data;
static uint8 save_flag;

static const tEnergyInfo EnergyInfo[]=
{      //dataID,   length           ,          scaler                     ,      unit                                    ram address	   

     {0x20, ENERGY_LENGTH, (uint8)ENERGY_SCALER, ACTIVE_ENERGY_UNIT, (uint32 *)&run_data.energy_all.energy_l1.act_pos.t[0]}, //正向有功   

	 {0x22, ENERGY_LENGTH, (uint8)ENERGY_SCALER, ACTIVE_ENERGY_UNIT,  (uint32 *)&run_data.energy_all.energy_l1.act_neg.t[0]}, //反向有功 
	
	 //{0x24, ENERGY_LENGTH, (uint8)ENERGY_SCALER, REACTIVE_ENERGY_UNIT, (uint32 *)&run_data.energy_all.energy_l1.react_pos.t[0]}, //正向无功

	 //{0x26, ENERGY_LENGTH, (uint8)ENERGY_SCALER, REACTIVE_ENERGY_UNIT, (uint32 *)&run_data.energy_all.energy_l1.react_neg.t[0]}, //反向无功

	 //{0x28, ENERGY_LENGTH, (uint8)ENERGY_SCALER, REACTIVE_ENERGY_UNIT, (uint32 *)&run_data.energy_all.energy_l1.react_q1.t[0]}, //I象限无功

	 //{0x2a, ENERGY_LENGTH, (uint8)ENERGY_SCALER, REACTIVE_ENERGY_UNIT, (uint32 *)&run_data.energy_all.energy_l1.react_q2.t[0]}, //II象限无功

	 //{0x2c, ENERGY_LENGTH, (uint8)ENERGY_SCALER, REACTIVE_ENERGY_UNIT, (uint32 *)&run_data.energy_all.energy_l1.react_q3.t[0]}, //III象限无功

	 //{0x2e, ENERGY_LENGTH, (uint8)ENERGY_SCALER, REACTIVE_ENERGY_UNIT, (uint32 *)&run_data.energy_all.energy_l1.react_q4.t[0]}, //IV象限无功

	 //{0x30, ENERGY_LENGTH, (uint8)ENERGY_SCALER, APPARENT_ENERGY_UNIT, (uint32 *)&run_data.energy_all.energy_l1.apparent_pos.t[0]}, //正向视在

	 //{0x32, ENERGY_LENGTH, (uint8)ENERGY_SCALER, APPARENT_ENERGY_UNIT, (uint32 *)&run_data.energy_all.energy_l1.apparent_neg.t[0]}, //反向视在

	 {0x34, ENERGY_LENGTH, (uint8)ENERGY_SCALER, ACTIVE_ENERGY_UNIT, (uint32 *)&run_data.energy_all.energy_l1.act_mix_add.t[0]}, //组合有功加

	 //{0x35, ENERGY_LENGTH, (uint8)ENERGY_SCALER, ACTIVE_ENERGY_UNIT, (uint32 *)&run_data.energy_all.energy_l1.act_mix_sub.t[0]}, //组合有功减

	 //{0x36, ENERGY_LENGTH, (uint8)ENERGY_SCALER, REACTIVE_ENERGY_UNIT, (uint32 *)&run_data.energy_all.energy_l1.react_mix_add.t[0]}, //组合无功加
};

static const tVariableInfo VariableInfo[]=
{	
	{0x07, 0x02, FREQUENCY_T, (uint8)FREQUENCY_SCALER, FREQUENCY_UNIT},

	{0x08, 0x04, I_RMS_L1, (uint8)CURRENT_SCALER, CURRENT_UNIT},
	//{0x11, 0x04, I_RMS_L2, (uint8)CURRENT_SCALER, CURRENT_UNIT},

	{0x09, 0x02, V_RMS_L1, (uint8)VOLTAGE_SCALER, VOLTAGE_UNIT},

    {0x0a, 0x02, PF_T, (uint8)FACTOR_SCALER, FACTOR_UNIT},//此处的dataID 原本是对应PF_L1
	
	{0x0b, 0x04, ACTIVE_POWER_T, (uint8)POWER_SCALER, ACTIVE_UNIT},//此处的dataID 原本是对应ACTIVE_POWER_L1
		
	{0x0f, 0x04, APPARENT_POWER_T, (uint8)POWER_SCALER, APPARENT_UNIT},//此处的dataID 原本是对应APPARENT_POWER_L1

	//{0x0c, 0x04, ACTIVE_POWER_L1, (uint8)POWER_SCALER, ACTIVE_UNIT},//reverse

	//{0x0d, 0x04, REACTIVE_POWER_L1, (uint8)POWER_SCALER, REACTIVE_UNIT},

	//{0x0e, 0x04, REACTIVE_POWER_L1, (uint8)POWER_SCALER, REACTIVE_UNIT},//reverse

	//{0x0f, 0x04, (uint16 *)&variable.vari_l1.apparent_pos_power},

	//{0x10, 0x04, (uint16 *)&variable.vari_l1.apparent_neg_power},

	//{0x23, 0x02, PHASE_ANGLE_L1, (uint8)ANGLE_SCALER, PHASE_ANGLE_UNIT},
};

/*Private functions----------------------------------------------------------*/

/*********************************************************** 
函数功能：无符号数十进制范围相加
入口参数：
出口参数：1=cf
备	  注：99999999翻转
		  summand+=addend
***********************************************************/
static uint8 Add_Unsign_99(uint8 *summand, uint32 addend, uint8 length)
{
	uint8 temp[4]={0};
	
	switch(length)
	{
		case 1:
			*summand+=(uint8)addend;
			if(*summand>=(uint8)100)
			{
				*summand-=(uint8)100;
				return 1;
			}
			break;
		case 2:
			*(uint16 *)summand+=(uint16)addend;
			if(*(uint16 *)summand>=(uint16)10000)
			{
				*(uint16 *)summand-=(uint16)10000;
				return 1;
			}
			break;
		case 3:
			*(uint32 *)temp=(*(uint32 *)summand)&0x00ffffff;
			*(uint32 *)temp+=addend;
			if(*(uint32 *)temp>=(uint32)1000000)
			{
				*(uint32 *)temp-=(uint32)1000000;
				summand[0]=temp[0];
				summand[1]=temp[1];
				summand[2]=temp[2];
				return 1;
			}
			else
			{
				summand[0]=temp[0];
				summand[1]=temp[1];
				summand[2]=temp[2];
			}
			break;
		case 4:
			*(uint32 *)summand+=addend;
			if(*(uint32 *)summand>=(uint32)100000000)
			{
                 *(uint32 *)summand-=(uint32)100000000;
				 return 1;
			}
			break;
	}
	return 0;
}

/*********************************************************** 
函数功能：符号数十进制范围相加
入口参数：
出口参数：
备	  注：summand+=addend
***********************************************************/
/*static uint8 Add_Sign_79(int8 *summand, int32 addend, uint8 length)
{
	char temp[4]={0};
	
	switch(length)
	{
		case 1:
			*summand+=(char)addend;
			if(*summand>=(char)80)
			{
				*summand-=(char)80;
				return 1;
			}
			break;
		case 2:
			*(short *)summand+=(short)addend;
			if(*(short *)summand>=(short)8000)
			{
				*(short *)summand-=(short)8000;
				return 1;
			}
			break;
		case 3:
			*(long *)temp=(*(long *)summand)&0x00ffffff;
			*(long *)temp+=addend;
			if(*(long *)temp>=(long)800000)
			{
				*(long *)temp-=(long)800000;
				summand[0]=temp[0];
				summand[1]=temp[1];
				summand[2]=temp[2];
				return 1;
			}
			else
			{
				summand[0]=temp[0];
				summand[1]=temp[1];
				summand[2]=temp[2];
			}
			break;
		case 4:
			*(long *)summand+=(long)addend;
			if(*(long *)summand>=(long)80000000)
			{
				*(long *)summand-=(long)80000000;
				return 1;
			}
			break;
	}
	return 0;
}*/

/*********************************************************** 
函数功能：变比的校验和检验
入口参数：
出口参数：
备	  注：RAM 校验和错误则从EEPROM 中恢复至RAM
                    若EEPROM 也出错，则初始化
***********************************************************/
/*static void RatioChecksumCompare(void)
{
	if(FALSE==IsCheckSumRight(run_data.ratio))
	{//若校验和出错则从EEPROM 重新读取变比
		if(FALSE==NvmBytesRead(EE_RATIO_ADDR, &run_data.ratio, sizeof(run_data.ratio)))
		{//若EEPROM 数据出错则对变比重新赋值
			run_data.ratio.vol_numerator=1; 
			run_data.ratio.vol_denominator=1;
			run_data.ratio.cur_numerator=1;
			run_data.ratio.cur_denominator=1; 
			run_data.ratio.checksum = 0;
		}
	}
}*/

/*********************************************************** 
函数功能：电能和余量的校验和检验
入口参数：
出口参数：
备	  注：RAM 校验和错误则从EEPROM 中恢复至RAM
                    若EEPROM 也出错，则清零
***********************************************************/
static BOOL_B L1EnergyChecksumCompare(void)
{
	if(FALSE==IsCheckSumRight(run_data.energy_all.energy_l1))
	{    //若RAM 的电能出错，则从EEPROM 中读出所有电能
		if(FALSE==NvmBytesRead(EE_ENERGY_L1_ADDR, &run_data.energy_all.energy_l1, sizeof(run_data.energy_all.energy_l1)))
		{    
			memset((uint8 *)&run_data.energy_all.energy_l1, 0, sizeof(run_data.energy_all.energy_l1));			
			return FALSE;
		}
	}
	if(FALSE==IsCheckSumRight(run_data.remainder_all.remain_l1))
	{    //若RAM 的余量出错，则从EEPROM 中读出所有余量
		if(FALSE==NvmBytesRead(EE_REMAINDER_L1_ADDR, &run_data.remainder_all.remain_l1, sizeof(run_data.remainder_all.remain_l1)))
		{    
			memset((uint8 *)&run_data.remainder_all.remain_l1, 0, sizeof(run_data.remainder_all.remain_l1));
		}
	}
	return TRUE;
}
  
/*********************************************************** 
函数功能：获取增量与保存余量
入口参数：能量余数，EMU测量值
出口参数：能量增量
备	  注：
***********************************************************/
static uint32 EnergyIncrementCalculation(uint8 *Remainder, int32 Source)
{
	uint32 energy_increment;

	energy_increment=(uint32)(((int32)Source+(int8)*Remainder)/IMPULSE_INC);
	//energy_increment=(uint32)((int32)energy_increment*METER_PRECISION);
	*Remainder=(uint8 )(((int32)Source+(int8)*Remainder)%IMPULSE_INC);
	return energy_increment;
}

/*External functions----------------------------------------------------------*/

/*********************************************************** 
函数功能：能量累计overall
入口参数：
出口参数：
备	  注：       Export  Function
***********************************************************/
void EnergyCalcProcess(void)
{
	uint32 Sour[ENERGY_TYPE_NUMBER];
	uint8 i, j;
	//uint8 tariff;
	uint8 act_mix_add_offset;
	//uint8 act_mix_sub_offset;
	//uint8 react_mix_add_offset;
	//uint8 react_mix_sub_offset;
	//uint8 apparent_offset;
	//uint8 react_q1_offset;
	uint32 increment;
	uint32 imp_const;
	uint32 Un, Imax;
	uint32 max_imp_allowed;
	tRemainder *pR;
	tENG *pE;
	tRunData run_data_temp;

	imp_const=Get_EMU_Measure_Data(SYS_PARAMETER_CONST_ACT);
	Un=Get_EMU_Measure_Data(SYS_PARAMETER_UN)/10;
	Imax=Get_EMU_Measure_Data(SYS_PARAMETER_IMAX)/1000;
	max_imp_allowed=(uint32)((uint64)Un*Imax*imp_const*4/3600000);

	for(i=0, j=ACTIVE_Energy_Pos_L1; i<ENERGY_TYPE_NUMBER-1; i++, j++)
	{
		Sour[i]=Get_EMU_Energy_Data((tEMUEnergyID)j);
	}
	Sour[ENERGY_TYPE_NUMBER-1]=Get_EMU_Energy_Data(ACTIVE_ADD_T_L1);
	
	for(i=0; i<ENERGY_TYPE_NUMBER; i++)
	{
		if(Sour[i] > max_imp_allowed)//4*Imax*Un*1< Sour/(imp_const/3600000)
		{
			return;
		}
	}
	
	/*tariff=TariffGetCurrentTariffNum();
	if((tariff<1)||(tariff>MAX_TARIFF))
	{
		tariff=1;
	}*/

	run_data_temp = run_data;

//---------------------------------L1---------------------------------//
	if(L1EnergyChecksumCompare())//在计算增量前先检查校验和是否正确
	{
		pE=&run_data.energy_all.energy_l1.act_pos;
		pR=&run_data.remainder_all.remain_l1.active_pos;
		act_mix_add_offset=(uint8)((tENG *)&run_data.energy_all.energy_l1.act_mix_add-(tENG *)&run_data.energy_all.energy_l1.act_pos);
		//act_mix_sub_offset=(uint8)((tENG *)&run_data.energy_all.energy_l1.act_mix_sub-(tENG *)&run_data.energy_all.energy_l1.act_pos);
		//react_mix_add_offset=(uint8)((tENG *)&run_data.energy_all.energy_l1.react_mix_add-(tENG *)&run_data.energy_all.energy_l1.act_pos);
		//react_mix_sub_offset=(uint8)((tENG *)&run_data.energy_all.energy_l1.react_mix_sub-(tENG *)&run_data.energy_all.energy_l1.act_pos);
		//apparent_offset=(uint8)((tENG *)&run_data.energy_all.energy_l1.apparent_pos - (tENG *)&run_data.energy_all.energy_l1.act_pos);
		//react_q1_offset=(uint8)((tENG *)&run_data.energy_all.energy_l1.react_q1-(tENG *)&run_data.energy_all.energy_l1.act_pos);

			//----------------active+, active-, reactive+, reactive- -----------------	
		for(i=0,j=ACTIVE_Energy_Pos_L1; j<=ACTIVE_Energy_Neg_L1; i++,j++)
		{
			increment=EnergyIncrementCalculation((uint8 *)&((pR+i)->remain_t[0]), Sour[j-6]);//总费率
			Add_Unsign_99((uint8 *)&((pE+i)->t[0]),increment,ENERGY_LENGTH);
			//increment=EnergyIncrementCalculation((uint8 *)&((pR+i)->remain_t[tariff]), Sour[j-2]);//分费率
			//Add_Unsign_99((uint8 *)&((pE+i)->t[tariff]),increment,ENERGY_LENGTH);
			
			CalcCheckSum(run_data.energy_all.energy_l1);//累加后计算并保存校验和
			CalcCheckSum(run_data.remainder_all.remain_l1);
		}

			//----------------quadrant reactive -----------------		
		/*for(i=react_q1_offset, j=REACTIVE_Energy_Q1_L1; j<=REACTIVE_Energy_Q4_L1; i++,j++)
		{
			increment=EnergyIncrementCalculation((uint8 *)&((pR+i)->remain_t[0]), Sour[j-2]);
			Add_Unsign_99((uint8 *)&((pE+i)->t[0]),increment,ENERGY_LENGTH);
			increment=EnergyIncrementCalculation((uint8 *)&((pR+i)->remain_t[tariff]), Sour[j-2]);
			Add_Unsign_99((uint8 *)&((pE+i)->t[tariff]),increment,ENERGY_LENGTH);

			CalcCheckSum(run_data.energy_all.energy_l1);//累加后计算并保存校验和
			CalcCheckSum(run_data.remainder_all.remain_l1);
		}*/

			//----------------active mix-----------------	
		increment=EnergyIncrementCalculation((uint8 *)&((pR+act_mix_add_offset)->remain_t[0]), Sour[ENERGY_TYPE_NUMBER-1]);//总费率
		Add_Unsign_99((uint8 *)&((pE+act_mix_add_offset)->t[0]),increment,ENERGY_LENGTH);//总费率组合有功+
		//increment=EnergyIncrementCalculation((uint8 *)&((pR+act_mix_add_offset)->remain_t[tariff]), Sour[ACTIVE_ADD_T_L1-2]);//分费率
		//Add_Unsign_99((uint8 *)&((pE+act_mix_add_offset)->t[tariff]),increment,ENERGY_LENGTH);//分费率组合有功+
		
		//increment=EnergyIncrementCalculation((uint8 *)&((pR+act_mix_sub_offset)->remain_t[0]), Sour[ACTIVE_SUB_T_L1-2]);//总费率
		//Add_Sign_79((int8 *)&((pE+act_mix_sub_offset)->t[0]),increment,ENERGY_LENGTH);//总费率组合有功-
		//increment=EnergyIncrementCalculation((uint8 *)&((pR+act_mix_sub_offset)->remain_t[tariff]), Sour[ACTIVE_SUB_T_L1-2]);//分费率
		//Add_Sign_79((int8 *)&((pE+act_mix_sub_offset)->t[tariff]),increment,ENERGY_LENGTH);//分费率组合有功-

		CalcCheckSum(run_data.energy_all.energy_l1);//累加后计算并保存校验和
		CalcCheckSum(run_data.remainder_all.remain_l1);

			//----------------reactive mix-----------------
		//increment=EnergyIncrementCalculation((uint8 *)&((pR+react_mix_add_offset)->remain_t[0]), Sour[REACTIVE_ADD_T_L1-2]);//总费率
		//Add_Unsign_99((uint8 *)&((pE+react_mix_add_offset)->t[0]),increment,ENERGY_LENGTH);//总费率组合无功+
		//increment=EnergyIncrementCalculation((uint8 *)&((pR+react_mix_add_offset)->remain_t[tariff]), Sour[REACTIVE_ADD_T_L1-2]);//分费率
		//Add_Unsign_99((uint8 *)&((pE+react_mix_add_offset)->t[tariff]),increment,ENERGY_LENGTH);//分费率组合无功+

		//increment=EnergyIncrementCalculation((uint8 *)&((pR+react_mix_sub_offset)->remain_t[0]), Sour[REACTIVE_SUB_T_L1-2]);//总费率
		//Add_Sign_79((int8 *)&((pE+react_mix_sub_offset)->t[0]),increment,ENERGY_LENGTH);//总费率组合无功-
		//increment=EnergyIncrementCalculation((uint8 *)&((pR+react_mix_sub_offset)->remain_t[tariff]), Sour[REACTIVE_SUB_T_L1-2]);//分费率
		//Add_Sign_79((int8 *)&((pE+react_mix_sub_offset)->t[tariff]),increment,ENERGY_LENGTH);//分费率组合无功-

		//CalcCheckSum(run_data.energy_all.energy_l1);//累加后计算并保存校验和
		//CalcCheckSum(run_data.remainder_all.remain_l1);

			//----------------apparent-----------------	
		//increment=EnergyIncrementCalculation((uint8 *)&((pR+apparent_offset)->remain_t[0]), Sour[APPARENT_Energy_L1-6]);//总费率
		//Add_Sign_79((int8 *)&((pE+apparent_offset)->t[0]),increment,ENERGY_LENGTH);//总费率视在
		//increment=EnergyIncrementCalculation((uint8 *)&((pR+apparent_offset)->remain_t[tariff]), Sour[APPARENT_Energy_L1-6]);//分费率
		//Add_Sign_79((int8 *)&((pE+apparent_offset)->t[tariff]),increment,ENERGY_LENGTH);//分费率视在

		//CalcCheckSum(run_data.energy_all.energy_l1);//累加后计算并保存校验和
		//CalcCheckSum(run_data.remainder_all.remain_l1);

	}

	if(memcmp(&run_data, &run_data_temp, sizeof(tRunData)) != 0)
	{
		save_flag=1;
	}
}

/*********************************************************** 
函数功能：当前能量的保存
入口参数：
出口参数：
备	  注：       
***********************************************************/
void SaveEnergy(void)
{
	if(IsCheckSumRight(run_data.energy_all.energy_l1))
	{
		NvmBytesWrite(EE_ENERGY_L1_ADDR, &run_data.energy_all.energy_l1, sizeof(run_data.energy_all.energy_l1));
	}
}

/*********************************************************** 
函数功能：当前余量的保存
入口参数：
出口参数：
备	  注：       
***********************************************************/
void SaveRemainder(void)
{
	if(IsCheckSumRight(run_data.remainder_all.remain_l1))
	{
		NvmBytesWrite(EE_REMAINDER_L1_ADDR, &run_data.remainder_all.remain_l1, sizeof(run_data.remainder_all.remain_l1));
	}
}

/*********************************************************** 
函数功能：电流变比与电压变比相乘
入口参数：
出口参数：
备	  注：       供需量模块调用
***********************************************************/
uint64 PowerRatioNumerator()
{
       return ((uint64)run_data.ratio.cur_numerator * run_data.ratio.vol_numerator);
}

uint64 PowerRatioDenominator()
{
       return ((uint64)run_data.ratio.cur_denominator * run_data.ratio.vol_denominator);
}

/*********************************************************** 
函数功能：当前电能相关的运行数据保存
入口参数：
出口参数：
备	  注：       Export Function
***********************************************************/
void SaveRundata(void)
{
	//RatioChecksumCompare();
	if(save_flag!=0)
	{
		SaveEnergy();
		SaveRemainder();
		//SaveRatio();		
		save_flag=0;
	}
}

/*********************************************************** 
函数功能：RAM 和EEPROM 的能量数据清零
入口参数：
出口参数：
备	  注：       Export  Function
***********************************************************/
void EnergyClear(void)
{
	memset((uint8 *)&run_data.energy_all.energy_l1.act_pos.t[0], 0, sizeof(run_data.energy_all));
	//NvmBytesWrite(EE_ENERGY_TOTAL_ADDR, &run_data.energy_all.energy_total, sizeof(run_data.energy_all.energy_total));
	NvmBytesWrite(EE_ENERGY_L1_ADDR, &run_data.energy_all.energy_l1, sizeof(run_data.energy_all.energy_l1));
	//NvmBytesWrite(EE_ENERGY_L2_ADDR, &run_data.energy_all.energy_l2, sizeof(run_data.energy_all.energy_l2));
	//NvmBytesWrite(EE_ENERGY_L3_ADDR, &run_data.energy_all.energy_l3, sizeof(run_data.energy_all.energy_l3));
	
	memset((uint8 *)&run_data.remainder_all.remain_l1.active_pos.remain_t[0], 0, sizeof(run_data.remainder_all));
	//NvmBytesWrite(EE_REMAINDER_TOTAL_ADDR, &run_data.remainder_all.remain_total, sizeof(run_data.remainder_all.remain_total));
	NvmBytesWrite(EE_REMAINDER_L1_ADDR, &run_data.remainder_all.remain_l1, sizeof(run_data.remainder_all.remain_l1));
	//NvmBytesWrite(EE_REMAINDER_L2_ADDR, &run_data.remainder_all.remain_l2, sizeof(run_data.remainder_all.remain_l2));
	//NvmBytesWrite(EE_REMAINDER_L3_ADDR, &run_data.remainder_all.remain_l3, sizeof(run_data.remainder_all.remain_l3));
}

/*********************************************************** 
函数功能：从EEPROM 恢复电能相关数据至RAM
入口参数：
出口参数：
备	  注：       Export  Function 电池模式调用
***********************************************************/
void EnergyDataInitBattery(void)
{
	Feed_watchdog();
	if(FALSE==NvmBytesRead(EE_ENERGY_L1_ADDR, &run_data.energy_all.energy_l1, sizeof(run_data.energy_all.energy_l1)))
	{
		memset(&run_data.energy_all.energy_l1, 0, sizeof(run_data.energy_all.energy_l1));
		energyEepError = 1;
	}
	Feed_watchdog();
	if(FALSE==NvmBytesRead(EE_REMAINDER_L1_ADDR, &run_data.remainder_all.remain_l1, sizeof(run_data.remainder_all.remain_l1)))
	{
		memset(&run_data.remainder_all.remain_l1, 0, sizeof(run_data.remainder_all.remain_l1));
		energyEepError = 1;
	}
	Feed_watchdog();
	//if(FALSE==NvmBytesRead(EE_RATIO_ADDR, &run_data.ratio, sizeof(run_data.ratio)))
	if(1)
	{
		run_data.ratio.vol_numerator=1;   //出错则赋值，防止死机
		run_data.ratio.vol_denominator=1;
		run_data.ratio.cur_numerator=1;
		run_data.ratio.cur_denominator=1; 
		run_data.ratio.checksum = 0;
	}
}

/*********************************************************** 
函数功能：从EEPROM 恢复电能相关数据至RAM，
入口参数：
出口参数：
备	  注：       Export  Function 上电模式调用
***********************************************************/
void CheckEnergyData(void)
{
	tRunData  run_datatemp;	
	uint32	  EnergySubb=0;

	memcpy((uint8 *)&run_datatemp.energy_all.energy_l1,(uint8 *)&run_data.energy_all.energy_l1,sizeof(run_datatemp.energy_all.energy_l1));

	if(FALSE==NvmBytesRead(EE_ENERGY_L1_ADDR, &run_data.energy_all.energy_l1, sizeof(run_data.energy_all.energy_l1)))
	{
		memset(&run_data.energy_all.energy_l1, 0, sizeof(run_data.energy_all.energy_l1));
		energyEepError = 1;
	}
	else
	{
		if(IsCheckSumRight(run_datatemp.energy_all.energy_l1))	 //15MIN 能量判断
		{ 		   
		    if(run_datatemp.energy_all.energy_l1.act_mix_add.t[0] >run_data.energy_all.energy_l1.act_mix_add.t[0])
			{
				EnergySubb =run_datatemp.energy_all.energy_l1.act_mix_add.t[0] -run_data.energy_all.energy_l1.act_mix_add.t[0];
				if(EnergySubb <MAX15MINENERGY) //差值应该不会很大.
				{
					memcpy((uint8 *)&run_data.energy_all.energy_l1,(uint8 *)&run_datatemp.energy_all.energy_l1,sizeof(run_datatemp.energy_all.energy_l1));
				}
			}
		}
	}
}


/*********************************************************** 
函数功能：从EEPROM 恢复电能相关数据至RAM
入口参数：
出口参数：
备	  注：       Export  Function 上电模式调用
***********************************************************/
uint16 EnergyDataInit(void)
{
	save_flag=0;
	if(PowerDownDetect()==1)//判断是否掉电
	{
		return POWER_UP_INIT_FAIL;
	}
	Feed_watchdog();
	/*if(FALSE==NvmBytesRead(EE_ENERGY_L1_ADDR, &run_data.energy_all.energy_l1, sizeof(run_data.energy_all.energy_l1)))
	{
		memset(&run_data.energy_all.energy_l1, 0, sizeof(run_data.energy_all.energy_l1));
		energyEepError = 1;
	}*/
	CheckEnergyData();       //20140730防止看门狗复位把电能清掉
	Feed_watchdog();
	if(PowerDownDetect()==1)//判断是否掉电
	{
		return POWER_UP_INIT_FAIL;
	}
	Feed_watchdog();
	if(PowerDownDetect()==1)//判断是否掉电
	{
		return POWER_UP_INIT_FAIL;
	}
	if(FALSE==NvmBytesRead(EE_REMAINDER_L1_ADDR, &run_data.remainder_all.remain_l1, sizeof(run_data.remainder_all.remain_l1)))
	{
		memset(&run_data.remainder_all.remain_l1, 0, sizeof(run_data.remainder_all.remain_l1));
		energyEepError = 1;
	}
	Feed_watchdog();
	//if(FALSE==NvmBytesRead(EE_RATIO_ADDR, &run_data.ratio, sizeof(run_data.ratio)))
	if(1)
	{
		run_data.ratio.vol_numerator=1;   //出错则赋值，防止死机
		run_data.ratio.vol_denominator=1;
		run_data.ratio.cur_numerator=1;
		run_data.ratio.cur_denominator=1; 
		run_data.ratio.checksum = 0;
	}
	TaskAdd(EnergyCalcProcess, 0, 1000, 1);
	TaskAdd(SaveRundata, 900000, 900000, 1);		//every 15Min to save
	return POWER_UP_INIT_SUCESS;
}

/*********************************************************** 
函数功能：掉电处理
入口参数：
出口参数：
备	  注：       Export  Function
***********************************************************/
void EnergyPowerOffProcess (void)
{
	if(save_flag!=0)
	{
		//SaveRundata();
		SaveEnergy();
		SaveRemainder();		
		save_flag=0;
	}
}


/*********************************************************** 
函数功能：电表清零记录信息，防止用户出现纠纷时用
入口参数：
出口参数：
备	  注：       Export  Function
***********************************************************/
void MeterClearRecord(void)
{
	 tDateTime DateTime; 
	 DRV_RTC_Get(&DateTime);
	 tEE_MeterClearRecord EE_MeterClearRecord;
	 if(NvmBytesRead(EE_METER_CLAER_RECORD_SAVE_ADDR, &EE_MeterClearRecord, sizeof(EE_MeterClearRecord)))
	 {
		 if(EE_MeterClearRecord.meterClearNum <= 9999)
		 {
			 EE_MeterClearRecord.meterClearNum++;
			 memcpy((uint8 *)&EE_MeterClearRecord.Time,(uint8 *)&DateTime,sizeof(EE_MeterClearRecord.Time));
			 memcpy((uint8 *)&EE_MeterClearRecord.meterClearEnergyData,(uint8 *)&run_data,sizeof(EE_MeterClearRecord.meterClearEnergyData));
			 NvmBytesWrite(EE_METER_CLAER_RECORD_SAVE_ADDR, &EE_MeterClearRecord, sizeof(EE_MeterClearRecord));
		 }
	 }
}

 /*********************************************************** 
 函数功能：电表清零记录信息初始化
 入口参数：
 出口参数：
 备    注： 	  Export  Function
 ***********************************************************/
void MeterClearRecordInit(void)
{
	tEE_MeterClearRecord EE_MeterClearRecord;
	memset((uint8 *)&EE_MeterClearRecord, 0 ,sizeof(EE_MeterClearRecord));
	NvmBytesWrite(EE_METER_CLAER_RECORD_SAVE_ADDR, &EE_MeterClearRecord, sizeof(EE_MeterClearRecord));
}

 /*********************************************************** 
 函数名称：EnergyParaGet()
 函数功能：
 入口参数：DataID, pBuf
 出口参数：length, error_type
 备    注：       Export  Function,  读任意一项当前电能数
                             据(包括属性2 和属性3)时调用
 ***********************************************************/
uint16 EnergyParaGet(uint32 DataID, uint8 *pBuf)
{
	uint8 i;	
	uint16 length;
	uint16 temp1, temp2, temp3;
	uint32 *addr;
	tEnergyInfo EnergyInfoTemp;
	
	temp1=(uint8)(DataID>>16);//type
	temp2=(uint8)(DataID>>8);//tariff
	temp3=(uint8)DataID;//attribute

	if(temp2>MAX_TARIFF)
	{
	       return (0x8000+2);
	}

	for(i=0;i<sizeof(EnergyInfo)/sizeof(tEnergyInfo);i++)
	{
		if(temp1==EnergyInfo[i].Id)
		{
			EnergyInfoTemp=EnergyInfo[i];
			break;
		}
	}	

	if(temp3==0x02)
	{
		addr=EnergyInfoTemp.RAM_Addr+temp2;
	       *(uint32 *)pBuf=(*addr) * ((uint64)run_data.ratio.vol_numerator*run_data.ratio.cur_numerator)/((uint64)run_data.ratio.vol_denominator*run_data.ratio.cur_denominator);//scaler=1
	       length=(uint16)EnergyInfoTemp.Len;
	}
	else if(temp3==0x03)
	{
	       *pBuf=EnergyInfoTemp.scaler;
		*(pBuf+1)=EnergyInfoTemp.unit;
		length=0x02;
	}
	else
	{
		return (0x8000+1);
	}
	return length;
}

 /*********************************************************** 
 函数名称：VariableParaGet()
 函数功能：
 入口参数：DataID, pBuf
 出口参数：length, error_type
 备    注：读功率因数，频率，合相、L1、L2、L3的
                      电压、电流、正向、反向、有功、
                      无功功率，相角等瞬时量调用
 ***********************************************************/
uint16 VariableParaGet(uint32 DataID, uint8 *pBuf)
{
       uint8 temp1, temp2;
	uint8 i;
	uint8 length;
	int32 vari;
	tVariableInfo VariableInfoTemp;

	temp1=(uint8)(DataID>>8);
	temp2=(uint8)DataID;

	for(i=0;i<sizeof(VariableInfo)/sizeof(tVariableInfo);i++)
	{
		if(temp1==VariableInfo[i].Id)
		{
			VariableInfoTemp=VariableInfo[i];
			break;
		}
	}

	if(temp2 == 0x02)
	{
		vari=Get_EMU_Measure_Data((tMeasureID)(VariableInfoTemp.Vari_type));
	       //if(VariableInfoTemp.Vari_type==ACTIVE_POWER_L1)
	       if((VariableInfoTemp.Vari_type==ACTIVE_POWER_T)||(VariableInfoTemp.Vari_type==APPARENT_POWER_T))
	       {
	       	*(uint32 *)pBuf=(uint32)((int64)vari*(run_data.ratio.vol_numerator*run_data.ratio.cur_numerator)/((int64)run_data.ratio.vol_denominator*run_data.ratio.cur_denominator*10));/////////// 1/100 = 10^1/10^3     (EMU: XXXXX.XXX)
	       }
		else if((VariableInfoTemp.Vari_type==I_RMS_L1) || (VariableInfoTemp.Vari_type==I_RMS_L2))//current
		{
		       *(uint32 *)pBuf=(uint32)((int64)vari*run_data.ratio.cur_numerator/((int64)(run_data.ratio.cur_denominator)));////////////// 1/10 = 10^3/10^4   (EMU: XXXX.XXXX)
		}
	       else if(VariableInfoTemp.Vari_type==V_RMS_L1)//voltage
	       {
		   	*(uint16 *)pBuf=(uint16)((int64)vari*run_data.ratio.vol_numerator/((int64)(run_data.ratio.vol_denominator*10)));///////////// 1/1000 = 10^1/10^4   (EMU: XXXX.XXXX)
	       }
		else if(VariableInfoTemp.Vari_type==FREQUENCY_T)//frequency
		{
		   	*(uint16 *)pBuf=(uint16)vari;/////////////////////// 1/100 = 10^2/10^4   (EMU: XX.XXXX)
		}
		else if(VariableInfoTemp.Vari_type==PF_T)//power factor
		{
		   	*(uint16 *)pBuf=(uint16)vari;// 1 = 10^3/10^3   (EMU: X.XXX)
		}
		else
		{
			return (0x8000+1);
		}
		length=VariableInfoTemp.Len;
	}

	else if(temp2 == 0x03)
	{
	       *pBuf=VariableInfoTemp.scaler;
		*(pBuf+1)=VariableInfoTemp.unit;
		length=0x02;
	}

	else
	{
		return (0x8000+1);
	}
	
	return length;
}
 
