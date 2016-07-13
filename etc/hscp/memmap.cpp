#include <windows.h>
#include "memmap.h"

//	[概要]
//	メモリマップドファイルを作る

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

//	[概要]
//	メモリマップドファイルを開く

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

//	[概要]
//	メモリマップドファイルを閉じる
//
//	[備考]
//	デストラクタからも呼び出されるため、後処理もちゃんと行う

BOOL CMemMapFile::Close()
{
	// まだ、メモリマップドファイルが作られていない
	if (!IsAvailable())	return TRUE;

	// マップされているときは、アンマップ
	if (IsMapped()) {
		if (!ReleasePointer()) {
			return FALSE;
		}
	}

	// クローズする。正常終了ならハンドルをNULLにする
	BOOL bRet = ::CloseHandle(m_hmmf);
	if (bRet == TRUE) {
		m_hmmf = NULL;
	}

	return bRet;
}

//	[概要]
//	メモリマップドファイルをマップする

void* CMemMapFile::GetPointer()
{
	_ASSERT(IsAvailable());
	_ASSERT(!IsMapped());

	return m_pvBase = ::MapViewOfFile(m_hmmf, FILE_MAP_ALL_ACCESS, 0, 0, 0);
}

//	[概要]
//	マップされている場合、アンマップする

BOOL CMemMapFile::ReleasePointer()
{
	_ASSERT(IsAvailable());
	_ASSERT(IsMapped());

	// アンマップする。正常終了ならポインタをNULLにする
	BOOL bRet = ::UnmapViewOfFile(m_pvBase);
	if (bRet == TRUE) {
		m_pvBase = NULL;
	}

	return bRet;
}
