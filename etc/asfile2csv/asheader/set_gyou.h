/* WAIS START */
#ifndef __set_gyou_h__
#define __set_gyou_h__          /* ‹Æ–±İ’è:  */

 /*    ‰Šúİ’èÌ§²Ù                                   */              /* ’¼Ú‘ƒf[ƒ^:  */
 /*    Version   Date      Programmer    Memo         */              /* ’¼Ú‘ƒf[ƒ^:  */
 /*    00000.0   97.04.07      y.I       V‹Kì¬     */              /* ’¼Ú‘ƒf[ƒ^:  */
 /*    \‘¢‘ÌÂ°Ù‚É‚ÄÄì¬                            */              /* ’¼Ú‘ƒf[ƒ^:  */
 /*  1997/10/6 11:29 osako            */           /* ’¼Ú‘ƒf[ƒ^:  */
#include "nassys.h"
#define L_SET_GYOU

/* ˆÃØ”Ô†:  */
typedef struct _fno1100 {
    unsigned long ansyou_no;    /* ˆÃØ”Ô†:  */
    unsigned long dmy1;         /* —\”õ:  */
} fno1100_t;


/* ‹x“úİ’è¥ÌÛ±”Ô†:  */
typedef struct _holidayfloor {
    unsigned char flag;         /* g—p¥–¢g—p:  */
    unsigned char floor;        /* ÌÛ±No:  */
    unsigned char date[2];      /* Œ“ú:  */
} holidayfloor_t;


/* ŠJ•Â“Xİ’è:  */
typedef struct _fno1101 {
// 2002/10/25 (“X•Ü•Ê‹x“ú‘Î‰)’Ç‰ÁC³ Fukuyama -->
    unsigned char open_time[2]; /* ŠJ“X:  */
    unsigned char close_time[2];/* •Â“X:  */
    unsigned char shut_time[2]; /* ’÷Ø:  */
    unsigned char kioku_time[2];/* ‹L‰¯:  */
    unsigned char clear_time[2];/* (¸Ø±):  */
    unsigned char getstop_time[2];/* (æ’â~):  */
    unsigned char revcany_time[2];/* ‘ä—\–ñæÁ—\:  */
    unsigned char revcan_time[2];/* ‘ä—\–ñæÁ:  */
    unsigned char open_time2[2]; /* ŠJ“X(2“X•Ü–Ú):  */
    unsigned char close_time2[2];/* •Â“X(2“X•Ü–Ú):  */
    unsigned char shut_time2[2]; /* ’÷Ø(2“X•Ü–Ú):  */
    unsigned char kioku_time2[2];/* ‹L‰¯(2“X•Ü–Ú):  */
    unsigned char clear_time2[2];/* (¸Ø±)(2“X•Ü–Ú):  */
    unsigned char getstop_time2[2];/* (æ’â~)(2“X•Ü–Ú):  */
    unsigned char revcany_time2[2];/* ‘ä—\–ñæÁ—\(2“X•Ü–Ú):  */
    unsigned char revcan_time2[2];/* ‘ä—\–ñæÁ(2“X•Ü–Ú):  */
//    unsigned char yobi1[16];    /* *:  */
//--> 2002/10/25 (“X•Ü•Ê‹x“ú‘Î‰)’Ç‰ÁC³ Fukuyama
    holidayfloor_t holiday[8];/* ‹x“úİ’è¥ÌÛ±”Ô†: (ÌÛ±–ˆ[0]`[7]) ÌÛ±=0:‘SÌÛ±  */
} fno1101_t;


/* ‹àŒÉ“_ŒŸİ’è:  */
typedef struct _fno1102 {
    unsigned short kinko_tenken;/* ‹àŒÉ“_ŒŸ’l: (»İÄŞŒv - ‹àŒÉŒv)  */
    unsigned short kinko_tentimer;/* ‹àŒÉ“_ŒŸÀ²Ï:  */
} fno1102_t;


