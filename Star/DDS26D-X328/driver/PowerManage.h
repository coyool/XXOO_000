/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：powermanage.h 
* 文件标识：见配置管理计划书 
* 摘要：电源管理移植层

* 当前版本：1.0.0 
* 作者：ffq
* 完成日期：20131024
*******************************************************************/
#ifndef _DRV_POWERMANAGE_H_
#define _DRV_POWERMANAGE_H_

/*--------------------宏定义-------------------------------*/
#define POWER_UP_INIT_SUCESS					0X55AA
#define POWER_UP_INIT_FAIL						0


/*--------------------外部常量申明-------------------------*/

/*--------------------外部变量申明-------------------------*/
extern uint8 scrKeyIntFlag;
extern uint8 VccChkIntFlag;
extern uint8 lvdIntFlag;
/*--------------------外部函数申明-------------------------*/
uint8 PowerUpDetect(void);
uint8 PowerDownDetect(void);
uint16 PowerUpInit(void);
void PowerUpBattInit(void);
void PowerDownInit(void);
uint16 PowerDowmMain(void);
void PowerManage(void);
void PowerUpInitManage(void);
uint16 BattVoltageGet(void);
uint8 BattLowStateGet(void);
void PowerWakeTierRst(void);
uint16 BattVoltageDataGet(uint32 DataId, uint8 *pBuf);





#endif
