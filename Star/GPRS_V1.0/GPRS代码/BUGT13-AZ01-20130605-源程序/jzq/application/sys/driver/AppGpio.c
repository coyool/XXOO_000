/**
* gpio.c -- GPIO驱动接口
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-4-23
* 最后修改时间: 2009-4-23
*/

//#include <stdio.h>
#include <string.h>

#include "include/debug.h"
#include "include/sys/AppGpio.h"
#include "include/sys/timeal.h"
#include "include/sys/mutex.h"
//#include "include/sys/schedule.h"

typedef union {
	unsigned long ul;
	struct {
		unsigned char pin;
		unsigned char value;
	} ch;
} gpio_arg_t;

typedef struct {
	unsigned char in_or_out[3];
	unsigned char plority[3];
}port_config;

static int FidGpio = -1;
static int FidTmp = -1;
static int FidRtc = -1;
static int FidEsam = -1;
static int FidKey = -1;
static int FidAdc[2] = {-1, -1};
static int FidWdg = -1;
static int FidAds = -1;
static int FidTcaio = -1;	//扩增io口

/**
* @brief GPIO驱动接口初始化
* @return 0成功, 否则失败
*/
int GpioInit(void)
{
	/*interface*/
	return 0;
}

/**
* @brief 设置GPIO管脚方向
* @param io 管脚号
* @param direct 方向, 0输入, 1输出
*/
void GpioSetDirect(unsigned char io, int direct)
{
	/*interface*/
	return;
}

/**
* @brief 设置GPIO管脚输入滤波
* @param io 管脚号
* @param ison 0关闭, 1打开
*/
void GpioSetDeglitch(unsigned char io, int ison)
{
	return;
}

/**
* @brief 设置GPIO管脚输出电平
* @param io 管脚号
* @param value 0低电平, 1高电平
*/
void GpioSetValue(unsigned char io, int value)
{
	/*interface*/
	return;
}

/**
* @brief 读取GPIO管脚输入电平
* @param io 管脚号
* @return 0低电平, 1高电平
*/
int GpioGetValue(unsigned char io)
{
	/*interface*/
	return 0;
}

/**
* @brief 设置CPU内部UART模式(慎用)
* @param port UART端口号
* @param mode 模式,1为RS485模式
*/
void GpioSetUartMode(int port, int mode)
{
	return;
}

/**
* @brief 读取终端温度
* @return 终端温度, 1=1摄氏度
*/
int GpioReadTemperature(void)
{
	/*interface*/
	return 0;
}

/**
* @brief 读取外部时钟
* @param 时钟变量指针
* @return 0成功, 否则失败
*/
int ExtClockRead(struct sysclock *clock)
{
	/*interface*/
	return 0;
}

/**
* @brief 设置外部时钟
* @param 时钟变量指针
* @return 0成功, 否则失败
*/
int ExtClockWrite(const extclock_t *clock)
{
	/*interface*/
	return 0;
}

/**
* @brief 检查并校正外部时钟配置
* @return 0-成功, -1失败
*/
/*int ExtClockCheck(void)
{
	AssertLog(-1==FidGpio, "invalid fid(%d)\n", FidGpio);

	if(0 != ioctl(FidGpio, 19, 0)) {
		DebugPrint(LOGTYPE_ALARM, "check ext clock error!!\n");
		return ioctl(FidGpio, 18, 0);
	}

	return 0;
}*/

/**
* @brief 读取驱动程序版本信息
*        版本信息如下:主版本号(1), 次版本号(1), 发布年月日(3)(BCD格式), 空(1)
* @param buf 缓存区指针
* @param len 缓存区长度
* @return 成功返回读取长度(6字节), 否则返回-1
*/
int ReadDriverVersion(unsigned char *buf, int len)
{
	/*interface*/
	return 0;
}

/**
* @brief 读取驱动程序版本名称
* @param buf 缓存区指针
* @param len 缓存区长度
* @return 成功返回读取长度(6字节), 否则返回-1
*/
int ReadDriverName(unsigned char *buf, int len)
{
	/*interface*/
	return 0;
}

/**
* @brief 读取左模块版号
* @return 左模块版号
*/
unsigned char ReadLeftModuleNo(void)
{
	return 0;
}

/**
* @brief 读取右模块版号
* @return 右模块版号
*/
unsigned char ReadRightModuleNo(void)
{
	return 0;
}
/**
* @brief 设置系统停电
* @return 0-成功, 1-失败
*/
int SetSysPowerDown(void)
{
	/*interface*/
	return 0;
}

