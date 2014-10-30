/**
* syslock.c -- 系统复位函数头文件
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-5
* 最后修改时间: 2009-5-5
*/

#ifndef _RESET_H
#define _RESET_H

/**
* @brief 复位系统
*/
void SysRestart(void);
/**
* @brief 系统掉电
*/
void SysPowerDown(void);
/**
* @brief 应用程序退出
*/
void SysAppExit(void);

#if BOARD_VERSION == 1
/**
* @brief 立刻掉电(不保存数据)
*/
void SysPowerDownImm(void);
#endif

#endif /*_RESET_H*/
