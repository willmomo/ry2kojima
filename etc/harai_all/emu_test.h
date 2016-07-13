/* メインループ */
int main_loop();

/* CSV読込みデータセット関数 */
void csv_data_set(csv_read *cpd, char *cbuf, int cnt);

/* 電文受信関数 */
int r_data(char *szBuf, size_t bufSize );

/* 電文送信関数 */
int s_data(char *data);

/* 電文解析関数 */
cr_recv rdata_anal(char *data);

/* CRC算出関数 */
void crc_cal(char *data, int n, char *crc_a);
void crc_cal_cmp(char *data, int n, char *crc_a);

/* CRCチェック関数 */
int crc_comp(char *data, int n);

/* 電文Verチェック関数 */
int ver_check(char *data, char *ver);

/* メーカーコードチェック関数 */
int mcd_check(char *data, char *mcd);

/* マトリクス処理 */
void cr_matrix(cr_recv pd, csv_read cpd);

/* アイドルイベント発生処理 */
void none_idle(cr_recv pd, csv_read cpd);		/* 通常状態 */
void cardin_idle(cr_recv pd, csv_read cpd);		/* カードIN中 */
void harai_idle(cr_recv pd, csv_read cpd);		/* 払出中 */
void error_idle(cr_recv pd, csv_read cpd);		/* エラー中 */

/* Ｎ受信発生中処理 */
void none_n(cr_recv pd, csv_read cpd);			/* 通常状態 */
void cardin_n(cr_recv pd, csv_read cpd);		/* カードＩＮ中 */
void harai_n(cr_recv pd, csv_read cpd);			/* 払出中 */
void error_n(cr_recv pd, csv_read cpd);			/* エラー中 */

/* Ｉ受信発生中処理 */
void none_i(cr_recv pd, csv_read cpd);			/* 通常状態 */
void cardin_i(cr_recv pd, csv_read cpd);		/* カードＩＮ中 */
void harai_i(cr_recv pd, csv_read cpd);			/* 払出中 */
void error_i(cr_recv pd, csv_read cpd);			/* エラー中 */

/* Ｏ受信発生中処理 */
void none_o(cr_recv pd, csv_read cpd);			/* 通常状態 */
void cardin_o(cr_recv pd, csv_read cpd);		/* カードＩＮ中 */
void harai_o(cr_recv pd, csv_read cpd);			/* 払出中 */
void error_o(cr_recv pd, csv_read cpd);			/* エラー中 */

/* Ｒ受信発生中処理 */
void none_r(cr_recv pd, csv_read cpd);			/* 通常状態 */
void cardin_r(cr_recv pd, csv_read cpd);		/* カードＩＮ中 */
void harai_r(cr_recv pd, csv_read cpd);			/* 払出中 */
void error_r(cr_recv pd, csv_read cpd);			/* エラー中 */

/* 送信データセット関数 */
void ndata_set(cr_recv pd, csv_read cpd, cr2tn_n *nd);	/* Ｎ電文用 */
void idata_set(cr_recv pd, csv_read cpd, cr2tn_i *nd);	/* Ｉ電文用 */
void odata_set(cr_recv pd, csv_read cpd, cr2tn_o *nd);	/* Ｏ電文用 */
void rdata_set(cr_recv pd, csv_read cpd, cr2tn_r *nd);	/* Ｒ電文用 */

/* エラー発生関数 */
void err_hapen();

/* コマンド拡張決定関数 */
char cmd_ex_rand( void );

/* エラーデータセット関数 */
void edata_set(csv_read *cpd, char *data);

/* エラーデータセット関数 */
void e_ndata_set(csv_read *cpd, char *data);
void e_idata_set(csv_read *cpd, char *data);
void e_odata_set(csv_read *cpd, char *data);
void e_rdata_set(csv_read *cpd, char *data);

/* 処理を指定した時間分だけ停止する関数 */
void sleep( clock_t wait );

/* CRCの値を変更する関数 */
void crc_change_func(char *data);

/* 電文を1char長くする関数 */
void char_len_chng(char *data);

int retry_err_func(char *data);
