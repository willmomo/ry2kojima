//---------------------------------------------------------------------
// MySQL�̃��C�u������C++�ň����B
//
// 20100312 r.kojima <�쐬>
//
// mysql.h �́A
// C:\Program Files\MySQL\MySQL Server 5.1\include �ɂ�����̂�z��B
//
// libmysql.lib �́A
// C:\Program Files\MySQL\MySQL Server 5.1\lib\debug
// �܂��́A
// C:\Program Files\MySQL\MySQL Server 5.1\lib\opt
// �ɂ�����̂�z��B
//---------------------------------------------------------------------
#pragma once


#include "mysql.h"

#if defined(_DEBUG)
#pragma comment(lib, "C:/Program Files/MySQL/MySQL Server 5.1/lib/debug/libmysql.lib")
#else
#pragma comment(lib, "C:/Program Files/MySQL/MySQL Server 5.1/lib/opt/libmysql.lib")
#endif


//---------------------------------------------------------------------
// MySQL��C++�ň����܂��B
//---------------------------------------------------------------------
class MySQL {
public:
	// �f�t�H���g�R���X�g���N�^
	MySQL() : m_mysql(NULL) {};

	// �f�X�g���N�^
	~MySQL() { close(); };

	// ������
	bool init();

	// MySQL�ɐڑ�
	bool real_connect(const char* host, const char* user, const char* passwd, const char* db, unsigned int port);

	// MySQL�̐ڑ���ؒf
	void close();

	// �N�G���[(SQL)�̎��s
	int query(const char* q);

	// �N�G���[�̌��ʃZ�b�g���擾
	MYSQL_RES* store_result();

	// MySQL�̃G���[���擾
	int get_errno();

	// �T�[�o�ւ̐ڑ����쓮���Ă��邩�ǂ������ׂ�
	int ping();

	// �ڑ��I�v�V������ݒ�
	int options(enum mysql_option option, const void* arg);

	// �Đڑ��@�\�̗L��/������ݒ�
	int opt_reconnect(my_bool reconnect) { return options(MYSQL_OPT_RECONNECT, &reconnect); };

private:
	// �R�s�[�Ƒ���𖳌�������
	MySQL(const MySQL& src) {};
	MySQL& operator=(const MySQL& rhs) { return *this; };

private:
	// MySQL�ւ̃n���h��
	MYSQL* m_mysql;
};


//---------------------------------------------------------------------
// MYSQL_RES ��C++�ň����܂��B
//---------------------------------------------------------------------
class MySQLRes {
public:
	// �f�t�H���g�R���X�g���N�^
	MySQLRes() : m_pMysqlRes(NULL) {};

	// ���ʃZ�b�g��n���R���X�g���N�^
	MySQLRes(MYSQL_RES* p) : m_pMysqlRes(p) {};

	// �f�X�g���N�^
	~MySQLRes() { free_result(); };

	// �������ʎ擾
	MYSQL_ROW fetch_row() { return mysql_fetch_row(m_pMysqlRes); };

	// �o�b�t�@�̔j��
	void free_result() {
		if (m_pMysqlRes)
			mysql_free_result(m_pMysqlRes);
		m_pMysqlRes = NULL;
	};

	// �擾�ł����s����Ԃ�
	//
	// ���ʃZ�b�g���擾�ł��Ȃ��������́A-1��Ԃ��B
	my_ulonglong get_row_count() {
		if (m_pMysqlRes == NULL)
			return -1;
		return m_pMysqlRes->row_count;
	};

	// result��NULL���ǂ����𒲂ׂ�
	bool is_null() { return (m_pMysqlRes == NULL); };

private:
	// ���ʃZ�b�g�ւ̃|�C���^
	MYSQL_RES* m_pMysqlRes;
};
