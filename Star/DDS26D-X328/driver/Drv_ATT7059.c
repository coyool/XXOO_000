/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：Drv_att7059s.c
* 文件标识：
* 摘要：att7059s计量驱动

* 当前版本：1.0.0 
* 作者：ffq
* 完成日期：20140516
*******************************************************************/
#include "Include.h"

/****************************************************************/
//--------------  计量芯片寄存器地址--------------------
/****************************************************************/

/****************************************************************/
//-----------------  EMU Uart通讯相关-------------------------
/****************************************************************/
//EMU通讯命令队列
const EMU_CS EMU_CS_BUFF[2]=
{
	{UART_EMU_L1,0x6A},
	{UART_EMU_L2,0x6A},
};

const EMU_CMD EMU_CMD_BUFF[EMU_Regisetr_NUM]=
{
	{EMU_L1,PowerP1_R,3},		//PowerP			0x26//4byte
	{EMU_L1,SUMChecksum_R,3}, 	//checksum			0x18//4byte
	{EMU_L1,Rms_U_R,3},			//PowerP			0x26//4byte
	{EMU_L1,Rms_I1_R,3},		//PowerP			0x26//4byte   
	{EMU_L1,PowerQ1_R,3},		//PowerP			0x26//4byte
    {EMU_L1,Power_S_R,3},		//PowerP			0x26//4byte
    {EMU_L1,Freq_U_R,3},		//PowerP			0x26//4byte
    
	{EMU_L2,PowerP1_R,3},		//PowerP			0x26//4byte
	{EMU_L2,SUMChecksum_R,3}, 	//checksum			0x18//4byte
	{EMU_L2,Rms_U_R,3},			//PowerP			0x26//4byte
	{EMU_L2,Rms_I1_R,3},		//PowerP			0x26//4byte
	{EMU_L2,PowerQ1_R,3},		//PowerP			0x26//4byte
	{EMU_L2,Power_S_R,3},
	{EMU_L2,Freq_U_R,3},		//PowerP			0x26//4byte
};

/**************************************************************/
//校表参数寄存器列表用于计算EMU校验和40H-7C
//参数和校验寄存器为所有校表参数寄存器的和，40h-7ch。其中
//其中连续地址中没有分配寄存器的部分不计算之内，default 0X0100BD (72H-74h不计算在内   ）
//计算方法 三字节无符号加法，不足三字节的高位补零，
/*************************************************************/
const uint32 EMU_REG_DATA[EMU_regSum]=
{
	0X00000000,//EMUCFG-- 0X40	1
	0X00000088,//FreqCFG--0X41
	0X0000007E,//MODULEEN--0X42
	0X00000003,//ANAEN--0X43
	0X00000000,//IOCFG--0X45
	
	0X00000000,//GP1 --0X50		1
	0X00000000,//GQ1--0X51		1
	0X00000000,//GS1-0X52		1
	0X00000000,//GP2--0X54		1
	0X00000000,//GQ2-0X55		1
	
	0X00000000,//GS2--0X56		1
	0X0000FF00,//QPHSCAL--0X58
	0X00000000,//ADCCON--0X59	1
	0X00000000,//I2GAIN-0X5B
	0X00000000,//I1OFF--0X5C
	
	0X00000000,//I2OFF-0X5D
	0X00000000,//UOFF-0X5E
	0X00000040,//PQSTART-0X5F	1//
	0X00000040,//HFCONST-0X61	1//
	0X00000010,//CHK-0X62
	
	0X00000020,//IPTAMP-0X63
	0X00000000,//P1OFFSETH-0X65	1
	0X00000000,//P2OFFSETH-0X66	1
	0X00000000,//QIOFFSETH--0X67	
	0X00000000,//Q2FFSETH--0X68
	
	0X00000000,//I1RMSOFFSET-0X69
	0X00000000,//I2RMSOFFSET -0X6A
	0X00000004,//ZCROSSCURRENT-0X6C
	0X00000000,//GPHS1--0X6D	1
	0X00000000,//GPHS2--0X6E	1
	
	0X00000000,//PFCNT--0X6F
	0X00000000,//QFCNT--0X70
	0X00000000,//SFCNT--0X71
	0X00000000,//MODECFG-0X75
	0X00000000,//P1OFFSETL-0X76	1
	
	0X00000000,//P2OFFSETL-0X77	1
	0X00000000,//Q1OFFSETL-0X78
	0x00000000,//Q2OFFSETL-0X79
	0X00000000,//UPEAKLV1-0X7A
	0X00000000,//USAGLV1-0X7B
	
	0X00000000,//UCYCLEN-0X7C
};


EMU_COMM EMUcommBuffer;//EMU当前通讯缓存

/****************************************************************/
//-------------- 读EMU结构体定义--------------------
/****************************************************************/
EMU_ENERGY EMU_energy;
EMU_VARIABLE EMU_variable;
tEE_CALIBRATE_METER Calibrate_meter;
EMU_REGISTER EMU_Register;
EMU_PARAMETER  EMU_Parmeter;
EMU_CALI_CHECKSUM EMU_CheckSum;

/****************************************************************/
//--------------  脉冲合成及输出--------------------------
/****************************************************************/
#define	PulseConst			 1600	    //脉冲常数
#define	PulseWidth			 240		//PulseWidth_60MS: 240			  
#define	SamplePerSecond		 4000	
#define OnePulsePower    	(((uint64)SamplePerSecond*3600*1000*100)/1600)//0.01W.250us
#define	EMU_Ib				 5          //5.000A
#define	PulseConst001		 PulseConst/100
#define	EMU_Imax			 100	    //20.000A
#define	EMU_Un				 220        //220.00V      
#define	EMU_MaxOneHourEng	((uint32)EMU_Imax*EMU_Un/500000)
#define	EMU_MaxPower		((uint64)60192*100)//XXXXXX.xx W 220V*1.9*100A*1.2*1.2
#define EMU_PowerStar       ((uint32)(EMU_Ib*EMU_Un*2*100)/1000)// 启动功率 2.20 w 两位小数点扩大100倍
#define EMU_RePowerStar     ((uint32)(EMU_Ib*EMU_Un*4*100)/1000)// 启动功率 2.20 w 两位小数点扩大100倍
#define	EMU_PulseConst		1000000 //脉冲常数,能量最小单位0.001wh

/****************************************************************/
//-------------- 电表状态及标志--------------------------
/****************************************************************/
uint8	EMU_err_ct[2]={0};
/****************************************************************/
//-------------- 瞬时量参数系数--------------------------
/****************************************************************/
#define	CONST_V		1//
#define	CONST_I		2//
#define	CONST_P		460928 //56250*10^6/2^23EC*HF    


/* Private functions-------------------------------------------------------------------*/
/*******************************************************************************
 * @function_name: String_Copy_L2H
 * @function_file: drv_ATT7059.C
 * @描述：字符串的拷贝程序
 * @入口参数: PPuB_StrOrigin
 * @出口参数: PPuB_StrReturn
 * @Attention: 数据 高低地址对调
 ******************************************************************************/
static void String_Copy_L2H(uint8 *PPuB_StrReturn,uint8 *PPuB_StrOrigin,uint8 PuB_Cnt)
{                                                                          
	uint8 i;                                                           
	for(i=0;i<PuB_Cnt;i++)                                             
	{                                                          
		*(PPuB_StrReturn+i)=*(PPuB_StrOrigin+PuB_Cnt-i-1);                   
	}                                                          
}  

long labs(long i)
{      /* compute absolute value of long argument */
	return (i < 0 ? -i : i);
}

/*******************************************************************************
 * @function_name: DRV_EMU_IOInit
 * @function_file: drv_ATT7059.C
 * @描述：
 * @入口参数: 
 * @出口参数: 
 * @Attention: 上电就开始配置IO高，防止上电就出脉冲
 *---------------------------------------------------------
 ******************************************************************************/
static void DRV_EMU_IOInit(void)
{
	//----------------Active Pulse LED--------------------
	IO_DisableFunc(IO_PORT_LED_ACT,IO_PIN_LED_ACT);//P55
  	IO_ConfigGPIOPin(IO_PORT_LED_ACT,IO_PIN_LED_ACT,IO_DIR_OUTPUT,IO_PULLUP_OFF);
	IO_WriteGPIOPin(IO_PORT_LED_ACT,IO_PIN_LED_ACT,IO_BIT_SET);//输出高电平
	//----------------Reactive Pulse LED--------------------
	//IO_DisableFunc(IO_PORT_LED_REA,IO_PIN_LED_REA);//P56
	//IO_ConfigGPIOPin(IO_PORT_LED_REA,IO_PIN_LED_REA,IO_DIR_OUTPUT,IO_PULLUP_OFF);
	//IO_WriteGPIOPin(IO_PORT_LED_REA,IO_PIN_LED_REA,IO_BIT_SET);//输出高电平
	//----------------EMU L1 ZX -------------------	
}


/*******************************************************************************
 * @function_name: EMUCheckSum
 * @function_file: drv_ATT7059.C
 * @描述：EMU检验和计算
 * @入口参数： 数据,长度
 * @出口参数: sum
 * @Attention:  sum=~(RAM[0]+RAM[1]+...)
 ******************************************************************************/
static uint8 EMUCheckSum(uint8 *RAM, uint8 len)
{
	uint8 temp1=0,i;

	for(i=0;i<len;i++)
	{
		temp1+=RAM[i]; 
	}
	temp1=~temp1;
	return temp1;
}


/*******************************************************************************
 * @function_name: Check_ATT7059_Calibrate
 * @function_file: drv_ATT7059.C
 * @描述：ATT7059修调参数校验
 * @入口参数：
 * @出口参数: 
 * @Attention:  
 ******************************************************************************/
