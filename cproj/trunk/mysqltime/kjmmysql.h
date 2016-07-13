#pragma once

#include <string>

#include "mysql.h"

namespace kjm {

	class mysqlRes {
	private:
		MYSQL_RES* m_mysqlRes;

	private:

	public:
		mysqlRes();
		mysqlRes(MYSQL_RES* result);
		mysqlRes(const mysqlRes& src);
		virtual ~mysqlRes();

		bool hasResult();

		unsigned int num_fields() { return ::mysql_num_fields(m_mysqlRes); }

		MYSQL_ROW fetch_row() { return ::mysql_fetch_row(m_mysqlRes); }

		my_ulonglong num_rows() { return ::mysql_num_rows(m_mysqlRes); }

		void free_result();
	};

	class mysql {
	public:
		MYSQL m_mysql;
		unsigned int m_errno;
		std::string m_error;

	public:
		mysql() {}
		virtual ~mysql() { ::mysql_close(&m_mysql); }

	public:
		bool init();

		bool real_connect(const char* host, const char* user, const char* passwd, const char* db, unsigned int port);

		int real_query(const char* stmt_str, unsigned long length);

		mysqlRes store_result();

		// 先行のSQLにSQL_BUFFER_RESULTを付けないとテーブルロックしてしまうらしい
		// for example: select SQL_BUFFER_RESULT * from mytable;
		mysqlRes use_result();
	};
}
