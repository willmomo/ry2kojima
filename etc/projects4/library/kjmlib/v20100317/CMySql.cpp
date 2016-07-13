//---------------------------------------------------------------------
// MySQL�̃��C�u������C++�ň����B
//
// 20100312 r.kojima <�쐬>
//---------------------------------------------------------------------
#include <windows.h>
#include <assert.h>
#include "CMySql.h"


//=====================================================================
// MySQL��C++�ň����܂��B
//=====================================================================

//---------------------------------------------------------------------
// ������
//
// [�߂�l]
// �������ɐ�������� true ��Ԃ��B���s����� false ��Ԃ��B
// Release �r���h�� assert �����𖞂����Ȃ��Ƃ��́Afalse ��Ԃ��B
//---------------------------------------------------------------------
bool MySQL::init() {
	// ���O����
	assert(m_mysql == NULL);
	if  (!(m_mysql == NULL))	return false;

	m_mysql = mysql_init(NULL);

	return (m_mysql != NULL);
}


//---------------------------------------------------------------------
// MySQL�ɐڑ�
//
// [�߂�l]
// �ڑ��ɐ�������� true ��Ԃ��B���s����� false ��Ԃ��B
// Release �r���h�� assert �����𖞂����Ȃ��Ƃ��́Afalse ��Ԃ��B
//---------------------------------------------------------------------
bool MySQL::real_connect(const char* host, const char* user, const char* passwd, const char* db, unsigned int port) {
	// ���O����
	assert(m_mysql != NULL);
	if  (!(m_mysql != NULL))	return false;

	return (mysql_real_connect(m_mysql, host, user, passwd, db, port, NULL, 0) != NULL);
}


//---------------------------------------------------------------------
// MySQL�̐ڑ���ؒf
//
// [���l]
// mysql_close �֐��� NULL ��^���Ă����퓮�삵�܂����B
// �Ȃ̂ŁANULL ����̏������Ȃ��Ă��܂��B
//---------------------------------------------------------------------
void MySQL::close() {
	mysql_close(m_mysql);
	m_mysql = NULL;
}


//---------------------------------------------------------------------
// �T�[�o�ւ̐ڑ����쓮���Ă��邩�ǂ������ׂ�
//
// [�߂�l]
// �ڑ��ł��Ă���Ƃ��� 0 ��Ԃ��B�G���[�̂Ƃ��� 0 �ȊO��Ԃ��B
// Release �r���h�ŁA assert �����𖞂����Ȃ��Ƃ��́A-1 ��Ԃ��B
//---------------------------------------------------------------------
int MySQL::ping() {
	// ���O����
	assert(m_mysql != NULL);
	if  (!(m_mysql != NULL))	return -1;

	return mysql_ping(m_mysql);
}


//---------------------------------------------------------------------
// �N�G���[(SQL)�̎��s
//
// [�߂�l]
// ��������� 0 ��Ԃ��B���s����� 0 �ȊO��Ԃ��B
// Release �r���h�ŁAassert �����𖞂����Ȃ��Ƃ��́A-1 ��Ԃ��B
//---------------------------------------------------------------------
int MySQL::query(const char* q) {
	// ���O����
	assert(m_mysql != NULL);
	if  (!(m_mysql != NULL))	return -1;
	assert(q != NULL);
	if  (!(q != NULL))	return -1;

	// reconnect ���L���ȂƂ��́A�ȉ��̏����� reconnect ���N���邩������Ȃ��B
	int ret = ping();
	if (ret != 0) {
		return ret;
	}

	return mysql_query(m_mysql, q);
}


//---------------------------------------------------------------------
// �N�G���[�̌��ʃZ�b�g���擾
//
// [�߂�l]
// ��������� NULL �ȊO��Ԃ��B���s����� NULL ��Ԃ��B
// Release �r���h�ŁAassert �����𖞂����Ȃ��Ƃ��́ANULL ��Ԃ��B
//---------------------------------------------------------------------
MYSQL_RES* MySQL::store_result() {
	// ���O����
	assert(m_mysql != NULL);
	if  (!(m_mysql != NULL))	return NULL;

	return mysql_store_result(m_mysql);
}


//---------------------------------------------------------------------
// MySQL�̃G���[���擾
//
// [�߂�l]
// �G���[�ԍ���Ԃ��B
// Release �r���h�ŁAassert �����𖞂����Ȃ��Ƃ��́A-1 ��Ԃ��B
//---------------------------------------------------------------------
int MySQL::get_errno() {
	// ���O����
	assert(m_mysql != NULL);
	if  (!(m_mysql != NULL))	return -1;

	return mysql_errno(m_mysql);
}


//---------------------------------------------------------------------
// �ڑ��I�v�V������ݒ�
//
// [�߂�l]
// ��������� 0 ��Ԃ��B���s����� 0 �ȊO��Ԃ��B
//---------------------------------------------------------------------
int MySQL::options(enum mysql_option option, const void* arg) {
	// ���O����
	assert(m_mysql != NULL);
	if  (!(m_mysql != NULL))	return 0;

	return mysql_options(m_mysql, option, arg);
}

