#include "plat_include/debug.h"
#include "hal_include/w25x64.h"
#include "hal_include/hal_config.h"
#include "plat_include/shellcmd.h"
#include "hal_include/switch.h"
#include "plat_include/plat_uart.h"

extern int SysTimeInit(void);

void PlatInit(void)
{
	SetDebugPort(DEBUG_PORT);
	DebugInit();	        /* 李工当时采用 UART输出字符串 盲调试的方式来调试程序 */   //XX
	w25x64_init();		    /* 存储器(flash)初始化 */
	SysTimeInit();			/* RTC init */
	ShellCmdInit();			/* shell command init 命令行调试 */
	shell_cms_init();		/* shell 调试命令行种类 */
	Signel_led_init();      /* 信号灯初始化	*/
}








