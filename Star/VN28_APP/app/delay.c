
#include "pdl_header.h"
#include "delay.h"
/*********************************************************** 
函数功能：延时函数 Xms
入口参数：ms
出口参数：
备注说明:
***********************************************************/
void DelayMs(uint32_t ms)
{
  SysCtlDelay((ms * (SystemCoreClock/3000)));
}

void SysCtlDelay(unsigned long ulCount)
{

  
    __asm("    subs    r0, #1\n"
          "    bne.n   SysCtlDelay\n"
          "    bx      lr");
}


