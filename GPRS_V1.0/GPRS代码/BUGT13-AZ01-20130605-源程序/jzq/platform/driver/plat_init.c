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
	DebugInit();
	w25x64_init();
	SysTimeInit();
	ShellCmdInit();
	shell_cms_init();
	Signel_led_init();//–≈∫≈µ∆≥ı ºªØ
}
