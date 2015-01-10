/*******************************************************************
	版权声明:
	文件功能:Key驱动程序
	备注说明:
			1 DTS27-VN31三相电能表
	文件版本:V1.0
********************************************************************/
#include "Include.h"

//常量定义-----------------------------------


//变量定义-----------------------------------

typedef enum
{
	KEY_NULL 		= 0,
	KEY_SHORT,
	KEY_LONG,
	KEY_LONG_15S,
}eKeyState;

typedef struct
{
	uint16 TimeCnt;
	uint16 Time15Cnt;
	uint8 Flag;
}tKeyDetectFlag;

#define KEY_FILTER_NUM			50

static tKeyDetectFlag sScrKeyDetectFlag, sClrKeyDetectFlag, sCoverDetectFlag;
/*--------------------内部函数申明-------------------------*/
/*********************************************************** 
函数功能：循显按键检测	5ms
入口参数：
出口参数：
备 	  注：1 短按键查寻
	      2 长按键查寻
	      3 防止30V/m干扰加入滤波处理
***********************************************************/
static void KeyScrDetectProcess(uint8 FilterNum)
{
	uint8 i,temp=1;

	//EMC滤波
	bFM3_LCDC_LCDCC3_VE3 = 0; 
    IO_DisableFunc(IO_PORT5,IO_PINx1);		//scr
    IO_ConfigGPIOPin(IO_PORT5,IO_PINx1,IO_DIR_INPUT,IO_PULLUP_OFF);
	for(i=0; i<FilterNum; i++)
	{
		if(KEY_SCR)
		{
			temp=0;
			break;
		}
	}

	if(temp)
	{//按下
		if(sScrKeyDetectFlag.TimeCnt < KEY_LONG_DELAY)
		{
			sScrKeyDetectFlag.TimeCnt++;
			if(sScrKeyDetectFlag.TimeCnt == KEY_LONG_DELAY)
			{//长按键处理
                Set_Long_KeyDisp_Code(); 
			}
		}
	}
	else
	{//弹起
		if((sScrKeyDetectFlag.TimeCnt >= KEY_SHORT_DELAY) && (sScrKeyDetectFlag.TimeCnt < KEY_LONG_DELAY))
		{//短按键处理
			Set_Short_KeyDisp_Code();
			PowerWakeTierRst();
		}
		sScrKeyDetectFlag.TimeCnt = 0;
	}
}


/*********************************************************** 
函数名称：Key_Clr_Detect()
函数功能：清零按键检测	5ms
入口参数：
出口参数：
备 	  注：1 编程按键检测
***********************************************************/
static void KeyClearDetectProcess(void)
{
    uint8 i;
    
    if(sClrKeyDetectFlag.Flag != KEY_NULL)
    {
        if(KEY_SCR || KEY_Clr)
        {
            sClrKeyDetectFlag.Time15Cnt = 0;
            sClrKeyDetectFlag.Flag = KEY_NULL;
        }
        return ;
    }
	for(i=0; i<KEY_FILTER_NUM; i++)
	{
		if(KEY_SCR || KEY_Clr)
		{
			break;
		}
		else if(i+1 >= KEY_FILTER_NUM)
		{
			if(++sClrKeyDetectFlag.Time15Cnt >= KEY_LONG_15S_DELAY)
			{
				sClrKeyDetectFlag.Time15Cnt = 0;
				MeterClearRecord();
			    BillingParaSet(0x09050081, NULL, 0);
				DisplayAlarm(DIS_ALARM_CLEAR_OK,TRUE);//显示OK
			}
			return ;
		}
	}
	sClrKeyDetectFlag.Time15Cnt = 0;
	sClrKeyDetectFlag.Flag = KEY_NULL;
}

static void KeyDetectProcess(void)
{
	KeyScrDetectProcess(50);
	KeyClearDetectProcess();
}