/**
* @brief 掉电检测
* @return 0-掉电, 1-有电状态
*/
int ReadPowerOffStat(void)
{
	/*interface*/
	return 0;
}

/**
* @brief 设置ESAM串口模式
* @param uartmode 1-7816_T0 2 - 7816_T1  3 - USART 
* @return 成功返回读取长度(6字节), 否则返回-1
*/

int SetEsamUartMode(int uartmode)
{
	/*interface*/
	return 0;
}

/**
* @brief 复位ESAM
* @return 0成功, 否则失败
*/
int ResetESAM(void)
{
	/*interface*/
	return 0;
}

static int EsamUartRecv(unsigned char *buf, int timeout)
{
	unsigned char *puc = buf;
	int i, recvlen = 0;

	recvlen = 0;
	while(timeout) {
		Sleep(10);
		while(1){
			///i = EsamRecv(puc, 1);
			if(i<=0) break;

			timeout = 15;
			
			puc++;
			recvlen++;
			if(recvlen >= 255) break;
		}
		
		if(recvlen >= 255) break;
		timeout--;
	}
	
	return recvlen;
}

static unsigned char SerialNum[8] = {0};

/**
* @brief 读ESAM序列号
* @param    revBuf  读到数据接口指针
* @return 0成功, 否则失败
*/
int ReadESAMSerialNum(unsigned char *revBuf)
{
	smallcpy(revBuf, SerialNum, 8);

	return 0;
}

int Esam_PPSInit(void)
{
	int len;
	unsigned char ruc[32];
	unsigned char uc[] = {0xff,0x10,0x94,0x7b};

	///EsamSend(uc, 4);

	len = EsamUartRecv(ruc, 30);

	if(len<4) return 1;

	if((0xff==ruc[len-4]) && (0x10==ruc[len-3]) && (0x94==ruc[len-2]) && (0x7b== 
	ruc[len-1])) return 0;

	return 1;	
}

/**
* @brief 复位且读ESAM序列号
* @param    revBuf  读到数据接口指针
* @return 0成功, 否则失败
*/
void InitEsamAndSerNum(void)
{
	/*interface*/
	return ;
}

/**
* @brief 读ESAM
* @param    etRead  读到数据接口指针
* @return 0成功, 否则失败
*/
int ReadESAM(esam_t *etRead )
{
	int rtn;

	AssertLog(-1==FidEsam, "invalid fid(%d)\n", FidEsam);

	rtn = EsamUartRecv(etRead->buf, 100);

	etRead->len = (unsigned short)rtn;

	return 0;
}
/**
* @brief 写ESAM
* @param    etRead  写入数据接口指针
* @return 0成功, 否则失败
*/
int WriteESAM(esam_t *cmd){

	esam_t et,etRev ;
	int rtn;
	AssertLog(-1==FidEsam, "invalid fid(%d)\n", FidEsam);

	et.len = cmd->len;
	memcpy(et.buf,cmd->buf,cmd->len);

	if(et.len>5) {
		et.len = 5;

		///rtn = EsamSend(et.buf, et.len);
		if(rtn == -1) {
			ErrorLog("set esam data fail\n");
			return rtn;
		}
		
		rtn = EsamUartRecv(etRev.buf, 30);
		if(0 == rtn){
			ErrorLog("read esam data fail\n");
			return rtn;
		}

		memcpy(et.buf,cmd->buf+5,cmd->len-5);
		et.len =cmd->len - 5;

		///rtn = EsamSend(et.buf, et.len);
		if(-1 == rtn){
			ErrorLog("set esam data fail\n");
			return rtn;
		}
	}
	else{
		///rtn = EsamSend(et.buf, et.len);
		if(-1 == rtn){
			ErrorLog("set esam data fail\n");
			return rtn;
		}
	}

	return 0;
}

/**
* @brief 读取备用电池电压
* @brief bimm 1-紧急读取, 0-正常读取
* @return 电压值, -1-失败
*/
int ReadBatterVolt(int bimm)
{
	/*interface*/
	return 0;
}

/**
* @brief 读取时钟电池电压
* @param buf 数据缓存区指针
* @return 0-成功, 1-失败
*/
int ReadClockBattVolt(unsigned char *buf)
{
	/*interface*/
	return 0;
}

/**
* @brief 喂看门狗
* @return 0-成功, 1-失败
*/
int FeedWatchdog(void)
{
	/*interface*/
	return 0;
}

/**
* @brief 读取运行秒数
* @return 返回运行秒数， -1 - 表示读取失败
*/
int ReadRunningSec(void)
{
	/*interface*/
	return 0;
}

