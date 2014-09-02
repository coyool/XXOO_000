#include "plat_include/plat_defines.h"
#include "plat_include/plat_network.h"
#include "plat_include/m35gprs.h"
#include "plat_include/mc55gprs.h"

/*
˵��:����socketͨ��������(TCP/UDP)
�������:
	type = PF_IP,PF_AT 
    proto =SOCKET_TCP TCP, =SOCKET_UDP UDP
�������:
    ��
����ֵ:
    ����ֵ���ɹ�����ͨ����������ʧ�ܷ��� -ERRFAILED
*/
int _type = -1;
int NetCreateSocket(int type,int proto)
{
	_type = type;
	if(_type == PF_AT)
		_GprsMode(proto);
	return 0;
}

/*
˵���趨socket����ģʽ
�������: 
sockfd socketͨ��������,Mode  1 ����ģʽ, 0 ������ģʽ
����ֵ:
���ɹ�����SUCCEED��ʧ�ܷ��� -ERRFAILED
*/
int NetSetSocketBlockMode(int sockfd,int Mode)
{
	return 	-ERRFAILED;		
}

/*
˵��:���ӵ�Զ������
�������:
    ip Զ������ip��port Զ�������˿�
�������:
    ��
����ֵ:
    SUCCEED��ʧ�� -ERRFAILED
*/
int NetConnectHost(int sockfd,varip_t ip,unsigned short port)
{
	if(_type == PF_AT)
		return _GprsConnect(ip.ul,port);
	return -ERRFAILED;	
}

/*
˵��:���׽��ֺͻ����ϵ�һ���Ķ˿ڹ���
�������:
    sockfd socketͨ��������,port �˿ں�
�������:
    ��
����ֵ:
    �ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int NetBindPort(int sockfd,unsigned short port)
{
	 return -ERRFAILED;	
}

/*
˵��:ʹ�����������׽ӿڱ�Ϊ�������׽ӿڣ�ʹ��һ�����̿��Խ����������̵����󣬴Ӷ���Ϊһ������������
�������:
    sockfd socketͨ��������
�������:
    ��
����ֵ:
    SUCCEED��ʧ�� -ERRFAILED
*/
int NetListen(int sockfd)
{
	return -ERRFAILED;
}

/*
˵��:�ȴ������ܿͻ�����
�������:
    sockfd socketͨ��������
�������:
    ip�ͻ���IP��ַָ��
	port�ͻ���portָ��
����ֵ:
    �ɹ����������׽��֣�ʧ�� -ERRFAILED
*/
int NetAcceptConnect(int sockfd, varip_t *ip,unsigned short* port)
{
	return -ERRFAILED;
}

/*
˵��:������ͨ�Žӿڷ�������(TCP)
�������:
    sockfd socketͨ����������buf �������ݻ�����ָ�룬len �������ݳ���
�������:
    ��
����ֵ:
    SUCCEED��ʧ�� -ERRFAILED
*/
int NetSendtoData(int sockfd,const char *buf,int len,const varip_t* to,const unsigned short port)
{
	return -ERRFAILED;	
}

/*
˵��:������ͨ�Žӿڶ�ȡ����(TCP)
�������:
    sockfd socketͨ����������len �������ݻ��泤��
�������:
    buf �������ݻ�����ָ��
����ֵ:
    �ɹ����ؽ������ݳ��ȣ�ʧ��-ERRFAILED
*/
int NetRecvfromData(int sockfd,char *buf,int len,varip_t* from,unsigned short* port)
{
	return -ERRFAILED;	
}

/*
˵��:������ͨ�Žӿڷ�������(UDP)
�������:
    sockfd socketͨ����������buf �������ݻ�����ָ�룬len �������ݳ���
�������:
    ��
����ֵ:
    SUCCEED��ʧ�� -ERRFAILED
*/
int NetSendData(int sockfd,const char *buf,int len)
{
	if(_type == PF_AT)
		return _GprsRawSend((const unsigned char*)buf,len);
	return -ERRFAILED;	
}

/*
˵��:������ͨ�Žӿڶ�ȡ����(UDP)
�������:
    sockfd socketͨ����������len ����������
�������:
    buf �������ݻ�����ָ��
����ֵ:
    �ɹ����ؽ������ݳ��ȣ�ʧ��-ERRFAILED
*/
int NetRecvData(int sockfd,char *buf,int len)
{
	if(_type == PF_AT)
		return _GprsGetChar((unsigned char*)buf,len);
	return -ERRFAILED;
}

/*
˵��:�ر�socketͨ��������
�������:
    sockfd socketͨ��������
�������:
    ��
����ֵ:
    SUCCEED��ʧ�� -ERRFAILED
*/
int NetCloseSocket(int sockfd)
{
	if(_type == PF_AT)
	{
		_GprsDisconnect();
		return SUCCEED;
	}
	return -ERRFAILED;
}
