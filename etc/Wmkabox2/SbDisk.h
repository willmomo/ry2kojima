#if !defined(__SBDISK_H)
#define __SBDISK_H

#include <windows.h>
#include <windowsx.h>
#include <assert.h>

/////////////////////////////////////////////////////////////////////
// �f�B�X�N�̏��������N���X

class SbDisk
{
public:
	int m_nDrive;		// �h���C�u�ԍ� (0:A ...)

public:
	// �R���X�g���N�^/�f�X�g���N�^
	SbDisk();
	SbDisk(int nDrive);
	virtual ~SbDisk() {};

	// ����
	BOOL GetFree(LPDWORD lpdwFree);
	BOOL GetFreeWR(LPDWORD lpdwFree, HWND hwnd/*=NULL*/, LPCTSTR lpszCaption/*=NULL*/);
};

#endif
