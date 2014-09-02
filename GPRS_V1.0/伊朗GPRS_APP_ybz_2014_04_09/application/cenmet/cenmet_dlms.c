/******************** *****(C) COPYRIGHT 2010 szly ************************
* File Name :  cenmet_comm.c
* Author : 
* Version : 
* Date : 
* Description :多功能表抄表应用层接口
*******************************************************************************
* 
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ucos_ii.h"
#include "plat_include/Plat_defines.h"
#include "app_include/sys/rs485.h"
#include "app_include/cenmet/cenmet_proto.h"
#include "plat_include/timeal.h"
#include "plat_include/debug.h"
#include "app_include/lib/bcd.h"
#include "app_include/param/meter.h"
#include "app_include/cenmet/forward.h"
#include "plat_include/sleep.h"
#include "plat_include/plat_uart.h"
#include "plat_include/dl645_2007.h"
#include "plat_include/sys_config.h"
#include "plat_include/event.h"
#include "plat_include/plat_thread.h"
#include "hal_include/switch.h"
#include "app_include/uplink/svrcomm.h"
#include "app_include/monitor/alarm.h"
#include "plat_include/shellcmd.h"
#include "app_include/lib/align.h"
#include "plat_include/flashsave.h"
#include "plat_include/reset.h"
#include "plat_include/func.h"
#include "plat_include/plat_network.h"
#include "app_include/gprs/gprs_dev.h"
#include "plat_include/plat_gprs.h"
#include "app_include/uplink/svrmsg.h"
#include "app_include/lib/crc.h"
#include "app_include/param/term.h"
#include "app_include/param/operation_inner.h"

#define ALARM_MID(mid)		(mid+1)
#ifdef DEFINE_SERIAL
#define METEV_RDMET			0x00000001   //抄表
#define METEV_RECHARGE		0x00000002   //充值

static sys_event_t CenMetEvent;

static unsigned char init_485 = 0;
static int cur_bps = -1;
static unsigned char MeterID[6]={0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
static void ProbeBps(void);
/*
	运行模式
	0 正常模式
	1 自检模式
	2 测试模式
*/
static unsigned char RunMode = 0;
/*
	询问主站标志字
	1 正在询问主站
	0 询问完成主站 
*/

void ReadMoney(void)
{
	 SysSendEvent(&CenMetEvent,METEV_RDMET);
}

/**
* @brief 初始化RS485端口
* @return 0成功, 否则失败
*/
int Rs485Init(void)
{
//	int rt;
    //rt = mutex_create(MUTEX_1);
    //AssertLog(rt!=0,"mutex creat mutex1 failed!\n");
	init_485 = 1;
	if(cur_bps == -1)
		ProbeBps();
    return 0;
}
void ReadMeter(void)
{
	SysSendEvent(&CenMetEvent, METEV_RDMET);
}

/**
* @brief 数据透明转发
* @param pcmd 命令缓存区指针
* @param pecho 返回数据缓存区指针
*/
static void _CenMetForward(const struct data_t *pcmd, struct data_t *pecho)
{
	int len,time,timeout = 20;
	unsigned char * puc;
	if(init_485 == 0)
		return;
	PrintLog(LOGTYPE_DEBUG,"485 Send:");
	PrintHexLog(LOGTYPE_DEBUG,pcmd->data,pcmd->len);
	PrintLog(LOGTYPE_DEBUG,"\r\n");
	
	//Rs485Lock();


	Rs485ClearRecBuf(UART_DOWN485_1);
	Rs485Send(UART_DOWN485_1, pcmd->data, pcmd->len);
	Sleep(5);
	len = 0;
	time = 0;
	puc=pecho->data;
	while(time < timeout) 
	{
		while(Rs485Recv(UART_DOWN485_1, puc, 1) > 0) 
		{
			time = 0;
			timeout = 10;

			puc++;
			len++;
#ifdef METER_MODULE	
			Shining485Led();
#endif
			if(len >= pecho->len) break;
		}
		time ++;
		Sleep(5);
		if(len >= pecho->len) break;
	}

	PrintLog(LOGTYPE_DEBUG,"\r\n485 Recive:");
	pecho->len = len;
	PrintHexLog(LOGTYPE_DEBUG,pecho->data,pecho->len);
}
static char TakenCode[20];
void DoRecharge(unsigned char* t)
{
	memcpy(TakenCode,t,20);
	SysSendEvent(&CenMetEvent, METEV_RECHARGE);	
}

