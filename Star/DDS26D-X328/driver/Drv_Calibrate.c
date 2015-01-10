/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：Drv_Calibrate.c
* 文件标识：
* 摘要：ATT7059校正及统一修调
*******************************************************************/
#include "Include.h"

//常量定义-----------------------------------
uint8 I0_PRO_SWI;//权限开关P04
					

//--------------- 统一修调版本及密码权限-----------
const uint8 CalibrateVersionCode[] =		//校表通信中的版本号
{
    'V','0','.','1'
};

const uint8 CalibratePermitCode[] =		//校表通信中的密码权限
{
    'S','T','A','R'
};

//-----------------统一修调结构体-----------
flag  CalibrationStatusType;
CalibrateComDataType  CalibrateComData;//上位机发送下来的较表参数
AutomaticCalibrationType  AutomaticCalibrationData; 

//-----------------统一修调通讯-----------
Calibrate_Uart_type  Calibrate_uart;//
Calib_Comm_id id;
flag Calib_uart_err={0};						//通讯错误标志

#define	P_Mxa  (uint32)(((uint32)Calibrate_meter.Cali_Par.Un ) * ((uint32)Calibrate_meter.Cali_Par.Imax )  /10000L* 14l*3l)  //2400*100000*1.4*3/1000=1008000 =100.8000KW
#define	P_Limit (uint32)(((uint32)Calibrate_meter.Cali_Par.Un )* ((uint32)Calibrate_meter.Cali_Par.Ib )/1000l * 2l /1000L)  //2400*10000/1000*2/1000=48000/1000 =48W //启动  0.2 % Ib



static uint32 Hex_Bcd(uint32 hex, uint8 n) 
{
	uint8 i;
	uint32 bcd=0;

	if((hex<=(uint32)99999999) && (n>0) && (n<=4))
	{
		n<<=1;
		for(i=0;i<n;i++)
		{
			bcd|=(hex%10)<<(i<<2);			
			hex/=10;
		}
	}
	return bcd;
}

 void DRV_Calibrate_PRO_SWI_INIT(void)
{
	IO_DisableFunc(IO_PORT_PRO_SWI,IO_PIN_PRO_SWI);
  	IO_ConfigGPIOPin(IO_PORT_PRO_SWI,IO_PIN_PRO_SWI,IO_DIR_INPUT,IO_PULLUP_ON);
}

 void DRV_Calibrate_PRO_SWI_POWER_DOWN_INIT(void)
{
	IO_DisableFunc(IO_PORT_PRO_SWI,IO_PIN_PRO_SWI);
  	IO_ConfigGPIOPin(IO_PORT_PRO_SWI,IO_PIN_PRO_SWI,IO_DIR_OUTPUT,IO_PULLUP_OFF);
    IO_WriteGPIOPin(IO_PORT_PRO_SWI,IO_PIN_PRO_SWI,IO_BIT_CLR);
}

void Drv_Calibrate_Init(void)
{
	//----------------Drv_Calibrate init-------------
	memset((uint8 *)&CalibrateComData,0, (uint16)sizeof(CalibrateComData));
	memset((uint8 *)&AutomaticCalibrationData,0, (uint16)sizeof(AutomaticCalibrationData));
	memset((uint8 *)&CalibrationStatusType,0, (uint16)sizeof(CalibrationStatusType));
}

