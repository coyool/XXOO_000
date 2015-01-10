/*******************************************************************
	版权声明:
	文件功能:display程序
	备注说明:
			1 DTS27-VN31三相电能表
	文件版本:V1.0
********************************************************************/

#include "include.h"
#include <stdlib.h>
#include <math.h>
/*--------------------内部函数申明-------------------------*/
void Disp_Task(void);
void Disp_Set_Code(void);
void DIsp_Information_Data(void);
void PointDateGet(int8 data);
void  DisplayData(int8 *Buf, uint8 Length, int8 scaler, uint8 unit,uint8 digit);
uint8 DisplayAlarmDis(void);
void DisplayAlarmUpdate(uint8 Flag, eDisplayFlag State);
uint8 DisplayAlarmGetState(eDisplayFlag State);
 

//常量定义-----------------------------------
uint8 monitor_display=0xaa;					                //RAM监控

//变量定义-----------------------------------
#define EE_DISPLAY_ALTERNATE_ADDR			EE_BAK_STRUCT_ADDR(EE_DisplayAlternate)	//eep地址

tEE_DisplayAlternate displayAlternate;		//轮显参数

typedef struct
{
	uint8 disp_state;     //当前显示模式0:自动轮显; 1:按键轮显; 2:扩展显示1; 3:扩展显示2 ...
	uint8 disp_timer;     //轮显时间计数
        uint8 disp_screen;    //显示项屏数计数
	uint16 disp_ct;       //当前DI指针
	uint32 disp_ID;       //当前显示数据ID   
}tDsiplayInfo;

static tDsiplayInfo sDisplayInfo;
static uint32 FlagErrdisp;

/*--------------------全局常量定义---------------------*/
const tEE_DisplayAlternate c_displayAlternate=
{
     1,
     5,
     8,
     {
         0x03340002, //35.8.0   A项组合有功电量   
         0x03800902, //32.7.0   A项电压
         0x03800802, //31.7.0   A项电流
         0x03800B02,//21.7.0    L1正向有功功率
         0x03800A02,//33.7.0    A项功率因素
         0x0181A002,//C.60.0    窃电总次数
         0x01720102, // 0.9.2 当前日期
         0x01720002, // 0.9.1 当前时间 
     },
};

const uint32 LongkeyDisplaycode[LongkeyPageScreen] =
{
         0x01010002,//C.1.1  表号
         0x03C10002,//C.6.3  电池电压  
         0,        //RF显示模式
};


const uint16 A[]={SEG_X4,SEG_X5,0xff}; 
const uint16 KW[]={SEG_k,SEG_V,SEG_X3,SEG_X4,0xff};
const uint16 KWh[]={SEG_k,SEG_V,SEG_X3,SEG_X4,SEG_X6,SEG_X7,0xff}; 
const uint8 rF_NET[8]={SEGDNULL,SEGDr,SEGDF,SEGD_,SEGDn,SEGDE,SEGDt,SEGDNULL};
const uint8 rF_SPI[8]={SEGDNULL,SEGDr,SEGDF,SEGD_,SEGDS,SEGDP,SEGD1,SEGDNULL};
const uint8 Clear[8]={SEGDNULL,SEGDNULL,SEGDC,SEGDL,SEGDE,SEGDA,SEGDr,SEGDNULL};
const uint8 Error[8]={SEGDNULL,SEGDNULL,SEGDE,SEGDr,SEGDr,SEGDo,SEGDr,SEGDNULL};
const uint8 Obis_190[8]={SEGD1,SEGD9,SEGD0,SEGDNULL,SEGDNULL};
const uint8 Obis_1270[5]={SEGD1,SEGD2 ,SEGD7,SEGD0,SEGDNULL};
const uint8 Obis_1170[5]={SEGD1,SEGD1,SEGD7,SEGD0,SEGDNULL};
const uint8 Obis_170[5]={SEGD1,SEGD7,SEGD0,SEGDNULL,SEGDNULL,};
const uint8 Obis_1370[5]={SEGD1,SEGD3,SEGD7,SEGD0,SEGDNULL};
const uint8 Obis_C600[5]={SEGDC,SEGD6,SEGD0,SEGD0,SEGDNULL};
const uint8 Obis_000[5]={SEGD0,SEGD0,SEGD0,SEGDNULL,SEGDNULL};
const uint8 Obis_C64[5]={SEGDC,SEGD6,SEGD4,SEGDNULL,SEGDNULL};
const uint8 Obis_092[5]={SEGD0,SEGD9,SEGD2,SEGDNULL,SEGDNULL};
const uint8 Obis_091[5]={SEGD0,SEGD9,SEGD1,SEGDNULL,SEGDNULL};