void RechargePro(void)
{
	unsigned int idRecharge = 0x04800004;
	unsigned char data_buf[18]={0x01,0x11,0x11,0x11,0x00,0x00,0x00,0x00};
	unsigned char* TakenBuf = data_buf+8;
	struct data_t cmd;
 	struct data_t echo;
	unsigned char sbuf[30]; 
	unsigned char rbuf[30];
	int i;
	for(i=0;i<10;i++)
	{
		TakenBuf[9-i] = ((TakenCode[i*2]-0x30)<<4)|(TakenCode[i*2+1]-0x30);
	}
	cmd.data = sbuf;
	echo.data = rbuf; 
	echo.len = sizeof(rbuf);
	cmd.len = Dl2007MakeWritePktRaw(MeterID,idRecharge,data_buf,sizeof(data_buf),sbuf);
	_CenMetForward(&cmd,&echo);
	if(echo.len>0)
	{
		int rt = Dl2007CheckWrite(MeterID,0,rbuf,echo.len);
		if(rt == 0)
		{
			if(rbuf[9] == 1)
			{
				rbuf[10] -= 0x33;
				switch(rbuf[10])
				{
				case 0x0A:
				case 0x1E:
					SendSmsMessage("+JACT:TokenACCEPT");
				break;
				case 0x0D:
					SendSmsMessage("+JACT:TokenUsed");
				break;
				default:
					SendSmsMessage("+JACT:TokenREJECT");
				break;
				}
			}
		}
		else
		{
			PrintLog(LOGTYPE_DEBUG,"DoRecharge rt:%d\r\n",rt);
		}
	}		
}
static int Read2007(const unsigned char *addr,unsigned int Itemid,unsigned char* buf,int len)
{
	struct data_t cmd;
 	struct data_t echo;
	unsigned char sbuf[16]; 
	static unsigned char rbuf[230];
	cmd.data = sbuf;
	echo.data = rbuf; 
	echo.len = sizeof(rbuf);
	cmd.len = Dl2007MakeReadPktRaw(addr,Itemid,sbuf,sizeof(sbuf));
	_CenMetForward(&cmd,&echo);
	if(echo.len>0)
	{
		 int data_len = 0;
		 data_len=SepDl2007CheckRead(MeterID,Itemid,echo.data,echo.len);
		 if(data_len>len)
		 	data_len = len;
		 if(data_len>0)
		 {
		 	memcpy(buf,echo.data,data_len);
			return data_len;
		 }
	}
	return -1;
}
static void ReadResidualAmount(void)
{
	#define idMoney 0x00900200
	#define idEnergy 0x00000000 

//	const unsigned int idMoneyWornThreshold1 = 0x04000F01; 	 
//	static unsigned int timeout = 0;

	unsigned long ev;
    

	SysWaitEvent(&CenMetEvent, 0, METEV_RDMET|METEV_RECHARGE, &ev);
	//read 00.90.02.00 剩余金额
	if(ev&METEV_RDMET)
	{
		unsigned char m[4];
		unsigned char k[4];


		if(Read2007(MeterID,idMoney,m,sizeof(m))>0)
		{
			if(Read2007(MeterID,idEnergy,k,sizeof(k))>0)
			{
				char buf[30];
				sprintf(buf,"+JACT:%02x%02x%02x.%02xAZN,%02x%02x%02x.%02xkWh",
				m[3],m[2],m[1],m[0],
				k[3],k[2],k[1],k[0]);
				SendSmsMessage(buf);		
			}
		}
	}
	if(ev&METEV_RECHARGE)
	{
		RechargePro();
	}
}
static void CalibrationTime(void)
{
	const unsigned int ymdw = 0x04000101;  	
	const unsigned int hms = 0x04000102;  	
	static unsigned int timeout = 60*30;
	struct data_t cmd;
 	struct data_t echo;
	unsigned char sbuf[16]; 
	unsigned char rbuf[30]={0};
	if(timeout >= 60*30)//30分钟
	{
		int rd_er = 1,data_len = 0;
		
		unsigned char ymdw_buf[4];
		unsigned char hms_buf[3];
		PrintLog(LOGTYPE_DEBUG,"CalibrationTime\r\n");
		cmd.data = sbuf;
		echo.data = rbuf; 
		echo.len = sizeof(rbuf);
		cmd.len = Dl2007MakeReadPktRaw(MeterID,ymdw,sbuf,sizeof(sbuf));
		_CenMetForward(&cmd,&echo);
		if(echo.len>0)
		{
			data_len=SepDl2007CheckRead(MeterID,0,echo.data,echo.len);
			if(data_len<=0)
			{
				PrintLog(LOGTYPE_DEBUG,"CalibrationTime Error! Data Framw Error\r\n");				
				PrintHexLog(LOGTYPE_DEBUG,echo.data,echo.len);
			}
			else
			{
				rd_er = 0;
				memcpy(ymdw_buf,echo.data,sizeof(ymdw_buf));
				BcdToHex(ymdw_buf,sizeof(ymdw_buf));
				PrintLog(LOGTYPE_DEBUG,"Read ymdw %d %d %d %d\r\n",ymdw_buf[0],ymdw_buf[1],ymdw_buf[2],ymdw_buf[3]);
			}
		}
		else
		{
			cur_bps = -1;
			PrintLog(LOGTYPE_DEBUG,"CalibrationTime Error! 232Error!\r\n");
		}
		if(rd_er == 0)
		{
			cmd.data = sbuf;
			echo.data = rbuf; 
			echo.len = sizeof(rbuf);
			cmd.len = Dl2007MakeReadPktRaw(MeterID,hms,sbuf,sizeof(sbuf));
			_CenMetForward(&cmd,&echo);
			if(echo.len>0)
			{	
				data_len = SepDl2007CheckRead(MeterID,0,echo.data,echo.len);
				if(data_len <= 0)
				{
					PrintLog(LOGTYPE_DEBUG,"CalibrationTime Error! Data Framw Error\r\n");	
					PrintHexLog(LOGTYPE_DEBUG,echo.data,echo.len);
				}
				else
				{
					struct sysclock c;

				 	memcpy(hms_buf,echo.data,sizeof(hms_buf));
					BcdToHex(hms_buf,sizeof(hms_buf));

					PrintLog(LOGTYPE_DEBUG,"Read hms %d %d %d\r\n",hms_buf[0],hms_buf[1],hms_buf[2]);

					c.year = ymdw_buf[3];
					c.month = ymdw_buf[2];
					c.day = ymdw_buf[1];
					c.hour = hms_buf[2];
					c.minute = hms_buf[1];
					c.second = hms_buf[0];
					sysclock_set(&c);		
				}	
			}
			else
			{
				cur_bps = -1;
				PrintLog(LOGTYPE_DEBUG,"CalibrationTime Error! 232Error!\r\n");
			}
		 }
		 timeout = 0;
	}
	timeout++;
}
//static unsigned char SendWorning = 0;
/*static int shell_SendWorning(int argc, char *argv[])
{
	SendWorning = 1;
	PrintLog(LOGTYPE_DEBUG,"CMD Worning Code\r\n");
	return 0;
} */
//DECLARE_SHELL_CMD("worning", shell_SendWorning, "发送告警");

