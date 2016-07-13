#pragma once

#include <crtdbg.h>

// 簡単メモリマップドファイル用例外 クラス

class CMemMapFileException
{
private:
	DWORD m_error;

public:
	CMemMapFileException(DWORD error) : m_error(error) {};
	virtual ~CMemMapFileException() {};

	DWORD GetError() { return m_error; };
};

// 簡単メモリマップドファイル クラス

class CMemMapFile
{
private:
	HANDLE m_hmmf;
	void* m_pvBase;

public:
	CMemMapFile() : m_hmmf(NULL), m_pvBase(NULL) {};
	virtual ~CMemMapFile() {
		if (Close() == FALSE) {
			throw CMemMapFileException(::GetLastError());
		}
	};

	BOOL IsAvailable() { return (m_hmmf != NULL) ? TRUE : FALSE; };
	BOOL IsMapped() { return (m_pvBase != NULL) ? TRUE : FALSE; };

	BOOL Create(LPCTSTR pszName, DWORD dwSize);
	BOOL Open(LPCTSTR pszName);
	BOOL Close();

	void* GetPointer();
	BOOL ReleasePointer();
};
