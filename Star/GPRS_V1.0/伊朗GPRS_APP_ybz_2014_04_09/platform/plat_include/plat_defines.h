/*
 *Ԥ����ĺ�
 */
#ifndef __PLAT_DEFINE_H__
#define __PLAT_DEFINE_H__

/*
�������
*/
#define  _LIB_NAME_ "plat"




/*
    �����з���ֵʱ����ֻ���غ���ִ�еĳɹ���ʧ��ʱ��0��ʾ�ɹ���
ʧ�ܵĴ�����Բο��������ֵ������������ֵ�Ǳ�ʾ���ݳ���ʱ��Ϊ
��ֵ��0��
*/

#define SUCCEED     0   /* Operation suceed */

/*
    ���з��صĴ�����붼�Ǹ�ֵ�����¶���Ĵ�����붼����ֵ����
����ʱǰ��Ӹ��ű�ɸ�ֵ�󷵻ء�
*/

#define ERRFAILED      1 /* ִ��ʧ�� */
#define	ERRPERM        2 /* ��ֹ���� */
#define	ERRNOENT       3 /* û�и��ļ���·�� */
#define	ERR2BIG        4 /* �����б�̫�� */
#define	ERRNOEXEC      5 /* ִ�д��� */
#define	ERRBADF        6 /* �ļ���Ŵ��� */
#define	ERRACCES       7 /* ��Ȩ�� */
#define	ERRFAULT       8 /* ��ַ���� */
#define	ERRBUSY	       9 /* �豸����Դ������ʹ�� */
#define	ERREXIST       10 /* �ļ��Ѵ��� */
#define	ERRNODEV       11 /* û�и��豸 */
#define	ERRNOTDIR      12 /* ������ȷ·�� */
#define	ERRINVAL       13 /* �������� */
#define	ERRNFILE       14 /* �ļ��б���� */
#define	ERRMFILE       15 /* �ļ��ظ��� */
#define	ERRFBIG        16 /* �ļ�̫�� */
#define	ERRRANGE       17 /* ����Խ�� */
#define ERRCONFIG      18 /* ���ô��� */
#define ERRGPRS        19 /* GPRS���� */
#define ERRSMSERROR	   20 /* ���Ŵ��� */
/*
    ����ֵ����
*/

#define VK_LEFT             0x25    //37
#define VK_UP               0x26	    
#define VK_RIGHT            0x27
#define VK_DOWN             0x28
#define VK_ENTER            0x0D    //13
#define VK_ESC              0x1B    //27
#define VK_PROGRAM          0x0E
#define VK_LONG_LEFT        0x0F
#define VK_LONG_UP          0x10
#define VK_LONG_RIGHT       0x11
#define VK_LONG_DOWN        0x12
#define VK_LONG_ENTER       0x13
#define VK_LONG_ESC         0x14
#define VK_LONG_PROGRAM     0x15
#define VK_GRP_ENTER_LEFT   0x16
#define VK_GRP_ENTER_UP     0x17
#define VK_GRP_ENTER_RIGHT  0x18
#define VK_GRP_ENTER_DOWN   0x19

#endif
