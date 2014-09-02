#ifndef _DS3231_H_
#define _DS3231_H_
/** 
  * @brief  Ds3231 ʱ��оƬI2C���߳�ʼ��  
  */
void ds3231_Init(void);
/** 
  * @brief ��ȡDs3231 ʱ��оƬ����������ʱ����
  * @param ����7���ֽڵ�����:��������ʱ����,ÿ���ֽڶ���BCD��
  */
void ds3231_read_ymdwhms(unsigned char *str);
/** 
  * @brief ����Ds3231 ʱ��оƬ����������ʱ����
  * @param ����7���ֽڵ�����:��������ʱ����,ÿ���ֽڶ���BCD��
  */
void ds3231_write_ymdwhms(const unsigned char *str);

#define  read_ymdwhms ds3231_read_ymdwhms
#define  write_ymdwhms ds3231_write_ymdwhms

#endif
