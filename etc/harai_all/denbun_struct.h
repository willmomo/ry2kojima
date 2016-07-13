typedef struct _csv_read{
	char			psend[48];		/* POS送信バッファ */
	char			hsend[48];		/* 払出機送信バッファ */
}csv_read;			/* CSVファイルから読み込むデータ */

typedef struct _cr_recv {
	char			seq;				/* シーケンス番号 */
	char			cmd;				/* コマンド */
	char			flag;				/* フラグ */
	unsigned long	kaiin_no[2];		/* 会員番号 */
	unsigned short	ansyo;				/* 暗証番号 */
	unsigned long	chotama;			/* 総貯玉数 */
	char			harai[6];				/* 払出可能度数 */
	unsigned long	point;				/* ポイント */
	int				event;				/* イベント */
} cr_recv;			/* 受け渡し用 */

typedef struct _cr2tn_n {
	char			seq;			/* 1:	シーケンス番号 */
	char			ver[2];			/* 2:	電文バージョン */
	char			maker_code[2];	/* 4:	メーカーコード */
	char			cmd;			/* 6:	コマンド'N' */
	char			cmd_ex;			/* 7:	コマンド拡張 */
	char			cr_status[4];	/* 8:	CRサンド状態ステータス */
	char			req_cmd;		/* 12:	要求コマンド */
	char			card_kind;		/* 13:	カード種類 */
	char			kai_no[16];		/* 14:	会員番号 */
	char			s_zankin[4];	/* 30:	（スティック）残り金額情報 */
	char			s_riyoukin[4];	/* 34:	（スティック）利用金額情報 */
	char			d_nyukin[4];	/* 38:	（台積算）入金情報 */
	char			d_riyokin[4];	/* 42:	（台積算）利用金額情報 */
	char			crc[2];			/* 46:	CRC */
} cr2tn_n;		/* N電文（CRサンド→TVナビ） */

typedef struct _cr2tn_i {
	char	seq;			/* 1:	シーケンス番号 */
	char	ver[2];			/* 2:	電文バージョン */
	char	maker_code[2];	/* 4:	メーカーコード */
	char	cmd;			/* 6:	コマンド'I' */
	char	cmd_ex;			/* 7:	コマンド拡張 */
	char	cr_status[4];	/* 8:	CRサンド状態ステータス */
	char	req_cmd;		/* 12:	要求コマンド */
	char	card_kind;		/* 13:	カード種類 */
	char	kai_no[16];		/* 14:	会員番号 */
	char	nyukin[4];		/* 30:	入金情報 */
	char	riyoukin[4];	/* 34:	利用金額情報 */
	char	d_nyukin[4];	/* 38:	（台積算）入金情報 */
	char	d_riyokin[4];	/* 42:	（台積算）利用金額情報 */
	char	crc[2];			/* 46:	CRC */
} cr2tn_i;		/* I電文（CRサンド→TVナビ） */

typedef struct _cr2tn_r {
	char	seq;			/* 1:	シーケンス番号 */
	char	ver[2];			/* 2:	電文バージョン */
	char	maker_code[2];	/* 4:	メーカーコード */
	char	cmd;			/* 6:	コマンド'N' */
	char	cmd_ex;			/* 7:	コマンド拡張 */
	char	cr_status[4];	/* 8:	CRサンド状態ステータス */
	char	kai_no[16];		/* 12:	会員番号 */
	char	harai_dosu[6];	/* 28:	払出可能度数 */
	char	crc[2];			/* 34:	CRC */
} cr2tn_r;		/* R電文（CRサンド→TVナビ） */

typedef struct _cr2tn_o {
	char	seq;			/* 1:	シーケンス番号 */
	char	ver[2];			/* 2:	電文バージョン */
	char	maker_code[2];	/* 4:	メーカーコード */
	char	cmd;			/* 6:	コマンド'N' */
	char	cmd_ex;			/* 7:	コマンド拡張 */
	char	cr_status[4];	/* 8:	CRサンド状態ステータス */
	char	req_cmd;		/* 12:	要求コマンド */
	char	card_kind;		/* 13:	カード種類 */
	char	kai_no[16];		/* 14:	会員番号 */
	char	nyukin[4];		/* 30:	入金情報 */
	char	riyoukin[4];	/* 34:	利用金額情報 */
	char	d_nyukin[4];	/* 38:	（台積算）入金情報 */
	char	d_riyokin[4];	/* 42:	（台積算）利用金額情報 */
	char	crc[2];			/* 46:	CRC */
} cr2tn_o;		/* O電文（TN→CRサンド） */

