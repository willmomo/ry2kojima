//---------------------------------------------------------------------
// kjmpath : Version 0.2015.12.8
//
// �v���R���p�C���ς݃w�b�_���g�p���Ȃ��悤�ɐݒ肵�Ă��������B
//
// r.kojima
//---------------------------------------------------------------------
#include "path.h"


//
// ������ \ ��ǉ�
//
kjm::_tstring kjm::path::addBackslash(const kjm::_tstring& path) {

	// MAX_PATH �𒴂���� PathAddBackslash ���g���Ȃ��Ȃ�̂ŁA����

	kjm::_tstring result = path;
#if defined(_MBCS)
	if (_ismbstrail((unsigned char *)&result[0], (unsigned char *)&result[result.length() - 1])) {
		result.push_back(_T('\\'));
	} else if (*result.rbegin() != _T('\\')) {
		result.push_back(_T('\\'));
	}
#else
	if (*result.rbegin() != _T('\\')) {
		result.push_back(_T('\\'));
	}
#endif
	return result;
}


//
// �t�@�C�������݂��邩�ǂ������ׂ�
//
bool kjm::path::fileExists(const kjm::_tstring& strPath) {
	kjm::_tstring work = strPath;
	if (work.length() > MAX_PATH) {
		work = kjm::path::makeVeryLongPath(work);
	}
	BOOL bRet = PathFileExists(work.c_str());
	return (bRet ? true : false);
}

//
// \\?\D:\very long path, \\?\UNC\server\share\very long path �ɕϊ�����
//
kjm::_tstring kjm::path::makeVeryLongPath(const kjm::_tstring& path) {

	// ���ł� very long path �̂Ƃ��́A���̂܂܃��^�[��
	if (path[0] == _T('\\') && path[1] == _T('\\') && path[2] == _T('?')) {
		return path;
	}

	kjm::_tstring result;

	if (path[0] == _T('\\')) {
		// UNC�p�X�̎�
		result = _T("\\\\?\\UNC");
		result += &path[1];
	} else {
		// �ʏ�p�X�̎�
		result = _T("\\\\?\\");
		result += path;
	}

	return result;
}

//
// \\?\ �܂��� \\?\UNC ����菜��
//
kjm::_tstring kjm::path::removeVeryLongPath(const kjm::_tstring& path) {
	// very long path �ȊO�̎��́A���̂܂܃��^�[��
	if (path[0] != _T('\\') || path[1] != _T('\\') || path[2] != _T('?')) {
		return path;
	}

	kjm::_tstring result;

	if (path[4] == _T('U')) {
		// UNC�p�X�̎�
		result = _T('\\');
		result += &path[7];
	} else {
		// �ʏ�p�X�̎�
		result = &path[4];
	}

	return result;
}

//
// �w�肵���p�X������̊g���q(�s���I�h���܂�)��Ԃ��B
//
kjm::_tstring kjm::path::getExtension(const kjm::_tstring& path) {
	std::vector<TCHAR> work(MAX_PATH + path.length());
	::lstrcpy(&work[0], path.c_str());
	kjm::_tstring result = ::PathFindExtension(&work[0]);
	return result;
}


//
// 2�̕������1�̃p�X�Ɍ������܂��B
//
kjm::_tstring kjm::path::combine(const kjm::_tstring& path1, const kjm::_tstring& path2) {
	return kjm::path::addBackslash(path1) + path2;
}

//
// �t�@�C�����������擾
//
kjm::_tstring kjm::path::getFileName(const kjm::_tstring& path) {
	return ::PathFindFileName(path.c_str());
}

//
// ���΃p�X���쐬����
//
kjm::_tstring kjm::path::relativePathTo(const kjm::_tstring& from, DWORD attrFrom, const kjm::_tstring& to, DWORD attrTo) {
	std::vector<TCHAR> path(MAX_PATH);
	if (::PathRelativePathTo(&path[0], from.c_str(), attrFrom, to.c_str(), attrTo) == FALSE) {
		kjm::_tostringstream oss;
		oss << _T("kjm::path::relativePathTo error: PathRelativePathTo(\"") << from << _T("\", ") 
			<< attrFrom << _T(", \"") << to << _T("\", ") << attrTo << _T(") �Ɏ��s���܂����B");
		throw std::runtime_error(kjm::base::toAscii(oss.str()));
	}
	return &path[0];
}

//
// PathRemoveExtension �֐��̃��b�p�[
//
kjm::_tstring kjm::path::removeExtension(const kjm::_tstring& strPath) {
	TCHAR szTemp[MAX_PATH];
	PathRemoveExtension(lstrcpy(szTemp, strPath.c_str()));
	return szTemp;
}

//
// �t���p�X����t�@�C��������菜��
//
kjm::_tstring kjm::path::removeFileSpec(const kjm::_tstring& strPath) {
	std::vector<TCHAR> work;
	work.insert(work.begin(), strPath.begin(), strPath.end());
	work.push_back(_T('\0'));
	BOOL b = PathRemoveFileSpec(&work[0]);
	if (b == FALSE) {
		kjm::_tostringstream oss;
		oss << _T("kjm::path::removeFileSpec error: PathRemoveFileSpec(\"") << &work[0] << _T("\") �Ɏ��s���܂����B");
		throw std::runtime_error(kjm::base::toAscii(oss.str()));
	}
	return &work[0];
}

//
// �w�肵���p�X������̃f�B���N�g������Ԃ��܂��B
//
// [�߂�l]
// path �����[�g �f�B���N�g���������ꍇ�A�󕶎����Ԃ��B
//
kjm::_tstring kjm::path::getDirectoryName(const kjm::_tstring& path) {
	TCHAR work[MAX_PATH];
	::PathRemoveFileSpec(::lstrcpy(work, path.c_str()));

	// �ϊ���̕�����ɕω����Ȃ��Ƃ��́A���[�g�f�B���N�g���Ɣ��f���āA�󕶎����Ԃ��B
	if (::lstrcmpi(path.c_str(), work) == 0) return kjm::_tstring();

	return work;
}