/* ‘S“X’â~İ’è:  */
typedef struct _fno1103 {
    unsigned short sanddan_stopfg;/* »İÄŞ’fü: ’â~Ì×¸Ş  */
        /*  (=1)‹–‰Â¥(=0)’â~       */
        /*  (b0)ŒŸ’m                */
        /*  (b1)‹q‰¹º              */
        /*  (b2)“X‰¹º              */
        /*  (b3)•\¦                */
        /*  (b4)ˆóš                */
        /*  (b5)¶Ò×                 */
        /*  (b6)¶Ò×—Dæ             */
        /*  ¥c                     */
        /*  (b15)’fü               */
    unsigned short sandijo_stopfg;/* »İÄŞˆÙí: ’â~Ì×¸Ş  */
    unsigned short fusdan_stopfg;/* •s³’fü: ’â~Ì×¸Ş  */
    unsigned short daifus_stopfg;/* ‘ä•s³: ’â~Ì×¸Ş  */
    unsigned short daiijo_stopfg;/* ‘äˆÙí: ’â~Ì×¸Ş  */
    unsigned short daiten_stopfg;/* ‘ä“_ŒŸ: ’â~Ì×¸Ş  */
    unsigned short kinkoten_stopfg;/* ‹àŒÉ“_ŒŸ: ’â~Ì×¸Ş  */
    unsigned short tanijo_stopfg;/* ’[––ˆÙí: ’â~Ì×¸Ş  */
    unsigned short revten_stopfg;/* ‘ä—\–ñ“_ŒŸ: ’â~Ì×¸Ş  */
    unsigned short risekiten_stopfg;/* —£È“_ŒŸ: ’â~Ì×¸Ş  */
    unsigned short shokuten_stopfg;/* H–“_ŒŸ: ’â~Ì×¸Ş  */
    unsigned short zkeidan_stopfg;/* ‘S“XŒv”’fü: ’â~Ì×¸Ş  */
    unsigned short zerror_stopfg;/* ‘S“X´×°“ü—Í: ’â~Ì×¸Ş  */
    unsigned short zdoor_stopfg;/* ‘S“XÄŞ±“ü—Í: ’â~Ì×¸Ş  */
    unsigned short zdoorten_stopfg;/* ‘S“XÄŞ±“_ŒŸ: ’â~Ì×¸Ş  */
#if 1		//2002.04.05 koketsu for WinSousa
    unsigned short daicall_stopfg;/* ‘äŒÄo: ’â~Ì×¸Ş  */
    unsigned short daidoor_stopfg;/* ‘äƒhƒA: ’â~Ì×¸Ş  */
    unsigned short yobi[15];     /* —\”õ:  */
#else
    unsigned char yobi[34];     /* —\”õ:  */
#endif
} fno1103_t;


