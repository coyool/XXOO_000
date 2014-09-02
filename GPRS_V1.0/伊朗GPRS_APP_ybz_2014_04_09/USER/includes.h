#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_usart.h"

#include "misc.h"

#include  "ucos_ii.h"  		//uC/OS-II系统函数头文件

#include  	"BSP.h"			//与开发板相关的函数
#include 	"app.h"			//用户任务函数
#include 	"led.h"			//LED驱动函数

 /*The larger of __x and __y*/
#define MAX(__x,__y) ((__x)>(__y)?(__x):(__y))
/*The smaller of __x and __y*/
#define MIN(__x,__y) ((__x)<(__y)?(__x):(__y)) 

#endif //__INCLUDES_H__