static uint8 Check_ATT7059_Calibrate(EMU_NO EMU_NUM)
{
	uint16 sum=0,EMUStatus=0;
	switch(EMU_NUM)
	{
		case EMU_L1:
			sum = CALC_CHECKSUM((uint8 *)&Calibrate_meter.Cali_EMU_L1, sizeof(Calibrate_meter.Cali_EMU_L1)-2);
			EMUStatus=Calibrate_meter.Cali_EMU_L1.ChkSum_7059;
			break;
		case EMU_L2:
			sum = CALC_CHECKSUM((uint8 *)&Calibrate_meter.Cali_EMU_L2, sizeof(Calibrate_meter.Cali_EMU_L2)-2);
			EMUStatus=Calibrate_meter.Cali_EMU_L2.ChkSum_7059;
			break;
	}
	if(sum == EMUStatus)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/*******************************************************************************
 * @function_name: EMUUartAutoSendTask
 * @function_file: drv_ATT7059.C
 * @描述：发送
 * @入口参数： 
 * @出口参数:  无
 * @Attention:	 当准备好发送后定时发送
 ******************************************************************************/
static void EMUUartAutoSendTask(void)
{
	if(PowerDownDetect()!=0)
	{
		return;
	}
	//时标
	EMUcommBuffer.timeMark++;
	//定时启动发送
	if(EMUcommBuffer.holdeState!=HOLDE)
	{
		if((LDRV_UsartGetStateRx(EMUcommBuffer.ch) == IDLE)
			&&(LDRV_UsartGetStateTx(EMUcommBuffer.ch) == IDLE))
		{
			LDRV_UsartSend(EMUcommBuffer.ch,EMUcommBuffer.bufferTx,EMUcommBuffer.lenTx);	
		}
	}
}


/*******************************************************************************
 * @function_name: DRV_EMU_Initial
 * @function_file: drv_ATT7059.C
 * @描述：计量模块修调数据初始化
 * @入口参数：
 * @出口参数: 
 * @Attention:   用于导入计量模块修调数据
 ******************************************************************************/
static void DRV_EMU_Initial(EMU_NO EMU_NUM)
{
	uint8 i;
	uint8 m=0;
	uint32 backup_temp11=0;
	uint32 backup_temp=0;
	Feed_watchdog();
	switch(EMU_NUM)
	{
		case EMU_L1:
			if(!NvmBytesRead(EE_CALIBRATE_ADDR_L1, &Calibrate_meter.Cali_EMU_L1, sizeof(Calibrate_meter.Cali_EMU_L1)))
			{
				caliEepError = 1;
				EMUUartAutoPause();
				for(i=0;i<2;i++)
				{    Feed_watchdog();
					EMURead(ChipIDr_R,3,(uint8 *)&backup_temp11,EMU_L1);
					EMURead(BackupData_R,3,(uint8 *)&backup_temp,EMU_L1);
					backup_temp11&=0x00ffff00;//只比较高2字节，防止芯片升级版本20140429
					backup_temp&=0x00ffff00;
					if(backup_temp11==backup_temp)
					{
						if(backup_temp11==0x00705300)
						{
						}
						else
						{
							m++;
						}
					}		
				}
				EMUUartAutoGoOn();
				if(m==2)
				{
					return;
				}
				Initial_Average_Calibrate_Data(EMU_NUM);
			}
			else
			{
				EMUUartAutoPause();
				for(i=0;i<2;i++)
				{
					EMURead(ChipIDr_R,3,(uint8 *)&backup_temp11,EMU_L1);
					EMURead(BackupData_R,3,(uint8 *)&backup_temp,EMU_L1);
					backup_temp11&=0x00ffff00;//只比较高2字节，防止芯片升级版本20140429
					backup_temp&=0x00ffff00;
					if(backup_temp11==backup_temp)
					{
						if(backup_temp11==0x00705300)
						{
						}
						else
						{
							m++;
						}
					}		
				}
				EMUUartAutoGoOn();
				if(m==2)
				{
					return;
				}
				
				if(!Set_ATT7059(EMU_NUM))
				{					
					Feed_watchdog();
					if(!Set_ATT7059(EMU_NUM))
					{	
					     Feed_watchdog();				
						if(!Set_ATT7059(EMU_NUM))
						{
							;
						}
					}
				}			
			}
			EMU_Cali_CheckSum_L1((uint32 *)EMU_REG_DATA,EMU_regSum);
			break;
		case EMU_L2:
			if(!NvmBytesRead(EE_CALIBRATE_ADDR_L2, &Calibrate_meter.Cali_EMU_L2, sizeof(Calibrate_meter.Cali_EMU_L2)))
			{
				caliEepError = 1;
				EMUUartAutoPause();
				for(i=0;i<2;i++)
				{
					EMURead(ChipIDr_R,3,(uint8 *)&backup_temp11,EMU_L2);
					EMURead(BackupData_R,3,(uint8 *)&backup_temp,EMU_L2);
					backup_temp11&=0x00ffff00;//只比较高2字节，防止芯片升级版本20140429
					backup_temp&=0x00ffff00;
					if(backup_temp11==backup_temp)
					{
						if(backup_temp11==0x00705300)
						{
						}
						else
						{
							m++;
						}
					}		
				}
				EMUUartAutoGoOn();
				if(m==2)
				{
					return;
				}
				Initial_Average_Calibrate_Data(EMU_NUM);
			}
			else
			{
				EMUUartAutoPause();
				for(i=0;i<2;i++)
				{
					EMURead(ChipIDr_R,3,(uint8 *)&backup_temp11,EMU_L2);
					EMURead(BackupData_R,3,(uint8 *)&backup_temp,EMU_L2);
					backup_temp11&=0x00ffff00;//只比较高2字节，防止芯片升级版本20140429
					backup_temp&=0x00ffff00;
					if(backup_temp11==backup_temp)
					{
						if(backup_temp11==0x00705300)
						{
						}
						else
						{
							m++;
						}
					}		
				}
				EMUUartAutoGoOn();
				if(m==2)
				{
					return;
				}
				if(!Set_ATT7059(EMU_NUM))    
				{					
					Feed_watchdog();
					if(!Set_ATT7059(EMU_NUM))
					{	
					     Feed_watchdog();				
						if(!Set_ATT7059(EMU_NUM))
						{
							;
						}
					}
				}
			}
			EMU_Cali_CheckSum_L2((uint32 *)EMU_REG_DATA,EMU_regSum);
			break;
	}
}


/*******************************************************************************
 * @function_name: EMUUartAutoProTask
 * @function_file: drv_ATT7059.C
 * @描述：自动读取处理任务
 * @入口参数： 
 * @出口参数:  无
 * @Attention:	自动读取到的数据处理,装载下一个读取命令,命令指针下移
                通讯帧异常侦听, 返回通讯状态
 ******************************************************************************/
uint8 sendcnt=0;
static void EMUUartAutoProTask(void)
{
	uint8 temp[10];
	uint8 EMU_CMD=0;
	EMU_NO EMU_NUM;
	uint32 Register_Data=0;
	uint16 lenth=0;
    
    Feed_watchdog() ;
	if(PowerDownDetect()==1)
	{
		return;
	}     
	if(LDRV_UsartRecv(EMUcommBuffer.ch))
	{
		switch(EMUcommBuffer.ch)
		{
			case UART_EMU_L1:
				EMU_NUM=EMU_L1;
				break;
			case UART_EMU_L2:
				EMU_NUM=EMU_L2;
				break;
		}
		
		//判断数据校验和,搬移数据
		memset(temp, 0, 10);
		memcpy(temp, EMUcommBuffer.bufferTx, 2);
		memcpy(temp+2, EMUcommBuffer.bufferRx, EMUcommBuffer.lenRx);
		if(EMUCheckSum(temp, EMUcommBuffer.lenTx+EMUcommBuffer.lenRx-1) == temp[EMUcommBuffer.lenTx+EMUcommBuffer.lenRx-1])
		{
			EMU_Register.EMU_Uart_Err_Cnt[EMU_NUM]<<=1;
			EMU_Register.EMU_Uart_Err_Cnt[EMU_NUM]&=~0x01;
			EMU_Register.EMU_Uart_Err_Cnt[EMU_NUM]&=0xff;
			if(EMU_Register.EMU_Uart_Err_Cnt[EMU_NUM]==0)
			{
				if(EMU_Register.EMU_Uart_Err_ing[EMU_NUM]==true)		
				{
					EMU_Register.EMU_Uart_Ok_Recover[EMU_NUM]=true; //恢复后需要重新载入校表值
					EMU_Register.EMU_Uart_Err_ing[EMU_NUM]=false;	  //从通讯异常中恢复标志
				}
			}
            EMU_Register.EMU_Uart_Tx_Err_Time[EMU_NUM] =0;
            Register_Data=0;
			if(EMUcommBuffer.commP>0)
			{
				EMU_CMD=(EMU_CMD_BUFF[EMUcommBuffer.commP-1].cmd);
				lenth =(EMU_CMD_BUFF[EMUcommBuffer.commP-1].len);
			}
			else
			{
				EMU_CMD=(EMU_CMD_BUFF[EMUcommBuffer.commP+EMU_Regisetr_NUM-1].cmd);
				lenth =(EMU_CMD_BUFF[EMUcommBuffer.commP+EMU_Regisetr_NUM-1].len);
			}
			memcpy(((uint8 *)&Register_Data)+1, EMUcommBuffer.bufferRx,3);		
			String_Copy_L2H((uint8 *)&Register_Data,((uint8 *)&EMUcommBuffer.bufferRx[0]) ,EMUcommBuffer.lenRx-1);
			switch(lenth)
			{
				case 1:
					Register_Data&=0x000000ff;
					break;
				case 2:
					Register_Data&=0x0000ffff;
					break;
				case 3:
					Register_Data&=0x00ffffff;
					break;
			}			
			switch(EMU_CMD)
			{
				case ADCCON_W://0x00,2byte
					EMU_Register.ADCCON_7059[EMU_NUM]=(uint16)Register_Data;
					break;
				case HFconst_W://0x02,2byte
					EMU_Register.HFConst_7059[EMU_NUM]=(uint16)Register_Data;
					break;
				case PFCnt_W://0x20,2byte
					//EMU_Register.PFCnt[EMU_NUM]=(uint16)Register_Data;
					break;
				case QFCnt_W://0x21,2byte
					//EMU_Register.QFCnt[EMU_NUM]=(uint16)Register_Data;
					break;
				case Rms_I1_R://0x22,3byte
					//EMU_Register.IRMS[EMU_NUM][1]=EMU_Register.IRMS[EMU_NUM][2];
					//EMU_Register.IRMS[EMU_NUM][2]=Register_Data;
					//if((EMU_Register.IRMS[EMU_NUM][2]>>8)==(EMU_Register.IRMS[EMU_NUM][1]>>8))
					//{
					//	EMU_Register.IRMS[EMU_NUM][0]=EMU_Register.IRMS[EMU_NUM][2];
					//}
					EMU_Register.IRMS[EMU_NUM][0]=Register_Data;
					break;
				case Rms_U_R://0x24,3byte
					//EMU_Register.URMS[EMU_NUM][1]=EMU_Register.URMS[EMU_NUM][2];
					//EMU_Register.URMS[EMU_NUM][2]=Register_Data;
					//if((EMU_Register.URMS[EMU_NUM][2]>>8)==(EMU_Register.URMS[EMU_NUM][1]>>8))
					//{
					//	EMU_Register.URMS[EMU_NUM][0]=EMU_Register.URMS[EMU_NUM][2];
					//}
					EMU_Register.URMS[EMU_NUM][0]=Register_Data;
					break;
				case Freq_U_R://0x25,2byte
					//EMU_Register.UFreq[EMU_NUM][1]=EMU_Register.UFreq[EMU_NUM][2];
					//EMU_Register.UFreq[EMU_NUM][2]=(uint16)Register_Data;
					//if((EMU_Register.UFreq[EMU_NUM][2]>>8)==(EMU_Register.UFreq[EMU_NUM][1]>>8))
					//{
					//	EMU_Register.UFreq[EMU_NUM][0]=EMU_Register.UFreq[EMU_NUM][2];
					//}
					EMU_Register.UFreq[EMU_NUM][0]=Register_Data;
					break;
				case PowerP1_R://0x26,4byte,active power 
					//EMU_Register.PowerP[EMU_NUM][1]=EMU_Register.PowerP[EMU_NUM][2];
					//EMU_Register.PowerP[EMU_NUM][2]=Register_Data;
					//if((EMU_Register.PowerP[EMU_NUM][2]>>8)==(EMU_Register.PowerP[EMU_NUM][1]>>8))
					//{
					//	EMU_Register.PowerP[EMU_NUM][0]=EMU_Register.PowerP[EMU_NUM][2];
					//}
					EMU_Register.PowerP[EMU_NUM][0]=Register_Data;
					break;
				case SUMChecksum_R://0x18,3byte
					//EMU_Register.CheckSum[EMU_NUM][1]=EMU_Register.CheckSum[EMU_NUM][2];
					//EMU_Register.CheckSum[EMU_NUM][2]=Register_Data;
					//if((EMU_Register.CheckSum[EMU_NUM][2]>>8)==(EMU_Register.CheckSum[EMU_NUM][1]>>8))
					//{
					//	EMU_Register.CheckSum[EMU_NUM][0]=EMU_Register.CheckSum[EMU_NUM][2];
					//
					//}
					EMU_Register.CheckSum[EMU_NUM][0] = Register_Data;
					break;
				case PowerQ1_R://0x28,4byte,reactive power 
					//EMU_Register.PowerQ[EMU_NUM][1]=EMU_Register.PowerQ[EMU_NUM][2];
					//EMU_Register.PowerQ[EMU_NUM][2]=Register_Data;
					//if((EMU_Register.PowerQ[EMU_NUM][2]>>8)==(EMU_Register.PowerQ[EMU_NUM][1]>>8))
					//{
					//	EMU_Register.PowerQ[EMU_NUM][0]=EMU_Register.PowerQ[EMU_NUM][2];
					//}
					EMU_Register.PowerQ[EMU_NUM][0]=Register_Data;
					break;
				case Power_S_R://0x28,4byte,reactive power 
					//EMU_Register.PowerS[EMU_NUM][1]=EMU_Register.PowerS[EMU_NUM][2];
					//EMU_Register.PowerS[EMU_NUM][2]=Register_Data;
					//if((EMU_Register.PowerS[EMU_NUM][2]>>8)==(EMU_Register.PowerS[EMU_NUM][1]>>8))
					//{
					//	EMU_Register.PowerS[EMU_NUM][0]=EMU_Register.PowerS[EMU_NUM][2];
					//}
					EMU_Register.PowerS[EMU_NUM][0]=Register_Data;
					break;                    
				case Energy_P_R://0x29,3byte
					//EMU_Register.EnergyP[EMU_NUM][0][0]=Register_Data;
					//EMU_Register.EnergyP[EMU_NUM][0][1]=EMUcommBuffer.timeMark;
					EMU_Register.EnergyP[EMU_NUM][1][0]=EMU_Register.EnergyP[EMU_NUM][2][0];
					EMU_Register.EnergyP[EMU_NUM][2][0]=Register_Data;
					EMU_Register.EnergyP[EMU_NUM][2][1]=EMUcommBuffer.timeMark;
					if((EMU_Register.EnergyP[EMU_NUM][2][0]>>8)==(EMU_Register.EnergyP[EMU_NUM][1][0]>>8))
					{
						EMU_Register.EnergyP[EMU_NUM][0][0]=EMU_Register.EnergyP[EMU_NUM][2][0];
						EMU_Register.EnergyP[EMU_NUM][0][1]=EMU_Register.EnergyP[EMU_NUM][2][1];
					}
					break;	
				//case Energy_P_R://0x2a,3byte
				//	EMU_Register.EnergyP2[EMU_NUM]=Register_Data;
				//	break;
				case Energy_Q_R://0x2b,3byte
					//EMU_Register.EnergyQ[EMU_NUM][0][0]=Register_Data;
					//EMU_Register.EnergyQ[EMU_NUM][0][1]=EMUcommBuffer.timeMark;
					EMU_Register.EnergyQ[EMU_NUM][1][0]=EMU_Register.EnergyQ[EMU_NUM][2][0];
					EMU_Register.EnergyQ[EMU_NUM][2][0]=Register_Data;
					EMU_Register.EnergyQ[EMU_NUM][2][1]=EMUcommBuffer.timeMark;
					if((EMU_Register.EnergyQ[EMU_NUM][2][0]>>8)==(EMU_Register.EnergyQ[EMU_NUM][1][0]>>8))
					{
						EMU_Register.EnergyQ[EMU_NUM][0][0]=EMU_Register.EnergyQ[EMU_NUM][2][0];
						EMU_Register.EnergyQ[EMU_NUM][0][1]=EMU_Register.EnergyQ[EMU_NUM][2][1];
					}
					break;
				case EMUSR_R://0x2d,3byte
					//EMU_Register.EMUStatus[EMU_NUM][1]=EMU_Register.EMUStatus[EMU_NUM][2];
					//EMU_Register.EMUStatus[EMU_NUM][2]=Register_Data;
					//if((EMU_Register.EMUStatus[EMU_NUM][2]>>8)==(EMU_Register.EMUStatus[EMU_NUM][1]>>8))
					//{
					//	EMU_Register.EMUStatus[EMU_NUM][0]=EMU_Register.EMUStatus[EMU_NUM][2];
					//}
					EMU_Register.EMUStatus[EMU_NUM][0]=Register_Data;
					break;					
				case EMUSR_R+1://未用 
					EMU_Register.SysStatus[EMU_NUM]=(uint8)Register_Data;
					break;	
				case ChipIDr_R://0x7f,3byte
					EMU_Register.DeviceID[EMU_NUM]=Register_Data;
					break;			
				}
		}
		else
		{
			PublicDelayUs(1);
		}
		//装载下一个读取命令
		EMUcommBuffer.lenTx=2;
		EMUcommBuffer.bufferTx[0]=EMU_CS_BUFF[EMU_CMD_BUFF[EMUcommBuffer.commP].NO].chipID;
		EMUcommBuffer.bufferTx[1]=EMU_CMD_BUFF[EMUcommBuffer.commP].cmd&0x7f;
		EMUcommBuffer.lenRx=4;	//len+sum
		EMUcommBuffer.ch=EMU_CS_BUFF[EMU_CMD_BUFF[EMUcommBuffer.commP].NO].ch;
		LDRV_UsartOpen(EMUcommBuffer.ch, USART_8E1,BAUDE_4800BPS, EMUcommBuffer.bufferRx, EMUcommBuffer.lenRx, FullDouble);
		//读取指针下移
		EMUcommBuffer.commP++;
		if(EMUcommBuffer.commP>=EMU_Regisetr_NUM)
		{
			EMUcommBuffer.commP=0;
		}
	}
	else
	{
        switch(EMUcommBuffer.ch)
		{
			case UART_EMU_L1:
				EMU_NUM=EMU_L1;
				break;
			case UART_EMU_L2:
				EMU_NUM=EMU_L2;
				break;
		}
		if(EMU_Register.EMU_Uart_Tx_Err_Time[EMU_NUM]<UART_ERR_MAX)
		{
			EMU_Register.EMU_Uart_Tx_Err_Time[EMU_NUM]++;			//计量通讯不通讯超时
		}
        else
		{
			EMU_Register.EMU_Uart_Tx_Err_Time[EMU_NUM]=0;
			EMU_Register.EMU_Uart_Err_Cnt[EMU_NUM]<<=1;
			EMU_Register.EMU_Uart_Err_Cnt[EMU_NUM]|=0x01;
			if((EMU_Register.EMU_Uart_Err_Cnt[EMU_NUM]&0xff)==0x0f)
			{
				switch(EMUcommBuffer.ch)
				{
					case UART_EMU_L1:
						EMU_Register.EMU_Uart_Err_ing[EMU_NUM]=true; //不通讯累计次数达到,说明通讯有问题
						break;
					case UART_EMU_L2:
						EMU_Register.EMU_Uart_Err_ing[EMU_NUM]=true;;
						break;
				}
			}          
			//装载下一个读取命令
            EMUcommBuffer.lenTx=2;
            EMUcommBuffer.bufferTx[0]=EMU_CS_BUFF[EMU_CMD_BUFF[EMUcommBuffer.commP].NO].chipID;
            EMUcommBuffer.bufferTx[1]=EMU_CMD_BUFF[EMUcommBuffer.commP].cmd&0x7f;
            EMUcommBuffer.lenRx=4;	//len+sum
            EMUcommBuffer.ch=EMU_CS_BUFF[EMU_CMD_BUFF[EMUcommBuffer.commP].NO].ch;
            LDRV_UsartOpen(EMUcommBuffer.ch, USART_8E1,BAUDE_4800BPS, EMUcommBuffer.bufferRx, EMUcommBuffer.lenRx, FullDouble);
            //读取指针下移
            EMUcommBuffer.commP++;
            if(EMUcommBuffer.commP>=EMU_Regisetr_NUM)
            {
                EMUcommBuffer.commP=0;
            }
        }
	}
}

/*******************************************************************************
 * @function_name: EMUUartAutoStart
 * @function_file: drv_ATT7059.C
 * @描述：自动读取起动
 * @入口参数： 
 * @出口参数:  无
 * @Attention:	上电初始化时调用
 ******************************************************************************/
static void EMUUartAutoStart(void)
{
	//清零
	memset((uint8 *)&EMUcommBuffer, 0, sizeof(EMUcommBuffer));
	//装载下一个读取命令
	EMUcommBuffer.lenTx=2;
	EMUcommBuffer.bufferTx[0]=EMU_CS_BUFF[EMU_CMD_BUFF[EMUcommBuffer.commP].NO].chipID;
	EMUcommBuffer.bufferTx[1]=EMU_CMD_BUFF[EMUcommBuffer.commP].cmd&0x7f;
	EMUcommBuffer.lenRx=4;	//len+sum
	EMUcommBuffer.ch=EMU_CS_BUFF[EMU_CMD_BUFF[EMUcommBuffer.commP].NO].ch;
	LDRV_UsartOpen(EMUcommBuffer.ch, USART_8E1,BAUDE_4800BPS, EMUcommBuffer.bufferRx, EMUcommBuffer.lenRx, FullDouble);
	EMUcommBuffer.holdeState=GO_ON;	
	
	EMUcommBuffer.commP++;
	if(EMUcommBuffer.commP>=EMU_Regisetr_NUM)
	{
		EMUcommBuffer.commP=0;
	}
}

/*******************************************************************************
 * @function_name: DRV_EMU_250us_Energy_Pulse
 * @function_file: drv_ATT7059.C
 * @描述：合成合相有功脉冲和无功脉冲
 * @入口参数：基本定时器溢出终端回调函数 250us
 * @出口参数:  无
 * @Attention: 注意此断程序的运行时间，大电流时灯闪烁情况 
 ******************************************************************************/
static void DRV_EMU_250us_Energy_Pulse(void)
{
    if(lvdIntFlag)
    {
        return;
    }
//---------Measure Ative energy Pulse--------
	EMU_energy.AddedPowerAct+= EMU_energy.PowerActPerSecond;
	if(EMU_energy.AddedPowerAct>=OnePulsePower)//判断累加的功率是否大于脉冲当量
	{
		EMU_energy.AddedPowerAct-= OnePulsePower;
		EMU_energy.PulseWidthCountAct=PulseWidth;
		EMU_energy.pulse_ct_act++;
		EMU_energy.Active_Add_T[1]++;              
        if(EMU_variable.EMU_Power_Direction[0] == 1)
       	{
        	EMU_energy.Rail_Nega_Active[1]++;
        }
        else
        {
        	EMU_energy.Rail_Plus_Active[1]++;
        }	
	}	
//---------Measure Reative energy Pulse--------
	//EMU_energy.AddedPowerRea+= EMU_energy.PowerReaPerSecond;
	//if(EMU_energy.AddedPowerRea>=OnePulsePower)
	//{
	//	EMU_energy.AddedPowerRea-= OnePulsePower;
	//	EMU_energy.PulseWidthCountRea=PulseWidth;
	//	EMU_energy.pulse_ct_react++;
	//}

	if(EMU_energy.PulseWidthCountAct)	
	{
		EMU_energy.PulseWidthCountAct--;
        FM3_GPIO->PDOR5&= ~IO_PINx5;
	}
	else	
	{
		FM3_GPIO->PDOR5|= IO_PINx5;
	}
	//if(EMU_energy.PulseWidthCountRea)
	//{
	//	EMU_energy.PulseWidthCountRea--;
	//	FM3_GPIO->PDOR5&= ~IO_PINx6;
	//}
	//else
	//{
		//FM3_GPIO->PDOR5|= IO_PINx6;
	//}
}

/*******************************************************************************
 * @function_name: DRV_BT_250us_Init
 * @function_file: drv_ATT7059.C
 * @描述：多功能定时器初始化 250us
 * @入口参数：
 * @出口参数:  无
 * @Attention:   要求使用最高中断优先级，根据主频变化计数值
 ******************************************************************************/
static void DRV_EMU_250us_Init(void)
{	
	/* Set clock division to 1, freq = SystemCoreClock/1 */
	MFT_FRTSetClkDiv(MFT_UNIT0, FRT_CH2, FRT_DIV1); 
	/* Set the free run timer mode */
	MFT_FrtSetCntMode(MFT_UNIT0, FRT_CH2, CNT_MODE_UP);
	/* Set the free run timer cycle */                      
	MFT_FRTSetCycleValBuf(MFT_UNIT0, FRT_CH2, 3999); /*n(s) = SystemCoreClock*Cycle 9000*/
	/* Clear interrupt flag and Enable free run timer peak interrupt */
	MFT_FRTClrPeakMatchIntFlag(MFT_UNIT0, FRT_CH2);
	MFT_FRTEnablePeakMatchInt(MFT_UNIT0, FRT_CH2, DRV_EMU_250us_Energy_Pulse);
	/* Enable free run timer IRQ */
	NVIC_EnableIRQ(FRTIM_IRQn);
	/* Start free run timer */
	MFT_FRTCancelStateInit(MFT_UNIT0, FRT_CH2);
	//MFT_FRTStart(MFT_UNIT0, FRT_CH2);
}


/*******************************************************************************
 * @function_name: 
 * @function_file: 
 * @描述：     比较两路,（功率或者电流）确定当前计量通道机窃电状态
 * @入口参数：
 * @出口参数: 
 * @Attention:  
 ******************************************************************************/
static void Process_compare_I_P(void)
{
	uint32 Imbalance_threshold;

   	EMU_variable.Measure_Chnsel=false;
   	EMU_variable.Current_Unbalanced=false;      
  	if((EMU_variable.Curr[1]<EMU_Parmeter.Itamp)&&(EMU_variable.Curr[2]<EMU_Parmeter.Itamp)) 
	{												//默认第一路计量,平衡
		EMU_variable.Measure_Chnsel=false;
		EMU_variable.Current_Unbalanced=false;
	}
	else
	{	
		if(EMU_variable.Curr[1]>EMU_variable.Curr[2])	
		{
			Imbalance_threshold=EMU_variable.Curr[1]>>EMU_Parmeter.Ichk;
		}
		else
		{
			Imbalance_threshold=EMU_variable.Curr[2]>>EMU_Parmeter.Ichk;
		}
		if((labs(EMU_variable.Curr[1]-EMU_variable.Curr[2]))>=Imbalance_threshold)
		{
			if(EMU_variable.Curr[1]>EMU_variable.Curr[2])	
			{											//第一路计量
				EMU_variable.Measure_Chnsel=false;		
			}
			else
			{											//第二路计量
				EMU_variable.Measure_Chnsel=true;			
			}
		}
		if(EMU_Parmeter.TampMode) //判断两值比例关系
		{  																//窃电事件还是按照 常规设计来做
																		//两路都小于10A是，门限为1A；
			if((EMU_variable.Curr[1]<EMU_Parmeter.CurrentHighthreshold)&&(EMU_variable.Curr[2]<EMU_Parmeter.CurrentHighthreshold)) 
			{
  				if((labs(EMU_variable.Curr[1]-EMU_variable.Curr[2]))>=EMU_Parmeter.CurrentLowthreshold)
				{
					EMU_variable.Current_Unbalanced=true;;
				}
				else
				{
				    EMU_variable.Current_Unbalanced=false;
				}
			}
			else
			{
				if((labs(EMU_variable.Curr[1]-EMU_variable.Curr[2]))>=Imbalance_threshold)
				{
					EMU_variable.Current_Unbalanced=true;	
				}
				else
				{
					EMU_variable.Current_Unbalanced=false;
				}			
			}		
		}
		else //固定值关系 
		{											//两路电流相差1A，就认为发生了不平衡 
		    if((labs(EMU_variable.Curr[1]-EMU_variable.Curr[2]))>=EMU_Parmeter.CurrentLowthreshold)
			{
				EMU_variable.Current_Unbalanced=true;
			}
			else
			{
				EMU_variable.Current_Unbalanced=false;
			}
		} 	
   	}			      
}

/*******************************************************************************
 * @function_name: 
 * @function_file: 
 * @描述：   根据配置的参数来决定当前能量累加方式
 * @入口参数：
 * @出口参数: 确认当前计量通道模式, 是否处于窃电方式
 * @Attention:  
 ******************************************************************************/
static void Process_Tample_mode(void)
{

	  EMU_variable.Current_Unbalanced=false;
	  EMU_variable.Measure_Chnsel=false;
	  //当前累加方式 I1  I2  I1+I2 或者防窃电方式
      if(EMU_Parmeter.EnergyChnsel==1)  //I1 路计量
      {
		EMU_energy.PowerActPerSecond=EMU_variable.Act_Power[1];
		EMU_energy.PowerReaPerSecond=EMU_variable.React_Power[1];
		Process_compare_I_P();
		EMU_variable.Measure_Chnsel=false;
      }
      else if(EMU_Parmeter.EnergyChnsel==2)  //I2 路计量
      {
 		EMU_energy.PowerActPerSecond=EMU_variable.Act_Power[2];
		EMU_energy.PowerReaPerSecond=EMU_variable.React_Power[2];		
		EMU_variable.Measure_Chnsel=true;
      }
      else if(EMU_Parmeter.EnergyChnsel==3)  //I1+I2 计量
      {
 		EMU_energy.PowerActPerSecond=EMU_variable.Act_Power[1]+EMU_variable.Act_Power[2];
		EMU_energy.PowerReaPerSecond=EMU_variable.React_Power[1]+EMU_variable.React_Power[2]; 
		EMU_variable.Measure_Chnsel=true; //两路合成液认为是第一路
      } 
      else   //防窃电方式计量
      {
         Process_compare_I_P();
		 if(EMU_variable.Measure_Chnsel) //第二路
		 {
			 EMU_energy.PowerActPerSecond=EMU_variable.Act_Power[2];
			 EMU_energy.PowerReaPerSecond=EMU_variable.React_Power[2];
		 }
		 else
		 {
			 EMU_energy.PowerActPerSecond=EMU_variable.Act_Power[1];
			 EMU_energy.PowerReaPerSecond=EMU_variable.React_Power[1];
		 }            
      } 
	   if(EMU_variable.Measure_Chnsel) //第二路
	   {
			EMU_variable.Curr[0]=EMU_variable.Curr[2];
		 	EMU_variable.Volt[0]=EMU_variable.Volt[2];
			EMU_variable.Freq[0]=EMU_variable.Freq[2];
			EMU_variable.Factor[0]=EMU_variable.Factor[2];
			EMU_variable.Freq[0]=EMU_variable.Freq[2];
	    	if(EMU_Parmeter.EnergyChnsel==3)  //I1+I2 计量
	  		{
				EMU_variable.Act_Power[0]=EMU_variable.Act_Power[1]+EMU_variable.Act_Power[2];
				EMU_variable.React_Power[0]=EMU_variable.React_Power[1]+EMU_variable.React_Power[2];
				EMU_variable.Apt_Power[0]=EMU_variable.Apt_Power[1]+EMU_variable.Apt_Power[2];
	  		}
			else
			{
			    EMU_variable.Act_Power[0]=EMU_variable.Act_Power[2];;
		  		EMU_variable.React_Power[0]=EMU_variable.React_Power[2];
				EMU_variable.Apt_Power[0]=EMU_variable.Apt_Power[2];
	  		}
	   }
	   else
	   {
			EMU_variable.Curr[0]=EMU_variable.Curr[1];;
			EMU_variable.Volt[0]=EMU_variable.Volt[1];
			EMU_variable.Freq[0]=EMU_variable.Freq[1];
			EMU_variable.Factor[0]=EMU_variable.Factor[1];
			EMU_variable.Freq[0]=EMU_variable.Freq[1];
			EMU_variable.Act_Power[0]=EMU_variable.Act_Power[1];;
			EMU_variable.React_Power[0]=EMU_variable.React_Power[1];
			EMU_variable.Apt_Power[0]=EMU_variable.Apt_Power[1];
	   }
	   if((EMU_variable.Measure_Chnsel == true)&&(EMU_variable.EMU_Power_Direction[2]==1))
	   	{
			EMU_variable.EMU_Power_Direction[0]= 1;
	   	}
	   else if((EMU_variable.Measure_Chnsel == false)&&(EMU_variable.EMU_Power_Direction[1]==1))
	   	{
			EMU_variable.EMU_Power_Direction[0]= 1;
	    }
	   else
	   	{
			EMU_variable.EMU_Power_Direction[0]= 0;
	   	}
}

/*******************************************************************************
 * @function_name: Process_ATT7059_I_V_F
 * @function_file: drv_ATT7059.C
 * @描述：处理ATT7059电压、电流、频率
 * @入口参数：
 * @出口参数: 
 * @Attention:  
 ******************************************************************************/
static void Process_ATT7059_I_V_F(EMU_NO EMU_NUM)
{
	uint32 Urms=0, ATT7059_U_temp=0, Frq=0, ATT7059_Fr_temp=0; //Irms=0,ATT7059_I_temp=0,

	if(PowerDownDetect()==1)
	{
		return;
	}	
	Urms=EMU_Register.URMS[EMU_NUM][0];
	//Irms=EMU_Register.IRMS[EMU_NUM][0];
	Frq=EMU_Register.UFreq[EMU_NUM][0];
	ATT7059_U_temp=Urms;
	//ATT7059_I_temp=Irms;
    if(Frq)
    {  
        ATT7059_Fr_temp=50000000/Frq;//XX.xxxHz  //femu/(2*UFREQf) =1MHZ /(2*UFREQf) 
    }
    else
    {
        ATT7059_Fr_temp =Calibrate_meter.Cali_Par.Freq;
    }  
	switch(EMU_NUM)
	{
		case EMU_L1://L相  Voltage & Current
			EMU_variable.Volt[1]=ATT7059_U_temp*100l/Calibrate_meter.Cali_EMU_L1.URMS_refer_7059;
			//EMU_variable.Curr[1]=ATT7059_I_temp*1000l/Calibrate_meter.Cali_EMU_L1.IRMS_refer_7059;
			//=========================================================
			//优化小电流的值
			EMU_variable.Curr[1] = 0;
			if(EMU_variable.Volt[1])
			{
				EMU_variable.Curr[1] = (uint32)(CalAppentPower(EMU_variable.Act_Power[1],EMU_variable.React_Power[1])*1000/EMU_variable.Volt[1]);
			}
			//=========================================================
			EMU_variable.Freq[1]=ATT7059_Fr_temp;
			if((EMU_variable.Act_Power[1]<EMU_PowerStar)&&(EMU_variable.React_Power[1]<EMU_RePowerStar))
			{
				EMU_variable.Curr[1]=0;
			}	
			break;
		case EMU_L2://N相 Voltage & Current
			EMU_variable.Volt[2]=ATT7059_U_temp*100l/Calibrate_meter.Cali_EMU_L2.URMS_refer_7059;
			//EMU_variable.Curr[2]=ATT7059_I_temp*1000l/Calibrate_meter.Cali_EMU_L2.IRMS_refer_7059;
			//=========================================================
			//优化小电流的值
			EMU_variable.Curr[2] = 0;
			if(EMU_variable.Volt[2])
			{
				EMU_variable.Curr[2] = (uint32)(CalAppentPower(EMU_variable.Act_Power[2],EMU_variable.React_Power[2])*1000/EMU_variable.Volt[2]);
			}
			//=========================================================
			EMU_variable.Freq[2]=ATT7059_Fr_temp;
			if((EMU_variable.Act_Power[2]<EMU_PowerStar)&&(EMU_variable.React_Power[2]<EMU_RePowerStar))
			{
				EMU_variable.Curr[2]=0;
			}	
			break;

	}
	
	// 如果发现通讯有异常, 需要对瞬时值对处理
	if(EMU_Register.EMU_Uart_Err_ing[EMU_L1])
	{
		EMU_variable.Volt[1]=0;
		EMU_variable.Curr[1]=0;
		EMU_variable.Freq[1]=0;
	}
	if(EMU_Register.EMU_Uart_Err_ing[EMU_L2])
	{
		EMU_variable.Volt[2]=0;
		EMU_variable.Curr[2]=0;
		EMU_variable.Freq[2]=0;			
	}	
    Process_Tample_mode();
}

/*******************************************************************************
 * @function_name: Process_ATT7059_Power
 * @function_file: drv_ATT7059.C
 * @描述：处理ATT7059有功功率，无功功率
 * @入口参数：
 * @出口参数: 
 * @Attention:  有功功率、无功功率采用无符号运算，功率因数需调试
 ******************************************************************************/
static void Process_ATT7059_Power(EMU_NO EMU_NUM)
{
	int32 PowerPA=0,PowerQA=0,PowerSA=0;
	int32 ATT7059_P_temp=0;
	int64 power_temp=0;
	uint32 power_temp_factor = 0;
		
	if(PowerDownDetect()==1)
	{
		EMU_variable.Act_Power[1]=0;
		EMU_variable.React_Power[1]=0;
		EMU_variable.Apt_Power[1]=0;
		EMU_variable.Act_Power[2]=0;
		EMU_variable.React_Power[2]=0;
		EMU_variable.Apt_Power[2]=0;		
		return;
	}
	//功率参数 Power 是二进制补码格式，32 位数据，其中最高位是符号位。
	PowerPA=EMU_Register.PowerP[EMU_NUM][0];
	switch(EMU_NUM)
	{
		case EMU_L1:
            if(PowerPA&0x800000)
			{
                PowerPA=-PowerPA;   ////瞬时反向向标志
                PowerPA &=0x00ffffff;
				EMU_variable.EMU_Power_Direction[EMU_NUM+1]=1;
            }
            else
            {
                EMU_variable.EMU_Power_Direction[EMU_NUM+1]=0;; //瞬时正向标志
            } 
            power_temp=(int64)PowerPA*CONST_P;//0.01W         
            ATT7059_P_temp=(int32)(power_temp/100000);

			break;
		case EMU_L2:
          if(PowerPA&0x800000)
			{
                PowerPA=-PowerPA;   ////瞬时反向向标志
                PowerPA &=0x00ffffff;
				EMU_variable.EMU_Power_Direction[EMU_NUM+1]=1;
            }
            else
            {
                EMU_variable.EMU_Power_Direction[EMU_NUM+1]=0;; //瞬时正向标志
            } 
            power_temp=(int64)PowerPA*CONST_P;//0.01W         
            ATT7059_P_temp=(int32)(power_temp/100000);

			break;
	}
	if((ATT7059_P_temp<=(uint32)(EMU_MaxPower)))
	{
	    if(ATT7059_P_temp<EMU_PowerStar)
	    {
		    ATT7059_P_temp=0;
			EMU_variable.EMU_Power_Direction[EMU_NUM+1]=0;
	    }
	}
	else
	{
		 ATT7059_P_temp=EMU_MaxPower; //超过最大使用最大值
	}	
	switch(EMU_NUM)
	{
		case EMU_L1:
			EMU_variable.Act_Power[1]=ATT7059_P_temp;//XXXXX.xx 0.01w
			break;
		case EMU_L2:
			EMU_variable.Act_Power[2]=ATT7059_P_temp;//XXXXX.xx 0.01w
			break;
	}
	
	PowerQA=EMU_Register.PowerQ[EMU_NUM][0];	

	switch(EMU_NUM)
	{
		case EMU_L1:
            if(PowerQA&0x800000)
			{
                PowerQA=-PowerQA;   ////瞬时反向向标志
                PowerQA &=0x00ffffff;				
            }
            else
            {
                ; //瞬时正向标志
            } 
            power_temp=(int64)PowerQA*CONST_P;//0.01var                
			ATT7059_P_temp=(int32)(power_temp/100000);
			break;
		case EMU_L2:
            if(PowerQA&0x800000)
			{
                PowerQA=-PowerQA;   ////瞬时反向向标志
                PowerQA &=0x00ffffff;
            }
            else
            {
                ; //瞬时正向标志
            } 
            power_temp=(int64)PowerQA*CONST_P;//0.01var                
			ATT7059_P_temp=(int32)(power_temp/100000);
			break;
	}
	if((ATT7059_P_temp<=(uint32)(EMU_MaxPower)))
	{
	    if(ATT7059_P_temp<EMU_PowerStar)
	    {
		    ATT7059_P_temp=0;
	    }
	}
	else
	{
		ATT7059_P_temp=EMU_MaxPower; //超过最大使用最大值
	}
	switch(EMU_NUM)
	{
		case EMU_L1:
			EMU_variable.React_Power[1]=ATT7059_P_temp;//XXXXX.xxx 0.01var
			break;
		case EMU_L2:
			EMU_variable.React_Power[2]=ATT7059_P_temp;//XXXXX.xxx 0.01var
			break;
	}
	PowerSA=EMU_Register.PowerS[EMU_NUM][0];
	switch(EMU_NUM)
	{
		case EMU_L1:
            if(PowerSA&0x800000)
			{
                PowerSA=-PowerSA;   ////瞬时反向向标志
                PowerSA &=0x00ffffff;
            }
            else
            {
                ; //瞬时正向标志
            } 
            power_temp=(int64)PowerSA*CONST_P;//0.01var                
			ATT7059_P_temp=(int32)(power_temp/100000);
			break;
		case EMU_L2:
            if(PowerSA&0x800000)
			{
                PowerSA=-PowerSA;   ////瞬时反向向标志
                PowerSA &=0x00ffffff;
            }
            else
            {
                ; //瞬时正向标志
            } 
            power_temp=(int64)PowerSA*CONST_P;//0.01var                
			ATT7059_P_temp=(int32)(power_temp/100000);
			break;
	}
    if((ATT7059_P_temp<=(uint32)(EMU_MaxPower)))
	{
	    if(ATT7059_P_temp<EMU_PowerStar)
	    {
		    ATT7059_P_temp=0;
	    }
	}
	else
	{
		ATT7059_P_temp=EMU_MaxPower; //超过最大使用最大值
	}	
	switch(EMU_NUM)
	{
		case EMU_L1:
			EMU_variable.Apt_Power[1]=ATT7059_P_temp;//XXXXX.xxx 0.01var
			break;
		case EMU_L2:
			EMU_variable.Apt_Power[2]=ATT7059_P_temp;//XXXXX.xxx 0.01var
			break;
	}
	//计算功率因数	
	switch(EMU_NUM)
	{
		case EMU_L1:
			if((EMU_variable.Act_Power[1]<EMU_PowerStar)&&(EMU_variable.React_Power[1]<EMU_RePowerStar))
			{
				EMU_variable.Factor[1]=1000;
			}
			else
			{
				power_temp_factor=(uint32)(EMU_variable.Act_Power[1]*1000);
				//===================================
				//优化小电流功率因数
				EMU_variable.Apt_Power[1] = CalAppentPower(EMU_variable.Act_Power[1],EMU_variable.React_Power[1]);
				//===================================
				EMU_variable.Factor[1]=(uint16)((power_temp_factor+(EMU_variable.Apt_Power[1]/2))/EMU_variable.Apt_Power[1]);
				
				if(EMU_variable.Factor[1]>1000)
				{
					EMU_variable.Factor[1]=1000;
				}
			}
			break;
		case EMU_L2:
			if((EMU_variable.Act_Power[2]<EMU_PowerStar)&&(EMU_variable.React_Power[2]<EMU_RePowerStar))
			{
				EMU_variable.Factor[2]=1000;
			}
			else
			{
				power_temp_factor=(uint32)(EMU_variable.Act_Power[2]*1000);
				//====================================
				//优化小电流功率因数
				EMU_variable.Apt_Power[2] = CalAppentPower(EMU_variable.Act_Power[2],EMU_variable.React_Power[2]);
				//====================================
				EMU_variable.Factor[2]=(uint16)((power_temp_factor+(EMU_variable.Apt_Power[2]/2))/EMU_variable.Apt_Power[2]);
				if(EMU_variable.Factor[2]>1000)
				{
					EMU_variable.Factor[2]=1000;
				}
			}
			break;
	}
	// 如果发现通讯有异常, 需要对瞬时值对处理
	if(EMU_Register.EMU_Uart_Err_ing[EMU_L1])
	{
		EMU_variable.Act_Power[1]=0;
		EMU_variable.React_Power[1]=0;
		EMU_variable.Apt_Power[1]=0;
		EMU_variable.Factor[1]=1000;
	}
	if(EMU_Register.EMU_Uart_Err_ing[EMU_L2])
	{
		EMU_variable.Act_Power[2]=0;
		EMU_variable.React_Power[2]=0;
		EMU_variable.Apt_Power[2]=0;
		EMU_variable.Factor[2]=1000;			
	}	
}

/*******************************************************************************
 * @function_name: Reset_ATT7059
 * @function_file: drv_ATT7059.C
 * @描述：软件复位ATT7059
 * @入口参数：
 * @出口参数: 
 * @Attention:  
 ******************************************************************************/
static void Reset_ATT7059S(EMU_NO EMU_NUM)
{
	uint8 temp=0x55;
	EMUUartAutoPause();
	EMUCommand(EMU_NUM,W_ENABLE_1);
	EMUWrite(EMU_NUM, SRSTREG_W, 1, (uint8*)&temp);//EMURESET
	EMUCommand(EMU_NUM,W_DISABLE);
	EMUUartAutoGoOn();
}


/*******************************************************************************
 * @function_name: Reset_EMU
 * @function_file: drv_ATT7059.C
 * @描述：软件复位单个计量芯片 
 * @入口参数：
 * @出口参数: 
 * @Attention:   
 ******************************************************************************/
static void Reset_EMU(EMU_NO EMU_NUM)
{
	EMUUartAutoPause();
	Reset_ATT7059S(EMU_NUM);
	EMUUartAutoGoOn();
}
/*******************************************************************************
 * @function_name: Read_EMU_IVF
 * @function_file: drv_ATT7059.C
 * @描述：读取ATT7059读取ATT7059电压、电流、频率
 * @入口参数：
 * @出口参数: 
 * @Attention:   
 ******************************************************************************/
static void DRV_EMU_Read_IVF(void)
{
	EMU_NO on;
	for(on=(EMU_NO)0;on<(EMU_NO)MAX_EMU_NUMBER;on++)
	{
		Process_ATT7059_I_V_F(on);
	}
}


/*******************************************************************************
 * @function_name: DRV_EMU_Read_Power
 * @function_file: drv_ATT7059.C
 * @描述：读取ATT7059有功率,有功电能累加
 * @入口参数：
 * @出口参数: 
 * @Attention:   
 ******************************************************************************/
static void DRV_EMU_Read_Power(void)
{
	EMU_NO on;
	for(on=(EMU_NO)0;on<(EMU_NO)MAX_EMU_NUMBER;on++)
	{
		Process_ATT7059_Power(on);
	}
}


/*******************************************************************************
 * @function_name: DRV_EMU_PAR_Initial
 * @function_file: drv_ATT7059.C
 * @描述：计量模块参数初始化
 * @入口参数：
 * @出口参数: 
 * @Attention:   用于导入计量电参数
 ******************************************************************************/
static void DRV_EMU_PAR_Initial(void)
{	
	EMU_Parmeter.EnergyChnsel=0;
	EMU_Parmeter.TampMode=1;
	EMU_Parmeter.Ichk=3;					 // 两电流相差 1/8 		
	EMU_Parmeter.Itamp=100;					 //100MA
	EMU_Parmeter.CurrentHighthreshold=10000;  //10A
	EMU_Parmeter.CurrentLowthreshold=1000;	  //1A 
	
	
	if(!NvmBytesRead(EE_CALIBRATE_ADDR_PAR, &Calibrate_meter.Cali_Par, sizeof(Calibrate_meter.Cali_Par)))
	{
		caliEepError = 1;
		Calibrate_meter.Cali_Par.Un=2200;		//xxx.x
		Calibrate_meter.Cali_Par.Ib=5000;		//xxx.xxx
		Calibrate_meter.Cali_Par.Imax=100000;		//xxx.xxx
		Calibrate_meter.Cali_Par.Meter_const_act=1600;
		Calibrate_meter.Cali_Par.Meter_const_react=1600;
		Calibrate_meter.Cali_Par.Freq=50;
		NvmBytesWrite(EE_CALIBRATE_ADDR_PAR, &Calibrate_meter.Cali_Par, sizeof(Calibrate_meter.Cali_Par));
	}
	else
	{
		
	}

	memset((uint8*)&EMU_Register,0,sizeof(EMU_Register));
	memset((uint8*)&EMU_energy,0,sizeof(EMU_energy));
	memset((uint8*)&EMU_variable,0,sizeof(EMU_variable));
	memset((uint8*)&EMU_CheckSum,0,sizeof(EMU_CheckSum));
}


/*******************************************************************************
 * @function_name: DRV_EMU_Monitor
 * @function_file: drv_ATT7059.C
 * @描述：
		根据当前UART通讯状态及ATT7059S的校验和判断是否需要恢复校表数据
		两种条件需要恢复
		1:  通讯线路断开后恢复
		2:  ATT7059S 发生复位
	//两路通讯都不正常, 两个都不要恢复校表参数
	// 通讯线路断开,不通讯了,后又恢复了,这种直接对ATT7059S进行初始化	
    //从ATT7059S读出来的校验和和RAM保存的校验和不一样		
 * @入口参数：
 * @出口参数: 
 * @Attention:   用于修调初始化
 ******************************************************************************/
static void DRV_EMU_Monitor(void)
{
	EMU_NO on;
	if(PowerDownDetect()!=0)
	{
		return;
	}
	for(on=(EMU_NO)0;on<(EMU_NO)MAX_EMU_NUMBER;on++)
	{
		if(EMU_Register.EMU_Uart_Err_ing[on])
		{
			continue;  //本次循环 发现通讯异常不进行恢复初始化操作
		}
		if(EMU_Register.EMU_Uart_Ok_Recover[on])
		{
			EMU_Register.EMU_Uart_Ok_Recover[on]=0;
			DRV_EMU_Initial(on);
		}
		else
		{    
			switch(on)
            {
              case EMU_L1:
			  		if(!Check_ATT7059_Calibrate(EMU_L1))   //
			  		{
						DRV_EMU_Initial(EMU_L1);
					}
					else
					{
	                    if(EMU_CheckSum.EMU_Calu_Data_L1==EMU_Register.CheckSum[EMU_L1][0])
	                    {
	                        EMU_err_ct[0]=0;
	                    }
	                    else if(EMU_err_ct[0]<15)
	                    {
	                        EMU_err_ct[0]++;
	                    }
	                    else
	                    {
	                        EMU_err_ct[0]=0;
							DRV_EMU_Initial(EMU_L1);
	                    }
					}	
                break;
              case EMU_L2:
			  		if(!Check_ATT7059_Calibrate(EMU_L2))   //
			  		{
						DRV_EMU_Initial(EMU_L2);
					}
					else
					{
	                    if(EMU_CheckSum.EMU_Calu_Data_L2==EMU_Register.CheckSum[EMU_L2][0])
	                    {
	                        EMU_err_ct[1]=0;
	                    }
	                    else if(EMU_err_ct[1]<15)
	                    {
	                        EMU_err_ct[1]++;
	                    }
	                    else
	                    {
	                        EMU_err_ct[1]=0;
	                        DRV_EMU_Initial(EMU_L2);
	                    }
					}	
                break;              
            } 
		}
	}
}





//====================public====================================================
//==============================================================================
//==============================================================================
/*******************************************************************************
 * @function_name: DRV_EMU_PowerDownInit
 * @function_file: drv_ATT7059.C
 * @描述：掉电后脉冲灯初始化，RAM清零。
 * @入口参数：
 * @出口参数: 
 * @Attention:   用于导入计量模块修调数据
 ******************************************************************************/
void DRV_EMU_PowerDownInit(void)
{
       //----------------Active Pulse LED--------------------
	IO_DisableFunc(IO_PORT_LED_ACT,IO_PIN_LED_ACT);//P80
  	IO_ConfigGPIOPin(IO_PORT_LED_ACT,IO_PIN_LED_ACT,IO_DIR_INPUT,IO_PULLUP_OFF);
	//----------------Reactive Pulse LED--------------------
	//IO_DisableFunc(IO_PORT_LED_REA,IO_PIN_LED_REA);//P0F
	//IO_ConfigGPIOPin(IO_PORT_LED_REA,IO_PIN_LED_REA,IO_DIR_INPUT,IO_PULLUP_OFF);
	//----------------EMU L1 ZX -------------------	
	memset((uint8*)&EMU_Register,0,sizeof(EMU_Register));
	memset((uint8*)&EMU_energy,0,sizeof(EMU_energy));
	memset((uint8*)&EMU_variable,0,sizeof(EMU_variable));
	memset((uint8*)&EMU_CheckSum,0,sizeof(EMU_CheckSum));
	EMU_variable.Factor[0]=1000;                      //掉电情况默认功率因数为1.0
} 


/*******************************************************************************
* @function_name: EMU_Cali_CheckSum_L1
* @function_file: drv_ATT7059.C
* @描述：所有校表参数寄存器的校验和的计算，用于监控EMU L1路校表参数是否正常
* @入口参数：
* @出口参数: 
* @Attention:	 
******************************************************************************/
void  EMU_Cali_CheckSum_L1(uint32 *RAM, uint8 len)
{
	uint32 temp1=0;
	uint32 temp2=0;
	uint8 i;
	for(i=0;i<len;i++)
	{
		temp1+=RAM[i]; 
	}
	temp1&=0x00ffffff;
	temp1-=0x00000040;//hfconst
	temp1+=(uint32)Calibrate_meter.Cali_EMU_L1.ADCCON_7059;
	temp1+=(uint32)Calibrate_meter.Cali_EMU_L1.EMUCFG_7059;//
	temp1+=(uint32)Calibrate_meter.Cali_EMU_L1.HFConst_7059;//
	temp1+=(uint32)Calibrate_meter.Cali_EMU_L1.GP1_7059;
	temp1+=(uint32)Calibrate_meter.Cali_EMU_L1.GQ1_7059;
	temp1+=(uint32)Calibrate_meter.Cali_EMU_L1.GS1_7059;
	temp1+=(uint32)Calibrate_meter.Cali_EMU_L1.Phs_7059;
	temp2=(uint32)((Calibrate_meter.Cali_EMU_L1.APOS_7059>>8)&0x00ff);// 两个寄存器，要分拆
	temp1+=temp2;	
	temp1+=(uint32)(Calibrate_meter.Cali_EMU_L1.APOS_7059&0x00ff);
	temp1&=0x00ffffff;
	EMU_CheckSum.EMU_Calu_Data_L1=temp1;
}


/*******************************************************************************
* @function_name: EMU_Cali_CheckSum_L1
* @function_file: drv_ATT7059.C
* @描述：所有校表参数寄存器的校验和的计算，用于监控EMU L2路校表参数是否正常
* @入口参数：
* @出口参数: 
* @Attention:	 
******************************************************************************/
void EMU_Cali_CheckSum_L2(uint32 *RAM, uint8 len)
{
	uint32 temp1=0;
	uint32 temp2=0;
	uint8 i;
	for(i=0;i<len;i++)
	{
		temp1+=RAM[i]; 
	}
	temp1&=0x00ffffff;
	temp1-=0x00000040;//hfconst
	temp1+=(uint32)Calibrate_meter.Cali_EMU_L2.ADCCON_7059;
	temp1+=(uint32)Calibrate_meter.Cali_EMU_L2.EMUCFG_7059;//
	temp1+=(uint32)Calibrate_meter.Cali_EMU_L2.HFConst_7059;//
	temp1+=(uint32)Calibrate_meter.Cali_EMU_L2.GP1_7059;
	temp1+=(uint32)Calibrate_meter.Cali_EMU_L2.GQ1_7059;
	temp1+=(uint32)Calibrate_meter.Cali_EMU_L2.GS1_7059;
	temp1+=(uint32)Calibrate_meter.Cali_EMU_L2.Phs_7059;
	temp2=(uint32)((Calibrate_meter.Cali_EMU_L2.APOS_7059>>8)&0x00ff);// 两个寄存器，要分拆
	temp1+=temp2;	
	temp1+=(uint32)(Calibrate_meter.Cali_EMU_L2.APOS_7059&0x00ff);
	temp1&=0x00ffffff;
	EMU_CheckSum.EMU_Calu_Data_L2=temp1;
}


/*******************************************************************************
 * @function_name: EMUCommand
 * @function_file: drv_ATT7059.C
 * @描述：手动命令发送
 * @入口参数： EMU编号，命令
 * @出口参数:  无
 * @Attention:	
 ******************************************************************************/
uint8 EMUCommand(EMU_NO NO, uint8 command) 
{
	return EMUWrite(NO, WPREG_R, 1, &command);
}


/*******************************************************************************
 * @function_name: EMUWrite
 * @function_file: 	 drv_att7059.c
 * @描述：	 手动写数据
 * @入口参数：EMU:       通道编号,
 *         commad: 		 命令,
 *              L:       数据长度, 
 *            RAM:       写入的缓存指针
 * @出口参数: succ
 * @Attention:	先传高字节，再传低字节
 ******************************************************************************/
uint8 EMUWrite(EMU_NO NO, uint8 command,uint8 L,uint8 *RAM)
{
	uint8 i,succ=FALSE;
	uint8 temp[4],temp1[4];

	memset(temp,0,4);
	memset(temp1,0,4);
	memcpy(temp, RAM,4);//数据copy
	for(i=0;i<4;i++)//倒数据
	{
		temp1[i]=temp[3-i];
	}
	for(i=0;i<4;i++)
	{
		temp[i]=temp1[i];
	}
	//装载当前发命令
	EMUcommBuffer.lenTx=5;	  //comm+len+sum 固定发5字节
	EMUcommBuffer.bufferTx[0]=0x6a;
	EMUcommBuffer.bufferTx[1]=command|0X80;
	if(L==1)
	{
		EMUcommBuffer.bufferTx[2]=0;
		memcpy(EMUcommBuffer.bufferTx+3, temp+3, 1);
	}
	else
	{
		memcpy(EMUcommBuffer.bufferTx+2, temp+2, L);		
	}
	EMUcommBuffer.bufferTx[4]=EMUCheckSum(EMUcommBuffer.bufferTx, 4);//固定4字节数据head+com+data1data2+sum
	EMUcommBuffer.lenRx=1;
      
	EMUcommBuffer.ch=EMU_CS_BUFF[NO].ch;
	LDRV_UsartOpen(EMUcommBuffer.ch, USART_8E1,BAUDE_4800BPS, EMUcommBuffer.bufferRx, EMUcommBuffer.lenRx, FullDouble);
	LDRV_UsartSend(EMUcommBuffer.ch,EMUcommBuffer.bufferTx,EMUcommBuffer.lenTx);	
	//等待当前发完成
	for(i=0;i<20;i++)
	{   
	    Feed_watchdog();
		PublicDelayMs(1);
		if(LDRV_UsartGetStateRx(EMUcommBuffer.ch)==FINISH)
		{
			temp[0]=EMUcommBuffer.bufferRx[0];
			if(temp[0]==0x54 )
			{
				succ=true;
				break;
			}
		}
	}
	PublicDelayMs(1);
	return succ;

}

/*******************************************************************************
 * @function_name: EMURead
 * @function_file: drv_ATT7059.C
 * @描述：读数据
 * @入口参数： 
 * @出口参数:  无
 * @Attention:	多字节时，先传输高字节内容，再传输低字节内容
 ******************************************************************************/
uint8 EMURead(uint8 command,uint8 L,uint8 *RAM,EMU_NO NO) 
{
	uint8 i,succ=FALSE;
	uint8 temp[10];

	//装载当前读取命令
	EMUcommBuffer.lenTx=2;
	EMUcommBuffer.bufferTx[0]=0X6A;
	EMUcommBuffer.bufferTx[1]=command&0x7f;
	EMUcommBuffer.lenRx=4;
	EMUcommBuffer.ch=EMU_CS_BUFF[NO].ch;
	LDRV_UsartOpen(EMUcommBuffer.ch, USART_8E1,BAUDE_4800BPS, EMUcommBuffer.bufferRx, EMUcommBuffer.lenRx, FullDouble);
	LDRV_UsartSend(EMUcommBuffer.ch,EMUcommBuffer.bufferTx,EMUcommBuffer.lenTx);	
	//等待当前读取完成
	for(i=0;i<20;i++)
	{
	    Feed_watchdog();
		PublicDelayMs(1);
		if(LDRV_UsartGetStateRx(EMUcommBuffer.ch)==FINISH)
		{
			//校验
			memcpy(temp, EMUcommBuffer.bufferTx, 2);
			memcpy(temp+2, EMUcommBuffer.bufferRx, 4);//L+1, 3个数据+1个sum
			if(EMUCheckSum(temp, 2+3) == temp[5])//固定返回3字节数据；2+L---固定第5位为sum 20131225 
			{
				for(i=0;i<3;i++)//先颠倒位置
				{
				     RAM[i]= EMUcommBuffer.bufferRx[3-i-1]; 
				}
			}
			break;
		}
	}	
	PublicDelayMs(1);		//命令之间必须间隔!!!
	return succ;

}

/*******************************************************************************
 * @function_name: EMUUartAutoPause
 * @function_file: drv_ATT7059.C
 * @描述：自动读取暂停
 * @入口参数： 
 * @出口参数:  无
 * @Attention:	手动操作EMU前必须调用该函数
 ******************************************************************************/
void EMUUartAutoPause(void) 
{
	//暂停自动读取
	EMUcommBuffer.holdeState=HOLDE;
	//等待当前自动读取结束
	PublicDelayMs(20);                       //20140710修改，以前10ms时间太短
}

/*******************************************************************************
 * @function_name: EMUUartAutoGoOn
 * @function_file: drv_ATT7059.C
 * @描述：自动读取继续
 * @入口参数： 
 * @出口参数:  无
 * @Attention:	手动操作EMU后必须调用该函数，指针要继续加，要测试。
 ******************************************************************************/
void EMUUartAutoGoOn(void) 
{
	//恢复自动读取
	EMUcommBuffer.lenTx=2;
	EMUcommBuffer.bufferTx[0]=EMU_CS_BUFF[EMU_CMD_BUFF[EMUcommBuffer.commP].NO].chipID;
	EMUcommBuffer.bufferTx[1]=EMU_CMD_BUFF[EMUcommBuffer.commP].cmd&0x7f;
	EMUcommBuffer.lenRx=EMU_CMD_BUFF[EMUcommBuffer.commP].len+1;	//len+sum
	EMUcommBuffer.ch=EMU_CS_BUFF[EMU_CMD_BUFF[EMUcommBuffer.commP].NO].ch;
	LDRV_UsartOpen(EMUcommBuffer.ch, USART_8E1,BAUDE_4800BPS, EMUcommBuffer.bufferRx, EMUcommBuffer.lenRx, FullDouble);
	EMUcommBuffer.holdeState=GO_ON;
	EMUcommBuffer.commP++;
	if(EMUcommBuffer.commP>=EMU_Regisetr_NUM)
	{
		EMUcommBuffer.commP=0;
	}
}

/*******************************************************************************
 * @function_name: Set_ATT7059
 * @function_file: drv_ATT7059.C
 * @描述：att7059初始化Load_Cal_Parameter
 * @入口参数：
 * @出口参数: 
 * @Attention:  向计量芯片写数据，注意写保护
 ******************************************************************************/
#define EMU_Init_count      9  //初始化校表寄存器的个数，如果初始化寄存器不同，需要修改此参数20140214
uint8 Set_ATT7059(EMU_NO EMU_NUM)
{
	uint8 m=0;
    uint8 i;
	U32Type	temp;
	EMUUartAutoPause();
	uint8 checkFlag=0;
	switch(EMU_NUM)
	{
		case EMU_L1:
			for(i=0;i<3;i++)
			{
				EMUCommand(EMU_NUM,W_ENABLE_2);//write emucfg 0x40
				temp.l = Calibrate_meter.Cali_EMU_L1.EMUCFG_7059;
				m=EMUWrite(EMU_NUM,EMUCFG_W,2,(uint8 *)&temp.l);

				EMUCommand(EMU_NUM,W_ENABLE_1);//WRITE 50-75H 
				temp.l = Calibrate_meter.Cali_EMU_L1.ADCCON_7059;
				m+=EMUWrite(EMU_NUM,ADCCON_W,2,(uint8 *)&temp.l);
				
				temp.l = Calibrate_meter.Cali_EMU_L1.HFConst_7059;
				m+=EMUWrite(EMU_NUM,HFconst_W,2,(uint8 *)&temp.l);
				
				temp.l = Calibrate_meter.Cali_EMU_L1.GP1_7059;
				m+=EMUWrite(EMU_NUM,GP1_W,2,(uint8 *)&temp.l);

				temp.l = Calibrate_meter.Cali_EMU_L1.GQ1_7059;
				m+=EMUWrite(EMU_NUM,GQ1_W,2,(uint8 *)&temp.l);

				temp.l = Calibrate_meter.Cali_EMU_L1.GS1_7059;
				m+=EMUWrite(EMU_NUM,GS1_W,2,(uint8 *)&temp.l);

				temp.l = Calibrate_meter.Cali_EMU_L1.Phs_7059;
				m+=EMUWrite(EMU_NUM,GPhs1_W,2,(uint8 *)&temp.l);

				temp.l = Calibrate_meter.Cali_EMU_L1.APOS_7059;
				m+=EMUWrite(EMU_NUM,P1OFFSETH_W,1,(uint8 *)&temp.l+1);
				m+=EMUWrite(EMU_NUM,P1OFFSETL_W,1,(uint8*)&temp.l);

				EMUCommand(EMU_NUM,W_DISABLE);
				if(m==EMU_Init_count)
				{
					break;
				}
			}
			if(i>=3)
			{
				checkFlag =1;
			}
			break;
		case EMU_L2:
			for(i=0;i<3;i++)
			{
				EMUCommand(EMU_NUM,W_ENABLE_2);//write emucfg 0x40
				temp.l = Calibrate_meter.Cali_EMU_L2.EMUCFG_7059;
				m=EMUWrite(EMU_NUM,EMUCFG_W,2,(uint8 *)&temp.l);

				EMUCommand(EMU_NUM,W_ENABLE_1);//WRITE 50-75H 
				temp.l = Calibrate_meter.Cali_EMU_L2.ADCCON_7059;
				m+=EMUWrite(EMU_NUM,ADCCON_W,2,(uint8 *)&temp.l);

				temp.l = Calibrate_meter.Cali_EMU_L2.HFConst_7059;
				m+=EMUWrite(EMU_NUM,HFconst_W,2,(uint8 *)&temp.l);

				temp.l = Calibrate_meter.Cali_EMU_L2.GP1_7059;
				m+=EMUWrite(EMU_NUM,GP1_W,2,(uint8 *)&temp.l);

				temp.l = Calibrate_meter.Cali_EMU_L2.GQ1_7059;
				m+=EMUWrite(EMU_NUM,GQ1_W,2,(uint8 *)&temp.l);

				temp.l = Calibrate_meter.Cali_EMU_L2.GS1_7059;
				m+=EMUWrite(EMU_NUM,GS1_W,2,(uint8 *)&temp.l);

				temp.l = Calibrate_meter.Cali_EMU_L2.Phs_7059;
				m+=EMUWrite(EMU_NUM,GPhs1_W,2,(uint8 *)&temp.l);

				temp.l = Calibrate_meter.Cali_EMU_L2.APOS_7059;
				m+=EMUWrite(EMU_NUM,P1OFFSETH_W,1,(uint8 *)&temp.l+1);
				m+=EMUWrite(EMU_NUM,P1OFFSETL_W,1,(uint8*)&temp.l);
				EMUCommand(EMU_NUM,W_DISABLE);
				if(m==EMU_Init_count)
				{
					break;
				}
			}			
			if(i>=3)
			{
				checkFlag =1;
			}
			break;
	}
    //--------------------------------------------    
	EMUUartAutoGoOn();
	if(1==checkFlag)      //3次写校验数据都出错了
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/*******************************************************************************
 * @function_name: Initial_Average_Calibrate_Data
 * @function_file: drv_ATT7059.C
 * @描述：写入修调数据经验值
 * @入口参数：
 * @出口参数: 
 * @Attention:   用于修调初始化
 ******************************************************************************/
void Initial_Average_Calibrate_Data(EMU_NO EMU_NUM)
{
	uint8 i;
	uint32 backup_temp11=0;
	uint32 backup_temp=0;
	
	switch(EMU_NUM)
	{
		case EMU_L1:
			Reset_EMU(EMU_NUM);
			memset((uint8 *)&Calibrate_meter.Cali_EMU_L1, 0,sizeof(Calibrate_meter.Cali_EMU_L1));
			Calibrate_meter.Cali_EMU_L1.ADCCON_7059=0x000c;
			Calibrate_meter.Cali_EMU_L1.EMUCFG_7059=0x0000;
			Calibrate_meter.Cali_EMU_L1.HFConst_7059=0x005B;  
			Calibrate_meter.Cali_EMU_L1.GP1_7059=0xFF1D;
			Calibrate_meter.Cali_EMU_L1.GQ1_7059=0xFF1D;
			Calibrate_meter.Cali_EMU_L1.GS1_7059=0xFF1D;
			Calibrate_meter.Cali_EMU_L1.Phs_7059=0xFF64;
			Calibrate_meter.Cali_EMU_L1.APOS_7059=0xFFEC;
			Calibrate_meter.Cali_EMU_L1.URMS_refer_7059=0x1D60;
			Calibrate_meter.Cali_EMU_L1.IRMS_refer_7059=0X43A9;
			Set_ATT7059(EMU_L1);
			PublicDelayMs(5);	
			Feed_watchdog();
			for(i=0;i<3;i++)
			{
				EMUUartAutoPause();	//先取出ATT7059S 自己计算出来的校验和
				EMURead(SUMChecksum_R,3,(uint8 *)&backup_temp11,EMU_L1);
				EMURead(BackupData_R,3,(uint8 *)&backup_temp,EMU_L1);
				EMUUartAutoGoOn();
				backup_temp11&=0x00ffffff;
				backup_temp&=0x00ffffff;
				if(backup_temp11==backup_temp)
				{
					Calibrate_meter.Cali_EMU_L1.EMU_checksum_7059=backup_temp11;
					break;
				}					
			}
			Calibrate_meter.Cali_EMU_L1.ChkSum_7059=CALC_CHECKSUM((uint8 *)&Calibrate_meter.Cali_EMU_L1, sizeof(Calibrate_meter.Cali_EMU_L1)-2);
			EMU_Cali_CheckSum_L1((uint32 *)EMU_REG_DATA,EMU_regSum);//下发经验值，不存EEP；但是需要重新计算经验值校表参数的校表参数校验和
			break;
		case EMU_L2:
			Reset_EMU(EMU_NUM);
			memset((uint8 *)&Calibrate_meter.Cali_EMU_L2, 0,sizeof(Calibrate_meter.Cali_EMU_L2));
			Calibrate_meter.Cali_EMU_L2.ADCCON_7059=0x000c;
			Calibrate_meter.Cali_EMU_L2.EMUCFG_7059=0x0000;
			Calibrate_meter.Cali_EMU_L2.HFConst_7059=0x005B;
			Calibrate_meter.Cali_EMU_L2.GP1_7059=0xFEC7;
			Calibrate_meter.Cali_EMU_L2.GQ1_7059=0xFEC7;
			Calibrate_meter.Cali_EMU_L2.GS1_7059=0xFEC7;
			Calibrate_meter.Cali_EMU_L2.Phs_7059=0xFF72;
			Calibrate_meter.Cali_EMU_L2.APOS_7059=0x0068;
			Calibrate_meter.Cali_EMU_L2.URMS_refer_7059=0x1D77;
			Calibrate_meter.Cali_EMU_L2.IRMS_refer_7059=0x423E;
			Set_ATT7059(EMU_L2);
			PublicDelayMs(5);	
			Feed_watchdog();
			for(i=0;i<3;i++)
			{
				EMUUartAutoPause();	//先取出ATT7059S 自己计算出来的校验和
				EMURead(SUMChecksum_R,3,(uint8 *)&backup_temp11,EMU_L2);
				EMURead(BackupData_R,3,(uint8 *)&backup_temp,EMU_L2);
				EMUUartAutoGoOn();
				backup_temp11&=0x00ffffff;
				backup_temp&=0x00ffffff;
				if(backup_temp11==backup_temp)
				{
					Calibrate_meter.Cali_EMU_L2.EMU_checksum_7059=backup_temp11;
					break;
				}					
			}
			Calibrate_meter.Cali_EMU_L2.ChkSum_7059=CALC_CHECKSUM((uint8 *)&Calibrate_meter.Cali_EMU_L2, sizeof(Calibrate_meter.Cali_EMU_L2)-2);
			EMU_Cali_CheckSum_L2((uint32 *)EMU_REG_DATA,EMU_regSum);//下发经验值，不存EEP；但是需要重新计算经验值校表参数的校表参数校验和
			break;
	}
}

/*******************************************************************************
 * @function_name: DRV_EMU_Initial_Calibrate
 * @function_file: drv_ATT7059.C
 * @描述：复位计量参数到初始值
 * @入口参数：
 * @出口参数: 
 * @Attention:   用于修调下发初始化修调数据
 ******************************************************************************/
void DRV_EMU_Initial_Calibrate(EMU_NO EMU_NUM)
{
	uint8 i;
	uint32 backup_temp11=0;
	uint32 backup_temp=0;

	//memset((uint8*)&EMU_Register,0,sizeof(EMU_Register));
	//memset((uint8*)&EMU_energy,0,sizeof(EMU_energy));
	//memset((uint8*)&EMU_variable,0,sizeof(EMU_variable));
	//memset((uint8*)&EMU_CheckSum,0,sizeof(EMU_CheckSum));
	
	switch(EMU_NUM)
	{
		case EMU_L1:
			Reset_EMU(EMU_NUM);
			memset((uint8 *)&Calibrate_meter.Cali_EMU_L1, 0,sizeof(Calibrate_meter.Cali_EMU_L1));
			Calibrate_meter.Cali_EMU_L1.ADCCON_7059=0x000c;
			Calibrate_meter.Cali_EMU_L1.EMUCFG_7059=0x0000;
			Calibrate_meter.Cali_EMU_L1.HFConst_7059=0x005B;  
			Calibrate_meter.Cali_EMU_L1.PQStart_7059=0x00;
			Calibrate_meter.Cali_EMU_L1.GP1_7059=0;
			Calibrate_meter.Cali_EMU_L1.GQ1_7059=0;
			Calibrate_meter.Cali_EMU_L1.GS1_7059=0;
			Calibrate_meter.Cali_EMU_L1.Phs_7059=0;
			Calibrate_meter.Cali_EMU_L1.APOS_7059=0;
			Calibrate_meter.Cali_EMU_L1.URMS_refer_7059=0x1D92;
			Calibrate_meter.Cali_EMU_L1.IRMS_refer_7059=0X42C4;
			Set_ATT7059(EMU_L1);
			PublicDelayMs(5);	
			Feed_watchdog();
			for(i=0;i<3;i++)
			{
				EMUUartAutoPause();	//先取出ATT7059S 自己计算出来的校验和
				EMURead(SUMChecksum_R,3,(uint8 *)&backup_temp11,EMU_L1);
				EMURead(BackupData_R,3,(uint8 *)&backup_temp,EMU_L1);
				EMUUartAutoGoOn();
				backup_temp11&=0x00ffffff;
				backup_temp&=0x00ffffff;
				if(backup_temp11==backup_temp)
				{
					Calibrate_meter.Cali_EMU_L1.EMU_checksum_7059=backup_temp11;
					break;
				}					
			}
			Calibrate_meter.Cali_EMU_L1.ChkSum_7059=CALC_CHECKSUM((uint8 *)&Calibrate_meter.Cali_EMU_L1, sizeof(Calibrate_meter.Cali_EMU_L1)-2);
			for(i=0;i<3;i++)
			{ 
				if(NvmBytesWrite(EE_CALIBRATE_ADDR_L1,&Calibrate_meter.Cali_EMU_L1,sizeof(Calibrate_meter.Cali_EMU_L1)))
				{
					break;
				}
			}
			EMU_Cali_CheckSum_L1((uint32 *)EMU_REG_DATA,EMU_regSum);//下发经验值，不存EEP；但是需要重新计算经验值校表参数的校表参数校验和
			break;
		case EMU_L2:
			Reset_EMU(EMU_NUM);
			memset((uint8 *)&Calibrate_meter.Cali_EMU_L2, 0,sizeof(Calibrate_meter.Cali_EMU_L2));
			Calibrate_meter.Cali_EMU_L2.ADCCON_7059=0x000c;
			Calibrate_meter.Cali_EMU_L2.EMUCFG_7059=0x0000;
			Calibrate_meter.Cali_EMU_L2.HFConst_7059=0x005B;
			Calibrate_meter.Cali_EMU_L2.PQStart_7059=0x00;
			Calibrate_meter.Cali_EMU_L2.GP1_7059=0;
			Calibrate_meter.Cali_EMU_L2.GQ1_7059=0;
			Calibrate_meter.Cali_EMU_L2.GS1_7059=0;
			Calibrate_meter.Cali_EMU_L2.Phs_7059=0;
			Calibrate_meter.Cali_EMU_L2.APOS_7059=0;
			Calibrate_meter.Cali_EMU_L2.URMS_refer_7059=0x1D42;
			Calibrate_meter.Cali_EMU_L2.IRMS_refer_7059=0X421D;
			Set_ATT7059(EMU_L2);
			PublicDelayMs(5);	
			Feed_watchdog();
			for(i=0;i<3;i++)
			{
				EMUUartAutoPause();	//先取出ATT7059S 自己计算出来的校验和
				EMURead(SUMChecksum_R,3,(uint8 *)&backup_temp11,EMU_L2);
				EMURead(BackupData_R,3,(uint8 *)&backup_temp,EMU_L2);
				EMUUartAutoGoOn();
				backup_temp11&=0x00ffffff;
				backup_temp&=0x00ffffff;
				if(backup_temp11==backup_temp)
				{
					Calibrate_meter.Cali_EMU_L2.EMU_checksum_7059=backup_temp11;
					break;
				}					
			}
			Calibrate_meter.Cali_EMU_L2.ChkSum_7059=CALC_CHECKSUM((uint8 *)&Calibrate_meter.Cali_EMU_L2, sizeof(Calibrate_meter.Cali_EMU_L2)-2);
			for(i=0;i<3;i++)
			{ 
				if(NvmBytesWrite(EE_CALIBRATE_ADDR_L2,&Calibrate_meter.Cali_EMU_L2,sizeof(Calibrate_meter.Cali_EMU_L2)))
				{
					break;
				}
			}
			EMU_Cali_CheckSum_L2((uint32 *)EMU_REG_DATA,EMU_regSum);//下发经验值，不存EEP；但是需要重新计算经验值校表参数的校表参数校验和
			break;
	}
}


/*******************************************************************************
 * @function_name: Get_Att7059_PUI
 * @function_file: drv_ATT7059.C
 * @描述：获取ATT7059的功率变量
 * @入口参数：
 * @出口参数: 
 * @Attention: 注意读取的次数  
 ******************************************************************************/
void Get_Att7059_PUI(EMU_NO EMU_NUM, uint8 readPowerCount)
{
	uint8 i;
    int32 Powertemp=0;
	//----calculate average active power & reactive power &current & voltage------L2路
	EMUUartAutoPause();
	AutomaticCalibrationData.Power_P[0]= 0;
	AutomaticCalibrationData.TempPointer1 = 0;
	for(i=0;i<readPowerCount;i++)
	{
		EMURead(PowerP1_R,3,(uint8 *)&Powertemp,EMU_NUM);
        EMU_Register.PowerP[EMU_NUM][0]=Powertemp;
		Process_ATT7059_Power(EMU_NUM);
		AutomaticCalibrationData.Power_P[0] +=EMU_variable.Act_Power[EMU_NUM + 1];
		Feed_watchdog();
		PublicDelayMs(250);//等待寄存器更新
		Feed_watchdog();
	}
	AutomaticCalibrationData.Power_P[0] = (AutomaticCalibrationData.Power_P[0] + (readPowerCount>>1))/readPowerCount; //readPowerCount>>1，4舍5入
	EMUUartAutoGoOn();
}


/*******************************************************************************
 * @function_name: DRV_EMU_Initial_PowerOn
 * @function_file: drv_ATT7059.C
 * @描述:上电初始化EMU
 * @入口参数: 
 * @出口参数: 
 * @Attention:     

 ******************************************************************************/
static uint8 DRV_EMU_Initial_PowerOn(void)
{
	if(PowerDownDetect()!=0)
	{
		return 0;
	}
	DRV_WD_FeedDog();
	DRV_EMU_IOInit();
	DRV_EMU_250us_Init();
	DRV_EMU_PAR_Initial();
	DRV_EMU_Initial(EMU_L1);
	DRV_EMU_Initial(EMU_L2);
	return 1;
}


/*******************************************************************************
 * @function_name: DRV_EMU_PowerUpInit
 * @function_file: drv_ATT7059.C
 * @描述:上电初始化EMU
 * @入口参数: 
 * @出口参数: 
 * @Attention:     
 ******************************************************************************/
uint8 DRV_EMU_PowerUpInit(void)
{
	uint8 ret_value=0;
	TaskAdd(EMUUartAutoSendTask,0,10,0);	
	TaskAdd(EMUUartAutoProTask,0,100,1);
	TaskAdd(DRV_EMU_Read_Power, 0, 1000 ,1);
	TaskAdd(DRV_EMU_Read_IVF, 0, 1000, 1);
	TaskAdd(DRV_EMU_Monitor, 1000, 1000, 1);
	TaskAdd(AutomaticCalibrationProcess, 5000, 1000, 1);//used for EMU calibrate
	ret_value=DRV_EMU_Initial_PowerOn();
	EMUUartAutoStart();
	return (ret_value);
}

/*********************************************************** 
函数功能：电表上电总清，为方便生产
入口参数：
出口参数：包括返回值说明
备注说明：包括功能概要，关键算法，调用说明
***********************************************************/
void MeterPowerUPClearAll(void)
{
	DRV_WD_FeedDog();
	if(!(IO_ReadGPIOPin(IO_PORT_PRO_SWI,IO_PIN_PRO_SWI)))
	{
		PublicDelayMs(50);
		if((!(IO_ReadGPIOPin(IO_PORT_PRO_SWI,IO_PIN_PRO_SWI))) && (!(KEY_Clr)))
		{
			IO_WriteGPIOPin(IO_PORT5,IO_PINx6,IO_BIT_CLR);             //报警灯开
			IO_WriteGPIOPin(IO_PORT_LED_ACT,IO_PIN_LED_ACT,IO_BIT_CLR);//秒冲灯开
			PublicDelayMs(100);
			DRV_WD_FeedDog();
			if((!(IO_ReadGPIOPin(IO_PORT_PRO_SWI,IO_PIN_PRO_SWI))) && (!(KEY_Clr)))
			{
				caliEepError = 0;
				DRV_EMU_PAR_Initial();
				DRV_EMU_Initial(EMU_L1);
				DRV_EMU_Initial(EMU_L2);
				if(caliEepError)
				{
					DRV_EMU_Initial_Calibrate(EMU_L1);
					DRV_EMU_Initial_Calibrate(EMU_L2);
					caliEepError=0;     //校表数据出错清除，用于显示出错
				}
				BillingParaSet(0x09050081, NULL, 0);
				PassWordSetDefault();              //密码恢复到默认值
				MeterClearRecordInit();            //按两个键清零记录清零        
				DRV_WD_FeedDog();
				DispParaClear();                   //显示参数恢复到默认值
				MeterAddressClear();               //表地址清零
				DisplayAlarm(DIS_ALARM_CLEAR_OK,TRUE);//显示OK
			}
			IO_WriteGPIOPin(IO_PORT5,IO_PINx6,IO_BIT_SET);             //报警灯关
			IO_WriteGPIOPin(IO_PORT_LED_ACT,IO_PIN_LED_ACT,IO_BIT_SET);//秒冲灯关
		}
	}
}

/*******************************************************************************
 * @function_name: Get_EMU_Measure_Data
 * @function_file: drv_ATT7059.C
 * @描述：读取ATT7059各种瞬时计量参数
 * @入口参数: id为EMU能量数据ID(枚举类型)
 * @出口参数: 指定ID的变量的值
 * @Attention:    
 ******************************************************************************/
uint32 CalAppentPower(uint32 acPower,uint32 reacPower)
{
	uint64 activePower,reactivePower;
	uint32 appentpower;
	activePower = (uint64)acPower*acPower;
	reactivePower = (uint64)reacPower*reacPower;
	appentpower =  (uint32)sqrt(activePower + reactivePower);
	return appentpower;
}

/*******************************************************************************
 * @function_name: Get_EMU_Measure_Data
 * @function_file: drv_ATT7059.C
 * @描述：读取ATT7059各种瞬时计量参数
 * @入口参数: id为EMU能量数据ID(枚举类型)
 * @出口参数: 指定ID的变量的值
 * @Attention:    
 ******************************************************************************/
int32 Get_EMU_Measure_Data(tMeasureID id)
{
    int32  RetVal;
    switch(id)
    {
		case V_RMS_L1://XXXX.xx V
		case V_RMS_L2:
		case V_RMS_L3:
			RetVal=EMU_variable.Volt[id-V_RMS_L1];
			break;
		case I_RMS_L1://XXXX.xxxx A
		case I_RMS_L2:
		case I_RMS_L3:
			RetVal=EMU_variable.Curr[id-I_RMS_L1];
			break;
		case I_RMS_N:
		case V_RMS_T:
			RetVal=EMU_variable.Volt_UtRms;
			break;
		case I_RMS_T:
			RetVal=EMU_variable.Curr_ItRms;
			break;
		case ACTIVE_POWER_T:  //XXXXX.xxx W
		case ACTIVE_POWER_L1: 
		case ACTIVE_POWER_L2: 
		case ACTIVE_POWER_L3: 
			RetVal=EMU_variable.Act_Power[id-ACTIVE_POWER_T];
			break;
		case REACTIVE_POWER_T:	//XXXXX.xxx Var
		case REACTIVE_POWER_L1: 
		case REACTIVE_POWER_L2: 
		case REACTIVE_POWER_L3: 
			RetVal=EMU_variable.React_Power[id-REACTIVE_POWER_T];
			break;
		case APPARENT_POWER_T:	//XXXXX.xxx VA
		case APPARENT_POWER_L1: 
		case APPARENT_POWER_L2: 
		case APPARENT_POWER_L3: 
			RetVal=EMU_variable.Apt_Power[id-APPARENT_POWER_T];
			break;
		case PHASE_ANGLE_L1:// XXXX.xxx °
		case PHASE_ANGLE_L2: 
		case PHASE_ANGLE_L3: 
			RetVal=EMU_variable.Phase_Angle[id-PHASE_ANGLE_L1];
			break;
		case ANGLE_L1L2: //Yab   XXXX.xxx °
		case ANGLE_L1L3: //Yac
		case ANGLE_L2L3: //Ybc 
			RetVal=EMU_variable.angle[id-ANGLE_L1L2];
			break;
		case PF_T: //X.xxx
		case PF_L1: 
		case PF_L2: 
		case PF_L3: 
			RetVal=EMU_variable.Factor[id-PF_T];
			break;
		case FREQUENCY_T: //XX.xxxxxx Hz
		case FREQUENCY_L1: 
		case FREQUENCY_L2: 
		case FREQUENCY_L3: 
			RetVal=EMU_variable.Freq[id-FREQUENCY_T];
			break;
		case QUADRANT_T: //0:I ;1:II ; Quadrant[i]=2:III ; Quadrant[i]=3:IV
		case QUADRANT_L1: 
		case QUADRANT_L2: 
		case QUADRANT_L3: 
			RetVal=EMU_variable.Quadrant[id-QUADRANT_T];
			break;
		case TEMPERATURE: 
			RetVal=EMU_variable.Temprature;
			break;
		case POWER_DIRECTION: 
			RetVal=EMU_variable.EMU_Power_Direction[0];
			break;
		case CurrentImbalance:
			RetVal=EMU_variable.Current_Unbalanced;
			break;
		case STATUS: 
			RetVal=EMU_variable.EMU_State_Register;
			break;
		case MEASURRE_MODEL: 
			RetVal=EMU_variable.Measure_Model;
			break;
		case MEASURE_CHNSEL:
			RetVal=EMU_variable.Measure_Chnsel;
			break;		
		case SYS_PARAMETER_UN: //XXX.x V
			RetVal=Calibrate_meter.Cali_Par.Un;
			break;
		case SYS_PARAMETER_IB: //XXX.xxx A
			RetVal=Calibrate_meter.Cali_Par.Ib;
			break;
		case SYS_PARAMETER_IMAX: //XXX.xxx A
			RetVal=Calibrate_meter.Cali_Par.Imax;
			break;
		case SYS_PARAMETER_CONST_ACT: //XXXX imp/kWh
			RetVal=Calibrate_meter.Cali_Par.Meter_const_act;
			break;
		case SYS_PARAMETER_CONST_REA: //XXXX imp/kvarh
			RetVal=Calibrate_meter.Cali_Par.Meter_const_react;
			break;
		case SYS_PARAMETER_FREQ: //XX Hz
			RetVal=Calibrate_meter.Cali_Par.Freq;
			break;
        default:
        {
            RetVal = 0;
            break;
        }
    }
    return RetVal;
}

/*******************************************************************************
 * @function_name: Get_EMU_Energy_Data
 * @function_file: drv_ATT7059.C
 * @描述：读取ATT7059累加的能量数据
 * @入口参数: id为EMU能量数据ID(枚举类型)。
 * @出口参数: 指定ID的变量的值
 * @Attention:    读后清零
 ******************************************************************************/
int32 Get_EMU_Energy_Data(tEMUEnergyID id)
{
    int32  RetVal;

    switch(id)
    {
		case	PULSE_CT_ACT://active pulse const
			RetVal = EMU_energy.pulse_ct_act;
			EMU_energy.pulse_ct_act=0;
			break;
		case	PULSE_CT_REA://reactive pulse const
			RetVal = EMU_energy.pulse_ct_react;
			//EMU_energy.pulse_ct_react=0;
			break;
		case	ACTIVE_Energy_Pos_T://Sum positive active energy
			RetVal = EMU_energy.Rail_Plus_Active[0]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Rail_Plus_Active[0]%=100L;
			break;
		case	ACTIVE_Energy_Neg_T://Sum negative active energy
			RetVal = EMU_energy.Rail_Nega_Active[0]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Rail_Nega_Active[0]%=100L;
			break;
		case	REACTIVE_Energy_Pos_T://Sum	positive reactive energy
			RetVal = EMU_energy.Rail_Plus_Reactive[0]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Rail_Plus_Reactive[0]%=100L;
			break;
		case	REACTIVE_Energy_Neg_T://Sum negative reactive energy
			RetVal = EMU_energy.Rail_Nega_Reactive[0]/100L;//--->0.1wh=0.001wh/100L
			 EMU_energy.Rail_Nega_Reactive[0]%=100L;
			break;
		case	ACTIVE_Energy_Pos_L1:
			//RetVal = EMU_energy.Rail_Plus_Active[1]/100L;//--->0.1wh=0.001wh/100L
			//EMU_energy.Rail_Plus_Active[1]%=100L;

			RetVal = EMU_energy.Rail_Plus_Active[1];//--->0.1wh=0.001wh/100L
			EMU_energy.Rail_Plus_Active[1]=0;
			break;
		case	ACTIVE_Energy_Neg_L1:
			//RetVal = EMU_energy.Rail_Nega_Active[1]/100L;//--->0.1wh=0.001wh/100L
			//EMU_energy.Rail_Nega_Active[1]%=100L;
			RetVal = EMU_energy.Rail_Nega_Active[1];//--->0.1wh=0.001wh/100L
			EMU_energy.Rail_Nega_Active[1] = 0;
			break;
		case	REACTIVE_Energy_Pos_L1:
			RetVal = EMU_energy.Rail_Plus_Reactive[1]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Rail_Plus_Reactive[1]%=100L;
			break;
		case	REACTIVE_Energy_Neg_L1:
			RetVal = EMU_energy.Rail_Nega_Reactive[1]/100L;//--->0.1wh=0.001wh/100L
			 EMU_energy.Rail_Nega_Reactive[1]%=100L;
			break;
		case	ACTIVE_Energy_Pos_L2:
			RetVal = EMU_energy.Rail_Plus_Active[2]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Rail_Plus_Active[2]%=100L;
			break;
		case	ACTIVE_Energy_Neg_L2:
			RetVal = EMU_energy.Rail_Nega_Active[2]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Rail_Nega_Active[2]%=100L;
			break;
		case	REACTIVE_Energy_Pos_L2:
			RetVal = EMU_energy.Rail_Plus_Reactive[2]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Rail_Plus_Reactive[2]%=100L;
			break;
		case	REACTIVE_Energy_Neg_L2:
			RetVal = EMU_energy.Rail_Nega_Reactive[2]/100L;//--->0.1wh=0.001wh/100L
			 EMU_energy.Rail_Nega_Reactive[2]%=100L;
			break;
		case	ACTIVE_Energy_Pos_L3:
			RetVal = EMU_energy.Rail_Plus_Active[3]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Rail_Plus_Active[3]%=100L;
			break;
		case	ACTIVE_Energy_Neg_L3:
			RetVal = EMU_energy.Rail_Nega_Active[3]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Rail_Nega_Active[3]%=100L;
			break;
		case	REACTIVE_Energy_Pos_L3:
			RetVal = EMU_energy.Rail_Plus_Reactive[3]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Rail_Plus_Reactive[3]%=100L;
			break;
		case	REACTIVE_Energy_Neg_L3:
			RetVal = EMU_energy.Rail_Nega_Reactive[3]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Rail_Nega_Reactive[3]%=100L;
			break;
		case	REACTIVE_Energy_Q1_T://Total quadrant reactive energy
		case	REACTIVE_Energy_Q2_T:
		case	REACTIVE_Energy_Q3_T:
		case	REACTIVE_Energy_Q4_T:
			RetVal = EMU_energy.Rail_Quad_Reactive[0][id-REACTIVE_Energy_Q1_T]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Rail_Quad_Reactive[0][id-REACTIVE_Energy_Q1_T]%=100L;
			break;
		case	REACTIVE_Energy_Q1_L1://Phase L1 quadrant reactive energy
		case	REACTIVE_Energy_Q2_L1:
		case	REACTIVE_Energy_Q3_L1:
		case	REACTIVE_Energy_Q4_L1:
			RetVal = EMU_energy.Rail_Quad_Reactive[0][id-REACTIVE_Energy_Q1_L1]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Rail_Quad_Reactive[0][id-REACTIVE_Energy_Q1_L1]%=100L;
			break;
		case	REACTIVE_Energy_Q1_L2://Phase L2 quadrant reactive energy
		case	REACTIVE_Energy_Q2_L2:
		case	REACTIVE_Energy_Q3_L2:
		case	REACTIVE_Energy_Q4_L2:
			RetVal = EMU_energy.Rail_Quad_Reactive[0][id-REACTIVE_Energy_Q1_L2]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Rail_Quad_Reactive[0][id-REACTIVE_Energy_Q1_L2]%=100L;
			break;
		case	REACTIVE_Energy_Q1_L3://Phase L3 quadrant reactive energy
		case	REACTIVE_Energy_Q2_L3:
		case	REACTIVE_Energy_Q3_L3:
		case	REACTIVE_Energy_Q4_L3:
			RetVal = EMU_energy.Rail_Quad_Reactive[0][id-REACTIVE_Energy_Q1_L3]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Rail_Quad_Reactive[0][id-REACTIVE_Energy_Q1_L3]%=100L;
			break;
		case	ACTIVE_ADD_T_T:
		case	ACTIVE_ADD_T_L1:
		case	ACTIVE_ADD_T_L2:
		case	ACTIVE_ADD_T_L3:
			RetVal = EMU_energy.Active_Add_T[id-ACTIVE_ADD_T_T];//--->0.1wh=0.001wh/100L
			EMU_energy.Active_Add_T[id-ACTIVE_ADD_T_T] = 0;
			//EMU_energy.Active_Add_T[id-ACTIVE_ADD_T_T]%=100L;
			break;
		case	REACTIVE_ADD_T_T:
		case	REACTIVE_ADD_T_L1:
		case	REACTIVE_ADD_T_L2:
		case	REACTIVE_ADD_T_L3:
			RetVal = EMU_energy.Reative_Add_T[id-REACTIVE_ADD_T_T]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Reative_Add_T[id-REACTIVE_ADD_T_T]%=100L;
			break;
		case	ACTIVE_SUB_T_T:
		case	ACTIVE_SUB_T_L1:
		case	ACTIVE_SUB_T_L2:
		case	ACTIVE_SUB_T_L3:
			RetVal = EMU_energy.Active_Sub_T[id-ACTIVE_SUB_T_T]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Active_Sub_T[id-ACTIVE_SUB_T_T]%=100L;
			break;
		case	REACTIVE_SUB_T_T:
		case	REACTIVE_SUB_T_L1:
		case	REACTIVE_SUB_T_L2:
		case	REACTIVE_SUB_T_L3:
			RetVal = EMU_energy.Reative_Sub_T[id-REACTIVE_SUB_T_T]/100L;//--->0.1wh=0.001wh/100L
			EMU_energy.Reative_Sub_T[id-REACTIVE_SUB_T_T]%=100L;
			break;
        default:
        {
            RetVal = 0;
            break;
        }
    }
    return RetVal;
}