/* ‹@í’â~İ’è:  */
typedef struct _fno1104 {
    unsigned short fus1_stopfg; /* •s³“ü—Í: ’â~Ì×¸Ş  */
        /*  (=1)‹–‰Â¥(=0)’â~       */
        /*  (b0)ŒŸ’m                */
        /*  (b1)‹q‰¹º              */
        /*  (b2)“X‰¹º              */
        /*  (b3)•\¦                */
        /*  (b4)ˆóš                */
        /*  (b5)¶Ò×                 */
        /*  (b6)¶Ò×—Dæ             */
        /*  ¥c                     */
        /*  (b15)’fü               */
    unsigned short fus2_stopfg; /* •s³2“ü—Í: ’â~Ì×¸Ş  */
    unsigned short door1_stopfg;/* ÄŞ±“ü—Í: ’â~Ì×¸Ş  */
    unsigned short door2_stopfg;/* ÄŞ±2“ü—Í: ’â~Ì×¸Ş  */
    unsigned short call_stopfg; /* ŒÄo: ’â~Ì×¸Ş  */
    unsigned short uchi_stopfg; /* ‘Å~: ’â~Ì×¸Ş  */
    unsigned short kaihou_stopfg;/* ŠJ•ú: ’â~Ì×¸Ş  */
    unsigned short toku_stopfg; /* “ÁÜ: ’â~Ì×¸Ş  */
    unsigned short ijo_stopfg;  /* ˆÙí’â~: ’â~Ì×¸Ş  */
    unsigned short safijo_stopfg;/* ¾°ÌˆÙí: ’â~Ì×¸Ş  */
    unsigned short doorijo_stopfg;/* ÄŞ±ˆÙí: ’â~Ì×¸Ş  */
    unsigned short tokuijo_stopfg;/* “ÁÜˆÙí: ’â~Ì×¸Ş  */
    unsigned short staijo_stopfg;/* ½À°ÄˆÙí: ’â~Ì×¸Ş  */
    unsigned short outten_stopfg;/* ±³Ä“_ŒŸ: ’â~Ì×¸Ş  */
    unsigned short saften_stopfg;/* ¾°Ì¥•â‹‹“_ŒŸ: ’â~Ì×¸Ş  */
    unsigned short staten_stopfg;/* ½À°Ä“_ŒŸ: ’â~Ì×¸Ş  */
#if 1			// 2002.04.05 Upd koketsu for WinSousa
    unsigned short genkin_stopfg;	/* Œ»‹à’â~Ì×¸Ş  */
    unsigned short card_stopfg;		/* ƒJ[ƒh’â~Ì×¸Ş  */
    unsigned short replay_stopfg;	/* ƒŠƒvƒŒƒC’â~Ì×¸Ş  */
    unsigned short kenbai_stopfg;	/* Œ””„‹@’â~Ì×¸Ş  */
    unsigned char yobi[24];     	/* —\”õ:  */
#else
    unsigned short dmy1;        /* —\”õ:  */
    unsigned short dmy2;        /* —\”õ:  */
    unsigned char yobi[28];     /* —\”õ:  */
#endif
} fno1104_t;


/* ‘S“X‰^—pİ’è:  */
typedef struct _fno1105 {
    unsigned short kado_timersec;/* ‰Ò“®À²Ï (*•b):  */
    unsigned short kado_timermin;/* ‰Ò“®À²Ï (*•b):  */
    unsigned short shoku_timer;/* H–À²Ï (•b): (0)ŒŸ’m–³‚µ¥(1-9999•b)İ’è‚Í•b’PˆÊ  */
    unsigned short riseki_timer;/* —£ÈÀ²Ï (•b): (0)ŒŸ’m–³‚µ¥(1-9999•b)İ’è‚Í•b’PˆÊ  */
    unsigned short revcan_timer;/* —\–ñæÁÀ²Ï: (0)ŒŸ’m–³‚µ¥(1-9999•b)İ’è‚Í•b’PˆÊ  */
    unsigned short fuscan_timer;/* •s³‰ğœÀ²Ï: (0)ŒŸ’m–³‚µ¥(1-9999•b)İ’è‚Í•b’PˆÊ  */
    unsigned short callten_timer;/* ŒÄo“_ŒŸÀ²Ï: (0)ŒŸ’m–³‚µ¥(1-9999•b)  */
    unsigned short kai_hikado_timer;/* ‰ïˆõ”ñ‰Ò“®“_ŒŸÀ²Ï: (0)ŒŸ’m–³‚µ¥(1-9999•b)  */
    unsigned short uriijo_sagaku;/* ”„ãŒÂ”ˆÙí: ·Šz’l (‹àŠz-ŒÂ”)  */
    unsigned short uriijo_kingaku;/* ”„ãˆÙí  ‹àŠz’l: ‹àŠz’l  */
    unsigned short uriijo_timer;/* ”„ãˆÙí  À²Ï’l: À²Ï’l  */
    unsigned short t_uriijo_kingaku;/* “ÁÜ1’†”„ãˆÙí: ‹àŠz’l  */
    unsigned short ukomi;       /* ‘Å‹Ê: ‘Å‹Ê  */
    unsigned short sg_doorten_timer;/* ‘S“XÄŞ±“_ŒŸÀ²Ï: (0-255)•b’PˆÊ  */
    unsigned short sg_errten_timer;/* “‡ŠO´×°‰ğœÀ²Ï: (0)ŒŸ’m–³‚µ¥(1-9999•b)İ’è‚Í•b’PˆÊ  */
    unsigned short kai_kado_timer;/* ‰ïˆõ‰Ò“®À²Ï: (0:60•b)(1-999•b)  */
    unsigned short kai_card_timer;/* ‰ïˆõ¶°ÄŞOUTÀ²Ï: (0:60•b)(1-999•b)  */
    unsigned char yobi2[94];    /* —\”õ:  */
} fno1105_t;