/*--------------------全局变量定义---------------------*/


/*********************************************************** 
函数名称：DisplayData()
函数功能：数据显示函数
入口参数：
出口参数：
备 	  注：1 *Buf-传递的数值 Length--数据长度 scaler--倍率 unit--单位
	      2、需要单位转换先调用uintchange()函数
***********************************************************/
static void  DisplayData(int8 *Buf, uint8 Length, int8 scaler, uint8 unit,uint8 digit)
{
    int8 scalersum=scaler;
    uint8 i,k,data,tempBuf[4]={0}; 
    uint32 Pdata=0,digitsum; 
	
    for(k=0;k<Length;k++)
    {
        tempBuf[k]=*Buf;
        *Buf++;
    }
    Pdata=(uint32)(tempBuf[0]|(tempBuf[1]<<8)|(tempBuf[2]<<16)|(tempBuf[3]<<24));
    if(digit<2)
    {
        scalersum+=(2-digit);
        digitsum=(uint32)pow(10,2-digit);    
        Pdata=(uint32)Pdata/digitsum;
    }
    for(i=8;i>0;i--)
    {    
        data=(uint8)(Pdata % 10);
        Pdata= Pdata / 10;
        SetDigit(i,data);
        if((Pdata==0 )&&(scalersum == 0)){break;}
	if(scalersum < 0) {scalersum++;}    
    }
    if(digit>=2)
    {
         PointDateGet(scaler);     
    }
    else if(digit==1)
    { 
        PointDateGet(-1);   
    }  
    UnitDateGet(unit);
}        

static void PointDateGet(int8 data)
{
    switch (data)//单位加载
    {
        case -4:
              SetSegment(SEG_P1,0);
              break;	
        case -3:
              SetSegment(SEG_P2,0);
              break;	
        case -2:
              SetSegment(SEG_P3,0);
              break;	
        case -1:
              SetSegment(SEG_P4,0);
              break;
        default: 
              break;    
    }
}


static void UnitDateGet(uint8 data)
{
    switch (data)//单位加载
    {
        case unit_A:{Display_OtherMessage(&A[0]);break;}
        case unit_V:{SetSegment(SEG_V,0);break;}  
        case unit_KW:{Display_OtherMessage(&KW[0]);break;}     
        case unit_Kwh:{Display_OtherMessage(&KWh[0]);break;}         
        case unit_COS:{SetSegment(SEG_cos,0);break;}
        default: break;    
    }
}  
  
static void DisplayParaInit(void)
{
    if(!NvmBytesRead(EE_DISPLAY_ALTERNATE_ADDR, &displayAlternate, sizeof(displayAlternate)))
    {
        memcpy((uint8 *)&displayAlternate, (uint8 *)&c_displayAlternate, sizeof(displayAlternate));
    }   
}
/*********************************************************** 
函数功能：显示上电初始化
入口参数：
出口参数：
备	  注：
***********************************************************/
void Init_Disp(void)
{
    DisplayParaInit();
    memset(&sDisplayInfo, 0, sizeof(sDisplayInfo));
    memset(&FlagErrdisp, 0, sizeof(FlagErrdisp));
    TaskAdd(PowerUpDisplayAlarmPro, 0, 1000, 1);
    TaskAdd(Com_Sign_flag,0,50,1);
    Disp_Set_Code();
    SetallLCD();
    TaskAdd(Disp_Auto_Cycle_Task, 3000, 1000, 1);	
}

