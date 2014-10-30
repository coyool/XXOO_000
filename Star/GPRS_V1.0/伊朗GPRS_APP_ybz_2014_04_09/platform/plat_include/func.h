
#ifndef __FUNC_H
#define __FUNC_H

#define OFSIZE(type,field)	sizeof(((type *)0)->field)
#define ARRAY_SIZE(a)	(sizeof(a)/sizeof(a[0]))

/**
 * u8����ת�����ַ�
 * @i: u8����
 * @return �ַ�����
*/
unsigned char hex_to_char(unsigned char i);

/**
 * �ַ�����ת����u8
 * @i: u8����
 * @return �ַ�����
*/
unsigned char char_to_hex(unsigned char i);

unsigned char bcdtohex(unsigned char bcd);

unsigned char hextobcd(unsigned char hex);

unsigned int isbcd(const unsigned char *bcd,unsigned int n);


unsigned int string_to_hex(unsigned int *a,const unsigned char *str);

unsigned int string_to_bcd(unsigned int *a,const unsigned char *str);

#define swap(x, y) ({typeof(x) t=x; x=y; y=t;}) 
#endif
