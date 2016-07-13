#include "SbDisk.h"

/////////////////////////////////////////////////////////////////////
// �R���X�g���N�^

SbDisk::SbDisk()
{
	m_nDrive = -1;
}

/////////////////////////////////////////////////////////////////////
// �R���X�g���N�^
// �����Ƀf�B�X�N���w�肷��

SbDisk::SbDisk(int nDrive)
{
	assert(('A'-'A' <= nDrive) && (nDrive <= 'Z' - 'A'));

	m_nDrive = nDrive;
}

/////////////////////////////////////////////////////////////////////
// �f�B�X�N�̋󂫗e�ʂ��擾����

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
// �f�B�X�N�̋󂫗e�ʂ��擾����(With Retry)

BOOL SbDisk::GetFreeWR(LPDWORD lpdwFree, HWND hwnd=NULL, LPCTSTR lpszCaption=NULL)
{
	while (!GetFree(lpdwFree)) {
		char szMessage[128];
		DWORD dwError = GetLastError();
		switch (dwError) {
		case ERROR_INVALID_DRIVE:	// �G���[�R�[�h 15
			wsprintf(szMessage, "%c:\\ �ɃA�N�Z�X�ł��܂���B\n�f�o�C�X�̏������ł��Ă��܂���B", m_nDrive + 'A');
			if (MessageBox(hwnd, szMessage, lpszCaption, MB_RETRYCANCEL|MB_ICONERROR) == IDCANCEL)
				return FALSE;
			break;
		default:
			wsprintf(szMessage, "�V�X�e���G���[�ł��B\n�G���[�R�[�h %d", dwError);
			if (MessageBox(hwnd, szMessage, lpszCaption, MB_RETRYCANCEL|MB_ICONERROR) == IDCANCEL)
				return FALSE;
			break;
		}
	}

	return TRUE;
}
