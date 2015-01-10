

#include "pdl_header.h"


static BT_RTInitRegT tRTInitRegT = 
{
    4999,                               //ִ�ж��ٸ���ʱ��������
    BT_CLK_DIV_1,                       //��ʱ��ʱ�ӷ�Ƶ
    BT_TRG_DISABLE,                     //��ʱ���������壨�������رգ�
    BT_32BIT_TIMER,                     //ѡ��32λ��ʱ��
    BT_POLARITY_NORMAL,                 //��ʱ������
    BT_MODE_CONTINUOUS,                 //��ʱ������ģʽ
};


/********************************************************/
//������ʱ������жϺ���
/********************************************************/
static void RTUnderflowIntHandler(void)
{
    TaskRunFlag = 1;
}


/*******************************************************/
//����:����ʱ�Ӷ�ʱ������
//�������:
//�������:	
//��������ֵ˵��:
//ʹ�õ���Դ��
//���ߣ�
//���ڣ�   		
//��ע:
/*******************************************************/
void BTInit(void)
{
    NVIC_EnableIRQ(BTIM0_7_IRQn);                               //������ʱ���ж�
    BT_SetIOMode(BT_CH_0,BT_IO_MODE_0);                         //IO��0ģʽ
    BT_RTInit(BT_CH_0,&tRTInitRegT);                            //��ʼ����ʱ��
    BT_RTEnableUnderflowInt(BT_CH_0,RTUnderflowIntHandler);     //ʹ��RT����ж�
    BT_RTEnableCount(BT_CH_0);                                  //ʹ��RT��������
    BT_RTStartSoftTrig(BT_CH_0);                                //����RT���������
}


