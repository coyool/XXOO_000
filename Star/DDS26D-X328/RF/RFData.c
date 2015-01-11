/*******************************************************************
* Copyright (c) 2015,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：RFData.c
* 文件标识：见配置管理计划书 
* 摘要：RF抄读数据项

* 当前版本：1.0.0 
* 作者：冯发侨
* 完成日期：20150111
*******************************************************************/
#include "Include.h"

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

static uint8 GetFreezeData(uint8 *Buf)
{
	uint8 temp[64] = {0};
	uint8 dataType = 0;
	uint8 result = 0;
	uint32 energy = 0;
	
	temp[0] = (*(Buf+1))&0x3F;
	dataType = (*(Buf+1))>>6;
	if(temp[0] > MaxSaveDays)
    	return result;
    if(temp[0]==0)
    {
    	return result;
    }
	API_GetDataValue(HisDayEng_IDCode,temp);
	switch(dataType)
	{
		case 1:
			memcpy(temp, temp+20, 4); //组合能量
			result +=4;
			break;
		case 2:
			memcpy(temp, temp+12, 4);  //正向能量
			result +=4;
			break;
		case 3:
			memcpy(temp, temp+16, 4);  //反向能量
			result +=4;
			break;
		default:
			break;
	}
    memcpy((uint32 *)&energy, (uint8 *)temp, result);
	HEX_TO_BCD(energy,result,temp,LowToHigh);
	temp[4] = ReadFreezeRFID;
	result +=1;
	
	memcpy(Buf, temp, sizeof(temp));
	return result;
}

static uint8 GetMonthBillingData(uint8 *Buf)
{
	uint8 temp[64] = {0};
	uint8 dataType = 0;
	uint8 result = 0;
	uint32 energy = 0;
	
	temp[0] = (*(Buf+1))&0x3F;
	dataType = (*(Buf+1))>>6;
	if(temp[0] > MaxSaveMonths)
    	return result;
    if(temp[0]==0)
    {
    	return result;
    }
	API_GetDataValue(HisMonthEng_IDCode,temp);
	switch(dataType)
	{
		case 1:
			memcpy(temp, temp+20, 4); //组合能量
			result +=4;
			break;
		case 2:
			memcpy(temp, temp+12, 4);  //正向能量
			result +=4;
			break;
		case 3:
			memcpy(temp, temp+16, 4);  //反向能量
			result +=4;
			break;
		default:
			break;
	}
    memcpy((uint32 *)&energy, (uint8 *)temp, result);
	HEX_TO_BCD(energy,result,temp,LowToHigh);
	temp[4] = ReadBillingRFID;
	result +=1;
	
	memcpy(Buf, temp, sizeof(temp));
	return result;
}

static uint8 GetTimeData(uint8 *Buf)
{
    uint8 result = 0;
	uint32 temp = 0;
	tDateTime  Time;
    
	API_GetDataValue(Clock_IDCode,(uint8 *)&Time);
    temp = DateTimeToSeconds(&Time);
	SecondsToDateTime(&Time,temp);
	memcpy((uint8 *)Buf, (uint32 *)&temp, sizeof(temp));
    result +=5;
    return result;
}

static uint8 GetGeographyData(uint8 *Buf)
{
	uint8 result = 0;
	uint8 temp[64] = {0};
	uint8 type = 0;

	type = *(Buf+1);
	API_GetDataValue(CurrentGeographicPos_IDCode,(uint8 *)temp); 
	switch(type)
	{
		case 0:
			memcpy(Buf, temp, 4); 	//经度
			result +=4;
			break;
		case 1:
			memcpy(Buf, temp+4, 4); //纬度
			result +=4;
			break;
		case 2:
			memcpy(Buf, temp+8, 3); //高度
			result +=3;
			break;
		default:
			break;
	}
	return result;
}

static uint8 GetCurrentEnergyData(uint8 *Buf)
{
	uint8 result = 0;
	uint8 temp[64] = {0};
	flag state = {0};
	uint32 energy=0;
	
	result = API_GetDataValue(AllEng_IDCode,temp);
	memcpy((uint32 *)&energy, (uint8 *)temp, result);
	HEX_TO_BCD(energy,result,temp,LowToHigh);
    state.bits.bit0 = REVERSEFLAG;
    state.bits.bit1 = CURRENTIMBALANCEFLAG;
    state.bits.bit3 = BATTERYLOWFLAG;  
    state.bits.bit7 = 1;          //新X328表
	temp[4] = state.byte;
 	memcpy(Buf, temp, sizeof(temp));
	result +=1;
	return result;
}

static uint8 GetVolCurData(uint8 *Buf)
{
	uint8 result = 0;
	uint8 temp[64] = {0};
	uint16 vol=0;
	uint32 cur=0;

	API_GetDataValue(CurrentVoltage_IDCode,(uint8 *)&vol);
	vol = vol*10;
	memcpy((uint8 *)temp,(uint8 *)&vol, sizeof(vol));
	result +=2;
	API_GetDataValue(CurrentCurrent_IDCode,(uint8 *)&cur);
	cur = cur/10;
	memcpy((uint8 *)temp+2,(uint8 *)&cur, sizeof(cur));
	result +=4;
	memcpy(Buf, temp, sizeof(temp));
    return result;
}

