
/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：drv_extint.h
* 文件标识：见配置管理计划书 
* 摘要：外部中断移植层

* 当前版本：1.0.0 
* 作者：ffq
* 完成日期：20131029
*******************************************************************/
#include "Include.h"



/*--------------------全局常量定义-------------------------*/
#define DLT645_RX_MAX_SIZE						256

/*--------------------全局变量定义-------------------------*/
struct uart_buff_type *uart_buff;  		        //通讯处理缓存结构体指针

flag uart_err={0};					//通讯错误标志
flag debug_err={0};

/*--------------------内部函数申明-------------------------*/


/*********************************************************** 
函数名称：Uart_Frame_Chk()
函数功能：UART帧格式检查
入口参数：
出口参数：
备 	  注：1 frame_beg2
		  2 length rang
		  3 cs
		  4 frame_end
***********************************************************/
uint8 Uart_Frame_Chk(uint8 *buffer)
{
    uint8 succ=0,k,sum=0;

    if(buffer[7]==0x68)					        //beg2
    {
        for(k=0;k<buffer[9]+10;k++)					//cs
        {
			sum+=((uint8 *)buffer)[k];
        }
        if(buffer[buffer[9]+10] == sum)
        {
			if(buffer[buffer[9]+10+1] == 0x16)                //end
            {
                succ=1;
			}
        }
    }
    return succ;
}

/*********************************************************** 
函数名称：Uart_Sub_33()
函数功能：UART 接收数据区减33
入口参数：
出口参数：
备 	  注：
***********************************************************/
void Uart_Sub_33(void)
{
	uint8 i;

	for(i=0;i<uart_buff->Buffer[9];i++)
	{
		uart_buff->Buffer[10+i]-=0x33;
	}
}

/*********************************************************** 
函数名称：Uart_Add_33()
函数功能：UART 发送数据区加33
入口参数：
出口参数：
备 	  注：
***********************************************************/
void Uart_Add_33(void)
{
	uint8 i;

	for(i=0;i<uart_buff->Buffer[9];i++)
	{
		uart_buff->Buffer[i+10]+=0x33;
	}
}

/*********************************************************** 
函数名称：Broadcast_Read_Addr()
函数功能：主站读通讯地址
入口参数：
出口参数：
备 	  注：
***********************************************************/
void Broadcast_Read_Addr(uint8* Rbuffer)
{
    uint8 i;//,LuB_Err,LuB_Dat;
    uart_err.byte=0;
    for(i=0;i<6;i++)
    {
        uart_buff->Buffer[i+1]=*(Rbuffer+i);
	uart_buff->Buffer[i+10]=*(Rbuffer+i);
    }			
    uart_buff->Buffer[8]=0x93;
    uart_buff->Buffer[9]=0x06;
}

/*********************************************************** 
函数名称：Broadcast_write_Addr()
函数功能：主站读通讯地址
入口参数：
出口参数：
备 	  注：
***********************************************************/
void Broadcast_write_Addr(uint8* Wbuffer)
{
    uint8 i;//,LuB_Err,LuB_Dat;
    uart_err.byte=0;
    for(i=0;i<6;i++)
    {
        uart_buff->Buffer[i+1]=*(Wbuffer+i);
    }			
    uart_buff->Buffer[8]=0x95;
    uart_buff->Buffer[9]=0x00;
}

/*********************************************************** 
函数名称：Get_Addr()
函数功能：广播读表地址
入口参数：
出口参数：
备 	  注：
***********************************************************/
void Get_Addr(void)
{
	uart_buff->Buffer[1]=0x11;
	uart_buff->Buffer[2]=0x11;
	uart_buff->Buffer[3]=0x11;
	uart_buff->Buffer[4]=0x11;
	uart_buff->Buffer[5]=0x11;
	uart_buff->Buffer[6]=0x11;
	uart_buff->Buffer[8]=0x81;
	uart_buff->Buffer[9]=0;
}