/* ‹@í‰^—pİ’è:  */
typedef struct _fno1106 {
    unsigned char pas;          /* í•Ê:  */
    unsigned char type;         /* À²Ìß:  */
    unsigned short uchi;        /* ‘Å~: ‘Å~’l  */
    unsigned short uchifg;      /* ‘Å~: ğŒÌ×¸Ş  */
        /*  (b0=1)©“®‘Å~‘ä        */
        /*  (b1=1)T2’†‘Å~’â~      */
    unsigned short kaihou_timer;/* ŠJ•ú: ŠJ•úÀ²Ï (•b)  */
        /*  (=0)©“®ŠJ•ú‚µ‚È‚¢      */
        /*  (=1)‘¦ŠJ•ú              */
        /*  (2-999•b)ŠJ•úÀ²Ï        */
    unsigned short dmy1;        /* —\”õ:  */
    unsigned short tohosei;     /* “ÁÜ: T0•â³’l  */
    unsigned short tokufg;      /* “ÁÜ: ğŒÌ×¸Ş  */
        /*  (b0=1)“ÁÜÍŞ°½          */
        /*  (b1=1)T2’†‰¹º’â~      */
        /*  (b2=1)T2’†“ÁÜo—Í      */
    unsigned short safijo_saf;  /* ¾°ÌˆÙí: ¾°Ì’l  */
    unsigned short safijo_out;  /* ¾°ÌˆÙí: ±³Ä’l  */
    unsigned short safijo_timer;/* ¾°ÌˆÙí: À²Ï’l  */
    unsigned short tcntijo_bo;  /* “ÁÜ‰ñ”ˆÙí: BO  */
    unsigned short tcntijo_t1;  /* “ÁÜ‰ñ”ˆÙí: T1‰ñ”  */
    unsigned short tdamaijo_tymax;/* “ÁÜo‹ÊˆÙí: Tymax’l  */
    unsigned short staijo_start;/* ½À°ÄˆÙí: ½À°Ä’l  */
    unsigned short staijo_out;  /* ½À°ÄˆÙí: ±³Ä’l  */
    unsigned short staijo_timer;/* ½À°ÄˆÙí: À²Ï’l  */
    unsigned short outten_saf;  /* ±³Ä“_ŒŸ: ¾°Ì’l  */
    unsigned short outten_start;/* ±³Ä“_ŒŸ: ½À°Ä’l  */
    unsigned short saften_out;  /* ¾°Ì“_ŒŸ: ±³Ä’l  */
    unsigned short saften_start;/* ¾°Ì“_ŒŸ: ½À°Ä’l  */
    unsigned short saften_t1out;/* ¾°Ì“_ŒŸ(T1’†): T1±³Ä’l  */
    unsigned short tdamaten_tymin;/* “ÁÜo‹Ê“_ŒŸ: Tymin’l  */
    unsigned short staten_saf;  /* ½À°Ä“_ŒŸ: ¾°Ì’l  */
    unsigned short staten_out;  /* ½À°Ä“_ŒŸ: ±³Ä’l  */
#if 0				// 2002.04.05 Upd. koketsu for Win‘€ì
    unsigned short dmy2;        /* —\”õ:  */
    unsigned short dmy3;        /* —\”õ:  */
    unsigned char yobi1[12];    /* —\”õ:  */
#else
    unsigned char  stslump_ss1; /* ½À°Ä½×İÌß s/s1‘I‘ğ */
    unsigned char  stslump_so; 	/* ½À°Ä½×İÌß so’l 	*/
    unsigned short stslump_bo; /* ½À°Ä½×İÌßBOŒŸõ’l */
    unsigned short stslump_sbst; /* ½À°Ä½×İÌß½À°Ä•ª•z”ÍˆÍŠJn’l*/
    unsigned char  stslump_sbkn; /* ½À°Ä½×İÌß½À°Ä•ª•z”ÍˆÍŒŸõ’l*/
    unsigned char  stslump_sbbn; /* ½À°Ä½×İÌß½À°Ä•ª•z”ÍˆÍ•ªŠ„’l*/
    unsigned short next_uti; 	/* Ÿ‰ñ‘Å~’l*/
// 2002/09/18 ’Ç‰ÁC³ Fukuyama -->
	unsigned char  outstart_out; /* ±³Ä’è”½À°ÄŠÖ˜A±³Ä’è”*/
	unsigned char  stslump_sharai; /* ±³Ä’è”½À°ÄŠÖ˜A½À°Ä•¥o”*/// ƒoƒEƒ“ƒ_ƒŠC³ (¬“‡)
    unsigned short outstart_start; /* ±³Ä’è”½À°ÄŠÖ˜A½À°Ä’è”*/
    unsigned char yobi1[2];    /* —\”õ:  */
// --> 2002/09/18 ’Ç‰ÁC³ Fukuyama
#endif
    unsigned char yobi2[64];    /* —\”õ:  */
} fno1106_t;


