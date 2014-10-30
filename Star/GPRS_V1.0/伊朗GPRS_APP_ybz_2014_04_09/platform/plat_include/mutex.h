#ifndef __MUTEX_H
#define __MUTEX_H



/*全局锁互斥量，在平台层被创建，每个线程的入口处要用mutex_lock(MUTEX_GLOBE)得到
这个全局锁之后才能运行*/
#define MUTEX_GLOBE	0

/*应用层能够用的锁
mutex_init是底层调过的，应用层禁止调用
使用方法：
1.创建	mutex_create(MUTEX_1)
2.得到 mutex_lock(MUTEX_1);
2.释放 mutex_unlock(MUTEX_1);
每个函数返回-1表示失败，0，成功
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