void Drv_Calibrate_DLT6452007(uint8 mode)
{
	uint16  i=0,EEPLenth=0;//,EMURegAddr=0;//,j,k,m,n;
	uint32 datac=0,EEPAddr=0;//,datad;
	uint8 * UartAddrr;
	uint8 UartCmdLength=0,UartDI[4],UartBuf[219]; 
	int16 temp=0;
	flag Calib_uart_err={0};//通讯错误标志
	tDateTime  AdjustTime_07;
	//U32Type	tempdata;

	//--------clear buffer------
	memset((uint8 *)&UartDI,0, (uint16)sizeof(UartDI));
	memset((uint8 *)&UartBuf,0, (uint16)sizeof(UartBuf));
	memset((uint8 *)&CalibrateComData,0, (uint16)sizeof(CalibrateComData));
	Calib_uart_err.byte=0x00;
	//--------copy buffer------
	memcpy(((uint8 *)&UartDI[0]), (uint8 *)&uart_buff->Buffer[10],4);
	if((uart_buff->Buffer[8]==0x14))//write data 
	{//L=DI0 DI1 DI2 DI3+ PA P0 P1 P2+C0 C1 C2 C3+DATA
		UartCmdLength=uart_buff->Buffer[9]-0x0c;
	}
	else if(uart_buff->Buffer[8]==0x11)//read data 
	{
		if((UartDI[3]==0xe0))//E0 00 10 00 
		{//0x68	AD	0x68	0x14  L  DI0~DI3	  AD0~AD3 	LD0~LD1 CS 0x16
			UartCmdLength=uart_buff->Buffer[9];
		}
	}
	memcpy(((uint8 *)&UartBuf[0]), (uint8 *)&uart_buff->Buffer[14],(uart_buff->Buffer[9]-4) );
	switch(UartDI[3])
	{
		case 0x02:
			switch(UartDI[2])
			{
				case 0x80:
					switch(UartDI[1])
					{
						case 0x00:
							switch(UartDI[0])
							{
								case 0x08://02 80 00 08 时钟电池电压(内部)
									temp=BattVoltageGet();
									datac=Hex_Bcd(temp,2);  
									UartAddrr=(uint8*)&datac; 
									UartCmdLength=2; 
									break;
								default:Calib_uart_err.byte=0x02;	
									break;	
							}		
							break;
						default:Calib_uart_err.byte=0x02;	
							break;	
					}
					break;
				default:Calib_uart_err.byte=0x02;	
					break;	
			}
			break;
		case 0xe0://E0 XX XX XX  统一修调使用
			switch(UartDI[2])
			{
				case 0x00://E0 00 XX XX
					if((I0_PRO_SWI==0)||(mode==0))
					{
						//短接权限开关
						if(((UartBuf[4]==CalibrateVersionCode[0])&& (UartBuf[5]==CalibrateVersionCode[1])
						&&(UartBuf[6]==CalibrateVersionCode[2])&&(UartBuf[7]==CalibrateVersionCode[3]))
						||(UartDI[1]==Calibrate_BasicParameterSet)||(UartDI[1]==Calibrate_ResetSet)||(mode==0))
						{//版本号验证
							if(((UartBuf[0]==CalibratePermitCode[0])&& (UartBuf[1]==CalibratePermitCode[1])
							&&(UartBuf[2]==CalibratePermitCode[2])&&(UartBuf[3]==CalibratePermitCode[3]))
							||(UartDI[1]==Calibrate_BasicParameterSet)||(UartDI[1]==Calibrate_ResetSet)||(mode==0))
							{//密码验证
								switch(UartDI[1])
								{
									case 0x00://E0 00 00 XX
										break;
									case Calibrate_BasicParameterSet:  //E0 00 01 XX 参数设置命令
										if(mode)
										{
											if(UartCmdLength==13)
											{
												memcpy((uint8*)&CalibrateComData.CalibrateBasicData.BasicData.Frequency,(uint8 *)&UartBuf[8],13); //1+n*12 bytes
												//CalibrateComData.Type = Calibrate_BasicParameterType;
												//CalibrationStartFlag = 1;
												Calibrate_meter.Cali_Par.Freq=CalibrateComData.CalibrateBasicData.BasicData.Frequency;//xxx.x
												Calibrate_meter.Cali_Par.Un=CalibrateComData.CalibrateBasicData.BasicData.VoltageB;//xxx.x
												Calibrate_meter.Cali_Par.Ib=CalibrateComData.CalibrateBasicData.BasicData.CurrentB*100;//xxx.xxx
												Calibrate_meter.Cali_Par.Imax=CalibrateComData.CalibrateBasicData.BasicData.CurrentMax*100;//xxx.xxx
												Calibrate_meter.Cali_Par.Meter_const_act=CalibrateComData.CalibrateBasicData.BasicData.Constant_Kwh;
												Calibrate_meter.Cali_Par.Meter_const_react=CalibrateComData.CalibrateBasicData.BasicData.Constant_Kvah;
												CalcCheckSum(Calibrate_meter.Cali_Par);
												if(!NvmBytesWrite(EE_CALIBRATE_ADDR_PAR, &Calibrate_meter.Cali_Par, sizeof(Calibrate_meter.Cali_Par)))
												{
													Calib_uart_err.byte=0x02;
												}
											}
											else	
											{
												Calib_uart_err.byte=0x02;
											}
										}
										else
										{
											datac=(uint32)Calibrate_meter.Cali_Par.Freq;//freq XX
											memcpy(((uint8 *)&UartBuf[0]), (uint8 *)&datac,1 );
											datac=(uint32)Calibrate_meter.Cali_Par.Un;//un XXX.X
											memcpy(((uint8 *)&UartBuf[1]), (uint8 *)&datac,2 );
											datac=(uint32)Calibrate_meter.Cali_Par.Ib/100;//Ib xxx.x
											memcpy(((uint8 *)&UartBuf[3]), (uint8 *)&datac,2 );
											datac =(uint32)Calibrate_meter.Cali_Par.Imax/100;//Imax xxx.x
											memcpy(((uint8 *)&UartBuf[5]), (uint8 *)&datac,2 );
											datac =(uint32)Calibrate_meter.Cali_Par.Meter_const_act;//	 xxxx
											memcpy(((uint8 *)&UartBuf[7]), (uint8 *)&datac,2 );
											datac =(uint32)Calibrate_meter.Cali_Par.Meter_const_react;//   xxxx
											memcpy(((uint8 *)&UartBuf[9]), (uint8 *)&datac,2 );
											datac =(uint32)2;//   xxxx
											memcpy(((uint8 *)&UartBuf[11]), (uint8 *)&datac,2 );
											UartAddrr=UartBuf;	
											UartCmdLength=0x0d;
										}
										break;
									case Calibrate_PGainSet://E0 00 02 XX 功率法修调增益
										if(mode)
										{
											if(UartCmdLength==13)
											{
												if(EMU_variable.Act_Power[0]>P_Limit||EMU_variable.Act_Power[0]<-(int32)P_Limit)
												{
													memcpy((uint8*)&CalibrateComData.CalibrateBasicData.PGainData.EmuNum,(uint8 *)&UartBuf[8],13); //1+n*12 bytes
													CalibrateComData.Type = Calibrate_PGainSetType;
													CalibrationStartFlag = 1;
												}
												else	Calib_uart_err.byte=0x05; 
											}
											else  Calib_uart_err.byte=0x02;
										}
										break;
									case Calibrate_EGainSet://E0 00 03 XX 误差法修调增益
										if(mode)
										{
											if(UartCmdLength==11)
											{
												if(EMU_variable.Act_Power[0]>P_Limit||EMU_variable.Act_Power[0]<-(int32)P_Limit)
												{
													memcpy((uint8*)&CalibrateComData.CalibrateBasicData.EGainData.EmuNum,(uint8 *)&UartBuf[8],11); //11bytes
													CalibrateComData.Type = Calibrate_EGainSetType;
													CalibrationStartFlag = 1;
												}
												else	Calib_uart_err.byte=0x05; 
											}
											else  Calib_uart_err.byte=0x02;
										}
										break;
									case Calibrate_PPhaseAngleSet:	//E0 00 04 XX 功率法相角修调
										if(mode)
										{
											if(UartCmdLength==7)
											{
												if(EMU_variable.Act_Power[0]>P_Limit||EMU_variable.Act_Power[0]<-(int32)P_Limit)
												{
													memcpy((uint8*)&CalibrateComData.CalibrateBasicData.PAngleData.EmuNum,(uint8 *)&UartBuf[8],7); //1+n*6 bytes
													CalibrateComData.Type = Calibrate_PPhaseAngleType;
													CalibrationStartFlag = 1;
												}
												else	Calib_uart_err.byte=0x05; 
											}
											else  Calib_uart_err.byte=0x02;
										}
										break;
									case Calibrate_EPhaseAngleSet://E0 00 05 XX误差法相角修调
										if(mode)
										{
											if(UartCmdLength==3)
											{
												if(EMU_variable.Act_Power[0]>P_Limit||EMU_variable.Act_Power[0]<-(int32)P_Limit)
												{
													memcpy((uint8*)&CalibrateComData.CalibrateBasicData.EAngleData.EmuNum,(uint8 *)&UartBuf[8],3); // 3 bytes
													CalibrateComData.Type =Calibrate_EPhaseAngleType;
													CalibrationStartFlag = 1;
												}
												else	Calib_uart_err.byte=0x05; 
											}
											else  Calib_uart_err.byte=0x02;
										}
										break;
									case Calibrate_POffsetSet://E0 00 06 XX功率法偏置修调
										if(mode)
										{
											if(UartCmdLength==9)
											{
												if(EMU_variable.Act_Power[0]>P_Limit||EMU_variable.Act_Power[0]<-(int32)P_Limit)//P_Limit
												{
													memcpy((uint8*)&CalibrateComData.CalibrateBasicData.POffsetData.EmuNum,(uint8 *)&UartBuf[8],9); //1+n*8 bytes
													CalibrateComData.Type = Calibrate_POffsetType;
													CalibrationStartFlag = 1;
												}
												else	Calib_uart_err.byte=0x05; 
											}							
											else  Calib_uart_err.byte=0x02;
										}
										break;
									case Calibrate_EOffsetSet://E0 00 07 XX误差法偏置修调
										if(mode)
										{
											if(UartCmdLength==3)
											{
												if(EMU_variable.Act_Power[0]>P_Limit||EMU_variable.Act_Power[0]<-(int32)P_Limit)//P_Limit
												{
													memcpy((uint8*)&CalibrateComData.CalibrateBasicData.EOffsetData.EmuNum,(uint8 *)&UartBuf[8],3);// 3 bytes 
													CalibrateComData.Type =Calibrate_EOffsetType;
													CalibrationStartFlag = 1;
												}
												else	Calib_uart_err.byte=0x05; 
											}
											else  Calib_uart_err.byte=0x02;
										}
										break;
									case Calibrate_Gain_NCurrentSet://E0 00 08 XX零线电流有效值修调
										if(mode)
										{
											if(UartCmdLength==4)
											{
												//if(EMU_variable.Act_Power[EMU_T]>P_Limit||EMU_variable.Act_Power[EMU_T]<-(int32)P_Limit)//P_Limit
												//{
													memcpy((uint8*)&CalibrateComData.Irms,(uint8 *)&UartBuf[8],4); // 4 bytes
													CalibrateComData.Type = Calibrate_Gain_NCurrentType;
													CalibrationStartFlag = 1;
												//}
												//else	Calib_uart_err.byte=0x05; 
											}
											else  Calib_uart_err.byte=0x02;
										}
										break;
									case Calibrate_TemperatureSet://E0 00 09 XX温度修调
										if(mode)
										{
											if(UartCmdLength==2)
											{
												memcpy((uint8*)&CalibrateComData.Toff,(uint8 *)&UartBuf[8],2);  // 2 bytes
												CalibrateComData.Type = Calibrate_TemperatureType;
												CalibrationStartFlag = 1;
											}
											else  Calib_uart_err.byte=0x02;
										}
										break;
									case Calibrate_ResetSet://E0 00 0a XX修调寄存器初始化
										if(mode)
										{
										//	if(EMU_variable.Act_Power[0]>P_Limit)//P_Limit
										//	{
												memset((uint8*)&CalibrateComData,0,sizeof(CalibrateComData));
												CalibrateComData.Type = Calibrate_ResetType;
												CalibrationStartFlag = 1;
										//	}
										//	else	
										//	{
										//		Calib_uart_err.byte=0x02;
										//	}
										}
										break;
									case Calibrate_PPMSet://E0 00 0b XXPPM值
										if(mode)
										{
											if(UartCmdLength==2)
											{
												memcpy((uint8*)&CalibrateComData.PPMOffset,(uint8 *)&UartBuf[8],2);// sizeof(CalibrateComData.PPMOffset)
												AdjustCryOffset(CalibrateComData.PPMOffset);		//求顶点温度PPM
												CalibrateComData.Type = Calibrate_PPMSetType;
												CalibrationStartFlag = 1;
											}
											else  Calib_uart_err.byte=0x02;
										}
										break;
									case Calibrate_Time:              //07协议校时
										if(mode)
										{
											if(UartCmdLength==7)
											{												
												AdjustTime_07.HighByteYear = (UartBuf[8] + 2000)/0x100;
												AdjustTime_07.LowByteYear = (UartBuf[8] + 2000)%0x100;
												AdjustTime_07.Month = UartBuf[9];
												AdjustTime_07.Day = UartBuf[10];
												AdjustTime_07.Week = CalcWeek(&UartBuf[8]);
												AdjustTime_07.Hour = UartBuf[12];
												AdjustTime_07.Minute = UartBuf[13];
												AdjustTime_07.Second = UartBuf[14];
												API_SetDataValue(0x08000002,(uint8 *)&AdjustTime_07,sizeof(AdjustTime_07));
												CalibrateComData.Type = Calibrate_Time;
												CalibrationStartFlag = 1;
											}
											else  Calib_uart_err.byte=0x02;
										}
										break;
									case 0x10://E0 00 10 00 读写存储器
										if(mode)
										{
											memcpy((uint8*)&EEPAddr,(uint8 *)&UartBuf[8],4);
											memcpy((uint8*)&EEPLenth,(uint8 *)&UartBuf[12],2);
											memcpy((uint8*)&UartBuf,(uint8 *)&UartBuf[14],EEPLenth);
											if(!NvmBytesDirectWrite(EEPAddr, &UartBuf, EEPLenth))//1--success  0--fail 
											{
												Calib_uart_err.byte=0x02;	
											}
											//------just for test-------
											//memset((uint8 *)&UartBuf,0, (uint16)sizeof(UartBuf));
											//EE_READ_DATA(EEPAddr, &UartBuf, EEPLenth);
											//---------end-----------
										}
										else
										{
											if(I0_PRO_SWI==0)
											{
												if(UartCmdLength==10)
												{
													memcpy((uint8*)&EEPAddr,(uint8 *)&UartBuf[0],4);
													memcpy((uint8*)&EEPLenth,(uint8 *)&UartBuf[4],2);
													//if(EEPLenth<=135)//限制抄读长度
													//{
														if(1==NvmBytesDirectRead(EEPAddr, &UartBuf, EEPLenth))//1--success  0--fail 
														{
															UartAddrr = UartBuf;
															UartCmdLength=EEPLenth;
														}
														else
														{
															Calib_uart_err.byte=0x02;	
														}
													//}
													//else
													//{
													//	Calib_uart_err.byte=0x02;
													//}
												}
												else 
												{
													Calib_uart_err.byte=0x02;											
												}
											}
											else
											{
												Calib_uart_err.byte=0x02;	
											}
										}
										break;
									default:Calib_uart_err.byte=0x02;
										break;
								}
							}
							else Calib_uart_err.byte=0x04; //密码错误
						}
						else Calib_uart_err.byte=0x08; //版本号验证
					}
					else Calib_uart_err.byte=0x04;  //无权限开关
					break;
				case 0x01://E0 01 XX XX
					switch(UartDI[0])
					{
						case 0x00://E0 01 00 00  合相参数
							datac= (uint32)EMU_variable.Volt[1]*10; //电压XXX.xxx
							memcpy(((uint8 *)&UartBuf[0]), (uint8 *)&datac,4 );
							datac= (uint32)EMU_variable.Curr[1]; //电流XXX.xxx
							memcpy(((uint8 *)&UartBuf[4]), (uint8 *)&datac,4 );
							datac =(uint32)EMU_variable.Act_Power[1]*10;//功率 XXX.xxx W
							memcpy(((uint8 *)&UartBuf[8]), (uint8 *)&datac,4 );
							datac =(uint32)EMU_variable.Factor[1];//功因X.xxx
							memcpy(((uint8 *)&UartBuf[12]), (uint8 *)&datac,2 );
							
							datac= (uint32)EMU_variable.Volt[2]*10; //电压XXX.xxx
							memcpy(((uint8 *)&UartBuf[14]), (uint8 *)&datac,4 );
							datac= (uint32)EMU_variable.Curr[2]; //电流XXX.xxx
							memcpy(((uint8 *)&UartBuf[18]), (uint8 *)&datac,4 );
							datac =(uint32)EMU_variable.Act_Power[2]*10;//功率 XXX.xxx
							memcpy(((uint8 *)&UartBuf[22]), (uint8 *)&datac,4 );
							datac =(uint32)EMU_variable.Factor[2];//功因X.xxx
							memcpy(((uint8 *)&UartBuf[26]), (uint8 *)&datac,2 );
							UartAddrr=UartBuf;	
							UartCmdLength=0x1c;
							break;
						case 0x01://E0 01 00 01 L相参数
							datac= (uint32)EMU_variable.Volt[1]*10; //电压XXX.xxx
							memcpy(((uint8 *)&UartBuf[0]), (uint8 *)&datac,4 );
							datac= (uint32)EMU_variable.Curr[1]; //电流XXX.xxx
							memcpy(((uint8 *)&UartBuf[4]), (uint8 *)&datac,4 );
							datac =(uint32)EMU_variable.Act_Power[1]*10;//功率 XXX.xxx W
							memcpy(((uint8 *)&UartBuf[8]), (uint8 *)&datac,4 );
							datac =(uint32)EMU_variable.Factor[1];//功因X.xxx
							memcpy(((uint8 *)&UartBuf[12]), (uint8 *)&datac,2 );
							UartAddrr=UartBuf;	
							UartCmdLength=0x0e;
							break;
						case 0x03://E0 01 00 03 N相参数
							datac= (uint32)EMU_variable.Volt[2]*10; //电压XXX.xxx
							memcpy(((uint8 *)&UartBuf[0]), (uint8 *)&datac,4 );
							datac= (uint32)EMU_variable.Curr[2]; //电流XXX.xxx
							memcpy(((uint8 *)&UartBuf[4]), (uint8 *)&datac,4 );
							datac =(uint32)EMU_variable.Act_Power[2]*10;//功率 XXX.xxx
							memcpy(((uint8 *)&UartBuf[8]), (uint8 *)&datac,4 );
							datac =(uint32)EMU_variable.Factor[2];//功因X.xxx
							memcpy(((uint8 *)&UartBuf[12]), (uint8 *)&datac,2 );
							UartAddrr=UartBuf;	
							UartCmdLength=0x0e;
							break;
						default:Calib_uart_err.byte=0x02;	
							break;	   
				 }
				 break;
			   case 0x02://E0 02 XX XX
			   		switch(UartDI[0])
					{
						case 0x00://E0 02 XX 00  零线电流
							//datac=EMU_variable.Curr[EMU_N]/10;
							UartAddrr=(uint8 *)&datac;
							UartCmdLength=4;
							break;
					}
					break;
			   case 0x03://E0 03 XX XX
			   		switch(UartDI[0])
					{
						case 0x00://E0 03 XX 00	PPM
							//datac=Calibrate_meter.w_RTC_PPM*10;
							temp=(int16)(CalParameter.CryOffset/(int32)100);
							UartAddrr=(uint8 *)&temp;
							UartCmdLength=2;	  
							break;
					}
					break;
			   case 0x04:
			   		switch(UartDI[0])          //读时间和日期
					{
						case 0x00:
							{
								API_GetDataValue(Clock_IDCode,&AdjustTime_07);
								UartBuf[0] = (uint8)(AdjustTime_07.HighByteYear*0x100 + AdjustTime_07.LowByteYear - 2000);
								memcpy((uint8 *)&UartBuf[1],(uint8 *)&AdjustTime_07.Month,6);
								UartAddrr = UartBuf;
								UartCmdLength=7;	  
								break;			
						    }
							
					}
			   		break;
			   case 0x80://E0 80 XX XX
			   		switch(UartDI[0])
					{
						case 0x07://E0 80 XX 07 表内温度
							//datac=EMU_variable.temperature_hex;
							//UartAddrr=(uint8 *)& datac;
							TempGet(&temp, 1);
							UartAddrr=(uint8 *)&temp;
							UartCmdLength=2;	
							break;
                        case 0xff:
                            memcpy((uint8 *)&UartBuf,(uint8 *)&RTCResetRecord,sizeof(RTCResetRecord));
                            UartAddrr = UartBuf;
							UartCmdLength=sizeof(RTCResetRecord);	    
                            break;      
					}
					break;
			   default:Calib_uart_err.byte=0x02;
			   		break;   	   
			}
			break;
		case 0xe1://E1 XX XX XX 统一修调使用
			switch(UartDI[0])
			{
				case 0x00://E1 00 00 00 厂家（生产）软件版本号(ASCII码)
					UartAddrr = UartBuf;
					MeterInfoParaGet(0x01510002, UartAddrr);
					PublicSwap(UartAddrr, 32);
					UartCmdLength=32;
					break;
				case 0x01://E1 00 00 01厂家（生产）硬件版本号(ASCII码)
					UartAddrr = UartBuf;
					MeterInfoParaGet(0x01520002, UartAddrr);
					PublicSwap(UartAddrr, 32);
					UartCmdLength=32;
					break;	
			}
			break;
		default:Calib_uart_err.byte=0x02;
			break;
	}
	if(!mode)// read data 
	{
		for(i=0;i<UartCmdLength;i++) 
		{
			UartBuf[i]=*(UartAddrr+i);
		}
		//return data back
		uart_buff->Buffer[9]=UartCmdLength+4;//data long
		memcpy((uint8 *)&(uart_buff->Buffer[10]),(uint8 *)&UartDI[0],4); //DI
		memcpy((uint8 *)&(uart_buff->Buffer[14]),(uint8 *)&UartBuf[0],UartCmdLength); //data	
	}
	else//write data
	{
		if(Calib_uart_err.byte==0x00)//return turn
		{
			uart_buff->Buffer[9]=0;
		}
	}
	uart_err.byte =Calib_uart_err.byte;
}


