/* WAIS START */
/************************************************************************/
/*	�V�X�e��������������						*/
/*	Version	Date	 Programmer	Memo				*/
/*	00000.0	97.05.21	y.i		�V�K�쐬			*/
/************************************************************************/
#define	L_APL_EXT

#ifdef	L_SET_SYS
extern FSET_SYS_TABLE *fset_sys;	/* ���ѐݒ�ر�߲��ð���	*/
#endif

#ifdef	L_SET_INIT
extern FSET_INIT_TABLE *fset_init;	/* �����ݒ�ر�߲��ð���	*/
#endif

#ifdef	L_INF_FILE
extern IDX_TABLE *idx_tbl;		/* INDEX�ر�߲��ð���		*/
extern INF_TABLE *inf_tbl;		/* ���ݏ���߲��ð���		*/
#endif

#ifdef __un_as_h__
extern	UNYOU_TABLE *un_tbl;		/* �^�p����߲��ð���		*/
#endif

#ifdef	L_DAT_FILE
extern FDATA_TABLE *fdata_tbl;		/* �ް��ر�߲��ð���		*/
#endif

#ifdef	L_REK_FILE
extern REKI_TABLE *reki_tbl;		/* ����ر�߲��ð���		*/
#endif

extern ushort	fileno_tbl[MAX_DFILES][9];/* ̧�ٔԍ��i�[�ر�߲��ð���	*/
/* WAIS END */

