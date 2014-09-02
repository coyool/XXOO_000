//----------------------------------------------
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//----------------------------------------------
#include "includes.h"
#include "os_cpu.h"
#include "plat_include/sys_config.h"
#include "plat_include/Plat_uart.h"
#include "plat_include/plat_defines.h"
#include "plat_include/ring.h"
#include "plat_include/debug.h"


//----------------------------------------------
void Uart_NVIC_Init(u8 irq)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = irq; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 	
}
void USART1_Configuration( int baud )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;

	Uart_NVIC_Init(USART1_IRQn);

	USART_Cmd( USART1, DISABLE );

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
		RCC_APB2Periph_AFIO |
		RCC_APB2Periph_USART1,
		ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // PA9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init( GPIOA, &GPIO_InitStructure );



	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init( GPIOA, &GPIO_InitStructure );

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b; 
	USART_InitStructure.USART_Parity = USART_Parity_Even;         //偶校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;        //1位停止位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     //发送接收使能
	USART_Init( USART1, &USART_InitStructure );

	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;       //时钟低电平活动
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit( USART1, &USART_ClockInitStructure );

	USART_ITConfig( USART1, USART_IT_RXNE, ENABLE );

	USART_Cmd( USART1, ENABLE );


}

//----------------------------------------------
void USART2_Configuration( int baud )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;

	Uart_NVIC_Init(USART2_IRQn);

	USART_Cmd( USART2, DISABLE );

	RCC_APB1PeriphClockCmd( RCC_APB2Periph_GPIOA |RCC_APB1Periph_USART2, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; // PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init( GPIOA, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; // PA3
#ifdef METER_MODULE
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
#else
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
#endif
	GPIO_Init( GPIOA, &GPIO_InitStructure );

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init( USART2, &USART_InitStructure );



	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit( USART2, &USART_ClockInitStructure );

	///========================================================


	USART_ITConfig( USART2, USART_IT_RXNE, ENABLE );

	USART_Cmd( USART2, ENABLE );
}

//----------------------------------------------
void USART3_Configuration( int baud )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;

	Uart_NVIC_Init(USART3_IRQn);

	USART_Cmd( USART3, DISABLE );

	RCC_APB1PeriphClockCmd( RCC_APB2Periph_GPIOB |RCC_APB1Periph_USART3, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init( GPIOB, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; // PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init( GPIOB, &GPIO_InitStructure );

	//	USART_InitStructure.USART_BaudRate = baud;
	//	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	//	USART_InitStructure.USART_Parity = USART_Parity_Even;
	//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//	USART_InitStructure.USART_Mode = USART_Mode_Rx;
	//	USART_Init( USART3, &USART_InitStructure );

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init( USART3, &USART_InitStructure );


	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit( USART3, &USART_ClockInitStructure );

	USART_ITConfig( USART3, USART_IT_RXNE, ENABLE );

	USART_Cmd( USART3, ENABLE );
}

//----------------------------------------------
void USART4_Configuration( int baud )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;

	Uart_NVIC_Init(UART4_IRQn);

	USART_Cmd( UART4, DISABLE );

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART4, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // PC10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init( GPIOC, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; // PC11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init( GPIOC, &GPIO_InitStructure );


	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b; 
	USART_InitStructure.USART_Parity = USART_Parity_Even;        
	USART_InitStructure.USART_StopBits = USART_StopBits_1;       
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     //发送接收使能
	USART_Init( UART4, &USART_InitStructure );

	//	USART_InitStructure.USART_BaudRate = baud;
	//	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	////	USART_InitStructure.USART_Parity = USART_Parity_Even;         //偶校验
	//	USART_InitStructure.USART_Parity = USART_Parity_No;
	//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//	USART_Init( UART4, &USART_InitStructure );

	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit( UART4, &USART_ClockInitStructure );

	USART_ITConfig( UART4, USART_IT_RXNE, ENABLE );

	USART_Cmd( UART4, ENABLE );
}

