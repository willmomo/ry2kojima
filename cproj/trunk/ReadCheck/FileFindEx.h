#pragma once

// CFileFind::GetFilePath のバグ対策クラス。

class CFileFindEx : public CFileFind {
public:
	CFileFindEx() {};
	virtual ~CFileFindEx() {};

	CString GetFilePath() const
	{
		TCHAR strResult[MAX_PATH];
		::PathAppend(::lstrcpy(strResult, m_strRoot), GetFileName());
		return strResult;
	};
};
