#ifndef __PLAT_NETWORK_H__
#define __PLAT_NETWORK_H__

typedef enum
{
	PF_IP = 0,
	PF_AT 
}PlatSocketType;
typedef enum
{
	SOCKET_TCP = 0,
	SOCKET_UDP
}PlatProtoType;

typedef union 
{
unsigned char uc[4];
unsigned int ul;
} varip_t;


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
int NetCreateSocket(int type,int proto);

/*
˵���趨socket����ģʽ
�������: 
sockfd socketͨ��������,Mode  1 ����ģʽ, 0 ������ģʽ
����ֵ:
���ɹ�����SUCCEED��ʧ�ܷ��� -ERRFAILED
*/
int NetSetSocketBlockMode(int sockfd,int Mode);
/*
˵��:���ӵ�Զ������
�������:
    ip Զ������ip��port Զ�������˿�
�������:
    ��
����ֵ:
    SUCCEED��ʧ�� -ERRFAILED
*/
int NetConnectHost(int sockfd,varip_t ip,unsigned short port);

/*
˵��:���׽��ֺͻ����ϵ�һ���Ķ˿ڹ���
�������:
    sockfd socketͨ��������,port �˿ں�
�������:
    ��
����ֵ:
    �ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int NetBindPort(int sockfd,unsigned short port);

/*
˵��:ʹ�����������׽ӿڱ�Ϊ�������׽ӿڣ�ʹ��һ�����̿��Խ����������̵����󣬴Ӷ���Ϊһ������������
�������:
    sockfd socketͨ��������
�������:
    ��
����ֵ:
    SUCCEED��ʧ�� -ERRFAILED
*/
int NetListen(int sockfd);

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
int NetAcceptConnect(int sockfd, varip_t *ip,unsigned short* port);

/*
˵��:������ͨ�Žӿڷ�������(TCP)
�������:
    sockfd socketͨ����������buf �������ݻ�����ָ�룬len �������ݳ���
�������:
    ��
����ֵ:
    SUCCEED��ʧ�� -ERRFAILED
*/
int NetSendtoData(int sockfd,const char *buf,int len,const varip_t* to,const unsigned short port);

/*
˵��:������ͨ�Žӿڶ�ȡ����(TCP)
�������:
    sockfd socketͨ����������len �������ݻ��泤��
�������:
    buf �������ݻ�����ָ��
����ֵ:
    �ɹ����ؽ������ݳ��ȣ�ʧ��-ERRFAILED
*/
int NetRecvfromData(int sockfd,char *buf,int len,varip_t* from,unsigned short* port);

/*
˵��:������ͨ�Žӿڷ�������(UDP)
�������:
    sockfd socketͨ����������buf �������ݻ�����ָ�룬len �������ݳ���
�������:
    ��
����ֵ:
    SUCCEED��ʧ�� -ERRFAILED
*/
int NetSendData(int sockfd,const char *buf,int len);

/*
˵��:������ͨ�Žӿڶ�ȡ����(UDP)
�������:
    sockfd socketͨ����������len ����������
�������:
    buf �������ݻ�����ָ��
����ֵ:
    �ɹ����ؽ������ݳ��ȣ�ʧ��-ERRFAILED
*/
int NetRecvData(int sockfd,char *buf,int len);

/*
˵��:�ر�socketͨ��������
�������:
    sockfd socketͨ��������
�������:
    ��
����ֵ:
    SUCCEED��ʧ�� -ERRFAILED
*/
int NetCloseSocket(int sockfd);


#endif

