
#ifndef __BOARD_SWITCH_H
#define __BOARD_SWITCH_H

void led_init(void);
//打开电源灯
void pwr_on(void);
//关闭电源灯
void pwr_off(void);

//看门狗
void watchdog_init(void);
void watchdog_feed(void);
//电源状态检测
unsigned char pwr_state(void);

void gprs_switch_init(void);
//Gprs电源脚高
void gprs_vcc_on(void);
//Gprs电源脚低
void gprs_vcc_off(void);
//复位
void SysReset(void);
//关闭中断,挂起
void SysHalt(void);

//PLC复位
void PlcRest(void);

void Signel_led_init(void);
//控制告警灯颜色
//on :1亮灯
#define Signel_OFF 0
#define Signel_R 1
#define Signel_G 2
#define Signel_Y 3
#define Signel_DEFAULT 0xff

void Signel_led(unsigned char state,unsigned char on);
void Shining485Led(void);
void Set485Led(unsigned char on);
void LedLongShining(void);
#ifdef BIG_CONCENT
void SystenLedInit(void);	 
void Enable485_init(void);
#endif
/*
	连接状态初始化
*/
unsigned char LineStateInit(void);
/*
	更新在线标志
	State  	1 已经上线	
			0 未上线	
*/
void UpdateLineStateInit(unsigned char State);

void Signel_red_led(unsigned char on);
void Signel_green_led(unsigned char on);
#endif

