//---------------------------------------------------------------------
// kjmpath : Version 0.2015.12.8
//
// �v���R���p�C���ς݃w�b�_���g�p���Ȃ��悤�ɐݒ肵�Ă��������B
//
// r.kojima
//---------------------------------------------------------------------

#pragma once

#pragma warning(disable : 4786)
#pragma warning(disable : 4996)

#include "kjmbase.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

namespace kjm {
	//
	// �p�X���Ɋւ��鏈��
	//
	class path {
	public:

		// �t�@�C�������݂��邩�ǂ������ׂ�
		static bool fileExists(const kjm::_tstring& strPath);

		// \\?\D:\very long path, \\?\UNC\server\share\very long path �ɕϊ�����
		static kjm::_tstring makeVeryLongPath(const kjm::_tstring& path);

		// \\?\ �܂��� \\?\UNC ����菜��
		static kjm::_tstring removeVeryLongPath(const kjm::_tstring& path);

		// ������ \ ��ǉ�
		static kjm::_tstring addBackslash(const kjm::_tstring& path);

		// �w�肵���p�X������̊g���q(�s���I�h���܂�)��Ԃ��B
		static kjm::_tstring getExtension(const kjm::_tstring& path);

		// 2�̕������1�̃p�X�Ɍ������܂��B
		static kjm::_tstring combine(const kjm::_tstring& path1, const kjm::_tstring& path2);

		// 2�̕������1�̃p�X�Ɍ������܂��B
		static kjm::_tstring append(const kjm::_tstring& path1, const kjm::_tstring& path2) {
			return kjm::path::combine(path1, path2);
		}

		// �t���p�X����t�@�C��������菜��
		static kjm::_tstring removeFileSpec(const kjm::_tstring& strPath);

		// �t���p�X����t�@�C������ύX����
		static kjm::_tstring renameFileSpec(const kjm::_tstring& strPath, const kjm::_tstring& strFileSpec) {
			return kjm::path::append(kjm::path::removeFileSpec(strPath), strFileSpec);
		}

		// �t�@�C�����������擾
		static kjm::_tstring getFileName(const kjm::_tstring& path);
		
		// �t���p�X����A�t�@�C���������o��
		static kjm::_tstring findFileName(const kjm::_tstring& strPath) {
			return PathFindFileName(strPath.c_str());
		}

		// �f�B���N�g�����ǂ������ׂ�
		static bool isDirectory(const kjm::_tstring& strPath) {
			return (PathIsDirectory(strPath.c_str()) != FALSE);
		}

		// ���΃p�X���쐬����
		static kjm::_tstring relativePathTo(const kjm::_tstring& from, DWORD attrFrom, const kjm::_tstring& to, DWORD attrTo);

		// PathRemoveExtension �֐��̃��b�p�[
		static kjm::_tstring removeExtension(const kjm::_tstring& strPath);

		// �w�肵���p�X������̃f�B���N�g������Ԃ��܂��B
		//
		// [�߂�l]
		// path �����[�g �f�B���N�g���������ꍇ�A�󕶎����Ԃ��B
		static kjm::_tstring getDirectoryName(const kjm::_tstring& path);

	private:
		path() {}
		~path() {}
	};
};
