//---------------------------------------------------------------------
//---------------------------------------------------------------------
#include "kjmlib.h"


//---------------------------------------------------------------------
// ���b�N�t�@�C�������S�Ɉ������߂̃N���X
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// ���b�N����
//---------------------------------------------------------------------
BOOL kjm::Lockfile::Lock(const std::string& strLockfile) {
	// ���b�N�t�@�C���̍폜�����݂�(���s���Ă��C�ɂ��Ȃ�)
	// �V�X�e���̋����I���ȂǂŁA���b�N�t�@�C�����c�邱�Ƃ����邽�߁B
	// ���̂悤�ɂ��āA�c�������b�N�t�@�C���́A�폜���邱�Ƃ��ł���B
	::DeleteFile(strLockfile.c_str());

	this->m_hFile = ::CreateFile(strLockfile.c_str(), CREATE_NEW, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, NULL);
	return (this->m_hFile != INVALID_HANDLE_VALUE) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// �A�����b�N����
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
// �������u������BVB��StrReplace�̂悤�Ȋ֐��B
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
// �t�H���_�̍쐬
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
	Winssa_Log(_T("KJMLIB"), LOGLV_ERR, _T("�t�H���_�쐬�ُ�: error %d: %s"), GetLastError(), pszFolder);
#endif

	return FALSE;
}