#pragma optimize=none
void AutomaticCalibrationProcess(void)
{
	uint8  Flag = 0,i;
	float  Err_Data;
    float  Err_k;
    uint32 temp=0;
	uint32 backup_temp11=0;
	uint32 backup_temp=0;
	EMU_NO  Emu_use_flag;
	U32Type Data_Temp;
	if(IO_ReadGPIOPin(IO_PORT_PRO_SWI,IO_PIN_PRO_SWI))
	{
		I0_PRO_SWI=1;
	}
	else
	{
		I0_PRO_SWI=0;//短接权限开关
	}
	if(I0_PRO_SWI)//断开权限开关	
	{ 
		CalibrateComData.Type = 0;
		CalibrationStartFlag = 0;
		return;
	}
	if((CalibrateComData.Type == Calibrate_ResetType)&&(I0_PRO_SWI==0))//修调参数初始化
	{
		if(I0_PRO_SWI==0)	 //短接权限开关才可以修调
		{
			PublicDelayMs(2);
			if(I0_PRO_SWI==0)	 //短接权限开关才可以修调
			{	
				CalibrateComData.Type = 0;
				DRV_EMU_Initial_Calibrate(EMU_L1);
				DRV_EMU_Initial_Calibrate(EMU_L2);
				caliEepError=0;     //校表数据出错清除，用于显示出错
			}	
			return;
		}
	}
	if(!CalibrationStartFlag)  	//触发修调
		return;
	CalibrationStartFlag = 0;
	if(CalibrateComData.Type == Calibrate_PGainSetType)//0x02 功率法修调增益类型
	{
		if(CalibrateComData.CalibrateBasicData.PGainData.EmuNum==1)     //L路
		{
			Emu_use_flag=EMU_L1;
			Calibrate_meter.Cali_EMU_L1.GP1_7059=0;
			Calibrate_meter.Cali_EMU_L1.GQ1_7059=0;
			Calibrate_meter.Cali_EMU_L1.GS1_7059=0;
			EMUUartAutoPause();
			EMUCommand(EMU_L1,W_ENABLE_1);
            Data_Temp.l = Calibrate_meter.Cali_EMU_L1.GP1_7059;
			EMUWrite(EMU_L1,GP1_W,2,(uint8 *)&Data_Temp.l);
			
			Data_Temp.l = Calibrate_meter.Cali_EMU_L1.GQ1_7059;
			EMUWrite(EMU_L1,GQ1_W,2,(uint8 *)&Data_Temp.l);

			Data_Temp.l = Calibrate_meter.Cali_EMU_L1.GS1_7059;
			EMUWrite(EMU_L1,GS1_W,2,(uint8 *)&Data_Temp.l);
			EMUCommand(EMU_L1,W_DISABLE);                          
			EMUUartAutoGoOn();
			for(i=0;i<8;i++)
			{
				Feed_watchdog();
				PublicDelayMs(250);
			}
			Get_Att7059_PUI(EMU_L1,BigCurrentCounts);//延迟2s再取7059寄存器值20140210	
			temp=CalibrateComData.CalibrateBasicData.PGainData.Power/10;
			if(temp>AutomaticCalibrationData.Power_P[0])
			{
			
				Err_Data=(float)(temp-AutomaticCalibrationData.Power_P[0])/AutomaticCalibrationData.Power_P[0];
				Calibrate_meter.Cali_EMU_L1.GP1_7059=(uint16)(Err_Data*32768);
				Calibrate_meter.Cali_EMU_L1.GQ1_7059=Calibrate_meter.Cali_EMU_L1.GP1_7059;
				Calibrate_meter.Cali_EMU_L1.GS1_7059=Calibrate_meter.Cali_EMU_L1.GP1_7059;
			}
			else if(temp<AutomaticCalibrationData.Power_P[0])
			{			
				Err_Data=(float)(AutomaticCalibrationData.Power_P[0]-temp)/AutomaticCalibrationData.Power_P[0];
				Calibrate_meter.Cali_EMU_L1.GP1_7059=(uint16)((float)(65536-Err_Data*32768));
				Calibrate_meter.Cali_EMU_L1.GQ1_7059=Calibrate_meter.Cali_EMU_L1.GP1_7059;
				Calibrate_meter.Cali_EMU_L1.GS1_7059=Calibrate_meter.Cali_EMU_L1.GP1_7059;
			}
			temp=CalibrateComData.CalibrateBasicData.PGainData.Voltage/10;
			Calibrate_meter.Cali_EMU_L1.URMS_refer_7059 =EMU_variable.Volt[1]*Calibrate_meter.Cali_EMU_L1.URMS_refer_7059/temp;
			temp=CalibrateComData.CalibrateBasicData.PGainData.Current;
			Calibrate_meter.Cali_EMU_L1.IRMS_refer_7059 =EMU_variable.Curr[1]*Calibrate_meter.Cali_EMU_L1.IRMS_refer_7059/temp;			
			Flag = 1;
		}
		else if(CalibrateComData.CalibrateBasicData.PGainData.EmuNum==3)//N路
		{
			Emu_use_flag=EMU_L2;
			Calibrate_meter.Cali_EMU_L2.GP1_7059=0;
			Calibrate_meter.Cali_EMU_L2.GQ1_7059=0;
			Calibrate_meter.Cali_EMU_L2.GS1_7059=0;
			EMUUartAutoPause();
			EMUCommand(EMU_L2,W_ENABLE_1);
			Data_Temp.l = Calibrate_meter.Cali_EMU_L2.GP1_7059;
			EMUWrite(EMU_L2,GP1_W,2,(uint8 *)&Data_Temp.l);

			Data_Temp.l = Calibrate_meter.Cali_EMU_L2.GQ1_7059;
			EMUWrite(EMU_L2,GQ1_W,2,(uint8 *)&Data_Temp.l);

			Data_Temp.l = Calibrate_meter.Cali_EMU_L2.GS1_7059;
			EMUWrite(EMU_L2,GS1_W,2,(uint8 *)&Data_Temp.l);
			EMUCommand(EMU_L2,W_DISABLE);
			EMUUartAutoGoOn();
			for(i=0;i<8;i++)
			{
				Feed_watchdog();
				PublicDelayMs(250);
			}
			Get_Att7059_PUI(EMU_L2,BigCurrentCounts);//延迟2s再取7059寄存器值20140210			
			temp=CalibrateComData.CalibrateBasicData.PGainData.Power/10;
			if(temp>AutomaticCalibrationData.Power_P[0])
			{
			
				Err_Data=(float)(temp-AutomaticCalibrationData.Power_P[0])/AutomaticCalibrationData.Power_P[0];
				Calibrate_meter.Cali_EMU_L2.GP1_7059=(uint16)(Err_Data*32768);
				Calibrate_meter.Cali_EMU_L2.GQ1_7059=Calibrate_meter.Cali_EMU_L2.GP1_7059;
				Calibrate_meter.Cali_EMU_L2.GS1_7059=Calibrate_meter.Cali_EMU_L2.GP1_7059;
			}
			else if(temp<AutomaticCalibrationData.Power_P[0])
			{
			
				Err_Data=(float)(AutomaticCalibrationData.Power_P[0]-temp)/AutomaticCalibrationData.Power_P[0];
				Calibrate_meter.Cali_EMU_L2.GP1_7059=(uint16)((float)(65536-Err_Data*32768));
				Calibrate_meter.Cali_EMU_L2.GQ1_7059=Calibrate_meter.Cali_EMU_L2.GP1_7059;
				Calibrate_meter.Cali_EMU_L2.GS1_7059=Calibrate_meter.Cali_EMU_L2.GP1_7059;
			}
			temp=CalibrateComData.CalibrateBasicData.PGainData.Voltage/10;
			Calibrate_meter.Cali_EMU_L2.URMS_refer_7059 =EMU_variable.Volt[2]*Calibrate_meter.Cali_EMU_L2.URMS_refer_7059/temp;
			temp=CalibrateComData.CalibrateBasicData.PGainData.Current;
			Calibrate_meter.Cali_EMU_L2.IRMS_refer_7059 =EMU_variable.Curr[2]*Calibrate_meter.Cali_EMU_L2.IRMS_refer_7059/temp;		
			Flag = 1;
		}
	}
	else if(CalibrateComData.Type == Calibrate_EGainSetType)//  0x03 误差法修调增益类型
	{;
	}
	else if(CalibrateComData.Type == Calibrate_PPhaseAngleType)// 0x04 功率法相角修调类型
	{
		if(CalibrateComData.CalibrateBasicData.PAngleData.EmuNum==1)//phase L1   
		{
			Emu_use_flag=EMU_L1;
			Calibrate_meter.Cali_EMU_L1.Phs_7059=(uint16)0;
			EMUUartAutoPause();
			EMUCommand(EMU_L1,W_ENABLE_1);//WRITE 50-75H 
			Data_Temp.l = Calibrate_meter.Cali_EMU_L1.Phs_7059;
			EMUWrite(EMU_L1,GPhs1_W,2,(uint8 *)&Data_Temp.l);//20131226
			EMUCommand(EMU_L1,W_DISABLE);
			EMUUartAutoGoOn();
			for(i=0;i<8;i++)
			{
				Feed_watchdog();
				PublicDelayMs(250);
			}
			Get_Att7059_PUI(EMU_L1,BigCurrentCounts);//延迟2s再取7059寄存器值20140210
			temp=CalibrateComData.CalibrateBasicData.PAngleData.Power/10;
			if(temp>AutomaticCalibrationData.Power_P[0])
			{
				Err_Data=(float)(temp-AutomaticCalibrationData.Power_P[0])/temp;
				Calibrate_meter.Cali_EMU_L1.Phs_7059=(uint16)((float)(Err_Data*32768+0.866)/1.732);
			}
			else if(temp<AutomaticCalibrationData.Power_P[0])
			{
				Err_Data=(float)(AutomaticCalibrationData.Power_P[0]-temp)/temp;
				Calibrate_meter.Cali_EMU_L1.Phs_7059=(uint16)(65536-(float)(Err_Data*32768+0.866)/1.732);
			}
			Flag = 1;
		}
		else if(CalibrateComData.CalibrateBasicData.PAngleData.EmuNum==3)//phase L2  
		{
			Emu_use_flag=EMU_L2;
			Calibrate_meter.Cali_EMU_L2.Phs_7059=(uint16)0;
			EMUUartAutoPause();
			EMUCommand(EMU_L2,W_ENABLE_1);//WRITE 50-75H 
			Data_Temp.l = Calibrate_meter.Cali_EMU_L2.Phs_7059;
			EMUWrite(EMU_L2,GPhs1_W,2,(uint8 *)&Data_Temp.l);//20131226
			EMUCommand(EMU_L2,W_DISABLE);
			EMUUartAutoGoOn();
			for(i=0;i<8;i++)
			{
				Feed_watchdog();
				PublicDelayMs(250);
			}
			Get_Att7059_PUI(EMU_L2,BigCurrentCounts);//延迟2s再取7059寄存器值20140210
			temp=CalibrateComData.CalibrateBasicData.PAngleData.Power/10;
			if(temp>AutomaticCalibrationData.Power_P[0])
			{
				Err_Data=(float)(temp-AutomaticCalibrationData.Power_P[0])/temp;
				Calibrate_meter.Cali_EMU_L1.Phs_7059=(uint16)((float)(Err_Data*32768+0.866)/1.732);
			}
			else if(temp<AutomaticCalibrationData.Power_P[0])
			{
				Err_Data=(float)(AutomaticCalibrationData.Power_P[0]-temp)/temp;
				Calibrate_meter.Cali_EMU_L2.Phs_7059=(uint16)(65536-(float)(Err_Data*32768+0.866)/1.732);
			}
			Flag = 1;
		}		
	}
	else if(CalibrateComData.Type == Calibrate_EPhaseAngleType)// 0x05 误差法相角修调类型
	{;
	}
	else if(CalibrateComData.Type == Calibrate_POffsetType)//0x06 功率法偏置修调类型
	{
		if(CalibrateComData.CalibrateBasicData.POffsetData.EmuNum==1)//phase  L1  
		{
			Emu_use_flag=EMU_L1;
			Calibrate_meter.Cali_EMU_L1.APOS_7059=0;	
			EMUUartAutoPause();
			EMUCommand(EMU_L1,W_ENABLE_1);//WRITE 50-75H 
			Data_Temp.l = Calibrate_meter.Cali_EMU_L1.APOS_7059;
			EMUWrite(EMU_L1,P1OFFSETH_W,1,(uint8 *)&Data_Temp.l);
			EMUWrite(EMU_L1,P1OFFSETL_W,1,(uint8*)&Data_Temp.l+1);
			EMUCommand(EMU_L1,W_DISABLE);
			EMUUartAutoGoOn();
			for(i=0;i<8;i++)
			{
				Feed_watchdog();
				PublicDelayMs(250);
			}
			Get_Att7059_PUI(EMU_L1,SmallCurrentCounts);//延迟2s再取7059寄存器值20140210
			temp=CalibrateComData.CalibrateBasicData.POffsetData.Power/10;
            Err_k=(float)0.0381775*(float)Calibrate_meter.Cali_Par.Meter_const_act*(float)Calibrate_meter.Cali_EMU_L1.HFConst_7059;//12613.841
			if(temp<AutomaticCalibrationData.Power_P[0])
			{
            	Calibrate_meter.Cali_EMU_L1.APOS_7059= (uint16)((uint32)65536-((AutomaticCalibrationData.Power_P[0]-temp)*(float)Err_k+50)/100);
			}
			else if (temp>AutomaticCalibrationData.Power_P[0])
			{
            	Calibrate_meter.Cali_EMU_L1.APOS_7059=(uint16)(((temp-AutomaticCalibrationData.Power_P[0])*(float)Err_k+50)/100);        
			}
			Flag = 1;				
		}
		else if(CalibrateComData.CalibrateBasicData.POffsetData.EmuNum==3)//phase L2  
		{
			Emu_use_flag=EMU_L2;
			Calibrate_meter.Cali_EMU_L2.APOS_7059=0;	
			EMUUartAutoPause();
			EMUCommand(EMU_L2,W_ENABLE_1);
			Data_Temp.l = Calibrate_meter.Cali_EMU_L2.APOS_7059;
			EMUWrite(EMU_L2,P1OFFSETH_W,1,(uint8 *)&Data_Temp.l);
			EMUWrite(EMU_L2,P1OFFSETL_W,1,(uint8*)&Data_Temp.l+1);
			EMUCommand(EMU_L2,W_DISABLE);
			EMUUartAutoGoOn();
			for(i=0;i<8;i++)
			{
				Feed_watchdog();
				PublicDelayMs(250);
			}
			Get_Att7059_PUI(EMU_L2,SmallCurrentCounts);
			temp=CalibrateComData.CalibrateBasicData.POffsetData.Power/10;
            Err_k=(float)0.0381775*(float)Calibrate_meter.Cali_Par.Meter_const_act*(float)Calibrate_meter.Cali_EMU_L2.HFConst_7059;//12613.841
			if(temp<AutomaticCalibrationData.Power_P[0])
			{
            	Calibrate_meter.Cali_EMU_L2.APOS_7059= (uint16)((uint32)65536-((AutomaticCalibrationData.Power_P[0]-temp)*(float)Err_k+50)/100);
			}
			else if (temp>AutomaticCalibrationData.Power_P[0])
			{
            	Calibrate_meter.Cali_EMU_L2.APOS_7059=(uint16)(((temp-AutomaticCalibrationData.Power_P[0])*(float)Err_k+50)/100);
			}
			Flag = 1;				
		
		}		
	}
	else if(CalibrateComData.Type == Calibrate_EOffsetType)// 0x07 误差法偏置修调类型
	{;
	}
	else if(CalibrateComData.Type == Calibrate_Gain_NCurrentType)// 0x08	 零线电流有效值修调类型
	{;
	}
	else if(CalibrateComData.Type == Calibrate_TemperatureType)//  0x09 温度修调类型
	{
	}
	//else if(CalibrateComData.Type == Calibrate_ResetType)// 0x0A 修调寄存器初始化类型
	//{
	//}
	else if(CalibrateComData.Type == Calibrate_PPMSetType)// 0x0b PPM值修调类型
	{
	}
	else
	{
		CalibrateComData.Type = 0;
	}
	if(Flag&&(I0_PRO_SWI==0))
	{	
		Flag=0;
		switch(Emu_use_flag)
		{
			case EMU_L1:
				Set_ATT7059(EMU_L1);
				EMUUartAutoPause();
				PublicDelayMs(5);//	
				for(i=0;i<3;i++)
				{
					EMUUartAutoPause(); //先取出ATT7059S 自己计算出来的校验和
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
				Calibrate_meter.Cali_EMU_L1.ChkSum_7059=CALC_CHECKSUM((uint8 *)&Calibrate_meter.Cali_EMU_L1, sizeof(Calibrate_meter.Cali_EMU_L1)-2);;
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
				Set_ATT7059(EMU_L2);
				EMUUartAutoPause();
				PublicDelayMs(5);//	
				for(i=0;i<3;i++)
				{
					EMUUartAutoPause(); //先取出ATT7059S 自己计算出来的校验和
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
				Calibrate_meter.Cali_EMU_L2.ChkSum_7059=CALC_CHECKSUM((uint8 *)&Calibrate_meter.Cali_EMU_L2, sizeof(Calibrate_meter.Cali_EMU_L2)-2);;
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
}

