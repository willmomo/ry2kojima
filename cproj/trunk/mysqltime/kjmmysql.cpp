#include "kjmmysql.h"


bool kjm::mysql::init() {
	MYSQL* ret = ::mysql_init(&m_mysql);
	return (ret ? true : false);
}

bool kjm::mysql::real_connect(const char* host, const char* user, const char* passwd, const char* db, unsigned int port) {
	bool result = true;
	MYSQL* ret = ::mysql_real_connect(&m_mysql, host, user, passwd, db, port, NULL, 0);
	if (!ret) {
		m_errno = ::mysql_errno(&m_mysql);
		m_error = ::mysql_error(&m_mysql);
		result = false;
	}
	return result;
}

int kjm::mysql::real_query(const char* stmt_str, unsigned long length) {
	int result;
	if ((result = ::mysql_real_query(&m_mysql, stmt_str, length)) != 0) {
		m_errno = ::mysql_errno(&m_mysql);
		m_error = ::mysql_error(&m_mysql);
	}
	return result;
}

kjm::mysqlRes kjm::mysql::store_result() {
	kjm::mysqlRes result(::mysql_store_result(&m_mysql));
	if (result.hasResult() == false) {
		m_errno = ::mysql_errno(&m_mysql);
		m_error = ::mysql_error(&m_mysql);
	}
	return result;
}

kjm::mysqlRes kjm::mysql::use_result() {
	kjm::mysqlRes result(::mysql_use_result(&m_mysql));
	if (result.hasResult() == false) {
		m_errno = ::mysql_errno(&m_mysql);
		m_error = ::mysql_error(&m_mysql);
	}
	return result;
}

kjm::mysqlRes::mysqlRes() : m_mysqlRes(NULL) {
}

kjm::mysqlRes::mysqlRes(MYSQL_RES* result) : m_mysqlRes(result) {
}

kjm::mysqlRes::mysqlRes(const mysqlRes& src) : m_mysqlRes(src.m_mysqlRes) {
}

kjm::mysqlRes::~mysqlRes() {
}

bool kjm::mysqlRes::hasResult() {
	return (m_mysqlRes ? true : false);
}

void kjm::mysqlRes::free_result() {
	::mysql_free_result(m_mysqlRes);
	m_mysqlRes = NULL;
}
