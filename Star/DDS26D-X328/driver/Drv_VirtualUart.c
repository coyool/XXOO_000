/*****************************************************************************************
*  	Function:  RF单元与电表虚拟串口
*   
*    Author:	    wzg 
*    Created on:	2014-06-30
*****************************************************************************************/
#include "Include.h"


/*******************************************************/
//函数功能:  RF 与电表之间的虚拟串口
//输入参数:  虚拟串口 数据提取到 真实Uart BUFF,同时启动UART 串口接收处理
//输出参数:	
//使用资源:
//备注说明:
/*******************************************************/
void VirtualUartInit(void)
{
	memset((uint8 *)&RF_Serial_buf,0,sizeof(RF_Serial_buf));
}

/*******************************************************/
//函数功能:  RF 与电表之间的虚拟串口
//输入参数:  虚拟串口 数据提取到 真实Uart BUFF,同时启动UART 串口接收处理
//输出参数:	
//使用资源:
//备注说明:
/*******************************************************/


void VirtualUart21C(void)
{
	uint8 *vTUart;
	IEC62056_Type *vTUartBuf;
    
	GetCommRxPoint(UART_VT,&vTUart);
	
	memset((uint8 *)&Comm[UART_VT],0,sizeof(Comm[UART_VT]));
	Comm[UART_VT].ucStR =FINISH; 
	Comm[UART_VT].ucRxPos=RF_Serial_buf.len;
	memcpy((uint8 *)&commRxBffer[UART_VT], (uint8 *)&RF_Serial_buf.data,RF_Serial_buf.len);
	
	vTUartBuf = (IEC62056_Type *)vTUart;
	BitSet((uint8 *)&vTUartBuf->Flag.byte, AddressCheckFlag);
	vTUartBuf->DLMSMode = ProgramMode1;
	vTUartBuf->PasswordLevel = Level3;
	Com_Sign_flag();      //增加显示通讯符号
}
/*******************************************************/
//函数功能:  RF 与电表之间的虚拟串口
//输入参数:  表程序中查看虚拟串口RAM信息,判断是否RF发送过来的信息
//           如果是提取信息, 发送返回信息
//输出参数:	
//使用资源:
//备注说明:
/*******************************************************/
void VirtualUartRx(void)
{
	//=============================================
	if(RF_Serial_buf.Direction==VIRTUAL_RF_TO_METER)
	{
        RFModel.RFInitTimes =RF_INITTIME;
		if(RF_Serial_buf.len)
		{
			switch(RF_Serial_buf.protocol)
			{
				case VIRTUAL_PROTOCOL_Tm:        					 //透传
					break;
				case VIRTUAL_PROTOCOL_97: 		 
					break;
				case VIRTUAL_PROTOCOL_07:         
					break;
                case VIRTUAL_PROTOCOL_21C:         											
					VirtualUart21C();								  //虚拟串口数据拷贝到UART串口中, 同时启动串口处理函数。
					ProtocolManageTask();
				    break;
				case VIRTUAL_PROTOCOL_21E:         
					break;
				case VIRTUAL_PROTOCOL_STARRF:      
					break;
				default:
					break;
			}                      
			
		}
        if(RF_Serial_buf.Direction !=VIRTUAL_METER_TO_RF)
        {
              RF_Serial_buf.Direction=VIRTUAL_DIREC_CLEAR;      //防止下次再进入
              Comm[UART_VT].ucStR =IDLE;
        }    
	}
	else //其他都不能处理
	{
        Comm[UART_VT].ucStR =IDLE;	//
	}
}
/*******************************************************/
//函数功能:  RF 与电表之间的虚拟串口
//输入参数:  接收处理处理完成,最后发送,添加虚拟串口协议帧
//输出参数:	
//使用资源:
//备注说明:
/*******************************************************/
void  VirtualUartTx(COM_TYPEDEF com, uint8 * file, uint16 len)
{
	RF_Serial_buf.Direction =VIRTUAL_METER_TO_RF;  //表到模块
	RF_Serial_buf.len =len;       //
    RF_Serial_buf.protocol=VIRTUAL_PROTOCOL_21C;
	Comm[UART_VT].ucStR =IDLE; 
	memcpy((uint8 *)&RF_Serial_buf.data, file,RF_Serial_buf.len);	
}

