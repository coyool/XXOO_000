/**
* sigin.h -- �����ź������ӿ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-16
* ����޸�ʱ��: 2009-5-16
*/

#ifndef _SYS_SIGIN_H
#define _SYS_SIGIN_H


#define SIGIN_NUM		1

#define SIGIN_MASK		((1<<SIGIN_NUM)-1)


/**
* @brief ��ȡ������״̬
* @return ������״̬,ÿBIT����һ��
*/
//unsigned int SiginReadState(void);
/**
* @brief ��ȡ���밴��
* @return ���밴����Ϣ,����0��ʾû������
*/
char SiginGetChar(void);

#endif /*_SYS_SIGIN_H*/

