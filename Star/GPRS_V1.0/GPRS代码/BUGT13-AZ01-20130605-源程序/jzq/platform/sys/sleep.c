
#include "ucos_ii.h"
#include "plat_include/mutex.h"
#include "plat_include/debug.h"
/**
 * 睡眠延时
 * @ticks: 睡眠节拍数，每节拍10ms
*/
void sleep(unsigned int ticks)
{
	__GlobalUnlock();
	OSTimeDly(ticks);
	__GlobalLock();
}


