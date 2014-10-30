/**
* datatype_gb.c -- ������������ת������ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-8
* ����޸�ʱ��: 2009-5-8
*/

#ifndef _DATATYPE_GB_H
#define _DATATYPE_GB_H

int Gbformat02ToPower(const unsigned char *psrc);
void PowerToGbformat02(int src, unsigned char *pdst);

#define MAX_GENE_SMALL		2000000000  //��int��������ֵ
/*#define MAX_GENE_LARGE		9999999000000ll
#define gene_t long long    //0.001kWh/��

gene_t Gbformat03ToEne(const unsigned char *psrc);
void EneToGbformat03(gene_t src, unsigned char *pdst);*/

#define gene_t int   // kWh/��

int Gbformat03ToShortEne(const unsigned char *psrc);
void ShortEneToGbformat03(int src, unsigned char *pdst);

#define Gbformat03ToEne Gbformat03ToShortEne
#define EneToGbformat03 ShortEneToGbformat03

#endif /*_DATATYPE_GB_H*/