struct GprsParam{
	unsigned char mask;	//有效标志第1位-IP1,第2位-PORT2,第3位-IP1,第4位-PORT2,5-APN,6-Web
	unsigned char IP1[4];	//ip地址
	unsigned char PORT1[2];	//端口号
	unsigned char IP2[4];	//ip地址
	unsigned char PORT2[2];	//端口号
	char APN[20];	//Apn
	char Web[32];	//网址
	unsigned char CRC[2];	//Crc校验
};
struct Tran645{
	unsigned char Magic[4];		//0x1374A5FC
	struct GprsParam Param;	//通讯参数
};

static void UpdataGprsParam(struct Tran645* buf)
{
	{
		unsigned char MAGIC_NUM[] = {0x13,0x74,0xA5,0xFC};	
		PrintLog(LOGTYPE_DEBUG,"UpdataGprsParam");
		PrintHexLog(LOGTYPE_DEBUG,(const unsigned char*)buf,sizeof(struct Tran645));
		PrintLog(LOGTYPE_DEBUG,"\r\n");
		
		if(memcmp(buf->Magic ,MAGIC_NUM,4)!=0)
		{
			PrintLog(LOGTYPE_DEBUG,"UpdataGprsParam Magic Error(%x%x%x%x)\r\n",buf->Magic[0],buf->Magic[1],buf->Magic[2],buf->Magic[3]);
			return;
		}
	}
	{
		unsigned short crc = CalculateCRC((const unsigned char*)&(buf->Param),sizeof(struct GprsParam)-2);
		swap(buf->Param.CRC[0],buf->Param.CRC[1]);
		if(memcmp(&crc,buf->Param.CRC,2) != 0)
		{
			PrintLog(LOGTYPE_DEBUG,"UpdataGprsParam crc Error(%x%x)(%x)\r\n",buf->Param.CRC[0],buf->Param.CRC[1],crc);
			
			return;
		}
	}
	{	//ip
		if((buf->Param.mask&0x01) == 0x01)
		{
			memcpy(ParaTerm.svrip.ipmain,buf->Param.IP1,4);	
		}
	}
	{	//PORT
		if((buf->Param.mask&0x02) == 0x02)
		{
			swap(buf->Param.PORT1[0],buf->Param.PORT1[1]);
			memcpy(&ParaTerm.svrip.portmain,&buf->Param.PORT1,2);		
		}
	}
		{	//ip
		if((buf->Param.mask&0x04) == 0x04)
		{
			memcpy(ParaTerm.svrip.ipbakup,buf->Param.IP2,4);	
		}
	}
	{	//PORT
		if((buf->Param.mask&0x08) == 0x08)
		{
			swap(buf->Param.PORT2[0],buf->Param.PORT2[1]);
			memcpy(&ParaTerm.svrip.portbakup,&buf->Param.PORT2,2);		
		}
	}
	{
		//APN
 		if((buf->Param.mask&0x10) == 0x10)
		{
			memcpy((char *)ParaTerm.svrip.apn,buf->Param.APN,sizeof(ParaTerm.svrip.apn));	
		}
	}
	{
		//Web
 		if((buf->Param.mask&0x20) == 0x20)
		{
			memcpy(ParaUni.web_addr,buf->Param.Web,sizeof(ParaUni.web_addr));		
		}
	}
	SaveParaTerm();
	
}

