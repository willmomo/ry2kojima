#include "SbFile.h"

/////////////////////////////////////////////////////////////////////
// �R���X�g���N�^

SbFile::SbFile()
{
	ZeroMemory(m_szName, sizeof(m_szName));
	m_hfile = INVALID_HANDLE_VALUE;
}

/////////////////////////////////////////////////////////////////////
// �f�X�g���N�^

SbFile::~SbFile()
{
	if (m_hfile != INVALID_HANDLE_VALUE)
		::CloseHandle(m_hfile);
}

/////////////////////////////////////////////////////////////////////
// �t�@�C�����J��

BOOL SbFile::Open(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
				  DWORD dwCreationDisposition, 
				  DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL)
{
	assert(m_hfile == INVALID_HANDLE_VALUE);
	assert(lpFileName);

	m_hfile = ::CreateFile(lpFileName, dwDesiredAccess, dwShareMode, NULL,
		dwCreationDisposition, dwFlagsAndAttributes, NULL);

	if (m_hfile == INVALID_HANDLE_VALUE)
		return FALSE;

	lstrcpy(m_szName, lpFileName);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// �t�@�C�����J��(With Retry)

BOOL SbFile::OpenWR(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
				  DWORD dwCreationDisposition, 
				  DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,
				  HWND hwnd = NULL, LPCTSTR lpCaption = NULL)
{
	char szErrMsg[256];

	while (!Open(lpFileName, dwDesiredAccess, dwShareMode, dwCreationDisposition, dwFlagsAndAttributes)) {
		DWORD dwError = GetLastError();
		switch (dwError) {
		case ERROR_WRITE_PROTECT:	// �G���[�R�[�h19
			if (MessageBox(hwnd, "���̃f�B�X�N�̓��C�g�v���e�N�g����Ă��܂��B\n"
				"���C�g�v���e�N�g���������邩�A�ʂ̃f�B�X�N���g���Ă��������B",
				lpCaption, MB_RETRYCANCEL|MB_ICONERROR) == IDCANCEL)
				return FALSE;
			break;
		default:
			wsprintf(szErrMsg, "�V�X�e���G���[�ł��B\n�G���[�R�[�h %d", GetLastError());
			if (MessageBox(hwnd, szErrMsg, lpCaption, MB_RETRYCANCEL|MB_ICONERROR) == IDCANCEL)
				return FALSE;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// �t�@�C�������

void SbFile::Close()
{
	assert(m_hfile != INVALID_HANDLE_VALUE);

	if (m_hfile != INVALID_HANDLE_VALUE)
		::CloseHandle(m_hfile);

	m_hfile = INVALID_HANDLE_VALUE;
	ZeroMemory(m_szName, sizeof(m_szName));
}
