/**
* adc.h -- A/D�����ӿ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-16
* ����޸�ʱ��: 2009-5-16
*/

#ifndef _SYS_ADC_H
#define _SYS_ADC_H

#define ADC_CHN_BAT		0   //��ص�ѹͨ��
#define ADC_CHN_TEMP	1   //�¶�ͨ��
#define ADC_CHN_MAX		2

/**
* @brief ��ȡADC����
* @param channel ͨ����
* @return ��ֵ (mV)
*/
int AdcRead(unsigned int channel);

#endif /*_SYS_ADC_H*/