#define WornCode 0xE0000001
static void ClearWornCode(unsigned char code)
{
	unsigned char data_buf[13]={0x01,0x11,0x11,0x11,0x00,0x00,0x00,0x00,code,0x00,0x00,0x00,0x00};
   	struct data_t cmd;
	struct data_t echo;
	unsigned char sbuf[30]; 
	unsigned char rbuf[30];
	cmd.data = sbuf;
	echo.data = rbuf; 
	echo.len = sizeof(rbuf);
	cmd.len = Dl2007MakeWritePktRaw(MeterID,WornCode,data_buf,sizeof(data_buf),sbuf);
	_CenMetForward(&cmd,&echo);
}
static void ReadWorningInfo(void)
{
	struct WornInfo
	{
		unsigned char code;
		unsigned char data[4];
	}__attribute((packed));
	struct WornInfo w;
	static unsigned int timeout = 0;
	int i;
	if(timeout>60)
	{
			PrintLog(LOGTYPE_DEBUG,"Read Worning Code\r\n");
			timeout = 0;
		/*	if(SendWorning == 1)
			{ //Test
				alarm_t alarmbuf;
				PrintLog(LOGTYPE_DEBUG,"Test Worn\r\n");
				SendWorning = 0;
				w.code = 1;
				*(unsigned int*)w.data = 0x12345678;
				alarmbuf.erc = 60;
				alarmbuf.len = 7;
				DEPART_SHORT(ALARM_MID(16), alarmbuf.data);
				memcpy(alarmbuf.data+2,&w,5);
				MakeAlarm(ALMFLAG_IMPORTANT,&alarmbuf);		
			}*/
			for(i=0;i<10;i++)
			{
				if(Read2007(MeterID,WornCode,(unsigned char*)&w,sizeof(w))>0)
				{
					if(w.code>0)
					{
						if(w.code == 1)
						{
							char buf[60];
							PrintLog(LOGTYPE_DEBUG,"\r\nSend Worn MSG for mony\r\n",w.code,*(unsigned int*)w.data);
							snprintf(buf,sizeof(buf),"+JACT：WORNNING,%02x%02x%02x.%02xAZN remaining amout prealarm",w.data[3],w.data[2],w.data[1],w.data[0]);
							SendUserSmsMessage(buf);;
						}
						else if(w.code < 0xff)
						{
							alarm_t alarmbuf;
							PrintLog(LOGTYPE_DEBUG,"\033[1;31;40mWorn:%d,%X \033[0m\r\n",w.code,*(unsigned int*)w.data);
							alarmbuf.erc = 60;
							alarmbuf.len = 7;
							DEPART_SHORT(ALARM_MID(16), alarmbuf.data);
							memcpy(alarmbuf.data+2,&w,5);
							MakeAlarm(ALMFLAG_IMPORTANT,&alarmbuf);
						}
						else 
						if(w.code == 0xff)
						{
							#define PARAM_CODE 0xE0000002
							
							if(Read2007(MeterID,PARAM_CODE,(unsigned char*)updata_rwbuf,200)>0)
							{
								 UpdataGprsParam((struct Tran645*)updata_rwbuf);
								 ClearWornCode(w.code);
								 Sleep(100);
								 SysReset();
							}
						}
						else
							break;
						ClearWornCode(w.code);
					}
					else
						break;
				} 
				else
					break;
			}
		PrintLog(LOGTYPE_DEBUG,"Leave Read Worning Code\r\n");
	}
	timeout++;
}
static void ProbeBps(void)
{
	const unsigned short bs[]={2400,1200,4800,9600}; 
	unsigned char fram[]={0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x13,0x00,0xDF,0x16};	  //??
	int i;
	struct data_t cmd;
 	struct data_t echo;
	unsigned char rbuf[30];

	for (i=0; i<4; i++)
	{
		cmd.data = fram;
		cmd.len = sizeof(fram);
		echo.data = rbuf;
		echo.len = sizeof(rbuf);
		UartSetPara(UART_DOWN485_1,bs[i],8,1,'E');
		_CenMetForward(&cmd,&echo);
		if(echo.len>0)
		{
			unsigned char m[6];
			unsigned char c;
			int data_len;
			data_len = SepDl2007CheckFrame(m,&c,echo.data,echo.len);
			if(data_len>0)
			{
				cur_bps = bs[i];
				memcpy(ParaUni.addr_area,m+1,4);
				memcpy(MeterID,m,6);
				break;
			}
			else
				cur_bps	= -1;
		}	
		Sleep(50);
	}
}

#define READ_MET_NOTE 1
struct READ_MET_T{
   sys_event_t e;
   int Readed;
   int timeout;
   const struct data_t *pcmd;
   struct data_t *pecho;
};
static struct READ_MET_T ReadMet={{0},0,NULL,NULL};

