/*******************************************************************
	版权声明:
	文件功能:LED驱动程序
	备注说明:
			1 DTS27-VN31三相电能表
	文件版本:V1.0
********************************************************************/

#include "include.h"


/*--------------------全局常量定义---------------------*/

/*--------------------全局变量定义---------------------*/
uint8 led_alarm_hold_ct;
uint8 led_back_hold_ct;
uint8 turn_led_alarm;

void LED_Alarm_Driver_Task(void);
void LED_Alarm_State_Pro_Task(void);
void LED_Back_Driver(void);
void LED_Back_Trig60s(void);

/*********************************************************** 
函数功能：led初始化
入口参数：
出口参数：
备 	  注：上电调用
***********************************************************/
void LEDInit(void)
{
    IO_DisableFunc(IO_PORT5,IO_PINx6);
    IO_ConfigGPIOPin(IO_PORT5,IO_PINx6,IO_DIR_OUTPUT,IO_PULLUP_OFF); 
    IO_WriteGPIOPin(IO_PORT5,IO_PINx6,IO_BIT_SET);                 //LED_ALARM 
    
    IO_DisableFunc(IO_PORT4,IO_PINxD);								//背光
    IO_ConfigGPIOPin(IO_PORT4,IO_PINxD,IO_DIR_OUTPUT,IO_PULLUP_OFF); 
    IO_WriteGPIOPin(IO_PORT4,IO_PINxD,IO_BIT_CLR);                 //SWCLK
    
    led_alarm_hold_ct=0; 
    led_back_hold_ct=0;
    //led驱动
    TaskAdd(LED_Alarm_State_Pro_Task,3000,1000,1);
    TaskAdd(LED_Alarm_Driver_Task,3000,1000,1);
//    TaskAdd(LED_Back_Driver,0,1000,1);  
}
/*********************************************************** 
函数功能：led掉电初始化
入口参数：
出口参数：
备 	  注：掉电调用
***********************************************************/
void LEDPowerDownInit(void)
{
   IO_DisableFunc(IO_PORT5,IO_PINx6);
   IO_ConfigGPIOPin(IO_PORT5,IO_PINx6,IO_DIR_INPUT,IO_PULLUP_OFF); 
   
   IO_DisableFunc(IO_PORT4,IO_PINxD);				     //背光
   IO_ConfigGPIOPin(IO_PORT4,IO_PINxD,IO_DIR_OUTPUT,IO_PULLUP_OFF); 
   IO_WriteGPIOPin(IO_PORT4,IO_PINxD,IO_BIT_CLR);
   
   led_alarm_hold_ct=0; 
   led_back_hold_ct=0; 
}

/*********************************************************** 
函数功能：报警灯闪烁	500ms
入口参数：
出口参数：
备 	  注：
***********************************************************/
void LED_Alarm_Driver_Task(void)
{
    if(led_alarm_hold_ct==0xff)			
    {
        SET_LED_ALARM;
    }
    else if(led_alarm_hold_ct==0xaa)	
    {
        if(turn_led_alarm=!turn_led_alarm)
        {
            SET_LED_ALARM;
        }
        else
        {
            CLR_LED_ALARM;
        }  
    }
    else							
    {
        CLR_LED_ALARM;
    }
}




/*********************************************************** 
函数功能：报警灯状态处理	500ms
入口参数：
出口参数：
备 	  注：
***********************************************************/
void LED_Alarm_State_Pro_Task(void)
{
    if(EventGetState(STATE_ACT_POWER_REVERSE_L1_LED))//L1反向
    {
        SET_LED_ALARM;      
    } 
    else if(EventGetState(STATE_CURRENT_Unbalanced_LED))//两路不平衡
    {
        if(EventGetState(STATE_ACT_POWER_REVERSE_L1_LED))//L1反向
        {
            SET_LED_ALARM;      
        }
        else
        {
            FLICKER_LED_ALARM; 
        }
    }
    else
    {
        CLR_LED_ALARM;
    }
}


/*********************************************************** 
函数功能：背光驱动	1000ms
入口参数：
出口参数：
备 	  注：1 背光常亮
		  2 背光定宽点亮
		  3 背光熄灭
***********************************************************/
void LED_Back_Driver(void)
{
    if(led_back_hold_ct==0xff)			//常亮
    {
        SET_LED_BACK;
    }
    else if(led_back_hold_ct)		//背光定宽点亮
    {
        SET_LED_BACK;
        led_back_hold_ct--;
    }
    else				//背光熄灭
    {
        CLR_LED_BACK;
    }
}

/*********************************************************** 
函数功能：背光触发 60s
入口参数：
出口参数：
备 	  注：1 60s
***********************************************************/
void LED_Back_Trig60s(void)
{
	TRIGGER_LED_BACK_IR;
}

void LED_test(void)
{
    LEDInit();
    //FLICKER_LED_ALARM;
    //FLICKER_LED_ENG_R;
//    SET_LED_ACT;
//    SET_LED_REACT;
    //SET_LED_ALARM;
    //SET_LED_ENG_G;
    //SET_LED_ENG_R;
    //FLICKER_LED_ENG_G;
}

