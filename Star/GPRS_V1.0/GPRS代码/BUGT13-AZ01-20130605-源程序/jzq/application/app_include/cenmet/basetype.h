
#ifndef _BASETYPE_H
#define _BASETYPE_H

typedef void (*pfvoid_t)(void);

#define SCHAR    char
#define UCHAR    unsigned char
#define VCHAR    volatile unsigned char
#define SSHORT    short
#define USHORT    unsigned short
#define VSHORT    volatile unsigned short
#define SINT    int
#define UINT    unsigned int
#define VINT    volatile unsigned int
#define SLONG    long
#define ULONG    unsigned long
#define VLONG    volatile unsigned long
#define INT64    long long

typedef struct {
	UCHAR year;
	UCHAR month;
	UCHAR day;
	UCHAR hour;
	UCHAR minute;
	UCHAR second;
} rtime_t;
/*
typedef struct {
	UCHAR year;
	UCHAR month;
	UCHAR day;
	UCHAR hour;
	UCHAR minute;
} dbtime_t;
*/
typedef union {
	USHORT us;
	UCHAR uc[4];
} VARSHORT;

typedef union {
	ULONG ul;
	UCHAR uc[4];
} VARLONG;

typedef union {
	float ft;
	UCHAR uc[4];
} VARFLOAT;

typedef union {
	double db;
	UCHAR uc[8];
} VARDOUBLE;

#define VARFLEX_ULONG    0
#define VARFLEX_INT    1
#define VARFLEX_UCHAR    2
#define VARFLEX_BCD    3
#define VARFLEX_USHORT    4
#define VARFLEX_SSHORT    5
typedef union {
	ULONG ul;
	int si;
	UCHAR uc[4];
	USHORT us;
	short ss;
} VARFLEX_4;

#endif /*_BASETYPE_H*/
