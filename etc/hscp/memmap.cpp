#include <windows.h>
#include "memmap.h"

//	[�T�v]
//	�������}�b�v�h�t�@�C�������

BOOL CMemMapFile::Create(LPCTSTR pszName, DWORD dwSize)
{
	_ASSERT(!IsAvailable());
	_ASSERT(!IsMapped());

	m_hmmf = ::CreateFileMapping((HANDLE)0xffffffff, NULL, PAGE_READWRITE, 0, dwSize, pszName);
	if (m_hmmf == NULL) {
		return FALSE;
	}

	return TRUE;
}

//	[�T�v]
//	�������}�b�v�h�t�@�C�����J��

BOOL CMemMapFile::Open(LPCTSTR pszName)
{
	_ASSERT(!IsAvailable());
	_ASSERT(!IsMapped());

	m_hmmf = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, pszName);
	if (m_hmmf == NULL) {
		return FALSE;
	}

	return TRUE;
}

//	[�T�v]
//	�������}�b�v�h�t�@�C�������
//
//	[���l]
//	�f�X�g���N�^������Ăяo����邽�߁A�㏈���������ƍs��

BOOL CMemMapFile::Close()
{
	// �܂��A�������}�b�v�h�t�@�C��������Ă��Ȃ�
	if (!IsAvailable())	return TRUE;

	// �}�b�v����Ă���Ƃ��́A�A���}�b�v
	if (IsMapped()) {
		if (!ReleasePointer()) {
			return FALSE;
		}
	}

	// �N���[�Y����B����I���Ȃ�n���h����NULL�ɂ���
	BOOL bRet = ::CloseHandle(m_hmmf);
	if (bRet == TRUE) {
		m_hmmf = NULL;
	}

	return bRet;
}

//	[�T�v]
//	�������}�b�v�h�t�@�C�����}�b�v����

void* CMemMapFile::GetPointer()
{
	_ASSERT(IsAvailable());
	_ASSERT(!IsMapped());

	return m_pvBase = ::MapViewOfFile(m_hmmf, FILE_MAP_ALL_ACCESS, 0, 0, 0);
}

//	[�T�v]
//	�}�b�v����Ă���ꍇ�A�A���}�b�v����

BOOL CMemMapFile::ReleasePointer()
{
	_ASSERT(IsAvailable());
	_ASSERT(IsMapped());

	// �A���}�b�v����B����I���Ȃ�|�C���^��NULL�ɂ���
	BOOL bRet = ::UnmapViewOfFile(m_pvBase);
	if (bRet == TRUE) {
		m_pvBase = NULL;
	}

	return bRet;
}
