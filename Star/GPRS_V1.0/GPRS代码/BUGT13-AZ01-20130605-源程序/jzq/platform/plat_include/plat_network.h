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
说明:建立socket通信描述符(TCP/UDP)
输入参数:
	type = PF_IP,PF_AT 
    proto =SOCKET_TCP TCP, =SOCKET_UDP UDP
输出参数:
    无
返回值:
    返回值：成功返回通信描述符，失败返回 -ERRFAILED
*/
int NetCreateSocket(int type,int proto);

/*
说明设定socket阻塞模式
输入参数: 
sockfd socket通信描述符,Mode  1 阻塞模式, 0 非阻塞模式
返回值:
：成功返回SUCCEED，失败返回 -ERRFAILED
*/
int NetSetSocketBlockMode(int sockfd,int Mode);
/*
说明:连接到远程主机
输入参数:
    ip 远程主机ip，port 远程主机端口
输出参数:
    无
返回值:
    SUCCEED，失败 -ERRFAILED
*/
int NetConnectHost(int sockfd,varip_t ip,unsigned short port);

/*
说明:将套接字和机器上的一定的端口关联
输入参数:
    sockfd socket通信描述符,port 端口号
输出参数:
    无
返回值:
    成功 SUCCEED，失败 -ERRFAILED
*/
int NetBindPort(int sockfd,unsigned short port);

/*
说明:使用主动连接套接口变为被连接套接口，使得一个进程可以接受其它进程的请求，从而成为一个服务器进程
输入参数:
    sockfd socket通信描述符
输出参数:
    无
返回值:
    SUCCEED，失败 -ERRFAILED
*/
int NetListen(int sockfd);

/*
说明:等待并接受客户请求
输入参数:
    sockfd socket通信描述符
输出参数:
    ip客户端IP地址指针
	port客户端port指针
返回值:
    成功返回连接套接字，失败 -ERRFAILED
*/
int NetAcceptConnect(int sockfd, varip_t *ip,unsigned short* port);

/*
说明:向网络通信接口发送数据(TCP)
输入参数:
    sockfd socket通信描述符，buf 发送数据缓冲区指针，len 发送数据长度
输出参数:
    无
返回值:
    SUCCEED，失败 -ERRFAILED
*/
int NetSendtoData(int sockfd,const char *buf,int len,const varip_t* to,const unsigned short port);

/*
说明:从网络通信接口读取数据(TCP)
输入参数:
    sockfd socket通信描述符，len 接收数据缓存长度
输出参数:
    buf 接收数据缓冲区指针
返回值:
    成功返回接收数据长度，失败-ERRFAILED
*/
int NetRecvfromData(int sockfd,char *buf,int len,varip_t* from,unsigned short* port);

/*
说明:向网络通信接口发送数据(UDP)
输入参数:
    sockfd socket通信描述符，buf 发送数据缓冲区指针，len 发送数据长度
输出参数:
    无
返回值:
    SUCCEED，失败 -ERRFAILED
*/
int NetSendData(int sockfd,const char *buf,int len);

/*
说明:从网络通信接口读取数据(UDP)
输入参数:
    sockfd socket通信描述符，len 缓存区长度
输出参数:
    buf 接收数据缓冲区指针
返回值:
    成功返回接收数据长度，失败-ERRFAILED
*/
int NetRecvData(int sockfd,char *buf,int len);

/*
说明:关闭socket通信描述符
输入参数:
    sockfd socket通信描述符
输出参数:
    无
返回值:
    SUCCEED，失败 -ERRFAILED
*/
int NetCloseSocket(int sockfd);


#endif

