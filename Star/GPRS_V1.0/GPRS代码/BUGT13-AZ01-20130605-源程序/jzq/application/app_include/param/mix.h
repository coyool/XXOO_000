/**
* mix.h -- 综合参数头文件
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-8
* 最后修改时间: 2009-5-8
*/

#ifndef _PARAM_MIX_H
#define _PARAM_MIX_H
#include "app_include/param/param_init.h"


#ifndef DEFINE_PARAMIX
//extern /*const*/ para_mix_t ParaMix;
//extern para_commport_t ParaCommPort[MAX_COMMPORT];
#endif

#define ParaMix para_comb.mix
#define ParaCommPort para_comb.ParaCommPort

#endif /*_PARAM_MIX_H*/