/*********************************************************** 
函数名称：Uart_Tran_Frame()
函数功能：UART发送帧组装
入口参数：
出口参数：
备 	  注：1 addr
		  2 frame_beg
		  3 data+33
		  4 cs
		  5 frame_end
***********************************************************/
void Uart_Tran_Frame(void)
{
    uint8 i=0,sum=0;
	
    uart_buff->Buffer[0]=0x68;							//begin1
    uart_buff->Buffer[1]=uart_buff->Buffer[1];	                //addr
    uart_buff->Buffer[2]=uart_buff->Buffer[2];
    uart_buff->Buffer[3]=uart_buff->Buffer[3];
    uart_buff->Buffer[4]=uart_buff->Buffer[4];
    uart_buff->Buffer[5]=uart_buff->Buffer[5];
    uart_buff->Buffer[6]=uart_buff->Buffer[6];
    uart_buff->Buffer[7]=0x68;							//begin2
    if(uart_buff->Buffer[9] > sizeof(*uart_buff)-16)		                //check length
    {
	debug_err.bits.bit1=1;
	return;
    }
    Uart_Add_33();							        //add 33
    for(i=0;i<uart_buff->Buffer[9]+10;i++)					        //cs
    {
	sum+=((uint8 *)uart_buff)[i];
    }
    uart_buff->Buffer[uart_buff->Buffer[9]+10]=sum;
    uart_buff->Buffer[uart_buff->Buffer[9]+10+1]=0x16;	                                //end
}

/*********************************************************** 
函数功能：AA主站读通讯地址
笆肟诓问箆

出口参数：
备 	  注：
***********************************************************/
void AA_Read_Addr(uint8 *addr)
{
    uint8 i;	
    uart_err.byte=0;
    for(i=0;i<6;i++)
    {
	uart_buff->Buffer[14+i]=*(addr+i);
    }			
    uart_buff->Buffer[9]=10;
}

/*********************************************************** 
函数功能：AA主站读表号
入口参数：
出口参数：
备 	  注：
***********************************************************/
void AA_Read_Meter_No(void)
{
    uint8 i;
    uart_err.byte=0;
    for(i=0;i<6;i++)
    {
        uart_buff->Buffer[14+i]=0x11;
    }			
    uart_buff->Buffer[9]=10;
}

/*********************************************************** 
函数功能：AA主站读表号
入口参数：
出口参数：
备 	  注：
***********************************************************/
void Read_EnergyData(void)
{
    uint8 i;	
    uart_err.byte=0;
    for(i=0;i<5;i++)
    {
	uart_buff->Buffer[12+i]=0x33;
    }			
    uart_buff->Buffer[9]=7;
}


/*********************************************************** 
函数功能：载波模块获取表地址
入口参数：
出口参数：
备 	  注：
***********************************************************/
void Mode_Read_Addr(void)
{
    //uart_buff->length=0x00;
    uart_buff->Buffer[9]=0x08;
    uart_buff->Buffer[10]=0xaf;
    uart_buff->Buffer[11]=0x02;
    uart_buff->Buffer[12]=0x11;
    uart_buff->Buffer[13]=0x11;
    uart_buff->Buffer[14]=0x11;
    uart_buff->Buffer[15]=0x11;
    uart_buff->Buffer[16]=0x11;
    uart_buff->Buffer[17]=0x11;
}


/*********************************************************** 
函数功能：获取字串头0xFE的个数
入口参数：
出口参数：
备 	  注：
***********************************************************/
uint8 GetFE(uint8 * buff)
{
    uint8 i;
	
	for(i=0;i<209;i++)
	{
		if(buff[i] != 0xFE)
		{
			break;
		}
	}
	return i;
}


void DLT645_PWOER_DOWN_Init(void)
{
    IO_DisableFunc(IO_PORT3,IO_PINx6);
    IO_ConfigGPIOPin(IO_PORT3,IO_PINx6,IO_DIR_OUTPUT,IO_PULLUP_OFF);
    IO_WriteGPIOPin(IO_PORT3,IO_PINx6,IO_BIT_CLR);
    //==========================================================================
    IO_DisableFunc(IO_PORT3,IO_PINx7);
    IO_ConfigGPIOPin(IO_PORT3,IO_PINx7,IO_DIR_OUTPUT,IO_PULLUP_OFF);
    IO_WriteGPIOPin(IO_PORT3,IO_PINx7,IO_BIT_CLR);
}

