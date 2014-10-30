#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_usart.h"

#include "misc.h"

#include  "ucos_ii.h"  		//uC/OS-IIϵͳ����ͷ�ļ�

#include  	"BSP.h"			//�뿪������صĺ���
#include 	"app.h"			//�û�������
#include 	"led.h"			//LED��������

 /*The larger of __x and __y*/
#define MAX(__x,__y) ((__x)>(__y)?(__x):(__y))
/*The smaller of __x and __y*/
#define MIN(__x,__y) ((__x)<(__y)?(__x):(__y)) 

#endif //__INCLUDES_H__
