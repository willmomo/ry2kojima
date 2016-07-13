#include "kjmlib.h"

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <vector>

using namespace kjm;

//-----------------------------------------------------------------
// ユーティリティ関数郡
//
// ユーティリティクラスなので、インスタンス化しない。
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// MBCSをUNICODEに変換
//-----------------------------------------------------------------
std::wstring util::to_stringW(const std::string &str) {
	int requiredSize = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	std::vector<wchar_t> buf(requiredSize);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buf.begin(), buf.size());

	return buf.begin();
}

//-----------------------------------------------------------------
// UNICODEをMBCSに変換
//-----------------------------------------------------------------
std::string util::to_stringA(const std::wstring &str) {
	int requiredSize = ::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);

	std::vector<char> buf(requiredSize);

	::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, buf.begin(), buf.size(), NULL, NULL);

	return buf.begin();
}

//-----------------------------------------------------------------
// GetTickCountを使って経過時間を計る
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// デフォルト コンストラクタ
//-----------------------------------------------------------------
tick_counter::tick_counter() : m_t1(::GetTickCount()), m_t2(0) {
	// 特になし
}

//-----------------------------------------------------------------
// コピー コンストラクタ
//-----------------------------------------------------------------
tick_counter::tick_counter(const tick_counter &src) : m_t1(src.m_t1), m_t2(src.m_t2) {
	// 特になし
}

//-----------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------
tick_counter::~tick_counter() {
	// 特になし
}

//-----------------------------------------------------------------
// 代入演算子
//-----------------------------------------------------------------
tick_counter &tick_counter::operator =(const tick_counter &rhs) {
	if (this != &rhs) {
		m_t1 = rhs.m_t1;
		m_t2 = rhs.m_t2;
	}

	return *this;
}

//-----------------------------------------------------------------
// 計測開始
//-----------------------------------------------------------------
void tick_counter::start() {
	m_t1 = ::GetTickCount();
	m_t2 = 0;
}

//-----------------------------------------------------------------
// 計測終了
//-----------------------------------------------------------------
DWORD tick_counter::stop() {
	m_t2 = ::GetTickCount();
	return elapse();
}

//-----------------------------------------------------------------
// 経過時間
//-----------------------------------------------------------------
DWORD tick_counter::elapse() {
	return (m_t2 == 0) ? (::GetTickCount() - m_t1) : (m_t2 - m_t1);
}

//-----------------------------------------------------------------
// 経過時間(秒)
//-----------------------------------------------------------------
DWORD tick_counter::elapse_sec() {
	return elapse() / 1000;
}

//-----------------------------------------------------------------
// 経過時間(分)
//-----------------------------------------------------------------
DWORD tick_counter::elapse_min() {
	return elapse_sec() / 60;
}

//-----------------------------------------------------------------
// パス文字列を扱う
//
// ユーティリティクラスなので、インスタンス化しない。
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// PathAppendW 相当の関数
//-----------------------------------------------------------------
std::wstring path::appendW(const std::wstring &strPath, const std::wstring &strMore) {
	std::vector<wchar_t> work(MAX_PATH);

	::PathAppendW(::lstrcpyW(work.begin(), strPath.c_str()), strMore.c_str());

	return work.begin();
}

//-----------------------------------------------------------------
// PathAppendA 相当の関数
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
// PathRemoveFileSpecW 相当の関数
//-----------------------------------------------------------------
std::wstring path::remove_file_specW(const std::wstring& strPath) {
	std::vector<wchar_t> work(MAX_PATH);

	::PathRemoveFileSpecW(::lstrcpyW(work.begin(), strPath.c_str()));

	return work.begin();
}

//-----------------------------------------------------------------
// PathRemoveFileSpecA 相当の関数
//-----------------------------------------------------------------
std::string path::remove_file_specA(const std::string& strPath) {
	return util::to_stringA(
		path::remove_file_specW(
			util::to_stringW(strPath)
		)
	);
}
