#if !defined(__SBFILE_H)
#define __SBFILE_H

#include <windows.h>
#include <windowsx.h>
#include <assert.h>

/////////////////////////////////////////////////////////////////////
// ファイルを扱うクラス

class SbFile
{
public:
	HANDLE m_hfile;				// ファイルハンドル
	char m_szName[MAX_PATH];	// ファイル名

public:
	// コンストラクタ/デストラクタ
	SbFile();
	virtual ~SbFile();

	// 操作
	BOOL Open(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
		DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes/*=FILE_ATTRIBUTE_NORMAL*/);
	BOOL OpenWR(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
		DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes/*=FILE_ATTRIBUTE_NORMAL*/, 
		HWND hwnd/*=NULL*/, LPCTSTR lpCaption/*=NULL*/);
	void Close();
	BOOL Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead) {
		assert(m_hfile != INVALID_HANDLE_VALUE);
		assert(lpBuffer);
		assert(lpNumberOfBytesRead);
		return ::ReadFile(m_hfile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, NULL);
	};
	BOOL Write(LPVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten) {
		assert(m_hfile != INVALID_HANDLE_VALUE);
		assert(lpBuffer);
		assert(lpNumberOfBytesWritten);
		return ::WriteFile(m_hfile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, NULL);
	};

	// 情報
	DWORD GetFileSize(LPDWORD lpFileSizeHigh = NULL) {
		assert(m_hfile != INVALID_HANDLE_VALUE);
		return ::GetFileSize(m_hfile, lpFileSizeHigh);
	};
};

#endif
