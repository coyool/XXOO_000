/**********************************************************************
Led.h
**********************************************************************/
#ifndef  _LED_C_
#define  _LED_C_

/*--------------------外部常量申明---------------------*/

/*--------------------外部变量申明---------------------*/

/*--------------------宏定义---------------------------*/
#define SET_LED_ALARM	         IO_WriteGPIOPin(IO_PORT5,IO_PINx6,IO_BIT_CLR);\
                                 led_alarm_hold_ct=0xff//长亮报警灯
     
#define CLR_LED_ALARM            IO_WriteGPIOPin(IO_PORT5,IO_PINx6,IO_BIT_SET);\
                                 led_alarm_hold_ct=0    //熄灭报警灯

#define FLICKER_LED_ALARM	 led_alarm_hold_ct=0xaa	//闪烁报警灯                                     
                                                                         
#define SET_LED_BACK		 IO_WriteGPIOPin(IO_PORT4,IO_PINxD,IO_BIT_CLR) //长亮背光

#define CLR_LED_BACK		 IO_WriteGPIOPin(IO_PORT4,IO_PINxD,IO_BIT_SET) //熄灭背光
							

#define TRIGGER_LED_BACK_IR	if((led_back_hold_ct!=0xff)&&(PowerDownDetect()==0))\
				  {\
				      SET_LED_BACK;\
				      led_back_hold_ct= 60;\
				   }        

extern uint8 led_alarm_hold_ct;
/*--------------------函数申明-------------------------*/
void LEDInit(void);
void LEDPowerDownInit(void);
void LED_test(void);
void LED_Alarm_State_Pro_Task(void);
void LED_Alarm_Driver_Task(void);
void LED_Back_Trig60s(void);

#endif
