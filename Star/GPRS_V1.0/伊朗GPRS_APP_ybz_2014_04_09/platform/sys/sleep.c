
#include "ucos_ii.h"
#include "plat_include/mutex.h"
#include "plat_include/debug.h"
/**
 * ˯����ʱ
 * @ticks: ˯�߽�������ÿ����10ms
*/
void sleep(unsigned int ticks)
{
	__GlobalUnlock();
	OSTimeDly(ticks);
	__GlobalLock();
}