/*********************************************************** 
函数功能：显示上电初始化
入口参数：
出口参数：
备	  注：
***********************************************************/
void Init_DispPowerUpBatt(void)
{
    DisplayParaInit();
    memset(&sDisplayInfo, 0, sizeof(sDisplayInfo));
	memset(&FlagErrdisp, 0, sizeof(FlagErrdisp));
    PowerDownDisplayAlarmPro();
    Disp_Set_Code();
    Disp_Task();
}

/*********************************************************** 
函数功能：停电显示初始化
入口参数：
出口参数：
备	  注：停电进入休眠前调用
          唤醒运行结束后进入休眠前调用
***********************************************************/
void Init_DispPowerDown(void)
{
    memset(&sDisplayInfo, 0, sizeof(sDisplayInfo));
	memset(&FlagErrdisp, 0, sizeof(FlagErrdisp));
    PowerDownDisplayAlarmPro();
    Disp_Set_Code();
    Disp_Task();
}

/*********************************************************** 
函数功能：加载显示代码
入口参数：
出口参数：
备	  注：
***********************************************************/
static void Disp_Set_Code(void)
{
    switch(sDisplayInfo.disp_state)
    {
        case 0:
        case 1:
               sDisplayInfo.disp_ID=displayAlternate.DI[sDisplayInfo.disp_ct];
               break;
        case 2:
               sDisplayInfo.disp_ID=LongkeyDisplaycode[sDisplayInfo.disp_ct];
               break;  
        default:     
               sDisplayInfo.disp_ID=0;
               break;
   }
}  
   
/*********************************************************** 
函数功能：自动显示函数任务
入口参数：
出口参数：
备注说明:
***********************************************************/
void Disp_Auto_Cycle_Task(void)
{
    sDisplayInfo.disp_timer++;		                                        //自动轮显定时
    if(sDisplayInfo.disp_timer>=displayAlternate.time)		                        //定时
    {
        sDisplayInfo.disp_timer=0;
        switch(sDisplayInfo.disp_state)//0自动轮显 1按键轮显
        {
            case 0:                                                             //自动能显
                  if(!sDisplayInfo.disp_screen)
                  {
                     sDisplayInfo.disp_ct++;
                     if(sDisplayInfo.disp_ct>=displayAlternate.objectSum) //自动显示屏数              
                     {
                        sDisplayInfo.disp_ct=0;
                     }
                  }
                  break;
            case 1:
                  sDisplayInfo.disp_state=0;                                                 //短按键
                  break;
            case 2:
                  sDisplayInfo.disp_state=0;                                                 //长按键
                  sDisplayInfo.disp_ct=0;
                  break;
            default:
                  sDisplayInfo.disp_state=0;
                  sDisplayInfo.disp_ct=0;                 
                  break;
        }
        if(sDisplayInfo.disp_screen)
        {
            sDisplayInfo.disp_screen--;
        }    
        else 
        {
            Disp_Set_Code();
        }      
    }
    Disp_Task();
}