/* D2İ’è:  */
typedef struct _fno1107 {
    unsigned char setei1;       /* İ’èÃŞ°À1:  */
        /*  í•Ê         b7   (0)P‘ä¥(1)AS‘ä                       */
        /*  ŒÄoSW“ü—Í   b6   (0)µÙÀÈ°ÄSW¥(1)Ó°ÒİÀØSW              */
        /*  ½À°ÄŒŸ’m     b5   (0)ONŒŸ’m¥(1)OFFŒŸ’m                 */
        /*  S2¥S3ì¬    b4   (0)–³:’Êí½À°Ä“ü—Í¥(1)—L:©“®”»•Ê    */
    unsigned char setei2;       /* İ’èÃŞ°À2:  */
        /*  ½À°ÄÀ²Ï(•b)     0.2•b’PˆÊ  (½À°ÄOFFŒŸ’m1/10•\¦)    */
    unsigned char setei3;       /* İ’èÃŞ°À3:  */
        /*  ”»•ÊÀ²Ï(•b)     0.02•b’PˆÊ    */
    unsigned char setei4;       /* İ’èÃŞ°À4:  */
        /*  T1À²Ï        b5-0     (0-31)•b’PˆÊ     */
    unsigned char setei5;       /* İ’èÃŞ°À5:  */
        /*  T1’x‰„À²Ï    b0-7     (0-255)•b’PˆÊ    */
    unsigned char setei6;       /* İ’èÃŞ°À6:  */
        /*  T2À²Ï        b5-0     (0-31)•b’PˆÊ     */
    unsigned char setei7;       /* İ’èÃŞ°À7:  */
        /*  T2’x‰„À²Ï    b0-7     (0-255)•b’PˆÊ    */
    unsigned char setei8;       /* İ’èÃŞ°À8:  */
        /*  ÄŞ±ˆÙíÀ²Ï   b0123:uŠÔÄŞ±µ°Ìßİ    (0)ŒŸ’m‚È‚µ¥(1-15)0.1•b’PˆÊ     */
        /*  ÄŞ±“ÁÜÀ²Ï   b4567:ÄŞ±µ°Ìßİ“ÁÜ    (0)ŒŸ’m‚È‚µ¥(1-15)1•b’PˆÊ       */
    unsigned char setei9;       /* İ’èÃŞ°À9:  */
        /*  ÄŞ±“_ŒŸÀ²Ï   b0-7     (0)ŒŸ’m‚È‚µ¥(1-225)1•b’PˆÊ       */
    unsigned char setei10;      /* İ’èÃŞ°À10:  */
        /*  •â‹‹“_ŒŸÀ²Ï  b0-7     (0)ŒŸ’m‚È‚µ¥(1-225)1•b’PˆÊ       */
    unsigned char setei11;      /* İ’èÃŞ°À11:  */
    unsigned char setei12;      /* İ’èÃŞ°À12:  */
    unsigned char setei13;      /* İ’èÃŞ°À13:  */
    unsigned char setei14;      /* İ’èÃŞ°À14:  */
        /*  “ü—Í‘I‘ğ1    b0123    (0)ÄŞ±1¥(1)ÄŞ±1¥(2)•s³1¥(3)ŒÄo1¥(4)ÄŞ±2¥(5)•s³2¥(6)ŒÄo2¥(7)•â‹‹     */
        /*  “ü—Í‘I‘ğ2    b4567    (0)•s³1¥(1)ÄŞ±1¥(2)•s³1¥(3)ŒÄo1¥(4)ÄŞ±2¥(5)•s³2¥(6)ŒÄo2¥(7)•â‹‹    */
    unsigned char setei15;      /* İ’èÃŞ°À15:  */
        /*  “ü—Í‘I‘ğ3    b0123    (0)ŒÄo1¥(1)ÄŞ±1¥(2)•s³1¥(3)ŒÄo1¥(4)ÄŞ±2¥(5)•s³2¥(6)ŒÄo2¥(7)•â‹‹    */
        /*  *                                                                                             */
    unsigned char setei16;      /* İ’èÃŞ°À16:  */
        /*  o—Í1İ’è    b0123    (=0)‘Å~’†¥(=1)T1’†¥(=2)T1+T2’†¥(3)T2’†    */
        /*  o—Í2İ’è    b4567    (=0)‘Å~’†¥(=1)T1’†¥(=2)T1+T2’†¥(3)T2’†    */
} fno1107_t;

