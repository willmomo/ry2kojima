#include "SbFile.h"

/////////////////////////////////////////////////////////////////////
// コンストラクタ

SbFile::SbFile()
{
	ZeroMemory(m_szName, sizeof(m_szName));
	m_hfile = INVALID_HANDLE_VALUE;
}

/////////////////////////////////////////////////////////////////////
// デストラクタ

SbFile::~SbFile()
{
	if (m_hfile != INVALID_HANDLE_VALUE)
		::CloseHandle(m_hfile);
}

/////////////////////////////////////////////////////////////////////
// ファイルを開く

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
// ファイルを開く(With Retry)

BOOL SbFile::OpenWR(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
				  DWORD dwCreationDisposition, 
				  DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,
				  HWND hwnd = NULL, LPCTSTR lpCaption = NULL)
{
	char szErrMsg[256];

	while (!Open(lpFileName, dwDesiredAccess, dwShareMode, dwCreationDisposition, dwFlagsAndAttributes)) {
		DWORD dwError = GetLastError();
		switch (dwError) {
		case ERROR_WRITE_PROTECT:	// エラーコード19
			if (MessageBox(hwnd, "このディスクはライトプロテクトされています。\n"
				"ライトプロテクトを解除するか、別のディスクを使ってください。",
				lpCaption, MB_RETRYCANCEL|MB_ICONERROR) == IDCANCEL)
				return FALSE;
			break;
		default:
			wsprintf(szErrMsg, "システムエラーです。\nエラーコード %d", GetLastError());
			if (MessageBox(hwnd, szErrMsg, lpCaption, MB_RETRYCANCEL|MB_ICONERROR) == IDCANCEL)
				return FALSE;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// ファイルを閉じる

void SbFile::Close()
{
	assert(m_hfile != INVALID_HANDLE_VALUE);

	if (m_hfile != INVALID_HANDLE_VALUE)
		::CloseHandle(m_hfile);

	m_hfile = INVALID_HANDLE_VALUE;
	ZeroMemory(m_szName, sizeof(m_szName));
}
