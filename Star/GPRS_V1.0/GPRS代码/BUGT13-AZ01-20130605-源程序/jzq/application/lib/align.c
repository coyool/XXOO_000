
typedef union {
	unsigned char uc[4];
	unsigned int ul;
} varul_t;

typedef union {
	unsigned char uc[2];
	unsigned short us;
} varus_t;
/*
#define MAKE_SHORT(buf) ({ \
	varus_t _tmp_varus; \
	_tmp_varus.uc[0] = ((unsigned char *)(buf))[0]; \
	_tmp_varus.uc[1] = ((unsigned char *)(buf))[1]; \
	_tmp_varus.us; \
})
*/
unsigned short MAKE_SHORT(const unsigned char* buf){
	varus_t _tmp_varus; 
	
	_tmp_varus.uc[0] = ((unsigned char *)(buf))[0]; 
	_tmp_varus.uc[1] = ((unsigned char *)(buf))[1]; 
	
	return _tmp_varus.us; 
}
/*
#define DEPART_SHORT(usv, buf) { \
	varus_t _tmp_varus; \
	_tmp_varus.us = usv; \
	((unsigned char *)(buf))[0] = _tmp_varus.uc[0]; \
	((unsigned char *)(buf))[1] = _tmp_varus.uc[1]; \
}
*/
void DEPART_SHORT(unsigned short usv,unsigned char* buf){
	varus_t _tmp_varus; 

	_tmp_varus.us = usv; 
	((unsigned char *)(buf))[0] = _tmp_varus.uc[0]; 
	((unsigned char *)(buf))[1] = _tmp_varus.uc[1]; 

}

/*
#define MAKE_LONG(buf) ({ \
	varul_t _tmp_varul; \
	_tmp_varul.uc[0] = ((unsigned char *)(buf))[0]; \
	_tmp_varul.uc[1] = ((unsigned char *)(buf))[1]; \
	_tmp_varul.uc[2] = ((unsigned char *)(buf))[2]; \
	_tmp_varul.uc[3] = ((unsigned char *)(buf))[3]; \
	_tmp_varul.ul; \
})*/
unsigned long MAKE_LONG(const unsigned char* buf){
	varul_t _tmp_varul; 

	_tmp_varul.uc[0] = ((unsigned char *)(buf))[0]; 
	_tmp_varul.uc[1] = ((unsigned char *)(buf))[1]; 
	_tmp_varul.uc[2] = ((unsigned char *)(buf))[2]; 
	_tmp_varul.uc[3] = ((unsigned char *)(buf))[3]; 

	return _tmp_varul.ul; 
}
/*
#define DEPART_LONG(ulv, buf) { \
	varul_t _tmp_varul; \
	_tmp_varul.ul = ulv; \
	((unsigned char *)(buf))[0] = _tmp_varul.uc[0]; \
	((unsigned char *)(buf))[1] = _tmp_varul.uc[1]; \
	((unsigned char *)(buf))[2] = _tmp_varul.uc[2]; \
	((unsigned char *)(buf))[3] = _tmp_varul.uc[3]; \
}
*/

void DEPART_LONG(unsigned long ulv,unsigned char* buf){
	varul_t _tmp_varul; 

	_tmp_varul.ul = ulv; 
	((unsigned char *)(buf))[0] = _tmp_varul.uc[0]; 
	((unsigned char *)(buf))[1] = _tmp_varul.uc[1]; 
	((unsigned char *)(buf))[2] = _tmp_varul.uc[2]; 
	((unsigned char *)(buf))[3] = _tmp_varul.uc[3]; 
}
















