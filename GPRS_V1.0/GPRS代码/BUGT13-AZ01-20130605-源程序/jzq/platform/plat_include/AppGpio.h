/**
* gpio -- GPIO驱动接口头文件
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-4-23
* 最后修改时间: 2009-4-23
*/

#ifndef _GPIO_H
#define _GPIO_H

#if 0
//GPIO管脚号
//x = 0~31
#define GPIO_PA(x)		((unsigned char)0x00|((x)&0x1f))
#define GPIO_PB(x)		((unsigned char)0x20|((x)&0x1f))
#define GPIO_PC(x)		((unsigned char)0x40|((x)&0x1f))
#define GPIO_PD(x)		((unsigned char)0x60|((x)&0x1f))
#endif

/**
* @brief 设置GPIO管脚方向
* @param io 管脚号
* @param direct 方向, 0输入, 1输出
*/
void GpioSetDirect(unsigned char io, int direct);
/**
* @brief 设置GPIO管脚输入滤波
* @param io 管脚号
* @param ison 0关闭, 1打开
*/
void GpioSetDeglitch(unsigned char io, int ison);
/**
* @brief 设置GPIO管脚输出电平
* @param io 管脚号
* @param value 0低电平, 1高电平
*/
void GpioSetValue(unsigned char io, int value);
/**
* @brief 读取GPIO管脚输入电平
* @param io 管脚号
* @return 0低电平, 1高电平
*/
int GpioGetValue(unsigned char io);
/**
* @brief 设置CPU内部UART模式(慎用)
* @param port UART端口号
* @param mode 模式,1为RS485模式
*/
void GpioSetUartMode(int port, int mode);

//外部时钟格式
typedef struct {
	unsigned char century;
	unsigned char year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned char week;
} extclock_t;

/**
* @brief 读取外部时钟
* @param 时钟变量指针
* @return 0成功, 否则失败
*/
int ExtClockRead(extclock_t *clock);

/**
* @brief 设置外部时钟
* @param 时钟变量指针
* @return 0成功, 否则失败
*/
int ExtClockWrite(const extclock_t *clock);

/**
* @brief 检查并校正外部时钟配置
* @return 0-成功, -1失败
*/
int ExtClockCheck(void);

/**
* @brief 读取驱动程序版本信息
*        版本信息如下:主版本号(1), 次版本号(1), 发布年月日(3)(BCD格式), 空(1)
* @param buf 缓存区指针
* @param len 缓存区长度
* @return 成功返回读取长度(6字节), 否则返回-1
*/
int ReadDriverVersion(unsigned char *buf, int len);

/**
???
*/
int ReadDriverName(unsigned char *buf, int len);

/**
* @brief 读取左模块版号
* @return 左模块版号
*/
unsigned char ReadLeftModuleNo(void);

/**
* @brief 读取右模块版号
* @return 右模块版号
*/
unsigned char ReadRightModuleNo(void);

/**
* @brief 读取终端温度
* @return 终端温度, 1=1摄氏度
*/
int GpioReadTemperature(void);

#define GPIO_POWER_RMODULE	6
#define GPIO_IGT			4
#define GPIO_GPRS_RESET		5
#define GPIO_GPRS_DTR		26
#define GPIO_GPRS_RTS		28
#define GPIO_GPRS_CTS		29
#define GPIO_GPRS_DCD		25
#define GPIO_GPRS_DSR		24
#define GPIO_GPRS_RI		27
//#define PLCM_NRST			4
//#define GPIO_RS485_CTR	7
#define GPIO_ALARMBEEP	20  //PC11

#define TE_7022SIG_PIN		12
#define TE_POCON_PIN		13

//扩展io
#define PLCM_SET			101
#define PLCM_NRST			102
#define AD_SW				103
#define GPIO_CHARGE		109
#define GPIO_SAMPLING		110
#define ESAM_DISABLE		117

typedef struct {
	unsigned short len;
	unsigned char buf[512];
}esam_t;

/**
* @brief 复位ESAM
* @return 0成功, 否则失败
*/
int ResetESAM(void);
/**
* @brief 读ESAM序列号
* @param    revBuf  读到数据接口指针
* @return 0成功, 否则失败
*/
int ReadESAMSerialNum(unsigned char *revBuf);
/**
* @brief 读ESAM
* @param    etRead  读到数据接口指针
* @return 0成功, 否则失败
*/
int ReadESAM(esam_t *rcv );
/**
* @brief 读ESAM
* @param    etRead  写入数据接口指针
* @return 0成功, 否则失败
*/
int WriteESAM(esam_t *cmd);

//#define GPIO_ALARM				GPIO_PC(14)
#if 1

void InitEsamAndSerNum(void);
int SetEsamUartMode(int uartmode);
int SetSysPowerDown(void);
int ReadPowerOffStat(void);
int ReadBatterVolt(int bimm);
int ReadClockBattVolt(unsigned char *buf);
int ReadState(unsigned char *buf);
int SetPowerDown(void);
int SetWatchdog(int flag);
int FeedWatchdog(void);
int ReadRunningSec(void);
#endif

#endif /*_GPIO_H*/
