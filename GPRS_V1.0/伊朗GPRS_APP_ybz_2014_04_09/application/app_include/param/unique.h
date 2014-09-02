/**
* unique.h -- 自定义参数
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-18
* 最后修改时间: 2009-5-18
*/

#ifndef _PARA_UNIQUE_H
#define _PARA_UNIQUE_H
#include "app_include/param/param_init.h"

#ifndef DEFINE_PARAUNI
//extern /*const*/ para_uni_t ParaUni;
#endif
#define ParaUni para_comb.uniq

/*#ifndef YUNNAN_GEJIU
#define YUNNAN_GEJIU	1
#endif*/

int SaveParaUni(void);
int SaveParaManuf(void);

//得到用户告警号码
char* GetUserPhoneNumber(void);
#endif /*_PARA_UNIQUE_H*/

