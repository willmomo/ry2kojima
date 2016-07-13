// rkMemFile.h : メモリマップドファイル簡単管理クラス

#include <windows.h>
#include "rkException.h"

// rkMemFile で使用する例外クラス

class rkMemFileException : public rkException
{
public:
	enum ErrorCode { errAlreadyUse = 1 };

public:
	rkMemFileException() {};
	rkMemFileException(int code) : rkException(code) {};
	virtual ~rkMemFileException() {};
};

// メモリマップドファイルの管理管理クラス

class rkMemFile
{
private:
	HANDLE m_hmmf;	// メモリマップドファイルへのハンドル
	LPVOID m_pt;	// メモリマップドファイルへのポインタ

public:

public:
	rkMemFile() : m_hmmf(NULL), m_pt(NULL) {};
	rkMemFile(LPCTSTR lpName, DWORD dwSize) : m_hmmf(NULL), m_pt(NULL)
	{
		rkMemFileException e;
		if (Create(lpName, dwSize, &e) == FALSE)
			throw e;
	};
	rkMemFile(LPCTSTR lpName) : m_hmmf(NULL), m_pt(NULL)
	{
		rkMemFileException e;
		if (Open(lpName, &e) == FALSE)
			throw e;
	};
	virtual ~rkMemFile()
	{
		Close();
	};

public:
	BOOL Create(LPCTSTR lpName, DWORD dwSize, rkMemFileException* e);	// メモリマップドファイルの作成
	BOOL Open(LPCTSTR lpName, rkMemFileException* e);	// メモリマップドファイルを開く
	VOID Close();	// メモリマップドファイルを閉じる

	LPVOID GetPointer();	// ポインタを取得する
	VOID ReleasePointer();	// ポインタを開放する
};
