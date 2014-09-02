/**
* term.h -- 终端参数头文件
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-6
* 最后修改时间: 2009-5-6
*/

#ifndef _PARAM_TERM_H
#define _PARAM_TERM_H

#include "app_include/param/param_init.h"

#ifndef DEFINE_PARATERM
//extern /*const*/ para_term_t ParaTerm;
void SetParaNetAddr(void);
#endif
#define ParaTerm para_comb.term

#endif /*_PARAM_TERM_H*/