typedef struct _fno1108_tenpo_t {
	unsigned char	t_holiday;			/* 00:‰c‹Æ“úFLAG (bit0=1)ŠJ“X“ú,(bit0=0)‹x“X“ú */
	unsigned char	dmy[3];				/* *:  */
	unsigned char	open_time[2];		/* ŠJ“X:  */
	unsigned char	close_time[2];		/* •Â“X:  */
	unsigned char	shut_time[2];		/* ’÷Ø:  */
	unsigned char	kioku_time[2];		/* ‹L‰¯:  */
	unsigned char	clear_time[2];		/* (¸Ø±):  */
	unsigned char	getstop_time[2];	/* (æ’â~):  */
	unsigned char	revcany_time[2];	/* ‘ä—\–ñæÁ—\:  */
	unsigned char	revcan_time[2];		/* ‘ä—\–ñæÁ:  */
} fno1108_tenpo_t;

/* “X•Ü•Ê•Ï‘¥‰c‹Æİ’è:  */
typedef struct _fno1108 {
	unsigned long	eigyo_pw_tim;		/* ‰c‹Æ“ú•t: ‰c‹Æ‚ÌŒ“ú ”N¥Œ¥“ú¥¥•ª¥•b = bit7¥4¥5¥5¥6¥5  1910+”N 0=“ú—j  */
	unsigned char	dmy[12];				/* *:  */
	fno1108_tenpo_t tenpo[2];
} fno1108_t;