/*********************************************************** 
函数功能：短按键显示函数任务
入口参数：
出口参数：
备注说明:
***********************************************************/
void Set_Short_KeyDisp_Code(void)
{
    uint16 displaymode=0;
    sDisplayInfo.disp_timer=0;
    if(sDisplayInfo.disp_state==0)
    {
        sDisplayInfo.disp_state++;
    }    
    switch(sDisplayInfo.disp_state)//0自动轮显 1按键轮显
    {
        case 0:
        case 1:                                                            
               displaymode=displayAlternate.objectSum; //自动显示屏数 
               break;
        case 2:
               displaymode=LongkeyPageScreen;
               break;
       default:
               sDisplayInfo.disp_state=0;
               sDisplayInfo.disp_ct=0;                 
               break;
    }
    if(sDisplayInfo.disp_screen)
    {
        sDisplayInfo.disp_screen--;
    }
    else
    {
        sDisplayInfo.disp_ct++;
        if(sDisplayInfo.disp_ct>=displaymode)              
        {
            sDisplayInfo.disp_ct=0;
        }
        Disp_Set_Code();
    }
    Disp_Task();
}

/*********************************************************** 
函数功能：长按键显示函数任务
入口参数：
出口参数：
备注说明:
***********************************************************/
void Set_Long_KeyDisp_Code(void)
{
    sDisplayInfo.disp_timer=0;
    sDisplayInfo.disp_state=2;
    sDisplayInfo.disp_ct=0;
    Disp_Set_Code();
    Disp_Task();
}

/*********************************************************** 
函数功能：标志状态
入口参数：
出口参数：
备注说明:
***********************************************************/
 void DisplayAlarmUpdate(uint8 Flag, eDisplayFlag State)
{
	if(Flag == 0)
	{
		FlagErrdisp &= (~((uint32)1<<State));
	}
	else
	{
		FlagErrdisp |= ((uint32)1<<State);
	}
}

