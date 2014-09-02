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
	DebugInit();	        /* ���ʱ���� UART����ַ��� ä���Եķ�ʽ�����Գ��� */   //XX
	w25x64_init();		    /* �洢��(flash)��ʼ�� */
	SysTimeInit();			/* RTC init */
	ShellCmdInit();			/* shell command init �����е��� */
	shell_cms_init();		/* shell �������������� */
	Signel_led_init();      /* �źŵƳ�ʼ��	*/
}








