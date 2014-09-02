
#ifndef __BOARD_SWITCH_H
#define __BOARD_SWITCH_H

void led_init(void);
//�򿪵�Դ��
void pwr_on(void);
//�رյ�Դ��
void pwr_off(void);

//���Ź�
void watchdog_init(void);
void watchdog_feed(void);
//��Դ״̬���
unsigned char pwr_state(void);

void gprs_switch_init(void);
//Gprs��Դ�Ÿ�
void gprs_vcc_on(void);
//Gprs��Դ�ŵ�
void gprs_vcc_off(void);
//��λ
void SysReset(void);
//�ر��ж�,����
void SysHalt(void);

//PLC��λ
void PlcRest(void);

void Signel_led_init(void);
//���Ƹ澯����ɫ
//on :1����
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
	����״̬��ʼ��
*/
unsigned char LineStateInit(void);
/*
	�������߱�־
	State  	1 �Ѿ�����	
			0 δ����	
*/
void UpdateLineStateInit(unsigned char State);

void Signel_red_led(unsigned char on);
void Signel_green_led(unsigned char on);
#endif