/**
* @brief 数据透明转发
* @param pcmd 命令缓存区指针
* @param pecho 返回数据缓存区指针
*/
void CenMetForward(const struct data_t *pcmd, struct data_t *pecho)
{

	ReadMet.Readed = 0;
	ReadMet.timeout = 60;

	ReadMet.pcmd = pcmd;
	ReadMet.pecho = pecho;


	SysSendEvent(&ReadMet.e, READ_MET_NOTE);

	while(ReadMet.Readed == 0)
	{
		ReadMet.timeout--;
		if(ReadMet.timeout==0)
		{
			PrintLog(LOGTYPE_DEBUG,"CenMetForward Timeout.\r\n");
			break;
		}
		
		sleep(10);
	}


}
#define STX 0x02
#define ETX 0x03

#define STX_STR "\x02"
#define ETX_STR "\x03"
static unsigned char send_crc;
void SendSTX(void)
{
	Rs485Send(UART_DOWN485_1,STX_STR,1);
	send_crc = 0;	
}

void SendETX(void)
{
	unsigned char crc[3]={0};
	Rs485Send(UART_DOWN485_1,ETX_STR,1);
	PrintLog(LOGTYPE_DEBUG,"Sendcrc:%X\r\n",send_crc);
	crc[0] = hex_to_char(send_crc>>4);
	crc[1] = hex_to_char(send_crc&0x0F);
	Rs485Send(UART_DOWN485_1,crc,2);
}
static inline void SendStringTo485(const char* d)
{
	while(*d)
	{
		send_crc += *d;
		Rs485Send(UART_DOWN485_1,(const unsigned char*)d,1);	
		d++;
	}
}
static inline void SendFrame(const char* cmd,const char* data)
{
   	SendSTX();
   	SendStringTo485(cmd);
	if(data)
   		SendStringTo485(data);
   	SendETX();
}
static void SendAskModeFrame(void)
{
#define CHECK_MODE "A1"
	SendFrame(CHECK_MODE,NULL);	
}
static void SendAskParamFrame(void)
{
#define PARAM_ASK "W1"
	SendFrame(PARAM_ASK,NULL);	
}
static void SendTestResultHead(void)
{
	send_crc = 0;
   	SendSTX();
   	SendStringTo485("R1");	
}
static void SendTestResultTail(void)
{
   	SendETX();
}
static void SendTestResult(const char* code,const char* info)
{
	SendStringTo485(code);
	SendStringTo485(",");
	SendStringTo485(info);
	SendStringTo485("\r\n");
} 
struct DataFramType{
	unsigned char TestTcp; 
	char ip[16];
	char port[6];
	char web[32];
	char apn[16];
	char Timeout[5];  
};
static struct DataFramType TestParam;
struct DataFramOff{
	char * data_buf;
	unsigned char len; 
};
static const struct DataFramOff dataBufList[]=
{
{TestParam.ip,sizeof(TestParam.ip)},
{TestParam.port,sizeof(TestParam.port)},
{TestParam.web,sizeof(TestParam.web)},
{TestParam.apn,sizeof(TestParam.apn)},
{TestParam.Timeout,sizeof(TestParam.Timeout)},
{(char*)&TestParam.TestTcp,sizeof(TestParam.TestTcp)}
};
static int CheckRecivedData(char d)
{
	static unsigned char step = 0;
	static int pos;
	static const struct DataFramOff* data = NULL;
	PrintLog(LOGTYPE_DEBUG,"R=> d:%02x step:%d\r\n",d,step);
	switch(step)
	{
	case 0:
		if((d>='0')&&(d<='4'))
		{
			data = dataBufList+ (d-'0');
			step = 1;
			pos = 0;	
			break;
		}else return -1;
		
	case 1:
		if(d == ',')
		{
			step = 2;
			break;
		}else 
		{
			step = 0;
			return -1;
		}
	case 2:
		if(pos == data->len)
		{
			break;
		}
		if(d == '\r')
		{
			step = 3;
		}
		data->data_buf[pos] = d;
		pos++;
		break;	
	case 3:
		if(d == '\n')
		{
			step = 0;
		}
	}

	return 0;
}
static int CheckRecivedFrame(char d)
{
	static unsigned char step = 0;
	static unsigned char crc;
	static unsigned char CrcState = 0;
	static int rtn = 0;
	static unsigned char _TestTcp = 0; 
	PrintLog(LOGTYPE_DEBUG,"A=> d:%02x step:%d crc:%d\r\n",d,step,crc);
	switch(step)
	{
	case 0:
		if(d == STX)
		{
			step = 1;
			CrcState = 1;

		}
		else
			CrcState = 0;
		crc = 0;
		rtn = 0;
		return 0;
	case 1:
		if(d == 'A')
		{
			step = 10;
		}else if(d == 'R')
		{
			step = 20;
		}else if(d == 'W') 
		{
			step = 30;
			memset(&TestParam,0,sizeof(TestParam));
		}else step = 0;
		break;
	case 10:
		if(d == '2')
		{
			step++;
		}else step = 0;
		break;
	case 11:
		if(d == '0')
		{
			step = 12;
			rtn = 1;//
		}else if(d == 'T')
		{
			step = 100;
			rtn = 2;
		}else step = 0;
		break;
	case 20:
		if(d == '2')
		{
			step = 100;
			rtn = 1;//确认回答
		}else step = 0;
		break;
	case 12:
		if(d == '0')
		{
			_TestTcp = 0;
		}else
		{
			_TestTcp = 1;
		}
 		step = 100;
		break;	
	case 30:
		if(d == '2')
		{
			step++;
		}else step = 0;
		break;
	case 31:
		{
			int rt;
			if(d == ETX)
			{
				{
					int i;
					for(i=0;i<5;i++)
						PrintLog(LOGTYPE_DEBUG,"TestParam:%s\r\n",dataBufList[i].data_buf);
				}
				step = 101;
				CrcState = 0;	
			}
			else
			{
				PrintLog(LOGTYPE_DEBUG,"Check Param\r\n");
				rt = CheckRecivedData(d);
				if(rt == -1)
				{
					 step = 0;
				}
			}
		}
		break;
	case 100:
		if(d == ETX)
		{
			step = 101;
			CrcState = 0;
		}	
		break;
	case 101:
		if(hex_to_char(crc>>4) == d)
		{
			step++;
		}else step = 0;
		return 0;
	case 102:
		if(hex_to_char(crc&0x0F) == d)
		{
			TestParam.TestTcp = _TestTcp;
			step = 0;
			crc = 0;
			CrcState = 0;
			//PrintLog(LOGTYPE_DEBUG,"Crc Ok\r\n");
			return rtn;
		}else step = 0;
		break;
	default:
		step = 0;
		break;
	}
	if(CrcState == 1)
		crc+=d;
	PrintLog(LOGTYPE_DEBUG,"B=> d:%02x step:%d crc:%d\r\n",d,step,crc);
	return 0;
}
/*
接受并处理返回的应答帧
返回值:
	0 超时
	1 自检模式帧
	2 测试模式	
参数
	timeout 超时时间单位20ms
*/
int ReciveTestFrame(short timeout)
{

	unsigned char uc;
	short time = 0;

	while(time < timeout) 
	{
		while(Rs485Recv(UART_DOWN485_1, &uc, 1) > 0) 
		{
			int rtn;
			rtn = CheckRecivedFrame(uc);
			if(rtn > 0)
			{
				return rtn;
			}
			time = 0;
		}
		time ++;
		Sleep(1);

	}
	return 0;
}
void TestLed(void)
{
//	int i;
	led_init();
//	for(i=0;i<2;i++)
	{
		Set485Led(1);
		Signel_red_led(1);
		Signel_green_led(1);
		Sleep(50);
		Set485Led(0);
		Signel_red_led(0);
		Signel_green_led(0);
		Sleep(50);
		Set485Led(1);
		Sleep(40);
		Set485Led(0);
		Sleep(40);
		Signel_red_led(1);
		Sleep(40);
		Signel_red_led(0);
		Sleep(40);
		Signel_green_led(1);
		Sleep(40);
		Signel_green_led(0);
		Sleep(40);
	}
}
void LoadDefParaTerm(void);
//返回-
//1 失败
//0 成功
int TestFlash(void)
{
	int rt;
   	rt = para_readflash_bin(PARA_FILEINDEX_COMB,(unsigned char *)&para_comb,sizeof(para_comb));
	if(rt == -1)
	{
		  LoadDefParaTerm();
		  para_writeflash_bin(PARA_FILEINDEX_COMB,(unsigned char *)&para_comb,sizeof(para_comb));
		  rt = para_readflash_bin(PARA_FILEINDEX_COMB,(unsigned char *)&para_comb,sizeof(para_comb));
		  if(rt == -1)
		  {
		  	  return -1;
		  }
	}
	return 0;
}
int M35DialOnTest(void);
/*const char* GprsInfo[]=
{
"Gprs模块测试成功",
"AT 命令通讯失败",
"AT 命令通讯失败",
"Sim卡失败",
"AT 命令通讯失败",
"AT 命令通讯失败",
"AT 命令通讯失败",
"AT 命令通讯失败",
"信号强度弱"
"寻找网络失败",
"AT 命令通讯失败",
"删除短信失败",
"网络附着失败"
};*/
/*const char* GprsInfo[]=
{
"0",
"1",
"2",
"3",
"4",
"5",
"6",
"7",
"8"
"9",
"10",
"11",
"12"
};*/
int TestGprs()
{
	return M35DialOnTest();
}

