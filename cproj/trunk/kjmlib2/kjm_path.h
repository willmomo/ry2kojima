#pragma once

#include "kjm_base.h"

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

//=========================================================================
// �p�X���Ɋւ��鏈��
//=========================================================================
namespace kjm {
	class path {
	public:
		static kjm::_tstring addBackslash(const kjm::_tstring& path) {
			TCHAR temp[MAX_PATH];
			::PathAddBackslash(lstrcpy(temp, path.c_str()));
			return temp;
		}

		// �w�肵���p�X������̊g���q(�s���I�h���܂�)��Ԃ��B
		static kjm::_tstring getExtension(const kjm::_tstring& path);

		// 2�̕������1�̃p�X�Ɍ������܂��B
		static kjm::_tstring combine(const kjm::_tstring& path1, const kjm::_tstring& path2);

		// 2�̕������1�̃p�X�Ɍ������܂��B
		static kjm::_tstring append(const kjm::_tstring& path1, const kjm::_tstring& path2) {
			return kjm::path::combine(path1, path2);
		}

		// �t���p�X����t�@�C��������菜��
		static kjm::_tstring removeFileSpec(const kjm::_tstring& strPath) {
			TCHAR szTemp[MAX_PATH];
			PathRemoveFileSpec(lstrcpy(szTemp, strPath.c_str()));
			return szTemp;
		}

		// �t���p�X����t�@�C������ύX����
		static kjm::_tstring renameFileSpec(const kjm::_tstring& strPath, const kjm::_tstring& strFileSpec) {
			return kjm::path::append(kjm::path::removeFileSpec(strPath), strFileSpec);
		}

		// �t�@�C�������݂��邩�ǂ������ׂ�
		static bool fileExists(const kjm::_tstring& strPath) {
			return (PathFileExists(strPath.c_str()) != FALSE);
		}

		// �t�@�C�����������擾
		static kjm::_tstring getFileName(const kjm::_tstring& path);
		
		// �t���p�X����A�t�@�C���������o��
		static kjm::_tstring findFileName(const kjm::_tstring& strPath) {
			return PathFindFileName(strPath.c_str());
		}

		// �f�B���N�g�����ǂ������ׂ�
		static bool isDirectory(const _tstring& strPath) {
			return (PathIsDirectory(strPath.c_str()) != FALSE);
		}

		// ���΃p�X���쐬����
		static _tstring relativePathTo(const _tstring& from, DWORD attrFrom, const _tstring& to, DWORD attrTo);

		// PathRemoveExtension �֐��̃��b�p�[
		static _tstring removeExtension(const _tstring& strPath);

		// �w�肵���p�X������̃f�B���N�g������Ԃ��܂��B
		//
		// [�߂�l]
		// path �����[�g �f�B���N�g���������ꍇ�A�󕶎����Ԃ��B
		static _tstring getDirectoryName(const _tstring& path) {
			TCHAR work[MAX_PATH];
			::PathRemoveFileSpec(::lstrcpy(work, path.c_str()));

			// �ϊ���̕�����ɕω����Ȃ��Ƃ��́A���[�g�f�B���N�g���Ɣ��f���āA�󕶎����Ԃ��B
			if (::lstrcmpi(path.c_str(), work) == 0) return kjm::_tstring();

			return work;
		}

		// �p�X�̃��[�g���ȊO�̕��������ׂĎ�菜��
		static _tsting stripToRoot(const _tstring& path);
		
	private:
		path() {}
		~path() {}
	};
};