/*******************************************************/
//函数功能:  RF 与电表之间的虚拟串口
//输入参数:  接收处理处理完成,最后发送,添加虚拟串口协议帧
//输出参数:	
//使用资源:
//备注说明:
/*******************************************************/
//void VirtualUartTest(void)
//{	
//	//vTKey++;
//    vTKey = 4;
//	memset((uint8 *)&RF_Serial_buf,0,sizeof(RF_Serial_buf));
//	RF_Serial_buf.Direction =VIRTUAL_RF_TO_METER;  //表到模块
//    RF_Serial_buf.protocol=VIRTUAL_PROTOCOL_21C;
//	if(vTKey == 1)
//	{
//		RF_Serial_buf.len =5; 
//		RF_Serial_buf.data[0] = 0x2F;
//		RF_Serial_buf.data[1] = 0x3F;
//		RF_Serial_buf.data[2] = 0x21;
//		RF_Serial_buf.data[3] = 0x0D;
//		RF_Serial_buf.data[4] = 0x0A;
//	}
//	else if(vTKey == 2)
//	{
//		RF_Serial_buf.len =6; 
//		RF_Serial_buf.data[0] = 0x06;
//		RF_Serial_buf.data[1] = 0x30;
//		RF_Serial_buf.data[2] = 0x35;
//		RF_Serial_buf.data[3] = 0x31;
//		RF_Serial_buf.data[4] = 0x0D;
//		RF_Serial_buf.data[5] = 0x0A;
//	}	
//	else if(vTKey == 3)
//	{
//		RF_Serial_buf.len =13; 
//		RF_Serial_buf.data[0] = 0x01;
//		RF_Serial_buf.data[1] = 0x52;
//		RF_Serial_buf.data[2] = 0x31;
//		RF_Serial_buf.data[3] = 0x02;
//		RF_Serial_buf.data[4] = 0x30;
//		RF_Serial_buf.data[5] = 0x2E;
//		RF_Serial_buf.data[6] = 0x30;
//		RF_Serial_buf.data[7] = 0x2E;
//		RF_Serial_buf.data[8] = 0x30;
//		RF_Serial_buf.data[9] = 0x28;
//		RF_Serial_buf.data[10] = 0x29;
//		RF_Serial_buf.data[11] = 0x03;
//		RF_Serial_buf.data[12] = 0x53;
//	}
//	else if(vTKey == 4)
//	{
//		RF_Serial_buf.len =21; 
//		RF_Serial_buf.data[0] = 0x01;
//		RF_Serial_buf.data[1] = 0x57;
//		RF_Serial_buf.data[2] = 0x31;
//		RF_Serial_buf.data[3] = 0x02;
//		RF_Serial_buf.data[4] = 0x30;
//		RF_Serial_buf.data[5] = 0x2E;
//		RF_Serial_buf.data[6] = 0x39;
//		RF_Serial_buf.data[7] = 0x2E;
//		RF_Serial_buf.data[8] = 0x32;
//		RF_Serial_buf.data[9] = 0x28;
//		RF_Serial_buf.data[10] = 0x31;
//		RF_Serial_buf.data[11] = 0x34;
//		RF_Serial_buf.data[12] = 0x2D;
//		RF_Serial_buf.data[13] = 0x30;
//		RF_Serial_buf.data[14] = 0x37;
//		RF_Serial_buf.data[15] = 0x2D;
//		RF_Serial_buf.data[16] = 0x30;
//		RF_Serial_buf.data[17] = 0x34;
//		RF_Serial_buf.data[18] = 0x29;
//		RF_Serial_buf.data[19] = 0x03;
//		RF_Serial_buf.data[20] = 0x5B;
//	}
//}
