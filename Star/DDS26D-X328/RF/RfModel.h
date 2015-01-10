/*******************************************************************
Copyright (c) 2013,深圳市思达仪表有限公司 
All rights reserved. 
 
文件名称：RFMODEL.h 
摘要：RF模式切换模块 
作者：ffq 
当前版本：1.0.0 -20140625

历史记录：
********************************************************************/

#ifndef _RFMODEL_H
#define _RFMODEL_H


/*---------------------------------------------------------------------------*/
/*外部常数和宏定义                                                           */
/*---------------------------------------------------------------------------*/
#define 	RF_PTP 					1
#define 	RF_MESH 				2
#define		RF_MODEL_DELAY_TIME 	4  //在低压抄表时间,DC-DC会出现短暂100ms的关断期,快速检测到,再次初始化RF模块,	
#define		RF_VOLTAGE_JUDGE_TIME 	200		

#define     RF_INITTIME             360000

#define CHK_M_PIN		((FM3_GPIO->PDIR3&IO_PINxE)>>14)
#define CHK_M_INPUT()   {FM3_GPIO->DDR3&= ~IO_PINxE;FM3_GPIO->PCR3&= ~IO_PINxE;} 

#define Disable_ExtInt()    CC1101_ExtInt_disable()


/*---------------------------------------------------------------------------*/
/*外部结构体、枚举和类声明                                                   */
/*---------------------------------------------------------------------------*/
typedef struct
{
	 uint16		PTPCounts;			 //模块不同的检测次数
     uint16		MeshCounts;			 //模块不同的检测次数
	 uint8		volCounts;	
	 uint8      Module;         
     uint32     RFInitTimes;    
}t_RFModel;

/*---------------------------------------------------------------------------*/
/*外部变量声明                                                               */
/*---------------------------------------------------------------------------*/
volatile extern t_RFModel RFModel;
extern uint8 lowVoltageFlag;

/*---------------------------------------------------------------------------*/
/*外部函数声明                                                               */
/*---------------------------------------------------------------------------*/
void RFModeChangeInit(void);
void RFExtInt_Init(void);
//void DisableRF_ExtInt(void);
void RFIOPowerUpInit(void);
void RFIOPowerDownInit(void);
void RFModuleCheckProcess(void);
uint8 ModelSelGet(void);
void RFMeshMain(void);



#endif
