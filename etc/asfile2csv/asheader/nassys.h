#ifndef __nassys_h__
#define __nassys_h__
/* ���C���w�b�_:  */

 /* __nassys_h__ */              /* ���ڏ����f�[�^:  */
 /* ���̃w�b�_�t�@�C���͍\���̃c�[����p���č쐬����Ă��܂��B  */              /* ���ڏ����f�[�^:  */
 /* ���ځA�C�����邱�Ƃ͍s��Ȃ��ŉ������B                    */              /* ���ڏ����f�[�^:  */
                                /* ���ڏ����f�[�^:  */
 /* WAIS START */               /* ���ڏ����f�[�^:  */
                                /* ���ڏ����f�[�^:  */
 #include "sk_cfg.h"              /* ���ڏ����f�[�^:  */
                                /* ���ڏ����f�[�^:  */
 #if defined(_SK_OS9_)||defined(_SK_OS9000_)              /* ���ڏ����f�[�^:  */
 #include <types.h>              /* ���ڏ����f�[�^:  */
#elif defined(_SK_LINUX_)
#include <sys/types.h>
 #else                          /* ���ڏ����f�[�^:  */
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
 #endif                         /* ���ڏ����f�[�^:  */

/* �������:  */
typedef struct _date_time {
    short year;                 /* �N: �N(0~9999)  */
    char month;                 /* ��: ��(1~12)  */
    char date;                  /* ��: ��(1~31)  */
    char week;                  /* �j��: �j��(0~6(��~�y))  */
    char hour;                  /* ��: ��(0~23)  */
    char minute;                /* ��: ��(0~59)  */
    char second;                /* �b: �b(0~59)  */
     /* WAIS END */              /* ���ڏ����f�[�^:  */
} date_time_t;

#endif                          /* __nassys_h__ */
