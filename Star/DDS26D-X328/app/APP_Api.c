/*****************************************************************************************
*  	Function:  应用程序接口单元，
*   
*    Author:	冯秋雄
*    Created on:	2013-11-18 
*****************************************************************************************/
#include "Include.h"


/*Export functions-------------------------------------------*/

/*****************************************************************************************
函数名称: SetDataValue
入口参数: DataId: 1. 设置电表数据：需设置的数据ID(自定义短名，包括属性)
                  2. 执行电表对象方法：需执行方法的数据ID(自定义短名)
          pDataBuf指向的缓存区存放需要设置的数据
出口参数: 
返回参数：1. 设置电表数据：0-数据设置成功；非0-返回数据设置失败原因
          2. 执行电表对象方法：0-方法执行成功；1-方法执行失败
函数功能：接口类设置电表数据，和执行电表对象方法.
*****************************************************************************************/
uint8 API_SetDataValue(uint32 DataId, void *pDataBuf, uint16 len)
{
	uint8 ClassId = (uint8)(DataId >> 24);
	uint8 Result = DA_READ_WRITE_DENIED;
	uint16 Item = (uint16)(DataId>>8);

	switch(ClassId)
	{
		case 1:
		case 15:
			if(Item == 0x9000)
            {
               Result = DispParaSet(DataId, pDataBuf, len);
            } 
            else
            {
               Result = MeterInfoParaSet(DataId, pDataBuf, len);
            }  
			break;
		case 3:
//			Result = EventParaSet(DataId, pDataBuf, len);
			break;
		case 5:
//			Result = DemandParaSet(DataId, pDataBuf);
			break;
		case 7:
			if(Item == 0x0200)		//load profile
			{
//				Result = LoadProfileParaSet(DataId, pDataBuf, len);
			}
			else if(Item == 0x1000)	//auto dispaly
			{
				Result = DispParaSet(DataId, pDataBuf, len);
			}
			break;
		case 8:
			Result = Clock_ParaSet(DataId, pDataBuf, len);
			break;
		case 9:
			if(Item == 0x0600)
			{
//				Result = EventParaSet(DataId, pDataBuf, len);
			}
			else
			{
//				Result = BillingParaSet(DataId, pDataBuf, len);
			}
			break;
		case 11:
		case 20:
//			Result = TariffParaSet(DataId, pDataBuf, len);
			break;
		case 18:
//			Result = IAP_ParaSet(DataId, pDataBuf, len);
			break;
		case 22:
//			Result = BillingParaSet(DataId, pDataBuf, len);
			break;
		default:
			break;
	}
	/*
	if((Result == DA_SUCCESS) && (DataId != 0x09060081) && (DataId != 0x09050081) && (DataId != 0x01A10002)
		&& ((DataId&0xff000000) != 18))		//电表与事件清零不记编程事件,程序升级不记事件
	{
		if(DLMS_HDLC_GetFirstConnectFlag())
		{
			DLMS_HDLC_ClearFirstConnectFlag();
			EventInsertLog(EVENT_PRAOGRAMMING, (uint8*)&DataId);
		}
	}
	*/
	return Result;
}

/*****************************************************************************************
函数名称: GetDataValue
入口参数: DataId: 代表需读取的数据ID(用OBIS码对应的自定义短名)
          pDataBuf指向的缓存区可以根据读取数据的需要存放相关的参数.
            返回的数据以相应的形式存放在pDataBuf所指向的缓存。
出口参数:  	
返回参数：返回数据的长度, 若出错则最高位置1返回相应的错误原因。
函数功能：接口类读取电表数据.
*****************************************************************************************/
uint16 API_GetDataValue(uint32 DataId, void *pDataBuf)
{
	uint16 Result = 0x8000+DA_OBJECT_UNDEFINED;
	uint16 Item = (uint16)(DataId >> 8);
	uint8 Class = (uint8)(DataId >> 24);
//	uint8 Temp;
	
	switch(Class)
	{
		case 1:
			if((uint8)DataId != 2)
			{
				break;
			}
			if((Item >= 0x8000) && (Item <= 0x8500))		//event counter
			{
				Result = EventParaGet(DataId, pDataBuf);
			}
		        else if(Item == 0x9000)
			{
				Result = DispParaGet(DataId, pDataBuf);
			}
			else if(Item == 0xC000)	//power direction
			{
				
			}
			else if((Item == 0x7100) || ((Item >= 0xD000) && (Item <= 0xD00C)))	//current tariff
			{
//				Result = TariffParaGet(DataId, pDataBuf);
			}
			else if((Item == 0x7200) || (Item == 0x7201))	// 显示用的时间日期
			{
				Result = Clock_ParaGet(DataId, pDataBuf);
			}
			else		//meter information
			{
				Result = MeterInfoParaGet(DataId, pDataBuf);
			}
			break;
		case 3:
/*			Temp = BillingIsDataIdBillingPeriod(DataId);
			if(Temp == 1)
			{
				Result = BillingParaGet(DataId, pDataBuf);
				break;
			}
			else if(Temp ==  2)
			{
				DataId &= 0xffff0fff;		//读属性的则直接去电能单元读取
			}
			*/
			if((Item >= 0xA001) && (Item <= 0xA004))
			{
				Result = MeterInfoParaGet(DataId, pDataBuf);
			}
			else if((Item >= 0xB001) && (Item <= 0xC001))	//voltage sag and swell parameter, and duration
			{
                Result = EventParaGet(DataId, pDataBuf);
			}
            else if(Item==0xc100)
           	{
                Result = BattVoltageDataGet(DataId, pDataBuf);
			}
            else	//energy & variable
			{
				if(Item>=0x8000 && Item<0xa001)
				{
				    Result = VariableParaGet(DataId, pDataBuf);
				}
				else if(Item<0x8000)
				{
					Result = EnergyParaGet(DataId, pDataBuf); 
				}
			}
			break;
		case 4:
//			Result = BillingParaGet(DataId, pDataBuf);
			break;
		case 5:
//			Result = DemandParaGet(DataId, pDataBuf);
			break;
		case 7:
			switch(Item)
			{
				case 0x0000:
					Result = BillingParaGet(DataId, pDataBuf);
					break;
				case 0x0200:		//负荷曲线
//					Result = LoadProfileParaGet(DataId, pDataBuf);
					break;
				case 0x0100:		//日冻结数据
				case 0x0300:		//整点冻结数据
					Result = FreezeParaGet(DataId, pDataBuf);
					break;
				case 0x1000:		//显示
                   Result = DispParaGet(DataId, pDataBuf);
					break;
				default:			//事件记录
					Result = EventParaGet(DataId, pDataBuf);
					break;
			}
			break;
		case 8:		//Clock
			Result = Clock_ParaGet(DataId, pDataBuf);
			break;
/*		case 9:
			if(Item == 0x0000)	//tariff script
			{
				Result = TariffParaGet(DataId, pDataBuf);
			}
			else if(Item == 0x0400)	//清最大需量脚本
			{
				Result = BillingParaGet(DataId, pDataBuf);
			}
			break;
			*/
		case 11:
		case 20:	
//			Result = TariffParaGet(DataId, pDataBuf);
			break;
		case 15:
		case 19:
			Result = MeterInfoParaGet(DataId, pDataBuf);
			break;
		case 18:
//			Result = IAP_ParaGet(DataId, pDataBuf);
			break;
		case 22:
//			Result = BillingParaGet(DataId, pDataBuf);
			break;
		default:
			break;
	}
	return Result;
}

