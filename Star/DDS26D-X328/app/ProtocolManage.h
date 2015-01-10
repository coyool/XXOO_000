
/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：ProtocolManage.h 
* 文件标识：见配置管理计划书 
* 摘要：协议管理

* 当前版本：1.0.0 
* 作者：张玉清
* 完成日期：20131028
*******************************************************************/
#ifndef _PROTOCOL_MANAGE_H
#define _PROTOCOL_MANAGE_H

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
#define  MAXCOMBUFFER        255           //最大通讯缓存长度
#define  DL645_07MODE         1
#define  IEC62056_21MODE      0



extern uint8 commRxBffer[MAXCHNELNUMBER-2][MAXCOMBUFFER];
extern uint8 comModeSel;

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
void ProtocolManageInit(void);
void ProtocolManagePowerDownInit(void);
void ProtocolUartInit(COM_TYPEDEF com,COM_DataFormat type,COM_BaudeType baude,uint8 * rxfile,uint16 size, COM_DoubleMode doubleM);
uint16 ProtocolDataRec(COM_TYPEDEF Comm, uint8 **pBuf); 
void GetCommRxPoint(COM_TYPEDEF Comm, uint8 **pBuf);
void ProtocolManageTask(void);
#endif
