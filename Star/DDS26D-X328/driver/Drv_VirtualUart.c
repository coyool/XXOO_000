/*****************************************************************************************
*  	Function:  RF��Ԫ�������⴮��
*   
*    Author:	    wzg 
*    Created on:	2014-06-30
*****************************************************************************************/
#include "Include.h"


/*******************************************************/
//��������:  RF ����֮������⴮��
//�������:  ���⴮�� ������ȡ�� ��ʵUart BUFF,ͬʱ����UART ���ڽ��մ���
//�������:	
//ʹ����Դ:
//��ע˵��:
/*******************************************************/
void VirtualUartInit(void)
{
	memset((uint8 *)&RF_Serial_buf,0,sizeof(RF_Serial_buf));
}

/*******************************************************/
//��������:  RF ����֮������⴮��
//�������:  ���⴮�� ������ȡ�� ��ʵUart BUFF,ͬʱ����UART ���ڽ��մ���
//�������:	
//ʹ����Դ:
//��ע˵��:
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
	Com_Sign_flag();      //������ʾͨѶ����
}
/*******************************************************/
//��������:  RF ����֮������⴮��
//�������:  ������в鿴���⴮��RAM��Ϣ,�ж��Ƿ�RF���͹�������Ϣ
//           �������ȡ��Ϣ, ���ͷ�����Ϣ
//�������:	
//ʹ����Դ:
//��ע˵��:
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
				case VIRTUAL_PROTOCOL_Tm:        					 //͸��
					break;
				case VIRTUAL_PROTOCOL_97: 		 
					break;
				case VIRTUAL_PROTOCOL_07:         
					break;
                case VIRTUAL_PROTOCOL_21C:         											
					VirtualUart21C();								  //���⴮�����ݿ�����UART������, ͬʱ�������ڴ�������
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
              RF_Serial_buf.Direction=VIRTUAL_DIREC_CLEAR;      //��ֹ�´��ٽ���
              Comm[UART_VT].ucStR =IDLE;
        }    
	}
	else //���������ܴ���
	{
        Comm[UART_VT].ucStR =IDLE;	//
	}
}
/*******************************************************/
//��������:  RF ����֮������⴮��
//�������:  ���մ��������,�����,������⴮��Э��֡
//�������:	
//ʹ����Դ:
//��ע˵��:
/*******************************************************/
void  VirtualUartTx(COM_TYPEDEF com, uint8 * file, uint16 len)
{
	RF_Serial_buf.Direction =VIRTUAL_METER_TO_RF;  //��ģ��
	RF_Serial_buf.len =len;       //
    RF_Serial_buf.protocol=VIRTUAL_PROTOCOL_21C;
	Comm[UART_VT].ucStR =IDLE; 
	memcpy((uint8 *)&RF_Serial_buf.data, file,RF_Serial_buf.len);	
}

/*******************************************************/
//��������:  RF ����֮������⴮��
//�������:  ���մ��������,�����,������⴮��Э��֡
//�������:	
//ʹ����Դ:
//��ע˵��:
/*******************************************************/
//void VirtualUartTest(void)
//{	
//	//vTKey++;
//    vTKey = 4;
//	memset((uint8 *)&RF_Serial_buf,0,sizeof(RF_Serial_buf));
//	RF_Serial_buf.Direction =VIRTUAL_RF_TO_METER;  //��ģ��
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
