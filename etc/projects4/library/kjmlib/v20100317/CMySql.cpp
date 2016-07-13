//---------------------------------------------------------------------
// MySQLのライブラリをC++で扱う。
//
// 20100312 r.kojima <作成>
//---------------------------------------------------------------------
#include <windows.h>
#include <assert.h>
#include "CMySql.h"


//=====================================================================
// MySQLをC++で扱います。
//=====================================================================

//---------------------------------------------------------------------
// 初期化
//
// [戻り値]
// 初期化に成功すると true を返す。失敗すると false を返す。
// Release ビルドで assert 条件を満たさないときは、false を返す。
//---------------------------------------------------------------------
bool MySQL::init() {
	// 事前条件
	assert(m_mysql == NULL);
	if  (!(m_mysql == NULL))	return false;

	m_mysql = mysql_init(NULL);

	return (m_mysql != NULL);
}


//---------------------------------------------------------------------
// MySQLに接続
//
// [戻り値]
// 接続に成功すると true を返す。失敗すると false を返す。
// Release ビルドで assert 条件を満たさないときは、false を返す。
//---------------------------------------------------------------------
bool MySQL::real_connect(const char* host, const char* user, const char* passwd, const char* db, unsigned int port) {
	// 事前条件
	assert(m_mysql != NULL);
	if  (!(m_mysql != NULL))	return false;

	return (mysql_real_connect(m_mysql, host, user, passwd, db, port, NULL, 0) != NULL);
}


//---------------------------------------------------------------------
// MySQLの接続を切断
//
// [備考]
// mysql_close 関数に NULL を与えても正常動作しました。
// なので、NULL 判定の処理を省いています。
//---------------------------------------------------------------------
void MySQL::close() {
	mysql_close(m_mysql);
	m_mysql = NULL;
}


//---------------------------------------------------------------------
// サーバへの接続が作動しているかどうか調べる
//
// [戻り値]
// 接続できているときは 0 を返す。エラーのときは 0 以外を返す。
// Release ビルドで、 assert 条件を満たさないときは、-1 を返す。
//---------------------------------------------------------------------
int MySQL::ping() {
	// 事前条件
	assert(m_mysql != NULL);
	if  (!(m_mysql != NULL))	return -1;

	return mysql_ping(m_mysql);
}


//---------------------------------------------------------------------
// クエリー(SQL)の実行
//
// [戻り値]
// 成功すると 0 を返す。失敗すると 0 以外を返す。
// Release ビルドで、assert 条件を満たさないときは、-1 を返す。
//---------------------------------------------------------------------
int MySQL::query(const char* q) {
	// 事前条件
	assert(m_mysql != NULL);
	if  (!(m_mysql != NULL))	return -1;
	assert(q != NULL);
	if  (!(q != NULL))	return -1;

	// reconnect が有効なときは、以下の処理で reconnect が起こるかもしれない。
	int ret = ping();
	if (ret != 0) {
		return ret;
	}

	return mysql_query(m_mysql, q);
}


//---------------------------------------------------------------------
// クエリーの結果セットを取得
//
// [戻り値]
// 成功すると NULL 以外を返す。失敗すると NULL を返す。
// Release ビルドで、assert 条件を満たさないときは、NULL を返す。
//---------------------------------------------------------------------
MYSQL_RES* MySQL::store_result() {
	// 事前条件
	assert(m_mysql != NULL);
	if  (!(m_mysql != NULL))	return NULL;

	return mysql_store_result(m_mysql);
}


//---------------------------------------------------------------------
// MySQLのエラーを取得
//
// [戻り値]
// エラー番号を返す。
// Release ビルドで、assert 条件を満たさないときは、-1 を返す。
//---------------------------------------------------------------------
int MySQL::get_errno() {
	// 事前条件
	assert(m_mysql != NULL);
	if  (!(m_mysql != NULL))	return -1;

	return mysql_errno(m_mysql);
}


//---------------------------------------------------------------------
// 接続オプションを設定
//
// [戻り値]
// 成功すると 0 を返す。失敗すると 0 以外を返す。
//---------------------------------------------------------------------
int MySQL::options(enum mysql_option option, const void* arg) {
	// 事前条件
	assert(m_mysql != NULL);
	if  (!(m_mysql != NULL))	return 0;

	return mysql_options(m_mysql, option, arg);
}

