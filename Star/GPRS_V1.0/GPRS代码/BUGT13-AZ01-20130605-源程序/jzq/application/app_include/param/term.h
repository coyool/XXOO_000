/**
* term.h -- �ն˲���ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-6
* ����޸�ʱ��: 2009-5-6
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