uint8 DisplayAlarmGetState(eDisplayFlag State)
{
	if((FlagErrdisp & ((uint32)1<<State)) != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


/*********************************************************** 
函数功能：显示函数任务
入口参数：Disp_Task()
出口参数：
备注说明:
***********************************************************/
static void Disp_Task(void)
{
    if(!DisplayAlarmDis())
    {    
        DIsp_Information_Data();
    }
    RefreshLCD();  
} 


void DisplayAlarm(eDisplayAlarm Alarm, uint8 Type)
{
    switch(Alarm)
    {
    case DIS_METER_NO:		//显示表号(地址)
        sDisplayInfo.disp_timer=0;
        sDisplayInfo.disp_ID=0x01010002;
        break;
	case DIS_DATA_NO:		//显示日期
        sDisplayInfo.disp_timer=0;
        sDisplayInfo.disp_ID=0x01720102;
        break;	
    case DIS_TIME_NO:		//显示时间
        sDisplayInfo.disp_timer=0;
        sDisplayInfo.disp_ID=0x01720002;
        break;	
    case DIS_ALARM_RF_NEL:
        Disp_Message(rF_NET);
        break;
    case DIS_ALARM_RF_SPI:
        Disp_Message(rF_SPI);
        break;	
    case DIS_ALARM_CLEAR_OK:
        if(Type==TRUE)
        {    
            DisplayAlarmUpdate(TRUE,DIS_CLEAR_OK);
            sDisplayInfo.disp_timer=0;
            sDisplayInfo.disp_state=0;
            sDisplayInfo.disp_ct=0;
			sDisplayInfo.disp_screen=1;
            sDisplayInfo.disp_ID=0x03340002;
        } 
        else
        {
            DisplayAlarmUpdate(FALSE,DIS_CLEAR_OK);
        }
        break;	
    case DIS_ALARM_ERROR:
        if(Type==TRUE)
        {    
			DisplayAlarmUpdate(TRUE,DIS_ERROR);
        } 
        else
        { 
            DisplayAlarmUpdate(FALSE,DIS_ERROR);
        }
        break;	
    default:
        break;
    }
}

static uint8 DisplayAlarmDis(void)
{
    uint8 Flag = FALSE;

    if((!sDisplayInfo.disp_state)&&(!sDisplayInfo.disp_ct)&&(sDisplayInfo.disp_screen))
    {
       if(DisplayAlarmGetState(DIS_CLEAR_OK))
       {
          Disp_Message(Clear);
          Flag = TRUE; 
       }
	   else
	   {
           sDisplayInfo.disp_screen=0;
	   }
    }
	else
	{
         DisplayAlarm(DIS_ALARM_CLEAR_OK,FALSE);//显示OK 
		 sDisplayInfo.disp_screen=0;
	}
    if((!sDisplayInfo.disp_state)&&(!DisplayAlarmGetState(DIS_CLEAR_OK)))
    {
	   if(DisplayAlarmGetState(DIS_ERROR))
       { 
     	  Disp_Message(Error);
          Flag = TRUE; 
       }
    }  
    return Flag;
}

static void DIsp_Information_Data(void)
{
    uint8 Data_buffer[20]={0};
    
    tDateTime DateTime;
    LCD_Dispdata_Clr();
    memset((uint8 *)&Data_buffer, 0, sizeof(Data_buffer));
    API_GetDataValue(sDisplayInfo.disp_ID, &Data_buffer);//获取数据
    switch(sDisplayInfo.disp_ID)//0自动轮显 1按键轮显
    { 
        case 0x0181A002: //窃电总次数 
                DisplayData((int8 *)Data_buffer,4,0,0,2);
                Disp_Obis_Message(Obis_C600);
				SetSegment(SEG_P5,0);
				SetSegment(SEG_P7,0);
                break;
        case 0x01010002: //表号
                Display_id((uint8*)Data_buffer);
                Disp_Obis_Message(Obis_000);
				SetSegment(SEG_P5,0);
				SetSegment(SEG_P6,0);
                break;
        case 0x01720102: //当前日期         
                Display_YMDHMS((tDateTime *)Data_buffer);
                Disp_Obis_Message(Obis_092);
			    SetSegment(SEG_P5,0);
				SetSegment(SEG_P6,0);
                break;
        case 0x01720002: //当前时间          
                DateTime.Hour=((tDlmsTime*)Data_buffer)->Hour;		      
                DateTime.Minute=((tDlmsTime*)Data_buffer)->Minute;            
                DateTime.Second=((tDlmsTime*)Data_buffer)->Second;            
                Display_YMDHMS(&DateTime);
                Disp_Obis_Message(Obis_091);
				SetSegment(SEG_P5,0);
				SetSegment(SEG_P6,0);
                break;
        case 0x03800802: //电流
                DisplayData((int8 *)Data_buffer,4,CURRENT_SCALER,CURRENT_UNIT,2);
                Disp_Obis_Message(Obis_1170);
			    SetSegment(SEG_P6,0);
				SetSegment(SEG_P7,0);
                break;
        case 0x03800902: //电压
                DisplayData((int8 *)Data_buffer,4,VOLTAGE_SCALER,VOLTAGE_UNIT,2);
                Disp_Obis_Message(Obis_1270);
				SetSegment(SEG_P6,0);
				SetSegment(SEG_P7,0);
                break;
        case 0x03800A02: //功率因素 
                DisplayData((int8 *)Data_buffer,4,FACTOR_SCALER,FACTOR_UNIT,2);
                Disp_Obis_Message(Obis_1370);
				SetSegment(SEG_P6,0);
				SetSegment(SEG_P7,0);
                break;
        case 0x03800B02: //有功功率
                DisplayData((int8 *)Data_buffer,4,POWER_SCALER-3,unit_KW,2);
                Disp_Obis_Message(Obis_170);
				SetSegment(SEG_P5,0);
				SetSegment(SEG_P6,0);
                break;
        case 0x03340002: //组合总点能听
                DisplayData((int8 *)Data_buffer,4,ENERGY_SCALER-3,unit_Kwh,displayAlternate.Decimal);
                Disp_Obis_Message(Obis_190);
				SetSegment(SEG_P5,0);
				SetSegment(SEG_P6,0);
                break;
        case 0x03C10002:// 电池电压
                DisplayData((int8 *)Data_buffer,4,BATTERY_SCALER,unit_V,2);
                Disp_Obis_Message(Obis_C64);
				SetSegment(SEG_P5,0);
				SetSegment(SEG_P6,0);
                break;
        case 0:         // RF显示模式
       	if(RFModel.Module == RF_MESH)
               {
		    DisplayAlarm(DIS_ALARM_RF_NEL,TRUE);
	        }
		else if(RFModel.Module == RF_PTP)
		{
		     DisplayAlarm(DIS_ALARM_RF_SPI,TRUE);						
		}
                break;
       default:                   
                break;
    }  
}              
           

/*********************************************************** 
函数名称：Display_id(uint8*Date_id)
函数功能：表号显示	
入口参数：
出口参数：
备 	  注：表号显示函数，第一屏高8位，第二屏低4位
***********************************************************/
static void Display_id(uint8*Date_id)  
{
    uint8 temp[8]={0},i;
    for(i=0;i<7;i++)
    { 
        temp[i]=*(Date_id+2);
        *Date_id++;
        SetDigit(1+i,((temp[i]&0xf0)>>4));  
        SetDigit(2+i,(temp[i]&0x0f));   
        i++;
    }  

}      

/*********************************************************** 
函数名称：Display_YYMMDD(tDateTime *DateTime)
函数功能：时间显示	
入口参数：
出口参数：
备 	  注：时间显示函数
***********************************************************/
static uint8 Bin_2bcd(uint8 bin)
{
    uint8 i,j,k;
    while(bin > 99)
    {
	bin = bin-100;
    }
    i = bin/10;
    j = bin%10;
    i = i<<4;
    k = i|j;
    return k;
}

static void Display_YMDHMS(tDateTime *DateTime)
{  
    uint8 temp[6]={0},i,j;
    uint16 Temp=0;
    Temp=((((uint16)(DateTime->HighByteYear))<<8)|((uint16)(DateTime->LowByteYear)));
    temp[0]=Bin_2bcd(Temp%100);
    temp[1]=Bin_2bcd(DateTime->Month);
    temp[2]=Bin_2bcd(DateTime->Day);
    temp[3]=Bin_2bcd(DateTime->Hour);
    temp[4]=Bin_2bcd(DateTime->Minute);
    temp[5]=Bin_2bcd(DateTime->Second);
   if(sDisplayInfo.disp_ID==0x01720102)
    {
       for(i=0,j=0;i<8;i++,j++)  
       {
           SetDigit(i+1,(temp[j]&0xf0)>>4);
           SetDigit(i+2,(temp[j]&0x0f));
           i=i+2;
       } 
       SetDigit(3,35);
       SetDigit(6,35);
    }    
    else
    {
       for(i=0,j=0;i<8;i++,j++)  
       {
           SetDigit(i+1,(temp[j+3]&0xf0)>>4);
           SetDigit(i+2,(temp[j+3]&0x0f));
           i=i+2;
       } 
       SetSegment(SEG_P9,0);
       SetSegment(SEG_P10,0);
    }    
}

/*********************************************************** 
函数名称：Disp_Message(const uint8 *Message)
函数功能：报警符号显示	
入口参数：
出口参数：
备 	  注：Disp_Message(OVEr_Po)
***********************************************************/
static void Disp_Message(const uint8 *Message)
{
   uint8 i;

   ClrallLCD();
   for(i=0;i<8;i++)
   {  
       SetDigit(1+i,Message[i]); 
   }   
}

static void Disp_Obis_Message(const uint8 *Message)
{
    uint8 i;
    
    for(i=0;i<5;i++)
    {  
        SetDigit(i+9,Message[i]); 
    } 
}


/*********************************************************** 
函数名称：DisplayAlarmPro()
函数功能：液晶显示报警显示函数
入口参数：
出口参数：
备 	  注：
***********************************************************/
void PowerUpDisplayAlarmPro(void)
{
     DisplayFlagAlarm();
     DispdataNodisp(); 
     LCD_Flag_Driver_Task();
}

void PowerDownDisplayAlarmPro(void)
{ 
     DispdataNodisp();  
     DisplayFlagAlarm();
}

/*********************************************************** 
函数名称：DisplayOtherPro()
函数功能：开上盖、端盖、电池、时钟报警
入口参数：
出口参数：
备 	  注：
***********************************************************/
void DisplayFlagAlarm(void)
{
    if(BattLowStateGet())//电池
    {      
        SetSegment(SEG_battery2,0);
    }
    else
    {
		SetSegment(SEG_battery2,1); 
    }
    if(ClockStateGet())//时钟
    {      
        SetSegment(SEG_clock,0); 
    }
    else
    {
        SetSegment(SEG_clock,1); 
    }
	if(ErrorStateGet())//校表数据错误
    {      
        DisplayAlarm(DIS_ALARM_ERROR,TRUE); 
    }
	else
    {
        DisplayAlarm(DIS_ALARM_ERROR,FALSE);  
    }
    if(PowerDownDetect()==1)
    {
        SetSegment(SEG_comm,1);     
    } 
}

/*********************************************************** 
函数功能：显示参数清零
入口参数：
出口参数：
备	  注：
***********************************************************/
void DispParaClear(void)
{
	memcpy((uint8 *)&displayAlternate, (uint8 *)&c_displayAlternate, sizeof(displayAlternate));
	NvmBytesWrite(EE_DISPLAY_ALTERNATE_ADDR, &displayAlternate, sizeof(displayAlternate));
}


/*********************************************************** 
函数功能：显示参数设置
入口参数：
出口参数：1. 设置电表数据：0-数据设置成功；非0-返回数据设置失败原因
备	  注：
***********************************************************/
uint8 DispParaSet(uint32 DataId, uint8 *pBuf, uint16 Len)
{
    uint8 succ=250;
    switch(DataId)
    {
       case 0x07100004: //时间
           if(((*((uint32 *)pBuf)) <= 60) && ((*((uint32 *)pBuf)) >= 3))
           {
               displayAlternate.time=(uint8)(*((uint32 *)pBuf));
               CalcCheckSum(displayAlternate);
               NvmBytesWrite(EE_DISPLAY_ALTERNATE_ADDR, &displayAlternate, sizeof(displayAlternate));
               succ=0;
           }
           break;
       case 0x01900002:
           if((*((uint32 *)pBuf)) <= 2) 
           {
               displayAlternate.Decimal=(uint8)(*((uint32 *)pBuf));
               CalcCheckSum(displayAlternate);
               NvmBytesWrite(EE_DISPLAY_ALTERNATE_ADDR, &displayAlternate, sizeof(displayAlternate));
               succ=0;
           }
           break; 
       default:
           break;
    }
	if(succ == 0)
	{
	    sDisplayInfo.disp_state=0;
	    sDisplayInfo.disp_timer=0;
	    sDisplayInfo.disp_ct=0;
	    sDisplayInfo.disp_screen=0;
	    Disp_Set_Code();
	    Disp_Task();
	} 
    return succ;
}

/*********************************************************** 
函数功能：显示参数设置
入口参数：
出口参数：返回数据的长度, 若出错则最高位置1返回相应的错误原因。
备	  注：
***********************************************************/
uint16 DispParaGet(uint32 DataId, uint8 *pBuf)
{
    uint16 result=0x8000;
    switch(DataId)
    {
       case 0x07100004:
           //时间
           (*((uint32 *)pBuf))=(uint32)displayAlternate.time;
           result=4;
           break;
       case 0x01900002:
           (*((uint32 *)pBuf))=(uint32)displayAlternate.Decimal;
           result=1;
           break;
       default:
           break;
    }
    return result;
}

         

  




    
