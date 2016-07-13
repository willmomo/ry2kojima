//-----------------------------------------------------------------------------
// �~���[�e�b�N�X�n���h�����Ǘ�����N���X
//-----------------------------------------------------------------------------
#include "kjmMutex.h"

#include <assert.h>

//-----------------------------------------------------------------------------
// �~���[�e�b�N�X�I�u�W�F�N�g�̍쐬(Unicode)
//-----------------------------------------------------------------------------
bool kjm::mutex::create(const std::wstring& strName) {
	assert(!(m_hMutex != NULL));
	if      (m_hMutex != NULL)	throw kjm::errInfo(-1, L"create(): �~���[�e�b�N�X�����������ꂽ�܂܂ł��B");

	return ((m_hMutex = ::CreateMutexW(NULL, FALSE, strName.c_str())) != NULL);
}

//-----------------------------------------------------------------------------
// �~���[�e�b�N�X�I�u�W�F�N�g�����
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
// ���b�N����
//-----------------------------------------------------------------------------
DWORD kjm::mutex::lock(DWORD dwMilliseconds) {
	assert(!(m_hMutex == NULL));
	if      (m_hMutex == NULL)	throw kjm::errInfo(-1, L"lock(): �~���[�e�b�N�X�����������ł��B");

	DWORD dwRet = ::WaitForSingleObject(m_hMutex, dwMilliseconds);
	return dwRet;
}

//-----------------------------------------------------------------------------
// �A�����b�N����
//-----------------------------------------------------------------------------
void kjm::mutex::unlock() {
	assert(!(m_hMutex == NULL));
	if      (m_hMutex == NULL)	throw kjm::errInfo(-1, L"unlock(): �~���[�e�b�N�X�����������ł��B");

	::ReleaseMutex(m_hMutex);
}