static uint8 GetPowerData(uint8 *Buf)
{
	uint8 result = 0;
	uint8 temp[64] = {0};
	uint32 power;

	API_GetDataValue(CurrentPower_IDCode,(uint8 *)&power);
	memcpy((uint8 *)temp,(uint8 *)&power, sizeof(power));
	memcpy(Buf, temp, sizeof(temp));
	result +=3;
    return result;
}

static uint8 GetPowerFactorData(uint8 *Buf)
{
	uint8 result = 0;
	uint8 temp[64] = {0};
	uint16 powerfactor;
	
	API_GetDataValue(CurrentPowerFactor_IDCode,(uint8 *)&powerfactor);
	powerfactor=powerfactor/10;
	memcpy((uint8 *)temp,(uint8 *)&powerfactor, sizeof(powerfactor));
	result +=1;
	memcpy(Buf, temp, sizeof(temp));
	return result;
}

static uint8 GetStealCountData(uint8 *Buf)
{
	uint8 result = 0;
	uint8 temp[64] = {0};
	uint16 stealcount;

	API_GetDataValue(CheatEnergyNum_IDCode,(uint8 *)&stealcount);
	memcpy((uint8 *)temp,(uint8 *)&stealcount, sizeof(stealcount));
	result +=2;
	memcpy(Buf, temp, sizeof(temp));
	return result;
}	

static uint8 SetMeterAddress(uint8 *Buf)
{
	uint8 result = 0;
	tMeterImportInfo MeterImportInfo;
	 
	if (!IO_ReadGPIOPin(IO_PORT_PRO_SWI,IO_PIN_PRO_SWI))
	{
		memset((uint8 *)&MeterImportInfo,0,sizeof(MeterImportInfo));
		memcpy(MeterImportInfo.MeterAddr+2, Buf, METER_ADDR_LEN);
		API_SetDataValue(CurrentADDR_IDCode,(uint8 *)&MeterImportInfo.MeterAddr,METER_ADDR_LEN);
		result = 1;
	}
	return result;
	
}

static uint8 SetBroadcastAdjustTime(uint8 *Buf)
{
	uint8 result = 0;
	uint8 temp[64] = {0};
	uint32 second = 0;
	tDateTime  Time;

	memcpy(temp, Buf, sizeof(temp));
	memcpy((uint8 *)&second, (uint8 *)&temp[6], sizeof(second));
	SecondsToDateTime(&Time,second);
	if ((temp[1]==0x99) && (temp[2]==0x99) 
         && (temp[3]==0x99) && (temp[4]==0x99))
	{
		result = API_SetDataValue(ClockBroadcast_IDCode,(uint8 *)&Time,sizeof(Time));
	}
	else
	{
		result = API_SetDataValue(Clock_IDCode,(uint8 *)&Time,sizeof(Time));
	}
	if(result !=DA_SUCCESS)
	{
		return 0; 
	}
	else
	{
		return 1; 
	}
	
}

static uint8 SetGeographyData(uint8 *Buf)
{
	uint8 result = 0;
	uint8 temp[64] = {0};
	uint8 type = 0;

	type = *(Buf+1);
	API_GetDataValue(CurrentGeographicPos_IDCode,(uint8 *)temp); 
	switch(type)
	{
		case 0:
			memcpy(temp, Buf+2, 4); 	//经度
			result +=4;
			break;
		case 1:
			memcpy(temp, Buf+2, 4); //纬度
			result +=4;
			break;
		case 2:
			memcpy(temp, Buf+2, 3); //高度
			result +=3;
			break;
		default:
			break;
	}
	API_SetDataValue(CurrentGeographicPos_IDCode,Buf,11);   
	return result;
	
}


//Public=========================================================================
uint8 RFGetData(uint8 RFDI,uint8 *pBuf)
{
	uint8 result = 0;
	
	switch(RFDI)
	{
		case ReadFreezeRFID:
			result = GetFreezeData(pBuf);
			break;
		case ReadBillingRFID:
			result = GetMonthBillingData(pBuf);
			break;
		case ReadTimeRFID:
			result = GetTimeData(pBuf);
			break;
		case ReadGeographyRFID:
			result = GetGeographyData(pBuf);
			break;
		case ReadBroadcastEnergyRFID:
		case ReadCurrentEnergy1RFID:
		case ReadCurrentEnergyRFID:
			result = GetCurrentEnergyData(pBuf);
			break;
		case ReadVoltageCurrentRFID:
			result = GetVolCurData(pBuf);
			break;
		case ReadPowerRFID:
			result = GetPowerData(pBuf);
			break;
		case ReadPowerFactorRFID:
			result = GetPowerFactorData(pBuf);
			break;
        case ReadStealCountRFID:
			result = GetStealCountData(pBuf);
			break;
		default:
			break;
	}
  	return result;  
}

uint8 RFSetData(uint8 RFDI,uint8 *pBuf)
{
	uint8 result = 0;
	switch(RFDI)
	{
		case WriteMeterIDRFID:
			result = SetMeterAddress(pBuf);
			break;
		case WriteBroadcastAdjustTime:
			result = SetBroadcastAdjustTime(pBuf);
			break;
		case WriteGeographyRFID:
			result = SetGeographyData(pBuf);
			break;
		case ReadDisplayMeterIDRFID:
			DisplayAlarm(DIS_METER_NO,TRUE); 
            break;
		default:
			break;
	}
	return result;
}