int TestLogin()
{
	unsigned short port;

	varip_t ip;
	int hsocket  = 0;
	int rt,i;
	hsocket = NetCreateSocket(PF_AT,SOCKET_TCP);
	if(strlen(TestParam.port)==0)
		return 0;
	port =  atoi(TestParam.port);
	if(strlen(TestParam.ip)>0)
	{
 		StrToIp(TestParam.ip,&ip);
	}else if(strlen(TestParam.web)>0)
	{
		varip_t tip;
		tip.ul = 0;
		for(i=0;i<3;i++)
		{
			GprsDns((const char*)TestParam.web,&tip);
			if(tip.ul != 0)
			{
				ip = tip;
			}
		}
		if(i == 3)
			return -1;
	}
	  	
	rt = NetConnectHost(hsocket,ip,port);
	if(rt != SUCCEED)
	{
		return -1;
	}	 
	{
		const unsigned char login_cmd[]={0x68,0x32,0x00,0x32,0x00,0x68,0xC9,0x45,0x01,0x00,0x14,0x00,0x02,0x70,0x00,0x00,0x01,0x00,0x96,0x16};
		//const unsigned char ok_cmd[]={0x68,0x32,0x00,0x32,0x00,0x68,0x09,0x45,0x01,0x00,0x14,0x00,0x00,0x60,0x00,0x00,0x01,0x00,0xC4,0x16};
		int Wait,r;
		if(strlen(TestParam.Timeout)>0)
		{
			Wait = atoi(TestParam.Timeout)*60;
		}
		NetSendData(hsocket,(const char*)login_cmd,sizeof(login_cmd));
		while(Wait-->0)
		{
			r = NetRecvData(hsocket,(char*)updata_rwbuf,1024);
			PrintHexLog(LOGTYPE_DEBUG,(unsigned char*)updata_rwbuf,r);
			if(r>0)
			{
				 return 0;
			}
			Sleep(50);
		}
	}
	return -2;
}
/*
编码	
1	DATA FLASH
2	GPRS模块
测试结果
0	成功
1	失败	
*/
int AskMode = 1;
void AskMainStation(void)
{
	const char* TestFlashInfo;
	char TestGprsInfo[5];
	UartSetPara(UART_DOWN485_1,9600,8,1,'E');	// 偶校验
	SendAskModeFrame();	    // "A1"
	RunMode = ReciveTestFrame(50);
	PrintLog(LOGTYPE_DEBUG,"RunMode:%d\r\n",RunMode);
	Sleep(10);
	if(RunMode == 0)
	{
		AskMode = 0;  //正常模式
		return;
	}
	else if(RunMode == 1)
	{	//自检模式
		int rtn,rtn1;
		
		SendAskParamFrame();

		ReciveTestFrame(50);

	   	TestLed();
	
		if(TestFlash() == 0)
		{
 			PrintLog(LOGTYPE_DEBUG,"Data Flash 测试成功\r\n");
			TestFlashInfo = "0";
		}else
		{
	
			TestFlashInfo = "1";
			PrintLog(LOGTYPE_DEBUG,"Data Flash 测试失败\r\n");
		}
		PrintLog(LOGTYPE_DEBUG,"TestGprs 测试 ...\r\n");
		Sleep(10);
		rtn = TestGprs();
		PrintLog(LOGTYPE_DEBUG,"TestGprs 测试完成 rtn=%d\r\n",rtn);
		rtn = -rtn;
		sprintf(TestGprsInfo,"%d",rtn);
		if(rtn == 0)
		{
			if(TestParam.TestTcp == 1)
			{
				rtn1 = TestLogin();
				rtn1 = -rtn1;
				if(rtn1>0)
					sprintf(TestGprsInfo,"%d",rtn1+12);
				else
					sprintf(TestGprsInfo,"%d",rtn1);					
			}
		}
		SendTestResultHead();
		SendTestResult("1",TestFlashInfo);
		SendTestResult("2",TestGprsInfo);
		SendTestResultTail();
		PrintLog(LOGTYPE_DEBUG,"\033[1;31;40m测试完成\033[0m\r\n");
		while(1)Sleep(1000);	
	}else if(RunMode == 2)
	{	//调试模式
		SetDebugPort(UART_DOWN485_1);
		UartSetPara(UART_DOWN485_1,115200,8,1,'E');
		Sleep(10);
		PrintLog(LOGTYPE_DEBUG,"RunMode\r\n");
		AskMode = 0;  //正常模式
		
	}		
}
/**
* @brief 多功能表计通信任务
*/
void *CenMetTask(void * arg)
{
	__GlobalLock();
	PrintLog(LOGTYPE_DEBUG,"CenMetTask start...\r\n");
//	INIT_SHELL_CMD(shell_SendWorning);
	SysInitEvent(&ReadMet.e);
	UartOpen(UART_DOWN485_1);   //端口 0
	
	AskMainStation();	  //现场升级用 
	
	if(RunMode == 2)
	{
		while(1)
		{
			Sleep(500);//等待询问主站
			SysClearCurrentTaskState();
		}
	}
	led_init();
	Rs485Init();
							  
	while (1)
	{
		if(cur_bps == -1)
		{
			PrintLog(LOGTYPE_DEBUG,"ProbeBps\r\n");
			ProbeBps();
		}
		else
		{
			ReadResidualAmount();//查询剩余金额
			CalibrationTime();
			ReadWorningInfo();
			{
				unsigned long ev;
				SysWaitEvent(&ReadMet.e,0,READ_MET_NOTE,&ev);
				if(ev&READ_MET_NOTE)
				{
					if((ReadMet.pcmd != NULL)&&(ReadMet.pecho))
					{
						ReadMet.timeout = 20;
					 	_CenMetForward(ReadMet.pcmd,ReadMet.pecho);
						ReadMet.Readed = 1;
					}

				}
			}
		}
		SysClearCurrentTaskState();
		Sleep(50);//一秒

	}	
}
#endif
#ifdef  DEFINE_PLCOMM
static int AutoReadMeter(unsigned int id)
{
// 	PrintLog(LOGTYPE_DEBUG,"\r\nAutoReadMeter cur_id = found_id %d\r\n",cur_id);
	{
		struct WornInfo
		{
			unsigned char code;
			unsigned int data;
		}__attribute((packed));
		struct WornInfo w;
		struct data_t cmd;
	 	struct data_t echo;
		unsigned char sbuf[30]; 
		unsigned char rbuf[30];
		unsigned int Itemid = 0xE0000001;
		cmd.data = sbuf;
		echo.data = rbuf; 
		echo.len = sizeof(rbuf);

		cmd.len = Dl2007MakeReadPktRaw(ParaMeter[id].addr,Itemid,sbuf,sizeof(sbuf));


		PlForwardF1(&cmd, &echo,2);//透传
		if(echo.len>0)
		{
			 int data_len = 0;
			 data_len = SepDl2007CheckRead(ParaMeter[id].addr,Itemid,echo.data,echo.len);
			 if(data_len>0)
			 {
			 	memcpy(&w,echo.data,5);
				PrintLog(LOGTYPE_DEBUG,"Worn:%d,%d\r\n",w.code,w.data);
				if(w.code>0)
				{
					if((w.code>1) && (w.code<0xff))
					{
						alarm_t alarmbuf;
						alarmbuf.erc = 60;
						alarmbuf.len = 7;
					 	DEPART_SHORT(ALARM_MID(id), alarmbuf.data);
						memcpy(alarmbuf.data+2,&w,5);
						MakeAlarm(ALMFLAG_IMPORTANT,&alarmbuf);
					}
					{
						unsigned char data_buf[13]={0x01,0x11,0x11,0x11,0x00,0x00,0x00,0x00,w.code,0x00,0x00,0x00,0x00};
					//	struct data_t cmd;
					//	struct data_t echo;
					//	unsigned char sbuf[30]; 
					//	unsigned char rbuf[30];
						cmd.data = sbuf;
						echo.data = rbuf; 
						echo.len = sizeof(rbuf);
						cmd.len = Dl2007MakeWritePktRaw(ParaMeter[id].addr,Itemid,data_buf,sizeof(data_buf),sbuf);
						PlForwardF1(&cmd,&echo,1);
					}
					return 0;
				}

			 }
		}
	}
	return 1;		

}	 