void Address_Change(uint8 *addr1,uint8 *addr2)
{
    uint8 i;
    for(i=0;i<6;i++)
    {
        *(addr1+i) = *(addr2+5-i);
    }
}
/*********************************************************** 
函数名称：Uart_Order_Pro()
函数功能：UART命令处理
入口参数：
出口参数：
备 	  注：1 帧格式检查；
		  2 命令处理；
***********************************************************/
uint8 DLT645Main(COM_TYPEDEF com)
{
	uint8 *pBuf;
	uint16 Len = 0;
        uint8 BUFFER1[6],BUFFER2[6],Address[6];

	Len = ProtocolDataRec(com, &pBuf);
	if(Len != 0)
	{
		uart_buff=(struct uart_buff_type *)(pBuf+GetFE(pBuf));		//丢弃0xFE	
		if(Uart_Frame_Chk(uart_buff->Buffer))							//帧格式检查
		{
                    MeterInfoParaGet(0x01020002, BUFFER2);
                    Address_Change(Address,BUFFER2);
		    Uart_Sub_33();
		    uart_err.byte = 0x00;
		    if((uart_buff->Buffer[1] == 0xaa) 			
		        && (uart_buff->Buffer[2] == 0xaa)
		        && (uart_buff->Buffer[3] == 0xaa) 
		        && (uart_buff->Buffer[4] == 0xaa)
		        && (uart_buff->Buffer[5] == 0xaa) 
		        && (uart_buff->Buffer[6] == 0xaa))
		    {
		        if((uart_buff->Buffer[8] == 0x13) && (uart_buff->Buffer[9] == 0x00))		//广播读地址
		        {
		            Broadcast_Read_Addr(Address);
		        }
                        else if((uart_buff->Buffer[8] == 0x15) && (uart_buff->Buffer[9] == 0x06))
                        {
                             Address_Change(BUFFER1,&uart_buff->Buffer[10]);
                             if(0 == MeterInfoParaSet(0x01020002, BUFFER1, 6))
                             {
							 	Broadcast_write_Addr(BUFFER1);
								//=======================================
								//清电能及密码
								BillingParaSet(0x09050081, NULL, 0);		
								PassWordSetDefault();
								DisplayAlarm(DIS_ALARM_CLEAR_OK,TRUE);//显示OK
								//=======================================
                             }
                             else
                             {
							 	uart_err.byte=0xff; 				//0xff无需应答
                             }
                        }
		        else if((uart_buff->Buffer[8] == 0x11) && (uart_buff->Buffer[9] == 0x04))	//AA广播读地址
				{
		            if((uart_buff->Buffer[10] == 0x01)&&(uart_buff->Buffer[11] == 0x04)&&(uart_buff->Buffer[12] == 0x00)&&(uart_buff->Buffer[13] == 0x04))
		            {
		                AA_Read_Addr(Address);
		            }
		            else if((uart_buff->Buffer[10] == 0x02)&&(uart_buff->Buffer[11] == 0x04)&&(uart_buff->Buffer[12] == 0x00)&&(uart_buff->Buffer[13] == 0x04))
		            {
		                AA_Read_Meter_No();
		            }
		            else
		            {
						uart_err.byte=0xff; 				//0xff无需应答
		            }
				}
				//-------- used for calibrate meter ----------
				else if(uart_buff->Buffer[8] == 0x11)//read data 
				{
					if((uart_buff->Buffer[13]==0xe0)||(uart_buff->Buffer[13]==0xe1)||(uart_buff->Buffer[13]==0x02))//calibrate meter
					{
						Drv_Calibrate_DLT6452007(0);
					}
				}
				else if(uart_buff->Buffer[8] == 0x14)//write data 
				{
					if((uart_buff->Buffer[13]==0xe0)||(uart_buff->Buffer[13]==0xe1))//calibrate meter
					{
						Drv_Calibrate_DLT6452007(1);
					}
				}
		        else
		        {
		            uart_err.byte=0xff; 			                //0xff无需应答.
		        }         
		    }
		    else if((uart_buff->Buffer[1] == 0x00) 
		        && (uart_buff->Buffer[2] == 0x00)
		        && (uart_buff->Buffer[3] == 0x00) 
		        && (uart_buff->Buffer[4] == 0x00)
		        && (uart_buff->Buffer[5] == 0x00) 
		        && (uart_buff->Buffer[6] == 0x00))
		    {
				//-------- used for calibrate meter ----------
				if(uart_buff->Buffer[8] == 0x11)//read data 
				{
					if((uart_buff->Buffer[13]==0xe0)||(uart_buff->Buffer[13]==0xe1)||(uart_buff->Buffer[13]==0x02))//calibrate meter
					{
						Drv_Calibrate_DLT6452007(0);
					}
				}
				else if(uart_buff->Buffer[8] == 0x14)//write data 
				{
					if((uart_buff->Buffer[13]==0xe0)||(uart_buff->Buffer[13]==0xe1))//calibrate meter
					{
						Drv_Calibrate_DLT6452007(1);
					}
				}
				//--------------end----------------------
		        else
		        {
		            uart_err.byte=0xff; 			                        //0xff无需应答.
		        }
		    }
            else if(memcmp(&uart_buff->Buffer[1],Address,6) == 0)
		    {
	        //-------- used for calibrate meter ----------
	            if(uart_buff->Buffer[8] == 0x11)//read data 
				{
					if((uart_buff->Buffer[13]==0xe0)||(uart_buff->Buffer[13]==0xe1)||(uart_buff->Buffer[13]==0x02))//calibrate meter
					{
						Drv_Calibrate_DLT6452007(0);
					}
				}
				else if(uart_buff->Buffer[8] == 0x14)//write data 
				{
					if((uart_buff->Buffer[13]==0xe0)||(uart_buff->Buffer[13]==0xe1))//calibrate meter
					{
						Drv_Calibrate_DLT6452007(1);
					}
				}
				//--------------end----------------------
                else
                {
                    uart_err.byte=0xff; 				//0xff无需应答
                }
		    }
		    else
		    {
		        uart_err.byte=0xff; 			                        //0xff无需应答.
		    }
		}
		else
		{
		    uart_err.byte=0xff; 			                                //0xff无需应答.
		}

		if(uart_err.byte == 0xff)
		{		//不是645协义，不处理数据
	//			LDRV_CommRxEnable(com);
		    return 0;
		}
		else
		{
		    if(uart_err.byte == 0x00)			                        //正常应答
		    {
		        uart_buff->Buffer[8]|=0x80;		                        //从站正常应答命令
		    }
			else	//异常应答
			{
				if(uart_buff->Buffer[8]==0x03)
				{								//安全认证命令异常应答
					uart_buff->Buffer[8]|=0xc0;//cmd
					uart_buff->Buffer[9]=2;//leng
					uart_buff->Buffer[10]=uart_err.byte;//data
					uart_buff->Buffer[11]=0x00;//data
				}
				else
				{
					uart_buff->Buffer[8]|=0xc0;		//从站正常应答命令
					uart_buff->Buffer[9]=1;//leng
					uart_buff->Buffer[10]=uart_err.byte;//data
				}
			}	
		}
		Uart_Tran_Frame();
		LDRV_UsartSend(com,(uint8 *)pBuf, uart_buff->Buffer[9]+12+GetFE(pBuf));	//应答是否要FE???
		return 1;
	}
	return 0;
}


void LDRV_Test_645(void)
{
	uint16 i,temp;
	COM_TYPEDEF com;

	for(com=(COM_TYPEDEF)0; com<(COM_TYPEDEF)(MAXCHNELNUMBER);com++)
	{
		if(temp=LDRV_UsartRecv(com))
		{
			//处理
			for(i=0;i<temp;i++)
			{
				Rxbuffer[i]=Rxbuffer[i]+0x33;
			}
			//需要应答则应答
			if(temp)
			{
				LDRV_UsartSend(com, Rxbuffer, temp);
			}
			//发送完成，驱动会自动打开接收使能
		}
	}
}





