#include "kjmlib.h"

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <vector>

using namespace kjm;

//-----------------------------------------------------------------
// ���[�e�B���e�B�֐��S
//
// ���[�e�B���e�B�N���X�Ȃ̂ŁA�C���X�^���X�����Ȃ��B
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// MBCS��UNICODE�ɕϊ�
//-----------------------------------------------------------------
std::wstring util::to_stringW(const std::string &str) {
	int requiredSize = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	std::vector<wchar_t> buf(requiredSize);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buf.begin(), buf.size());

	return buf.begin();
}

//-----------------------------------------------------------------
// UNICODE��MBCS�ɕϊ�
//-----------------------------------------------------------------
std::string util::to_stringA(const std::wstring &str) {
	int requiredSize = ::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);

	std::vector<char> buf(requiredSize);

	::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, buf.begin(), buf.size(), NULL, NULL);

	return buf.begin();
}

//-----------------------------------------------------------------
// GetTickCount���g���Čo�ߎ��Ԃ��v��
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// �f�t�H���g �R���X�g���N�^
//-----------------------------------------------------------------
tick_counter::tick_counter() : m_t1(::GetTickCount()), m_t2(0) {
	// ���ɂȂ�
}

//-----------------------------------------------------------------
// �R�s�[ �R���X�g���N�^
//-----------------------------------------------------------------
tick_counter::tick_counter(const tick_counter &src) : m_t1(src.m_t1), m_t2(src.m_t2) {
	// ���ɂȂ�
}

//-----------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------
tick_counter::~tick_counter() {
	// ���ɂȂ�
}

//-----------------------------------------------------------------
// ������Z�q
//-----------------------------------------------------------------
tick_counter &tick_counter::operator =(const tick_counter &rhs) {
	if (this != &rhs) {
		m_t1 = rhs.m_t1;
		m_t2 = rhs.m_t2;
	}

	return *this;
}

//-----------------------------------------------------------------
// �v���J�n
//-----------------------------------------------------------------
void tick_counter::start() {
	m_t1 = ::GetTickCount();
	m_t2 = 0;
}

//-----------------------------------------------------------------
// �v���I��
//-----------------------------------------------------------------
DWORD tick_counter::stop() {
	m_t2 = ::GetTickCount();
	return elapse();
}

//-----------------------------------------------------------------
// �o�ߎ���
//-----------------------------------------------------------------
DWORD tick_counter::elapse() {
	return (m_t2 == 0) ? (::GetTickCount() - m_t1) : (m_t2 - m_t1);
}

//-----------------------------------------------------------------
// �o�ߎ���(�b)
//-----------------------------------------------------------------
DWORD tick_counter::elapse_sec() {
	return elapse() / 1000;
}

//-----------------------------------------------------------------
// �o�ߎ���(��)
//-----------------------------------------------------------------
DWORD tick_counter::elapse_min() {
	return elapse_sec() / 60;
}

//-----------------------------------------------------------------
// �p�X�����������
//
// ���[�e�B���e�B�N���X�Ȃ̂ŁA�C���X�^���X�����Ȃ��B
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// PathAppendW �����̊֐�
//-----------------------------------------------------------------
std::wstring path::appendW(const std::wstring &strPath, const std::wstring &strMore) {
	std::vector<wchar_t> work(MAX_PATH);

	::PathAppendW(::lstrcpyW(work.begin(), strPath.c_str()), strMore.c_str());

	return work.begin();
}

//-----------------------------------------------------------------
// PathAppendA �����̊֐�
//-----------------------------------------------------------------
std::string path::appendA(const std::string &strPath, const std::string &strMore) {
	return util::to_stringA(
		path::appendW(
			util::to_stringW(strPath),
			util::to_stringW(strMore)
		)
	);
}

//-----------------------------------------------------------------
// PathRemoveFileSpecW �����̊֐�
//-----------------------------------------------------------------
std::wstring path::remove_file_specW(const std::wstring& strPath) {
	std::vector<wchar_t> work(MAX_PATH);

	::PathRemoveFileSpecW(::lstrcpyW(work.begin(), strPath.c_str()));

	return work.begin();
}

//-----------------------------------------------------------------
// PathRemoveFileSpecA �����̊֐�
//-----------------------------------------------------------------
std::string path::remove_file_specA(const std::string& strPath) {
	return util::to_stringA(
		path::remove_file_specW(
			util::to_stringW(strPath)
		)
	);
}
