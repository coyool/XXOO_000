/**
* mdbconf.h -- �����������
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-11
* ����޸�ʱ��: 2009-5-11
*/

#ifndef _MDB_CONF_H
#define _MDB_CONF_H

#include "app_include/cenmet/dbase.h"

//��Ʒ�����
#define MAXNUM_FEEPRD   4
#define MAXNUM_METPRD	(MAXNUM_FEEPRD+1)

//�������ݶ����ܶ�
//#define CURVE_FREZ		1  // 15 minute
#define CURVE_FREZ		DBFREZ_1HOUR      // 60 minute

#endif /*_MDB_CONF_H*/
