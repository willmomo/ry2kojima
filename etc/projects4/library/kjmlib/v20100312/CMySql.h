//---------------------------------------------------------------------
// MySQLのライブラリをC++で扱う。
//
// 20100312 r.kojima <作成>
//
// mysql.h は、
// C:\Program Files\MySQL\MySQL Server 5.1\include にあるものを想定。
//
// libmysql.lib は、
// C:\Program Files\MySQL\MySQL Server 5.1\lib\debug
// または、
// C:\Program Files\MySQL\MySQL Server 5.1\lib\opt
// にあるものを想定。
//---------------------------------------------------------------------
#pragma once


#include "mysql.h"

#if defined(_DEBUG)
#pragma comment(lib, "C:/Program Files/MySQL/MySQL Server 5.1/lib/debug/libmysql.lib")
#else
#pragma comment(lib, "C:/Program Files/MySQL/MySQL Server 5.1/lib/opt/libmysql.lib")
#endif


//---------------------------------------------------------------------
// MySQLをC++で扱います。
//---------------------------------------------------------------------
class MySQL {
public:
	// デフォルトコンストラクタ
	MySQL() : m_mysql(NULL) {};

	// デストラクタ
	~MySQL() { close(); };

	// 初期化
	bool init();

	// MySQLに接続
	bool real_connect(const char* host, const char* user, const char* passwd, const char* db, unsigned int port);

	// MySQLの接続を切断
	void close();

	// クエリー(SQL)の実行
	int query(const char* q);

	// クエリーの結果セットを取得
	MYSQL_RES* store_result();

	// MySQLのエラーを取得
	int get_errno();

	// サーバへの接続が作動しているかどうか調べる
	int ping();

	// 接続オプションを設定
	int options(enum mysql_option option, const void* arg);

	// 再接続機能の有効/無効を設定
	int opt_reconnect(my_bool reconnect) { return options(MYSQL_OPT_RECONNECT, &reconnect); };

private:
	// コピーと代入を無効化する
	MySQL(const MySQL& src) {};
	MySQL& operator=(const MySQL& rhs) { return *this; };

private:
	// MySQLへのハンドル
	MYSQL* m_mysql;
};


//---------------------------------------------------------------------
// MYSQL_RES をC++で扱います。
//---------------------------------------------------------------------
class MySQLRes {
public:
	// デフォルトコンストラクタ
	MySQLRes() : m_pMysqlRes(NULL) {};

	// 結果セットを渡すコンストラクタ
	MySQLRes(MYSQL_RES* p) : m_pMysqlRes(p) {};

	// デストラクタ
	~MySQLRes() { free_result(); };

	// 検索結果取得
	MYSQL_ROW fetch_row() { return mysql_fetch_row(m_pMysqlRes); };

	// バッファの破棄
	void free_result() {
		if (m_pMysqlRes)
			mysql_free_result(m_pMysqlRes);
		m_pMysqlRes = NULL;
	};

	// 取得できた行数を返す
	//
	// 結果セットが取得できなかった時は、-1を返す。
	my_ulonglong get_row_count() {
		if (m_pMysqlRes == NULL)
			return -1;
		return m_pMysqlRes->row_count;
	};

	// resultがNULLかどうかを調べる
	bool is_null() { return (m_pMysqlRes == NULL); };

private:
	// 結果セットへのポインタ
	MYSQL_RES* m_pMysqlRes;
};
