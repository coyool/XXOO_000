
/*******************************************************************
	版权声明:
	文件功能:主程序
	备注说明:
			1 DTS27-VN31三相电能表
	文件版本:V1.0
********************************************************************/
#include "Include.h"

/*--------------------全局常量定义---------------------*/

/*--------------------全局变量定义---------------------*/
uint16 ResetCause=0;
__no_init uint16 RTCResetRecord[5];
uint8  energyEepError=0;                    //电能出错
uint8  caliEepError=0;                      //校表数据出错
/*--------------------内部函数申明-------------------------*/

/*******************************************************/
//函数功能:主函数
//输入参数:
//输出参数:	
//使用资源:
//备注说明:
/*******************************************************/
void main(void)
{
    __enable_irq();
	ResetCause = FM3_CRG->RST_STR;
	DRV_WD_Init();		//首先打开看门狗
    LCD_ConfigInputIOMode(LCD_INPUT_IO_CONNECT);
	//打开存储器
	IIcPowerDownInit();
	DRV_RTC_ValueRest();
	if(0 == DRV_RTC_Chk(&CalcPpmDateTime.dateTime))
	{	
		if(!IsCheckSumRight(CalcPpmDateTime))
		{
			memcpy((uint8 *)&CalcPpmDateTime, (uint8 *)&G_CurDateTime.dateTime, sizeof(CalcPpmDateTime));
			CalcCheckSum(CalcPpmDateTime);
		}
	}
    if(((ResetCause & RST_SRST) != RST_SRST) || (CheckRTCSfr() != 0))
    {
    	DRV_RTC_Rest(0);
    }
	//复位后电源检测
	PowerUpInitManage();
	//主循环
	for(;;)
	{
		Feed_watchdog();
		TaskFlash();
		TaskDispatch();
		PowerManage();
        RFMeshMain();              
	} 
}

