//-----------------------------------------------------------------------------
// �G���[����ێ�����N���X
//-----------------------------------------------------------------------------
#include "kjmErrInfo.h"

#include <sstream>

//-----------------------------------------------------------------------------
// �G���[�ԍ��������󂯎��R���X�g���N�^
//-----------------------------------------------------------------------------
kjm::errInfo::errInfo(DWORD dwErr) : m_dwErr(dwErr) {
	LPWSTR pszBuffer = NULL;
	::SetLastError(ERROR_SUCCESS);
	DWORD dwRet = ::FormatMessageW(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL,
		m_dwErr,
		0,
		(LPWSTR)&pszBuffer,
		512,
		NULL);
	if (dwRet != 0) {
		m_dwInnerErr = 0;
		m_strMsg = kjm::util::chomp(pszBuffer);
	} else {
		m_dwInnerErr = ::GetLastError();
		m_strMsg = L"";
	}
	if (pszBuffer) ::LocalFree(pszBuffer);
}
