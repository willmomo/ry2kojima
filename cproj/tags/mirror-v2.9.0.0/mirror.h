/*	�C������
	v2.7.0.5	v2.7.0.5	2014/08/27	RECYCLER ���f�t�H���g���O�t�H���_�ɒǉ��B
	v2.7.0.3	v2.7.0.3	2014/07/14	Thumbs.db ����ɏ��O����悤�ɏC���B
	v2.7.0.2	v2.7.0.2	2014/06/30	�e�X�g���[�h(--test)�̂Ƃ��A�t�@�C�������o�Ȃ������̂��C���B
	v2.7.0.0	v2.7.0.1	2014/06/18	�R�s�[�̐i���\����}���C���ɕύX�B
	v2.7.0.0	v2.7.0.0	2014/06/18	$RECYCLE.BIN, System Volume Information ��̃t�H���_����ɏ��O����悤�ɏC���B
*/
#pragma once

#include <windows.h>

#include <iostream>
#include <string>
#include <deque>
#include <list>

using namespace std;

#include "kjmlib.h"
///#pragma comment(lib, "./kjmlib_20130801/wsslog.lib")

#include "resource.h"

#define WARNING_SEC			60		// �t�@�C��/�t�H���_�񋓂ɂ���ȏォ����ƌx������B

class BackupMirror {
public:

	// ���샂�[�h
	enum modeOption {
		modeBackupOnly = 0,		// �o�b�N�A�b�v����
		modeMirrorOnly,			// �~���[�����O����
		modeBackupMirror,		// �o�b�N�A�b�v���Ă���~���[�����O
		modeMirrorBackup		// �~���[�����O���Ă���o�b�N�A�b�v
	};

public:
	BackupMirror();
	virtual ~BackupMirror() {}

public:
	// �o�b�N�A�b�v�ƃ~���[�����O�𓮍샂�[�h�ɂ���Ď��s����
	int doMain();

private:
	// �t�@�C���̃R�s�[����
	bool copy_file(const kjm::fileInfo& fiOrg, const _tstring& strMir, const _tstring& orgBase);

	// �o�b�N�A�b�v�̎��s
	bool StartBackup();

	// �~���[�����O�̎��s
	bool StartMirror();

	// �o�b�N�A�b�v����
	bool backup(const _tstring& strOrg, const _tstring& strMir);

	// �~���[�����O����
	bool mirror(const _tstring& strOrg, const _tstring& strMir);
	
	// ���O������s��
	bool is_exclude(const _tstring& strFname);

public:
	// ���{�p�X�̎擾/�ݒ�
	_tstring get_OriginalPath() const { return m_strOrg; }
	void set_OriginalPath(const _tstring& newValue) { m_strOrg = newValue; }

	// �����p�X�̎擾/�ݒ�
	_tstring get_MirrorPath() const { return m_strMir; }
	void set_MirrorPath(const _tstring& newValue) { m_strMir = newValue; }

	// ���샂�[�h�̎擾/�ݒ�
	modeOption get_Mode() const { return m_mode; }
	void set_Mode(modeOption newValue) { m_mode = newValue; }

	// �e�X�g���[�h�̎擾/�ݒ�
	bool get_TestMode() const { return m_fTest; }
	void set_TestMode(bool newValue) { m_fTest = newValue; }

	// �A�[�J�C�u�̂݃��[�h�擾/�ݒ�
	bool get_ArchiveOnlyMode() const { return m_fArchive; }
	void set_ArchiveOnlyMode(bool newValue) { m_fArchive = newValue; }

	// �����l�����[�h�擾/�ݒ�
	bool get_TimeGup() const { return m_fTimeGup; }
	void set_TimeGup(bool newValue) { m_fTimeGup = newValue; }

	// ��t�H���_�쐬���Ȃ����[�h
	bool get_NoEmptyFolder() const { return m_fNoEmptyFolder; }
	void set_NoEmptyFolder(bool newValue) { m_fNoEmptyFolder = newValue; }

	// ������ׂ胂�[�h�擾/�ݒ�
	bool get_Verbose() const { return m_fVerbose; }
	void set_Verbose(bool newValue) { m_fVerbose = newValue; }

	// �x���\���b���̎擾/�ݒ�
	DWORD get_WarningSec() const { return m_warningSec; }
	void set_WarningSec(DWORD newValue) { m_warningSec = newValue; }

	// ���O�t�@�C���ꗗ�̎擾/�ݒ�
	std::vector<_tstring> get_ExcludeList() const { return m_excludeList; }
	void set_ExcludeList(const std::vector<_tstring>& newValue) { m_excludeList = newValue; }

	// �t�����[�h�擾/�ݒ�
	bool get_Reverse() const { return m_fReverse; }
	void set_Reverse(bool newValue) { m_fReverse = newValue; }

	// ���t���`�F�b�N�ΏۂƂ��Ȃ��ݒ�̎擾/�ݒ�
	bool get_IgnoreDate() const { return m_fIgnoreDate; }
	void set_IgnoreDate(bool newValue) { m_fIgnoreDate = newValue; }

	// �T�C�Y���`�F�b�N�ΏۂƂ��Ȃ��ݒ�̎擾/�ݒ�
	bool get_IgnoreSize() const { return m_fIgnoreSize; }
	void set_IgnoreSize(bool newValue) { m_fIgnoreSize = newValue; }

	// �ċA���������Ȃ��ݒ�̎擾/�ݒ�
	bool get_NoRecurse() const { return m_fNoRecurse; }
	void set_NoRecurse(bool newValue) { m_fNoRecurse = newValue; }

	// ���L�ᔽ�R�s�[�G���[���G���[�Ƃ��Ĉ���
	bool get_Strict() const { return m_fStrict; }
	void set_Strict(bool newValue) { m_fStrict = newValue; }

private:
	_tstring m_strOrg;					// ���{�̃p�X
	_tstring m_strMir;					// �����̃p�X
	modeOption m_mode;						// ���샂�[�h
	bool m_fTest;							// �e�X�g���[�h�� true
	DWORD m_warningSec;						// �x����\������b��
	std::vector<_tstring> m_excludeList;	// ���O�t�@�C���ꗗ
	bool m_fArchive;						// �A�[�J�C�u�̂݃��[�h�� true
	bool m_fTimeGup;						// �����l�����[�h�� true
	bool m_fNoEmptyFolder;					// ��t�H���_���쐬���Ȃ����[�h�� true
	bool m_fVerbose;						// ������ׂ胂�[�h�� true
	bool m_fIgnoreDate;					// ���t���`�F�b�N�ΏۂƂ��Ȃ��� true
	bool m_fIgnoreSize;					// �T�C�Y���`�F�b�N�ΏۂƂ��Ȃ��� true

	bool m_fReverse;						// �t�H���_�𖼑O�t���\�[�g�ŏ������� true
		// �t�H���_�������t�̏ꍇ�A�V�����t�H���_���珈�������邱�Ƃ��ł���悤�ɂȂ�B

	bool m_fNoRecurse;						// (--norecurse)�ċA�I�ɃR�s�[���s��Ȃ��Ƃ��� true
	bool m_fStrict;							// (--strict)���L�ᔽ�R�s�[�G���[���G���[�Ƃ��Ĉ����Ƃ��� true

	kjm::_tstrings m_defaultExcludeFolders;	// �W���̏��O�t�H���_���X�g
	kjm::_tstrings m_defaultExcludeFiles;	// �W���̏��O�t�@�C�����X�g
};
