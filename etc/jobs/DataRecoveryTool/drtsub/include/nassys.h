#ifndef __nassys_h__
#define __nassys_h__
/* メインヘッダ:  */

 /* __nassys_h__ */              /* 直接書込データ:  */
 /* このヘッダファイルは構造体ツールを用いて作成されています。  */              /* 直接書込データ:  */
 /* 直接、修正することは行わないで下さい。                    */              /* 直接書込データ:  */
                                /* 直接書込データ:  */
 /* WAIS START */               /* 直接書込データ:  */
                                /* 直接書込データ:  */
 #include "sk_cfg.h"              /* 直接書込データ:  */
                                /* 直接書込データ:  */
 #if defined(_SK_OS9_)||defined(_SK_OS9000_)              /* 直接書込データ:  */
 #include <types.h>              /* 直接書込データ:  */
#elif defined(_SK_LINUX_)
#include <sys/types.h>
 #else                          /* 直接書込データ:  */
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
 #endif                         /* 直接書込データ:  */

/* 日時情報:  */
typedef struct _date_time {
    short year;                 /* 年: 年(0~9999)  */
    char month;                 /* 月: 月(1~12)  */
    char date;                  /* 日: 日(1~31)  */
    char week;                  /* 曜日: 曜日(0~6(日~土))  */
    char hour;                  /* 時: 時(0~23)  */
    char minute;                /* 分: 分(0~59)  */
    char second;                /* 秒: 秒(0~59)  */
     /* WAIS END */              /* 直接書込データ:  */
} date_time_t;

#endif                          /* __nassys_h__ */
