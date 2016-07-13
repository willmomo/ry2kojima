// rkMemFile.cpp : メモリマップドファイル簡単アクセスクラス

#include <assert.h>
#include "rkMemFile.h"

// メモリマップドファイルの作成

BOOL rkMemFile::Create(LPCTSTR lpName, DWORD dwSize, rkMemFileException* e = NULL)
{
	// すでにメモリマップドファイルを管理中のときはエラー
	if (m_hmmf != NULL) {
		if (e) e->m_error = rkMemFileException::errAlreadyUse | rkMemFileException::customerCodeFlag;
		return FALSE;
	}

	// メモリマップドファイルの作成
	HANDLE hmmf = CreateFileMapping((HANDLE)0xffffffff, NULL, PAGE_READWRITE, 0, dwSize, lpName);
	if (hmmf == NULL) {
		if (e) e->m_error = GetLastError();
		return FALSE;
	}

	// ERROR_ALREADY_EXISTS の確認が必要なときは、このエラーコードを取得する必要がある。
	if (e) e->m_error = GetLastError();
	m_hmmf = hmmf;

	return TRUE;
}

// メモリマップドファイルを開く

BOOL rkMemFile::Open(LPCTSTR lpName, rkMemFileException* e = NULL)
{
	// すでにメモリマップドファイルを管理中のときはエラー
	if (m_hmmf != NULL) {
		if (e) e->m_error = rkMemFileException::errAlreadyUse | rkMemFileException::customerCodeFlag;
		return FALSE;
	}

	// メモリマップドファイルを開く
	HANDLE hmmf = OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, lpName);
	if (hmmf == NULL) {
		if (e) e->m_error = GetLastError();
		return FALSE;
	}

	if (e) e->m_error = GetLastError();	// 通常は正常終了コード
	m_hmmf = hmmf;

	return TRUE;
}

// メモリマップドファイルを閉じる

VOID rkMemFile::Close()
{
	// メモリマップドファイルをクローズ
	if (m_hmmf != NULL) {
		ReleasePointer();	// エラーにはならないから開放

		CloseHandle(m_hmmf);
		m_hmmf = NULL;
	}
}

// メモリマップドファイルへのポインターを取得する

LPVOID rkMemFile::GetPointer()
{
	if (m_hmmf == NULL)	return NULL;	// メモリマップドファイルを持っていない
	if (m_pt != NULL)	return m_pt;	// まだ、リリースしてない

	m_pt = MapViewOfFile(m_hmmf, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	return m_pt;
}

// メモリマップドファイルへのポインターを開放する

VOID rkMemFile::ReleasePointer()
{
	if (m_hmmf == NULL)	return;			// メモリマップドファイルを持っていない
	if (m_pt == NULL)	return;			// ポインタも取得していない

	UnmapViewOfFile(m_pt);
	m_pt = NULL;
}
