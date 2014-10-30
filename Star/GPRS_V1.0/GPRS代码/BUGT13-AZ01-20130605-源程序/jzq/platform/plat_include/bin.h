/**
* bin.h -- �������ļ�����ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-6
* ����޸�ʱ��: 2009-5-6
*/

#ifndef _STORAGE_BIN_H
#define _STORAGE_BIN_H

/**
* @brief ����һ��BIN�ļ�
* @param file �ļ���
* @param magic �ļ���ʶ��
* @param buffer ������ָ��
* @param len ����������
* @return �ɹ�����0, ����ʧ��
*/
int SaveBinFile(const char *file, unsigned long magic, const unsigned char *buffer, int len);
int SaveBinFileUnlock(const char *file, unsigned long magic, const unsigned char *buffer, int len);

/**
* @brief ��ȡһ��BIN�ļ�
* @param file �ļ���
* @param magic �ļ���ʶ��
* @param buffer ������ָ��
* @param len ����������
* @return �ɹ�����ʵ�ʶ�ȡ����,ʧ�ܷ���-1
*/
int ReadBinFile(const char *file, unsigned long magic, unsigned char *buffer, int len);

/**
* @brief ��ȡһ��BIN�ļ�
*    ��ReadBinFile��ͬ����,buffer�����ڶ�ȡʧ�ܵ������Ҳ�п����޸�
*    �����ҪӦ�ó������ר�ŵ�buffer
*    һ��������ȡ�����ļ�
*    ������ReadBinFile�����ڴ�������, �����������ݲ���ȫ��, ʹ��ʱҪС��
* @param file �ļ���
* @param magic �ļ���ʶ��
* @param buffer Cache������ָ��
* @param len ����������
* @return �ɹ�����ʵ�ʶ�ȡ����,ʧ�ܷ���-1
*/
int ReadBinFileCache(const char *file, unsigned long magic, unsigned char *buffer, int len);

struct bin_buflist {
	struct bin_buflist *next;  //��һ��ָ��
	unsigned char *buffer;  //��������ַ
	int buflen;  //����������
};
#define INIT_BINLIST(list, pbuf, len)		{ \
	(list).buffer = pbuf; \
	(list).buflen = len; \
	(list).next = NULL; \
}

#define APPEND_BINLIST(list, list2) { (list).next = &(list2); }

/**
* @brief ����һ��BIN�ļ�
* @param file �ļ���
* @param magic �ļ���ʶ��
* @param buffer ������ָ��
* @param len ����������
* @return �ɹ�����0, ����ʧ��
*/
int SaveBinFileList(const char *file, unsigned long magic, const struct bin_buflist *plist);
int SaveBinFileListUnlock(const char *file, unsigned long magic, const struct bin_buflist *plist);

/**
* @brief ��ȡһ��BIN�ļ�
* @param file �ļ���
* @param magic �ļ���ʶ��
* @param plist ����������ָ��
* @return �ɹ�����ʵ�ʶ�ȡ����,ʧ�ܷ���-1
*/
int ReadBinFileList(const char *file, unsigned long magic, const struct bin_buflist *plist);



#endif /*_STORAGE_BIN_H*/

