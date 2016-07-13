#pragma once

// CFileFind::GetFilePath �̃o�O�΍�N���X�B

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