/**************************************************
* 函数功能:    
* 输入参数:              
* 输出参数:          
* 函数说明:  读取电表相关数据
* 使用资源:
**************************************************/
void *PlMetTask(void * arg)   
{
	int run = 0;
	__GlobalLock();
    PrintLog(LOGTYPE_DEBUG,"\r\nCenMetTask start...\r\n");
	while(1)
	{
		if(run>60*15)
		{
			int id = 0;
			PrintLog(LOGTYPE_DEBUG,"\r\nRead Worning...\r\n");
			for(;id<MAX_METER;id++)
				if(ParaMeter[id].metp_id != 0)
				{
					AutoReadMeter(id);
					SysClearCurrentTaskState();
					Sleep(20);//1秒	
				}
			run = 0;
		}
		else
			Sleep(20);
		run++;
		SysClearCurrentTaskState();
	}
		
}
#endif

void InitCenmet(void)
{
#ifndef DEFINE_PLCOMM
	#define TASK_CENMET_STK_SIZE 300
	#define TASK_CENMET_PRIO 	 22
	AskMode = 1;
	SysCreateTask(CenMetTask,0,"Task Cen Meter",-1,TASK_CENMET_STK_SIZE,TASK_CENMET_PRIO);//APP 300 22

	while (1 == AskMode) 
	{
		Sleep(500);//等待询问主站
		SysClearCurrentTaskState();
	}
#else 
	
	#define TASK_CENMET_STK_SIZE 150
	#define TASK_CENMET_PRIO 	 22
	SysCreateTask(PlMetTask,0,"Task Cen Meter",-1,TASK_CENMET_STK_SIZE,TASK_CENMET_PRIO);//APP 300 22
	
#endif
} 

