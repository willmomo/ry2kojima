//=========================================================================
// �p�X���Ɋւ��鏈��
//=========================================================================

#include "kjm_path.h"

//=============================================================================
// �p�X���Ɋւ��鏈��
//=============================================================================
//---------------------------------------------------------------------
// �w�肵���p�X������̊g���q(�s���I�h���܂�)��Ԃ��B
//---------------------------------------------------------------------
kjm::_tstring kjm::path::getExtension(const kjm::_tstring& path) {
	std::vector<TCHAR> work(MAX_PATH + path.length());
	::lstrcpy(&work[0], path.c_str());
	kjm::_tstring result = ::PathFindExtension(&work[0]);
	return result;
}

//---------------------------------------------------------------------
// 2�̕������1�̃p�X�Ɍ������܂��B
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::path::combine(const std::basic_string<TCHAR>& path1, const std::basic_string<TCHAR>& path2) {
	std::vector<TCHAR> work(MAX_PATH);
	
	std::runtime_error e("ng");

	if (!::PathAppend(::lstrcpy(&work[0], path1.c_str()), path2.c_str())) {
		DWORD dwErr = GetLastError();
		throw kjm::error_exception(dwErr);
	}

	return &work[0];
}

//---------------------------------------------------------------------
// �t�@�C�����������擾
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::path::getFileName(const std::basic_string<TCHAR>& path) {
	return ::PathFindFileName(path.c_str());
}

//---------------------------------------------------------------------
// ���΃p�X���쐬����
//---------------------------------------------------------------------
kjm::_tstring kjm::path::relativePathTo(const kjm::_tstring& from, DWORD attrFrom, const kjm::_tstring& to, DWORD attrTo) {
	std::vector<TCHAR> path(MAX_PATH);
	if (::PathRelativePathTo(&path[0], from.c_str(), attrFrom, to.c_str(), attrTo) == FALSE) {
		_otstringstream oss;
		oss << _T("kjm::path::relativePathTo error: PathRelativePathTo(\"") << from << _T("\", ") 
			<< attrFrom << _T(", \"") << to << _T("\", ") << attrTo << _T(") �Ɏ��s���܂����B");
		throw kjm::error_exception(-1, oss.str());
	}
	return &path[0];
}

//---------------------------------------------------------------------
// PathRemoveExtension �֐��̃��b�p�[
//---------------------------------------------------------------------
kjm::_tstring kjm::path::removeExtension(const kjm::_tstring& strPath) {
	TCHAR szTemp[MAX_PATH];
	PathRemoveExtension(lstrcpy(szTemp, strPath.c_str()));
	return szTemp;
}


// �p�X�̃��[�g���ȊO�̕��������ׂĎ�菜��

kjm::_tsting kjm::path::stripToRoot(const kjm::_tstring& path) {
	std::vector<TCHAR> work(MAX_PATH);
	PathStripRoot(lstrcpy(&work[0], path.c_str()));
	return &work[0];
}
