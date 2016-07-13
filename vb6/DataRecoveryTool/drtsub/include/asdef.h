#ifndef __asdef_h__
#define __asdef_h__
 /* $Id: asdef.h,v 1.5 2002/08/01 04:40:21 nisimura Exp $ */               /* ’¼Ú‘žƒf[ƒ^:  */
                                /* ’¼Ú‘žƒf[ƒ^:  */
 /*  1998/9/18 18:05 osako            */           /* ’¼Ú‘žƒf[ƒ^:  */
 /* WAIS START */               /* ’¼Ú‘žƒf[ƒ^:  */
#define ASDEF
#ifdef _KDAI_MAX_1536
#define TNSU 1536               /* TN”:  */
#define DAISU 1536              /* ‘ä”:  */
#else
#define TNSU 1024               /* TN”:  */
#define DAISU 1024              /* ‘ä”:  */
#endif
#define ZENTENSU 256            /* ‘S“X”:  */
#define SIMASU 96               /* “‡”:  */
#define FLOORSU 8
#define KINKO 64                /* ‹àŒÉ”:  */
#define HANBAIKISU 256          /* ”Ì”„‹@”:  */
#define TYPE 16                 /* ƒ^ƒCƒv:  */
#define TYPEMEICHO 46           /* ƒ^ƒCƒv–¼’·:  */
#define KISHU 96                /* ‹@Ží”:  */
#define KISHUMEICHO 28          /* ‹@Ží–¼’·:  */
#define PORT_USER 128

#define SETSUZOKU ZENTENSU      /* ‘S“XÚ‘±Ý’èŒ”:  */
#define SHUBETSU 8              /* Ží•Ê”:  */
#define ANSHO 8
#define TVCH_SU 12              /* TV chÝ’èŒ”:  */
#define REMOTE 32
 /* WAIS END */                 /* ’¼Ú‘žƒf[ƒ^:  */
#endif                          /* __asdef_h__ */
/*
 * $Log: asdef.h,v $
 * Revision 1.5  2002/08/01 04:40:21  nisimura
 * struct ’è‹`‚ªŒÃ‚©‚Á‚½‚½‚ß nas2002(20020731Žž“_) ‚Ì‚à‚Ì‚ðƒRƒs[‚µ‚½B
 *
 * Revision 1.4  2002/06/17 06:43:03  nisimura
 * SHUBETSU ‚ð‚R‚©‚ç‚W‚Ö•ÏXBi‚Q“X•Ü‚WŽí•Ê‘Î‰žj
 *
 * Revision 1.3  2001/02/19 02:31:18  kojima
 * no message
 *
 * Revision 1.6  2000/07/10 15:36:54  ohno-t
 * ‚P‚T‚R‚U‘ä‘Î‰ž‘€ì•”³Ž®“o˜^
 *
 * Revision 1.5  2000/07/06 05:24:02  osako
 * merge 1536 sourse files
 *
 * $Log: asdef.h,v $
 * Revision 1.5  2002/08/01 04:40:21  nisimura
 * struct ’è‹`‚ªŒÃ‚©‚Á‚½‚½‚ß nas2002(20020731Žž“_) ‚Ì‚à‚Ì‚ðƒRƒs[‚µ‚½B
 *
 * Revision 1.4  2002/06/17 06:43:03  nisimura
 * SHUBETSU ‚ð‚R‚©‚ç‚W‚Ö•ÏXBi‚Q“X•Ü‚WŽí•Ê‘Î‰žj
 *
 * Revision 1.3  2001/02/19 02:31:18  kojima
 * no message
 *
 * Revision 1.6  2000/07/10 15:36:54  ohno-t
 * ‚P‚T‚R‚U‘ä‘Î‰ž‘€ì•”³Ž®“o˜^
 *
 * Revision 1.4  2000/02/23 04:08:35  matsuo
 * no message
 *
 * Revision 1.3  2000/01/11 05:15:08  ohno-t
 * ƒRƒƒ“ƒgC³
 *
 * Revision 1.2  1999/12/29 15:36:22  sakurada
 * remove hankaku-kana, add keyword, fix indenting
 *
 * Revision 1.1.1.1  1999/12/26 05:57:10  sakurada
 * 1st try to import from VSS
 *
 * 2     99/10/01 9:41 Ohno-t
 *
 * 1     99/09/29 10:15a Sakurada
 * INCLUDE -> include
 * Revision 1.1  1999/09/29 01:13:28  sakurada
 * change INCLUDE -> include
 */
