/**
* version.h -- �汾��ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-3-31
* ����޸�ʱ��: 2009-5-6
*/

#ifndef _VERSION_H
#define _VERSION_H

//���̰汾��
#define VERPROJ_MAIN		0

#define VERSION_MAJOR		0   //���汾��

#define VERSION_MINOR		03   //�ΰ汾��

#define VERSION_PROJECT		2   //���̰汾��


//�汾��������
#if 1
#define RELEASE_DATE_YEAR	12
#define RELEASE_DATE_MONTH 2
#define RELEASE_DATE_DAY	21
#else
extern const unsigned char _mk_year;
extern const unsigned char _mk_month;
extern const unsigned char _mk_day;
extern const unsigned char _mk_hour;
extern const unsigned char _mk_minute;
#endif

#endif /*_VERSION_H*/

