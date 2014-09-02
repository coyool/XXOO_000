#ifndef __MUTEX_H
#define __MUTEX_H



/*ȫ��������������ƽ̨�㱻������ÿ���̵߳���ڴ�Ҫ��mutex_lock(MUTEX_GLOBE)�õ�
���ȫ����֮���������*/
#define MUTEX_GLOBE	0

/*Ӧ�ò��ܹ��õ���
mutex_init�ǵײ�����ģ�Ӧ�ò��ֹ����
ʹ�÷�����
1.����	mutex_create(MUTEX_1)
2.�õ� mutex_lock(MUTEX_1);
2.�ͷ� mutex_unlock(MUTEX_1);
ÿ����������-1��ʾʧ�ܣ�0���ɹ�
*/
#define MUTEX_1		1
#define MUTEX_2		2
#define MUTEX_3		3
#define MUTEX_4		4
#define MUTEX_5		5
#define MUTEX_6		6
#define MUTEX_7		7
#define MUTEX_8		8
#define MUTEX_9		9
#define PLC_MUTEX  MUTEX_2

void mutex_init(void);
int mutex_create(unsigned int n);
int mutex_lock(unsigned int n);
int mutex_unlock(unsigned int n);
#define __init_GlobalLock() mutex_create(MUTEX_GLOBE)
#define  __GlobalLock()  	mutex_lock(MUTEX_GLOBE)
#define  __GlobalUnlock()   mutex_unlock(MUTEX_GLOBE)



#endif


