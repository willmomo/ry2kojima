#include "SbDisk.h"

/////////////////////////////////////////////////////////////////////
// コンストラクタ

SbDisk::SbDisk()
{
	m_nDrive = -1;
}

/////////////////////////////////////////////////////////////////////
// コンストラクタ
// 同時にディスクを指定する

SbDisk::SbDisk(int nDrive)
{
	assert(('A'-'A' <= nDrive) && (nDrive <= 'Z' - 'A'));

	m_nDrive = nDrive;
}

/////////////////////////////////////////////////////////////////////
// ディスクの空き容量を取得する

BOOL SbDisk::GetFree(LPDWORD lpdwFree)
{
	assert(lpdwFree);
	assert(m_nDrive != -1);

	DWORD dwSectorsPerCluster, dwBytesPerSector, 
		dwNumberOfFreeClusters, dwTotalNumberOfClusters;
	char szRootPathName[8];
	::wsprintf(szRootPathName, "%c:\\", m_nDrive + 'A');
	if (!::GetDiskFreeSpace(szRootPathName, &dwSectorsPerCluster, 
		&dwBytesPerSector, &dwNumberOfFreeClusters, &dwTotalNumberOfClusters))
		return FALSE;

	*lpdwFree = dwNumberOfFreeClusters * dwSectorsPerCluster * dwBytesPerSector;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// ディスクの空き容量を取得する(With Retry)

BOOL SbDisk::GetFreeWR(LPDWORD lpdwFree, HWND hwnd=NULL, LPCTSTR lpszCaption=NULL)
{
	while (!GetFree(lpdwFree)) {
		char szMessage[128];
		DWORD dwError = GetLastError();
		switch (dwError) {
		case ERROR_INVALID_DRIVE:	// エラーコード 15
			wsprintf(szMessage, "%c:\\ にアクセスできません。\nデバイスの準備ができていません。", m_nDrive + 'A');
			if (MessageBox(hwnd, szMessage, lpszCaption, MB_RETRYCANCEL|MB_ICONERROR) == IDCANCEL)
				return FALSE;
			break;
		default:
			wsprintf(szMessage, "システムエラーです。\nエラーコード %d", dwError);
			if (MessageBox(hwnd, szMessage, lpszCaption, MB_RETRYCANCEL|MB_ICONERROR) == IDCANCEL)
				return FALSE;
			break;
		}
	}

	return TRUE;
}
