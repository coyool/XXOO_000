/**
* unique.h -- �Զ������
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-18
* ����޸�ʱ��: 2009-5-18
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

//�õ��û��澯����
char* GetUserPhoneNumber(void);
#endif /*_PARA_UNIQUE_H*/