/* ŠJ•Â“X“®ìƒe[ƒuƒ‹ */
typedef fno1108_tenpo_t fno4824_t;

#if 0
/* “X•Ü–ˆİ’è */
typedef struct _fno1108_d1 {
	unsigned char	t_holiday;		/* ‰c‹Æ“úƒtƒ‰ƒO (bit7=1)ŠJ“X“ú,(bit7=0)‹x“X“ú */
	unsigned char	t_dmy2;
	unsigned char	t_clear_hm[2];		/* («—ˆ)ƒNƒŠƒA([0] [1]•ª) */
	unsigned char	t_open_hm[2];		/* ŠJ“X([0] [1]•ª) */
	unsigned char	t_close_hm[2];		/* •Â“X([0] [1]•ª) */
	unsigned char	t_sime_hm[2];		/* ’÷Ø([0] [1]•ª) */
	unsigned char	t_kioku_hm[2];		/* ©“®‹L‰¯([0] [1]•ª) */
	unsigned char	t_tstop_hm[2];		/* («—ˆ)æ’â~([0] [1]•ª) */
	unsigned char	t_opmae_hm[2];		/* ŠJ“X‘OŒx([0] [1]•ª) */
	unsigned char	t_dmy3[8];
} fno1108_d1_t;

typedef struct _fno1108_d2 {
	unsigned char	t_holiday;		/* ‰c‹Æ“úƒtƒ‰ƒO (bit7=1)ŠJ“X“ú,(bit7=0)‹x“X“ú */
	unsigned char	t_dmy2;		
	unsigned short	t_clear_hm;		/* («—ˆ)ƒNƒŠƒA(bit15-8= bit7-0=•ª) */
	unsigned short	t_open_hm;		/* ŠJ“X(bit15-8= bit7-0=•ª) */
	unsigned short	t_close_hm;		/* •Â“X(bit15-8= bit7-0=•ª) */
	unsigned short	t_sime_hm;		/* ’÷Ø(bit15-8= bit7-0=•ª) */
	unsigned short	t_kioku_hm;		/* ©“®‹L‰¯(bit15-8= bit7-0=•ª) */
	unsigned short	t_tstop_hm;		/* («—ˆ)æ’â~(bit15-8= bit7-0=•ª) */
	unsigned short	t_opmae_hm;		/* ŠJ“X‘OŒx(bit15-8= bit7-0=•ª) */
	unsigned char	t_dmy3[8];
} fno1108_d2_t;

typedef union _fno1108_tenpo {
	fno1108_d1_t a;
	fno1108_d2_t b;
} fno1108_tenpo_t;

/* ƒJƒŒƒ“ƒ_[İ’è */
typedef struct _fno1108 {
	unsigned char	month;			/* Œ */
	unsigned char	day;			/* “ú */
	unsigned char	dmy[6];
	fno1108_tenpo_t	f1108_tenpo[8];		/* “X•Ü•Ê‚WƒOƒ‹[ƒv */
} fno1108_t;
#endif

typedef fno1100_t FSET_GYOU_ANSYOU;
typedef fno1101_t FSET_GYOU_OPCL;
typedef fno1102_t FSET_GYOU_KINKO;
typedef fno1103_t FSET_GYOU_ZENTEI;
typedef fno1104_t FSET_GYOU_KSHUTEI;
typedef fno1105_t FSET_GYOU_ZEN;
typedef fno1106_t FSET_GYOU_KSHU;
typedef fno1107_t FSET_GYOU_D2;
typedef fno1108_t FSET_GYOU_HENSOKU; //©2002/10/29 (“X•Ü•Ê•Ï‘¥‰c‹Æ‘Î‰)’Ç‰Á Fukuyama
#endif                          /* __set_gyou_h__ */
/* WAIS END */
