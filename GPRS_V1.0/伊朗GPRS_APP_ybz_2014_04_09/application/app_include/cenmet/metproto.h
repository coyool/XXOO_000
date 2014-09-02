#ifndef _METPROTO_H
#define _METPROTO_H
#include "app_include/cenmet/basetype.h"

typedef struct {
	int baudrate;
	UCHAR port;
	UCHAR metid;
	USHORT itemid;

	UCHAR addr[6];
	char databits;
	char stopbits;
	char parity;

	UCHAR *rbuf;
	int rlen;

	UCHAR *password;
	UCHAR *usrname;
} metinfo_t;

#define FUNCTYPE_READ    0
#define FUNCTYPE_SET    1

typedef struct {
	UCHAR id;
	UCHAR attr;
	UCHAR read_num;
	UCHAR reserv;

	int speed;
	char databits;
	char stopbits;
	char parity;

	int (*read)(metinfo_t *pmet);
	int (*set)(metinfo_t *pmet, UCHAR *sbuf, int slen);
	USHORT (*get_itemid)(int idx);
} metfunc_t;

typedef struct {
	UCHAR mkyear;
	UCHAR mkmonth;
	UCHAR mkday;
	UCHAR mkhour;
	UCHAR mkminute;
	UCHAR ver_major;
	UCHAR ver_minor;
	UCHAR ver_area[2];
	UCHAR num;
	UCHAR id[2];
} protoinfo_t;

#define PROMPT_RCVOK		0
#define PROMPT_TIMEOUT		1
#define PROMPT_MARKERR		2
#define PROMPT_CHKERR		3
#define PROMPT_FAIL			4
#define PROMPT_LENERR		5
#define PROMPT_SEND			6

int get_stdlen(USHORT itemid);
int dl645_read(metinfo_t *pmet);
USHORT dl645_get_itemid(int idx);

int get_2007_stdlen(USHORT itemid);
int dl645_2007_read(metinfo_t *pmet);
USHORT dl645_2007_get_itemid(int idx);

//int get_metfunc(UCHAR proto_id, UCHAR functype, metfunc_t **ppfunc);


#define VERSION_AREA  0
#define VERSION_JIANGXI 1
#endif  /*_METPROTO_H*/
