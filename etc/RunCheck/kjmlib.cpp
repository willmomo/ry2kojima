//---------------------------------------------------------------------
//---------------------------------------------------------------------
#include "kjmlib.h"


//---------------------------------------------------------------------
// ロックファイルを安全に扱うためのクラス
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// ロックする
//---------------------------------------------------------------------
BOOL kjm::Lockfile::Lock(const std::string& strLockfile) {
	// ロックファイルの削除を試みる(失敗しても気にしない)
	// システムの強制終了などで、ロックファイルが残ることがあるため。
	// このようにして、残ったロックファイルは、削除することができる。
	::DeleteFile(strLockfile.c_str());

	this->m_hFile = ::CreateFile(strLockfile.c_str(), CREATE_NEW, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, NULL);
	return (this->m_hFile != INVALID_HANDLE_VALUE) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// アンロックする
//---------------------------------------------------------------------
BOOL kjm::Lockfile::Unlock() {
	BOOL bRet = TRUE;

	if (this->m_hFile != INVALID_HANDLE_VALUE) {
		bRet = ::CloseHandle(this->m_hFile);
		this->m_hFile = INVALID_HANDLE_VALUE;
	}

	return bRet;
}


//---------------------------------------------------------------------
// 文字列を置換する。VBのStrReplaceのような関数。
//---------------------------------------------------------------------
std::string kjm::util::str_replace(const std::string& src, LPCTSTR pFind, LPCTSTR pReplace) {
	std::string result = src;
	std::string::size_type pos = 0;
	while ((pos = result.find(pFind, pos)) != std::string::npos) {
		result.replace(pos, lstrlen(pFind), pReplace);
	}
	return result;
}


//---------------------------------------------------------------------
// フォルダの作成
//---------------------------------------------------------------------
BOOL kjm::util::MakeDirectory(LPCTSTR pszFolder) {
	if (PathIsRoot(pszFolder))	return TRUE;
	if (PathFileExists(pszFolder))	return TRUE;

	TCHAR szParent[MAX_PATH];
	PathRemoveFileSpec(lstrcpy(szParent, pszFolder));

	if (kjm::util::MakeDirectory(szParent)) {
		if (CreateDirectory(pszFolder, FALSE)) {
			return TRUE;
		}
	}

#if defined(USE_WSSLOG)
	Winssa_Log(_T("KJMLIB"), LOGLV_ERR, _T("フォルダ作成異常: error %d: %s"), GetLastError(), pszFolder);
#endif

	return FALSE;
}
