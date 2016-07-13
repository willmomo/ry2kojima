/* WAIS START */
/************************************************************************/
/*	システムｅｘｔｅｒｎ						*/
/*	Version	Date	 Programmer	Memo				*/
/*	00000.0	97.05.21	y.i		新規作成			*/
/************************************************************************/
#define	L_APL_EXT

#ifdef	L_SET_SYS
extern FSET_SYS_TABLE *fset_sys;	/* ｼｽﾃﾑ設定ｴﾘｱﾎﾟｲﾝﾀﾃｰﾌﾞﾙ	*/
#endif

#ifdef	L_SET_INIT
extern FSET_INIT_TABLE *fset_init;	/* 初期設定ｴﾘｱﾎﾟｲﾝﾀﾃｰﾌﾞﾙ	*/
#endif

#ifdef	L_INF_FILE
extern IDX_TABLE *idx_tbl;		/* INDEXｴﾘｱﾎﾟｲﾝﾀﾃｰﾌﾞﾙ		*/
extern INF_TABLE *inf_tbl;		/* 現在情報ﾎﾟｲﾝﾀﾃｰﾌﾞﾙ		*/
#endif

#ifdef __un_as_h__
extern	UNYOU_TABLE *un_tbl;		/* 運用情報ﾎﾟｲﾝﾀﾃｰﾌﾞﾙ		*/
#endif

#ifdef	L_DAT_FILE
extern FDATA_TABLE *fdata_tbl;		/* ﾃﾞｰﾀｴﾘｱﾎﾟｲﾝﾀﾃｰﾌﾞﾙ		*/
#endif

#ifdef	L_REK_FILE
extern REKI_TABLE *reki_tbl;		/* 履歴ｴﾘｱﾎﾟｲﾝﾀﾃｰﾌﾞﾙ		*/
#endif

extern ushort	fileno_tbl[MAX_DFILES][9];/* ﾌｧｲﾙ番号格納ｴﾘｱﾎﾟｲﾝﾀﾃｰﾌﾞﾙ	*/
/* WAIS END */

