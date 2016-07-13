/* WAIS START */
#ifndef __ordercom_h__
#define __ordercom_h__

#define	OD_SHIMA_SUU		128
#define	OD_HINMOKU_SUU		64
#define	OD_SHINAGIRE_SUU	128

/* オーダー・島グループ設定 */
typedef struct _fno5016 {
	unsigned short	sima;		/* 島番号 */
	unsigned char	grp_num;	/* グループ番号 */
	unsigned char	pad;
} fno5016_t;	/* GRP_SIMA */

/* オーダー・品目設定 */
typedef struct _fno5018 {
	unsigned char	namae[24];		/* メニューの品名: 全角12文字  */
	unsigned short	tanka_genkin;	/* 単価(現金金額): 0～65535  */
	unsigned short	tanka_tama;		/* 単価(玉数): 0～65535  */
	unsigned short	tanka_coin;		/* 単価(コイン数): 0～65535  */
	unsigned char	reserve[2];		/* 予備:  */
} fno5018_t;	/* d_2450_hinmoku_t */

/* オーダー・品切れ設定 */
typedef struct _fno5019 {
	unsigned char	sinagire;		/* 品切れフラグ: ($00)在庫あり ($10)品切れ*/
	unsigned char	reserve;		/* 予備:  */
	unsigned short	zaiko_suu;		/* 在庫数: 0～65535  */
} fno5019_t;	/* d_2430_hinmoku_t */

#endif	/*	__ordercom_h__	*/
/* WAIS END */