//----------------------------------------------
void USART5_Configuration( int baud )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;

	Uart_NVIC_Init(UART5_IRQn);

	USART_Cmd( UART5, DISABLE );

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART5, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; // PC12
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init( GPIOC, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; // PD2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init( GPIOD, &GPIO_InitStructure );

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init( UART5, &USART_InitStructure );

	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
//	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;	 //程序跑飞
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit( UART5, &USART_ClockInitStructure );

	USART_ITConfig( UART5, USART_IT_RXNE, ENABLE );
	USART_ITConfig( UART5, USART_IT_TXE, ENABLE);
	USART_Cmd( UART5, ENABLE );
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/


#ifdef METER_MODULE
unsigned char Uart1Recvebuf[1024];
unsigned char Uart2Recvebuf[1024];
unsigned char Uart3Recvebuf[2000];
unsigned char Uart4Recvebuf[1024];
unsigned char Uart5Recvebuf[1024];
#else
unsigned char Uart1Recvebuf[2000];
unsigned char Uart2Recvebuf[256];
unsigned char Uart3Recvebuf[256];
unsigned char Uart4Recvebuf[256];
unsigned char Uart5Recvebuf[512];
#endif
unsigned char Uart1Sendbuf[256];
unsigned char Uart2Sendbuf[256];
unsigned char Uart3Sendbuf[256];
unsigned char Uart4Sendbuf[256];
unsigned char Uart5Sendbuf[256];


typedef void (*INIT_UART_TYPE)(int);
typedef struct{
	USART_TypeDef* name;
	INIT_UART_TYPE init;
	unsigned char* RecBuf;
	int Reclen;
	unsigned char* SendBuf;
	int Sendlen;
	RingBuffer* RecRing;
	RingBuffer* SendRing;
}UartType;
static UartType uarts[MAX_PORT]=
{
	{USART1,USART1_Configuration,Uart1Recvebuf,sizeof(Uart1Recvebuf),Uart1Sendbuf,sizeof(Uart1Sendbuf),NULL,NULL},
	{USART2,USART2_Configuration,Uart2Recvebuf,sizeof(Uart2Recvebuf),Uart2Sendbuf,sizeof(Uart1Sendbuf),NULL,NULL},
	{USART3,USART3_Configuration,Uart3Recvebuf,sizeof(Uart3Recvebuf),Uart3Sendbuf,sizeof(Uart1Sendbuf),NULL,NULL},
	{UART4, USART4_Configuration,Uart4Recvebuf,sizeof(Uart4Recvebuf),Uart4Sendbuf,sizeof(Uart1Sendbuf),NULL,NULL},
	{UART5, USART5_Configuration,Uart5Recvebuf,sizeof(Uart5Recvebuf),Uart5Sendbuf,sizeof(Uart1Sendbuf),NULL,NULL},
};
extern void shining_led(void);
void UartIsr(int port)
{
	if ( USART_GetITStatus( uarts[port].name, USART_IT_RXNE ) != RESET )
	{
		USART_ClearITPendingBit( uarts[port].name, USART_IT_RXNE );
		RingSet(uarts[port].RecRing,USART_ReceiveData(uarts[port].name));
		USART_ClearFlag( uarts[port].name, USART_FLAG_ORE );
	}
	if ( USART_GetITStatus( uarts[port].name, USART_IT_TXE)  != RESET )
	{
		int SendData = RingPop(uarts[port].SendRing);
		if(SendData != -1)
			USART_SendData(	uarts[port].name,(u8)SendData);
		else
		{
			USART_ClearITPendingBit(uarts[port].name,USART_IT_TXE);
			USART_ITConfig(uarts[port].name, USART_IT_TXE, DISABLE);
		}
	}

	if ( USART_GetFlagStatus( uarts[port].name, USART_FLAG_ORE ) == SET )
	{ 
		USART_ClearFlag( uarts[port].name, USART_FLAG_ORE );
		USART_ReceiveData( uarts[port].name );
	}
}
void USART1_IRQHandler(void)
{
	OSIntEnter();
	UartIsr(0);
	OSIntExit();
}
void USART2_IRQHandler(void)
{
	OSIntEnter();
	UartIsr(1);
	OSIntExit();
}
void USART3_IRQHandler(void)
{
	OSIntEnter();
	UartIsr(2);
	OSIntExit();
}
void UART4_IRQHandler(void)
{
	OSIntEnter();
	UartIsr(3);
	OSIntExit();
}
void UART5_IRQHandler(void)
{
	OSIntEnter();
	UartIsr(4);
	OSIntExit();
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

/*
说明:打开某个指定的串口
输入参数:
port 端口号（0~9）
输出参数:
无
返回值:
成功 SUCCEED，失败 -ERRFAILED
*/
int UartOpen(unsigned int port)
{
	if(port<MAX_PORT)
	{
		uarts[port].RecRing = RingInit(uarts[port].RecBuf,uarts[port].Reclen);
		uarts[port].SendRing = RingInit(uarts[port].SendBuf,uarts[port].Sendlen);
		uarts[port].init(9600);
		return SUCCEED;
	}
	return -ERRFAILED;
}


/*
说明:关闭一个已打开的串口
输入参数:
port 端口号（0~9）
输出参数:
无
返回值:
成功 SUCCEED，失败 -ERRFAILED
*/
void UartClose(unsigned int port)
{
	USART_Cmd( uarts[port].name, DISABLE );
}
/*
说明:设置串口通信参数
输入参数:
port 端口号（0~9），baud 波特率，databits 数据位（5~8），
stopbits 停止位（1~2），parity 校验位（N 无校验，E 偶校验，O奇校验）
输出参数:
无
返回值:
无
*/
void UartSetPara(unsigned int port, int baud, int databits, int stopbits, char parity)
{
	USART_InitTypeDef USART_InitStructure;

	if (port >= MAX_PORT)
		return;

	USART_InitStructure.USART_BaudRate = baud;
	switch(parity)
	{
	case 'N':
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_Parity = USART_Parity_No; //无校验
		break;
	case 'E':
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		USART_InitStructure.USART_Parity = USART_Parity_Even; //偶校验
		break;
	case 'O':
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		USART_InitStructure.USART_Parity = USART_Parity_Odd; //奇校验
		break;
	default:
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_Parity = USART_Parity_No; //无校验
	}
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init( uarts[port].name, &USART_InitStructure );	
}

/*
说明:从串口接收数据
输入参数:
port 端口号（0~9），len 缓存区长度
输出参数:
buf 接收缓存区
返回值:
成功 数据长度，失败 -ERRFAILED
*/
int UartRecvData(unsigned int port, unsigned char *buf, int len)
{
	int cp_len;

	OS_CPU_SR  cpu_sr = 0;

	if(port>=MAX_PORT)
		return -ERRFAILED;
	if(RingLength(uarts[port].RecRing) == 0)
		return 0;
//	USART_ITConfig(uarts[port].name, USART_IT_RXNE, DISABLE);
	OS_ENTER_CRITICAL();
	cp_len = RingGetBuffer(uarts[port].RecRing,buf,len);
	OS_EXIT_CRITICAL();
	USART_ITConfig(uarts[port].name, USART_IT_RXNE, ENABLE);
	return cp_len;
}


/*
说明:通过串口发送数据
输入参数:
port 端口号（0~9），buf 发送缓存区，len 缓存区长度
输出参数:
无
返回值:
成功 SUCCEED，失败 -ERRFAILED
*/
int UartSendData(unsigned int port,const unsigned char *buf, int len)
{	  
	OS_CPU_SR  cpu_sr = 0;
	
	while( len > 0 )
	{
		//USART_ITConfig( uarts[port].name, USART_IT_TXE, DISABLE);
		while(RingLength(uarts[port].SendRing)>=uarts[port].Sendlen)
//			sleep(1);
			OSTimeDly(1);	

		OS_ENTER_CRITICAL();
		RingSet(uarts[port].SendRing,*buf++);
		OS_EXIT_CRITICAL();
		USART_ITConfig( uarts[port].name, USART_IT_TXE, ENABLE);
		len--;
	}
	return SUCCEED;	
}
static unsigned char DebugPort;
void SetDebugPort(unsigned char Port)
{
	DebugPort = Port;
}

/*
说明:清接受缓存
*/
void UartClearRecive(unsigned int port)
{
	if(port>=MAX_PORT)
		return ;
	RingClear(uarts[port].RecRing);
}
/*
说明:接受缓存中数据的长度
*/
int UartRecvDataLength(unsigned int port)
{
	return RingLength(uarts[port].RecRing);
}
void DebugInit(void)			   /* 李工当时采用 UART输出字符串 盲调试的方式来调试程序 */
{
	UartOpen(DebugPort);	           
	UartSetPara(DebugPort,DEBUG_BPS,8,1,'N');
}
int fputc(int ch, FILE *f)
{

	/* Write a character to the USART */
	//USART_SendData(uarts[DEBUG_PORT].name, (u8) ch);

	/* Loop until the end of transmission */
	/*while(USART_GetFlagStatus(uarts[DEBUG_PORT].name, USART_FLAG_TXE) == RESET)
	{
	}*/
	UartSendData(DebugPort,(u8*)&ch,1);
	return ch;
}
int fgetc(FILE *f)
{
	int d;
	OS_CPU_SR  cpu_sr = 0;
	OS_ENTER_CRITICAL();
	d = RingPop(uarts[DebugPort].RecRing);	
	OS_EXIT_CRITICAL();
	if(d == -1)
		return EOF;
	else
		return (u8)d;
}
void PError(const char* e)
{
	while(*e)
	{
	USART_SendData(uarts[DebugPort].name, (u8) *e++);

	/* Loop until the end of transmission */
	while(USART_GetFlagStatus(uarts[DebugPort].name, USART_FLAG_TXE) == RESET)
	{
	}		
	}
}

