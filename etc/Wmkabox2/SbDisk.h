#if !defined(__SBDISK_H)
#define __SBDISK_H

#include <windows.h>
#include <windowsx.h>
#include <assert.h>

/////////////////////////////////////////////////////////////////////
// ディスクの情報を扱うクラス

class SbDisk
{
public:
	int m_nDrive;		// ドライブ番号 (0:A ...)

public:
	// コンストラクタ/デストラクタ
	SbDisk();
	SbDisk(int nDrive);
	virtual ~SbDisk() {};

	// 操作
	BOOL GetFree(LPDWORD lpdwFree);
	BOOL GetFreeWR(LPDWORD lpdwFree, HWND hwnd/*=NULL*/, LPCTSTR lpszCaption/*=NULL*/);
};

#endif
