/**
* diskspace.h -- �����������ʣ��ռ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-6-16
* ����޸�ʱ��: 2009-6-16
*/

#ifndef _SYS_DISKSPACE_H
#define _SYS_DISKSPACE_H

/**
* @brief ��������Ŀռ�������
* @param diskpath ������
* @return �ɹ����ؿռ�������(1%), ʧ�ܷ���-1
*/
int DiskUsage(const char *diskpath);

#endif /*_SYS_DISKSPACE_H*/