/*--------------------外部函数申明-------------------------*/
/*********************************************************** 
函数功能：按键初始化
入口参数：
出口参数：
备注说明:
***********************************************************/
void KeyInit(void)
{
    //按键管脚初始化
    bFM3_LCDC_LCDCC3_VE3 = 0;
	bFM3_LCDC_LCDCC3_VE2 = 0;
    IO_DisableFunc(IO_PORT5,IO_PINx1);		//scr
    IO_ConfigGPIOPin(IO_PORT5,IO_PINx1,IO_DIR_INPUT,IO_PULLUP_OFF);
	 
    IO_DisableFunc(IO_PORT5,IO_PINx2);		//clr
    IO_ConfigGPIOPin(IO_PORT5,IO_PINx2,IO_DIR_INPUT,IO_PULLUP_OFF);
        
	memset(&sScrKeyDetectFlag, 0, sizeof(sScrKeyDetectFlag));
	memset(&sClrKeyDetectFlag, 0, sizeof(sClrKeyDetectFlag));
	memset(&sCoverDetectFlag, 0, sizeof(sCoverDetectFlag));
    //按键驱动
    TaskAdd(KeyDetectProcess, 0, 5, 1); 	//test_LCD
}

/*********************************************************** 
函数功能：按键初始化
入口参数：
出口参数：
备注说明:
***********************************************************/
void KeyPowerDownInit(void)
{
	bFM3_LCDC_LCDCC3_VE3 = 0;
	bFM3_LCDC_LCDCC3_VE2 = 0;
	IO_DisableFunc(IO_PORT5,IO_PINx1);		//scr
	IO_ConfigGPIOPin(IO_PORT5,IO_PINx1,IO_DIR_INPUT,IO_PULLUP_OFF);
	
    IO_DisableFunc(IO_PORT5,IO_PINx2);		//clr
	IO_ConfigGPIOPin(IO_PORT5,IO_PINx2,IO_DIR_INPUT,IO_PULLUP_OFF);
	
	memset(&sScrKeyDetectFlag, 0, sizeof(sScrKeyDetectFlag));
	memset(&sClrKeyDetectFlag, 0, sizeof(sClrKeyDetectFlag));
	memset(&sCoverDetectFlag, 0, sizeof(sCoverDetectFlag));

	KeyScrDetectProcess(1);

}

/***********************************************************
函数名称：Open_Cover_Detect()
函数功能：开上盖检查	IRQ3 //int5ms
入口参数：
出口参数：
备 	  注：1 
***********************************************************/
void CoverDetectProcess(void)
{
	uint8 i;
	
	if(sCoverDetectFlag.Flag != 0)	//合盖检测
	{
		for(i=0; i<KEY_FILTER_NUM; i++)
		{
			if(KEY_COVER)
			{
				break;
			}
			else if(i+1 >= KEY_FILTER_NUM)
			{
				if(++sCoverDetectFlag.TimeCnt > OPEN_COVER_DELAY)
				{
					sCoverDetectFlag.Flag = TRUE;
					sCoverDetectFlag.TimeCnt = 0;
				}
				return ;
			}
		}
	}
	else
	{
		for(i=0; i<KEY_FILTER_NUM; i++)
		{
			if(!KEY_COVER)
			{
				break;
			}
			else if(i+1 >= KEY_FILTER_NUM)
			{
				if(++sCoverDetectFlag.TimeCnt > OPEN_COVER_DELAY)
				{
					sCoverDetectFlag.Flag = FALSE;
					sCoverDetectFlag.TimeCnt = 0;
				}
				return ;
			}
		}
	}
	sCoverDetectFlag.TimeCnt = 0;
}

/*********************************************************** 
函数功能：循显按键检测	5ms
入口参数：
出口参数：
备 	  注：1 短按键查寻
	      2 长按键查寻
***********************************************************/
void Key_Scr_DetectPowerDown_Task(void)
{
	KeyScrDetectProcess(1);
}

/*********************************************************** 
函数功能：获取上盖端盖状态
入口参数：
出口参数：
备注说明：
***********************************************************/
uint8 CoverKeyStateGet(void)
{
	return sCoverDetectFlag.Flag;
}



