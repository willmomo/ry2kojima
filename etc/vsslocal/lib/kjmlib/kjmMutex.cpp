//-----------------------------------------------------------------------------
// ミューテックスハンドルを管理するクラス
//-----------------------------------------------------------------------------
#include "kjmMutex.h"

#include <assert.h>

//-----------------------------------------------------------------------------
// ミューテックスオブジェクトの作成(Unicode)
//-----------------------------------------------------------------------------
bool kjm::mutex::create(const std::wstring& strName) {
	assert(!(m_hMutex != NULL));
	if      (m_hMutex != NULL)	throw kjm::errInfo(-1, L"create(): ミューテックスが初期化されたままです。");

	return ((m_hMutex = ::CreateMutexW(NULL, FALSE, strName.c_str())) != NULL);
}

//-----------------------------------------------------------------------------
// ミューテックスオブジェクトを閉じる
//-----------------------------------------------------------------------------
bool kjm::mutex::close() {
	bool bRet = true;
	if (m_hMutex) {
		bRet = (::CloseHandle(m_hMutex) != FALSE);
		m_hMutex = NULL;
	}
	return bRet;
}

//-----------------------------------------------------------------------------
// ロックする
//-----------------------------------------------------------------------------
DWORD kjm::mutex::lock(DWORD dwMilliseconds) {
	assert(!(m_hMutex == NULL));
	if      (m_hMutex == NULL)	throw kjm::errInfo(-1, L"lock(): ミューテックスが未初期化です。");

	DWORD dwRet = ::WaitForSingleObject(m_hMutex, dwMilliseconds);
	return dwRet;
}

//-----------------------------------------------------------------------------
// アンロックする
//-----------------------------------------------------------------------------
void kjm::mutex::unlock() {
	assert(!(m_hMutex == NULL));
	if      (m_hMutex == NULL)	throw kjm::errInfo(-1, L"unlock(): ミューテックスが未初期化です。");

	::ReleaseMutex(m_hMutex);
}
